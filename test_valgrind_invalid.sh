#!/bin/bash

# Script específico para testar mapas inválidos com Valgrind
# Uso: ./test_valgrind_invalid.sh

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Configurações
PROGRAM="./cub3D"
INVALID_MAPS_DIR="./maps/invalid"
SUPPRESSION_FILE="./suppression.sup"
LOG_DIR="./logs"
VALGRIND_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=$SUPPRESSION_FILE"

# Contadores
total_tests=0
passed_tests=0
failed_tests=0
memory_errors=0

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  TESTE VALGRIND - MAPAS INVÁLIDOS      ${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Verificar se o programa existe
if [ ! -f "$PROGRAM" ]; then
    echo -e "${RED}Erro: Programa $PROGRAM não encontrado!${NC}"
    echo -e "${YELLOW}Execute 'make' para compilar o programa.${NC}"
    exit 1
fi

# Verificar se o Valgrind está instalado
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}Erro: Valgrind não está instalado!${NC}"
    echo -e "${YELLOW}Instale com: sudo apt-get install valgrind${NC}"
    exit 1
fi

# Verificar se o diretório de mapas existe
if [ ! -d "$INVALID_MAPS_DIR" ]; then
    echo -e "${RED}Erro: Diretório $INVALID_MAPS_DIR não encontrado!${NC}"
    exit 1
fi

# Verificar arquivo de supressão
if [ ! -f "$SUPPRESSION_FILE" ]; then
    echo -e "${YELLOW}Aviso: Arquivo de supressão $SUPPRESSION_FILE não encontrado.${NC}"
    echo -e "${YELLOW}Executando sem supressões...${NC}"
    VALGRIND_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes"
    echo
fi

# Criar diretório de logs
mkdir -p "$LOG_DIR"

# Criar arquivo de log principal
TIMESTAMP=$(date '+%Y%m%d_%H%M%S')
MAIN_LOG="$LOG_DIR/valgrind_invalid_$TIMESTAMP.log"

echo -e "${YELLOW}Testando mapas inválidos com Valgrind...${NC}"
echo -e "${YELLOW}Verificando vazamentos de memória e file descriptors...${NC}"
echo -e "${YELLOW}(Isso pode demorar alguns minutos)${NC}"
echo -e "${YELLOW}Logs serão salvos em: $LOG_DIR/${NC}"
echo -e "${YELLOW}Log principal: $MAIN_LOG${NC}"
echo

# Inicializar arquivo de log
echo "======================================" > "$MAIN_LOG"
echo "TESTE VALGRIND - MAPAS INVÁLIDOS" >> "$MAIN_LOG"
echo "Data: $(date)" >> "$MAIN_LOG"
echo "======================================" >> "$MAIN_LOG"
echo >> "$MAIN_LOG"

