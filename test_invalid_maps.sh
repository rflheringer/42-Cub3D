#!/bin/bash

# Script para testar o programa cub3D com todos os mapas inválidos
# Uso: ./test_invalid_maps.sh

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Diretórios
PROGRAM="./cub3D"
INVALID_MAPS_DIR="./maps/invalid"

# Contador de testes
total_tests=0
passed_tests=0
failed_tests=0

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  TESTANDO MAPAS INVÁLIDOS - cub3D     ${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Verificar se o programa existe
if [ ! -f "$PROGRAM" ]; then
    echo -e "${RED}Erro: Programa $PROGRAM não encontrado!${NC}"
    echo -e "${YELLOW}Execute 'make' para compilar o programa.${NC}"
    exit 1
fi

# Verificar se o diretório de mapas existe
if [ ! -d "$INVALID_MAPS_DIR" ]; then
    echo -e "${RED}Erro: Diretório $INVALID_MAPS_DIR não encontrado!${NC}"
    exit 1
fi

echo -e "${YELLOW}Testando mapas inválidos...${NC}"
echo -e "${YELLOW}Mapas que devem FALHAR (retornar erro):${NC}"
echo

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
    
    # Executar o programa e capturar o código de saída
    # Redirecionar stdout e stderr para /dev/null para não poluir a saída
    "$PROGRAM" "$map_file" > /dev/null 2>&1
    exit_code=$?
    
    # Para mapas inválidos, esperamos que o programa retorne um código de erro (não 0)
    if [ $exit_code -ne 0 ]; then
        echo -e "${GREEN}PASSOU${NC} (erro detectado corretamente)"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "${RED}FALHOU${NC} (deveria ter retornado erro)"
        failed_tests=$((failed_tests + 1))
    fi
done

echo
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           RESUMO DOS TESTES            ${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Total de testes: ${BLUE}$total_tests${NC}"
echo -e "Testes que passaram: ${GREEN}$passed_tests${NC}"
echo -e "Testes que falharam: ${RED}$failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}✓ Todos os testes passaram!${NC}"
    echo -e "${GREEN}O programa detectou corretamente todos os mapas inválidos.${NC}"
else
    echo -e "${RED}✗ Alguns testes falharam.${NC}"
    echo -e "${RED}O programa não detectou alguns mapas inválidos como errôneos.${NC}"
fi

echo
echo -e "${YELLOW}Nota: Para mapas inválidos, o comportamento esperado é que o programa${NC}"
echo -e "${YELLOW}retorne um código de erro (diferente de 0) e exiba uma mensagem de erro.${NC}"
