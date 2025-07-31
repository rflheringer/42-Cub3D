#!/bin/bash

# Script para limpar logs antigos do Valgrind
# Uso: ./clean_valgrind_logs.sh [minutos]

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configurações
LOG_DIR="./logs"
MINUTES=${1:-30}  # Padrão: 30 minutos

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    LIMPADOR DE LOGS VALGRIND           ${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Verificar se o diretório de logs existe
if [ ! -d "$LOG_DIR" ]; then
    echo -e "${YELLOW}Diretório de logs '$LOG_DIR' não encontrado.${NC}"
    exit 0
fi

echo -e "${YELLOW}Limpando logs do Valgrind mais antigos que $MINUTES minutos...${NC}"
echo

# Encontrar logs antigos (mais antigos que X minutos)
old_logs=$(find "$LOG_DIR" -name "valgrind_*.log" -type f -mmin +$MINUTES 2>/dev/null)

if [ -z "$old_logs" ]; then
    echo -e "${GREEN}✓ Nenhum log antigo encontrado.${NC}"
    echo -e "${GREEN}Todos os logs são mais recentes que $MINUTES minutos.${NC}"
else
    echo -e "${YELLOW}Logs que serão removidos:${NC}"
    echo "$old_logs" | while read -r log; do
        log_date=$(stat -c %y "$log" 2>/dev/null | cut -d' ' -f1-2)
        minutes_old=$(( ($(date +%s) - $(stat -c %Y "$log" 2>/dev/null)) / 60 ))
        echo -e "  ${RED}$(basename "$log")${NC} (criado em: $log_date, há $minutes_old minutos)"
    done
    
    echo
    read -p "Confirma a remoção destes logs? (y/N): " confirm
    
    if [[ $confirm =~ ^[Yy]$ ]]; then
        removed_count=$(echo "$old_logs" | wc -l)
        echo "$old_logs" | xargs rm -f
        echo -e "${GREEN}✓ $removed_count arquivo(s) removido(s).${NC}"
    else
        echo -e "${YELLOW}Operação cancelada.${NC}"
    fi
fi

# Mostrar estatísticas dos logs restantes
echo
echo -e "${BLUE}📊 ESTATÍSTICAS DOS LOGS:${NC}"
total_logs=$(find "$LOG_DIR" -name "valgrind_*.log" -type f 2>/dev/null | wc -l)
total_size=$(find "$LOG_DIR" -name "valgrind_*.log" -type f -exec du -ch {} + 2>/dev/null | tail -1 | cut -f1)

echo -e "Logs restantes: ${BLUE}$total_logs${NC}"
echo -e "Tamanho total: ${BLUE}$total_size${NC}"

if [ $total_logs -gt 0 ]; then
    echo
    echo -e "${YELLOW}Logs mais recentes:${NC}"
    find "$LOG_DIR" -name "valgrind_*.log" -type f -printf '%T@ %p\n' 2>/dev/null | sort -nr | head -5 | while read -r timestamp file; do
        date_str=$(date -d "@$timestamp" '+%Y-%m-%d %H:%M' 2>/dev/null)
        echo -e "  ${GREEN}$(basename "$file")${NC} ($date_str)"
    done
fi