# Iterar sobre todos os arquivos .cub no diretório de mapas inválidos
for map_file in "$INVALID_MAPS_DIR"/*.cub; do
    # Verificar se existem arquivos .cub
    if [ ! -f "$map_file" ]; then
        echo -e "${RED}Nenhum arquivo .cub encontrado em $INVALID_MAPS_DIR${NC}"
        exit 1
    fi
    
    total_tests=$((total_tests + 1))
    map_name=$(basename "$map_file")
    
    echo -ne "${BLUE}[$total_tests]${NC} Testando: ${YELLOW}$map_name${NC} ... "
    
    # Arquivo temporário para saída do Valgrind
    valgrind_output="/tmp/valgrind_output_$$_$total_tests.txt"
    individual_log="$LOG_DIR/valgrind_invalid_${map_name%.*}_$TIMESTAMP.log"
    
    # Executar com Valgrind
    $VALGRIND_CMD "$PROGRAM" "$map_file" > /dev/null 2>"$valgrind_output"
    exit_code=$?
    
    # Verificar se há vazamentos de memória (apenas se valores > 0)
    has_memory_leak=false
    has_fd_leak=false
    
    # Extrair valores numéricos dos vazamentos (apenas bytes, não o número do processo)
    definitely_lost=$(grep "definitely lost:" "$valgrind_output" | sed 's/.*definitely lost: \([0-9]*\) bytes.*/\1/')
    indirectly_lost=$(grep "indirectly lost:" "$valgrind_output" | sed 's/.*indirectly lost: \([0-9]*\) bytes.*/\1/')
    possibly_lost=$(grep "possibly lost:" "$valgrind_output" | sed 's/.*possibly lost: \([0-9]*\) bytes.*/\1/')
    
    # Definir valores padrão como 0 se não encontrados ou se a substituição falhou
    definitely_lost=${definitely_lost:-0}
    indirectly_lost=${indirectly_lost:-0}
    possibly_lost=${possibly_lost:-0}
    
    # Verificar se são números válidos
    [[ "$definitely_lost" =~ ^[0-9]+$ ]] || definitely_lost=0
    [[ "$indirectly_lost" =~ ^[0-9]+$ ]] || indirectly_lost=0
    [[ "$possibly_lost" =~ ^[0-9]+$ ]] || possibly_lost=0
    
    if [ $definitely_lost -gt 0 ] || [ $indirectly_lost -gt 0 ] || [ $possibly_lost -gt 0 ]; then
        has_memory_leak=true
        memory_errors=$((memory_errors + 1))
    fi
    
    if grep -q "Open file descriptor" "$valgrind_output"; then
        has_fd_leak=true
    fi
    
    # Para mapas inválidos, esperamos que o programa retorne um código de erro (não 0)
    if [ $exit_code -ne 0 ]; then
        # Programa retornou erro corretamente
        if [ "$has_memory_leak" = true ] || [ "$has_fd_leak" = true ]; then
            echo -e "${YELLOW}PASSOU/LEAK${NC}"
            if [ "$has_memory_leak" = true ]; then
                echo -e "    ${YELLOW}⚠ Memory leak detectado${NC}"
            fi
            if [ "$has_fd_leak" = true ]; then
                echo -e "    ${YELLOW}⚠ File descriptor leak detectado${NC}"
            fi
        else
            echo -e "${GREEN}PASSOU${NC} (erro detectado, sem leaks)"
        fi
        passed_tests=$((passed_tests + 1))
    else
        echo -e "${RED}FALHOU${NC} (deveria ter retornado erro)"
        failed_tests=$((failed_tests + 1))
    fi
    
    # Salvar logs se houver problemas ou sempre
    if [ "$has_memory_leak" = true ] || [ "$has_fd_leak" = true ] || [ $exit_code -eq 0 ]; then
        # Criar log individual detalhado
        echo "======================================" > "$individual_log"
        echo "VALGRIND LOG - $map_name" >> "$individual_log"
        echo "Data: $(date)" >> "$individual_log"
        echo "Comando: $VALGRIND_CMD $PROGRAM $map_file" >> "$individual_log"
        echo "Exit code: $exit_code" >> "$individual_log"
        echo "======================================" >> "$individual_log"
        echo >> "$individual_log"
        cat "$valgrind_output" >> "$individual_log"
        
        # Adicionar ao log principal
        echo "TESTE: $map_name" >> "$MAIN_LOG"
        echo "Exit code: $exit_code" >> "$MAIN_LOG"
        if [ $exit_code -eq 0 ]; then
            echo "- PROBLEMA: Programa não retornou erro para mapa inválido" >> "$MAIN_LOG"
        fi
        if [ "$has_memory_leak" = true ]; then
            echo "- Memory leak detectado:" >> "$MAIN_LOG"
            echo "  definitely lost: $definitely_lost bytes" >> "$MAIN_LOG"
            echo "  indirectly lost: $indirectly_lost bytes" >> "$MAIN_LOG"
            echo "  possibly lost: $possibly_lost bytes" >> "$MAIN_LOG"
            grep -A 5 -B 2 "definitely lost\|indirectly lost\|possibly lost" "$valgrind_output" | sed 's/^/  /' >> "$MAIN_LOG"
        fi
        if [ "$has_fd_leak" = true ]; then
            echo "- File descriptor leak detectado:" >> "$MAIN_LOG"
            grep -A 3 -B 1 "Open file descriptor" "$valgrind_output" | sed 's/^/  /' >> "$MAIN_LOG"
        fi
        echo "Log detalhado: $individual_log" >> "$MAIN_LOG"
        echo "----------------------------------------" >> "$MAIN_LOG"
        echo >> "$MAIN_LOG"
    else
        # Apenas adicionar ao log principal se não houver problemas
        echo "OK: $map_name (programa retornou erro, sem leaks)" >> "$MAIN_LOG"
    fi
    
    # Mostrar detalhes dos leaks se houver (na tela)
    if [ "$has_memory_leak" = true ]; then
        echo -e "    ${PURPLE}Detalhes salvos em: $individual_log${NC}"
    fi
    
    # Limpar arquivo temporário
    rm -f "$valgrind_output"
    
    echo
done

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           RESUMO DOS TESTES            ${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Total de testes: ${BLUE}$total_tests${NC}"
echo -e "Testes que passaram: ${GREEN}$passed_tests${NC}"
echo -e "Testes que falharam: ${RED}$failed_tests${NC}"
echo -e "Memory/FD leaks detectados: ${YELLOW}$memory_errors${NC}"

echo
if [ $failed_tests -eq 0 ]; then
    if [ $memory_errors -eq 0 ]; then
        echo -e "${GREEN}✓ Todos os testes passaram sem vazamentos!${NC}"
        echo -e "${GREEN}O programa está funcionando corretamente.${NC}"
    else
        echo -e "${YELLOW}⚠ Todos os testes passaram, mas há vazamentos de memória.${NC}"
        echo -e "${YELLOW}Verifique o código para corrigir os memory leaks.${NC}"
    fi
else
    echo -e "${RED}✗ Alguns testes falharam.${NC}"
    echo -e "${RED}O programa não detectou alguns mapas inválidos como errôneos.${NC}"
fi

echo
echo -e "${YELLOW}📄 LOGS DO VALGRIND:${NC}"
echo -e "Log principal: ${BLUE}$MAIN_LOG${NC}"

# Contar arquivos de log individuais
individual_logs=$(find "$LOG_DIR" -name "valgrind_invalid_*_$TIMESTAMP.log" 2>/dev/null | wc -l)
if [ $individual_logs -gt 0 ]; then
    echo -e "Logs individuais: ${YELLOW}$individual_logs arquivo(s)${NC}"
    echo -e "Diretório de logs: ${BLUE}$LOG_DIR/${NC}"
    echo
    echo -e "${YELLOW}Para ver problemas específicos:${NC}"
    echo -e "  cat $MAIN_LOG"
    echo -e "  ls $LOG_DIR/valgrind_invalid_*_$TIMESTAMP.log"
else
    echo -e "${GREEN}Nenhum problema detectado - sem logs individuais criados.${NC}"
fi

echo
echo -e "${YELLOW}Dica: Para ver detalhes completos do Valgrind, execute:${NC}"
echo -e "${YELLOW}$VALGRIND_CMD $PROGRAM <arquivo.cub>${NC}"
