#!/bin/bash

# Script para analisar logs do Valgrind
# Uso: ./analyze_valgrind_logs.sh [diretório_logs]

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Configurações
LOG_DIR="${1:-./logs}"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    ANALISADOR DE LOGS VALGRIND         ${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Verificar se o diretório de logs existe
if [ ! -d "$LOG_DIR" ]; then
    echo -e "${RED}Erro: Diretório de logs '$LOG_DIR' não encontrado!${NC}"
    echo -e "${YELLOW}Use: $0 [diretório_logs]${NC}"
    exit 1
fi

# Encontrar logs do Valgrind
valgrind_logs=$(find "$LOG_DIR" -name "valgrind_*.log" 2>/dev/null)

if [ -z "$valgrind_logs" ]; then
    echo -e "${YELLOW}Nenhum log do Valgrind encontrado em $LOG_DIR${NC}"
    exit 1
fi

echo -e "${YELLOW}Logs encontrados em: $LOG_DIR${NC}"
echo

# Analisar logs principais (resumos)
main_logs=$(find "$LOG_DIR" -name "valgrind_test_*.log" -o -name "valgrind_invalid_*.log" 2>/dev/null)

if [ -n "$main_logs" ]; then
    echo -e "${PURPLE}📋 LOGS PRINCIPAIS (RESUMOS):${NC}"
    echo
    
    for log in $main_logs; do
        log_name=$(basename "$log")
        echo -e "${BLUE}📄 $log_name${NC}"
        
        # Contar problemas
        problems=$(grep -c "PROBLEMA DETECTADO\|Memory leak detectado\|File descriptor leak detectado" "$log" 2>/dev/null || echo "0")
        total_tests=$(grep -c "OK:\|TESTE:\|PROBLEMA DETECTADO:" "$log" 2>/dev/null || echo "0")
        
        echo -e "   Testes: $total_tests"
        echo -e "   Problemas: ${RED}$problems${NC}"
        
        if [ $problems -gt 0 ]; then
            echo -e "   ${YELLOW}Problemas encontrados:${NC}"
            grep -E "PROBLEMA DETECTADO:|TESTE:" "$log" | head -5 | sed 's/^/     /'
            if [ $(grep -c "PROBLEMA DETECTADO\|TESTE:" "$log") -gt 5 ]; then
                echo -e "     ${YELLOW}... e mais$(($(grep -c "PROBLEMA DETECTADO\|TESTE:" "$log") - 5)) problema(s)${NC}"
            fi
        fi
        echo
    done
fi

# Analisar logs individuais
individual_logs=$(find "$LOG_DIR" -name "valgrind_*.log" ! -name "valgrind_test_*.log" ! -name "valgrind_invalid_*.log" 2>/dev/null)

if [ -n "$individual_logs" ]; then
    echo -e "${PURPLE}🔍 LOGS INDIVIDUAIS (DETALHADOS):${NC}"
    echo
    
    total_individual=0
    memory_leaks=0
    fd_leaks=0
    
    for log in $individual_logs; do
        total_individual=$((total_individual + 1))
        log_name=$(basename "$log")
        
        # Verificar tipos de problemas (apenas valores > 0)
        definitely_lost_bytes=$(grep "definitely lost:" "$log" | sed 's/.*definitely lost: \([0-9]*\) bytes.*/\1/')
        indirectly_lost_bytes=$(grep "indirectly lost:" "$log" | sed 's/.*indirectly lost: \([0-9]*\) bytes.*/\1/')
        possibly_lost_bytes=$(grep "possibly lost:" "$log" | sed 's/.*possibly lost: \([0-9]*\) bytes.*/\1/')
        
        # Definir valores padrão como 0 se não encontrados ou se a substituição falhou
        definitely_lost_bytes=${definitely_lost_bytes:-0}
        indirectly_lost_bytes=${indirectly_lost_bytes:-0}
        possibly_lost_bytes=${possibly_lost_bytes:-0}
        
        # Verificar se são números válidos
        [[ "$definitely_lost_bytes" =~ ^[0-9]+$ ]] || definitely_lost_bytes=0
        [[ "$indirectly_lost_bytes" =~ ^[0-9]+$ ]] || indirectly_lost_bytes=0
        [[ "$possibly_lost_bytes" =~ ^[0-9]+$ ]] || possibly_lost_bytes=0
        
        has_memory_leak=0
        if [ $definitely_lost_bytes -gt 0 ] || [ $indirectly_lost_bytes -gt 0 ] || [ $possibly_lost_bytes -gt 0 ]; then
            has_memory_leak=1
        fi
        
        has_fd_leak=$(grep -c "Open file descriptor" "$log" 2>/dev/null || echo "0")
        
        if [ $has_memory_leak -gt 0 ]; then
            memory_leaks=$((memory_leaks + 1))
        fi
        
        if [ $has_fd_leak -gt 0 ]; then
            fd_leaks=$((fd_leaks + 1))
        fi
        
        echo -e "${BLUE}📄 $log_name${NC}"
        
        if [ $has_memory_leak -gt 0 ]; then
            echo -e "   ${RED}⚠ Memory leaks detectados${NC}"
            echo -e "     definitely lost: $definitely_lost_bytes bytes"
            echo -e "     indirectly lost: $indirectly_lost_bytes bytes"
            echo -e "     possibly lost: $possibly_lost_bytes bytes"
        fi
        
        if [ $has_fd_leak -gt 0 ]; then
            echo -e "   ${RED}⚠ File descriptor leaks detectados${NC}"
        fi
        
        if [ $has_memory_leak -eq 0 ] && [ $has_fd_leak -eq 0 ]; then
            echo -e "   ${GREEN}✓ Nenhum problema detectado${NC}"
        fi
        
        echo
    done
    
    echo -e "${BLUE}📊 ESTATÍSTICAS DOS LOGS INDIVIDUAIS:${NC}"
    echo -e "Total de logs: $total_individual"
    echo -e "Com memory leaks: ${RED}$memory_leaks${NC}"
    echo -e "Com FD leaks: ${RED}$fd_leaks${NC}"
    echo -e "Sem problemas: ${GREEN}$((total_individual - memory_leaks - fd_leaks))${NC}"
    echo
fi

# Análise de tipos de memory leaks mais comuns
echo -e "${PURPLE}🔬 ANÁLISE DE TIPOS DE LEAKS:${NC}"
echo

definitely_lost=$(find "$LOG_DIR" -name "valgrind_*.log" -exec grep -l "definitely lost" {} \; 2>/dev/null | wc -l)
indirectly_lost=$(find "$LOG_DIR" -name "valgrind_*.log" -exec grep -l "indirectly lost" {} \; 2>/dev/null | wc -l)
possibly_lost=$(find "$LOG_DIR" -name "valgrind_*.log" -exec grep -l "possibly lost" {} \; 2>/dev/null | wc -l)

echo -e "Definitely lost: ${RED}$definitely_lost arquivo(s)${NC}"
echo -e "Indirectly lost: ${YELLOW}$indirectly_lost arquivo(s)${NC}"
echo -e "Possibly lost: ${YELLOW}$possibly_lost arquivo(s)${NC}"

if [ $definitely_lost -gt 0 ]; then
    echo
    echo -e "${RED}⚠ ATENÇÃO: 'Definitely lost' são vazamentos críticos que devem ser corrigidos!${NC}"
fi

echo
echo -e "${YELLOW}💡 COMANDOS ÚTEIS:${NC}"
echo -e "Ver log principal mais recente:"
echo -e "  ${BLUE}cat \$(ls -t $LOG_DIR/valgrind_test_*.log $LOG_DIR/valgrind_invalid_*.log 2>/dev/null | head -1)${NC}"
echo
echo -e "Ver todos os memory leaks encontrados:"
echo -e "  ${BLUE}grep -r \"definitely lost\\|indirectly lost\" $LOG_DIR/${NC}"
echo
echo -e "Listar arquivos com problemas:"
echo -e "  ${BLUE}grep -l \"definitely lost\\|Open file descriptor\" $LOG_DIR/valgrind_*.log${NC}"
