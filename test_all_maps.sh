#!/bin/bash

# Script completo para testar o programa cub3D
# Uso: ./test_all_maps.sh [opções]
# Opções: 
#   -v, --valid     : Testar apenas mapas válidos
#   -i, --invalid   : Testar apenas mapas inválidos  
#   -h, --help      : Mostrar ajuda

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Configurações
PROGRAM="./cub3D"
VALID_MAPS_DIR="./maps/valid"
INVALID_MAPS_DIR="./maps/invalid"

# Flags
test_valid=true
test_invalid=true

# Função de ajuda
show_help() {
    echo -e "${BLUE}Uso: $0 [opções]${NC}"
    echo
    echo -e "${YELLOW}Opções:${NC}"
    echo -e "  -v, --valid     Testar apenas mapas válidos"
    echo -e "  -i, --invalid   Testar apenas mapas inválidos"
    echo -e "  -h, --help      Mostrar esta ajuda"
    echo
    echo -e "${YELLOW}Exemplos:${NC}"
    echo -e "  $0              # Testa todos os mapas"
    echo -e "  $0 -v           # Testa apenas mapas válidos"
    echo -e "  $0 -i           # Testa apenas mapas inválidos"
}

# Processar argumentos
while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--valid)
            test_valid=true
            test_invalid=false
            shift
            ;;
        -i|--invalid)
            test_valid=false
            test_invalid=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}Opção desconhecida: $1${NC}"
            show_help
            exit 1
            ;;
    esac
done

# Função para testar mapas
test_maps() {
    local maps_dir=$1
    local expected_result=$2  # 0 para válidos, 1 para inválidos
    local test_type=$3
    
    echo -e "${PURPLE}Testando mapas $test_type...${NC}"
    echo
    
    local total=0
    local passed=0
    local failed=0
    
    # Verificar se o diretório existe
    if [ ! -d "$maps_dir" ]; then
        echo -e "${YELLOW}Aviso: Diretório $maps_dir não encontrado, pulando...${NC}"
        echo
        return
    fi
    
    for map_file in "$maps_dir"/*.cub; do
        # Verificar se existem arquivos .cub
        if [ ! -f "$map_file" ]; then
            echo -e "${YELLOW}Nenhum arquivo .cub encontrado em $maps_dir${NC}"
            return
        fi
        
        total=$((total + 1))
        map_name=$(basename "$map_file")
        
        echo -ne "${BLUE}[$total]${NC} $map_name ... "
        
        # Executar o programa e capturar o código de saída
        "$PROGRAM" "$map_file" > /dev/null 2>&1
        exit_code=$?
        
        # Verificar resultado baseado no tipo de teste
        if [ $expected_result -eq 0 ]; then
            # Mapas válidos devem retornar 0
            if [ $exit_code -eq 0 ]; then
                echo -e "${GREEN}PASSOU${NC}"
                passed=$((passed + 1))
            else
                echo -e "${RED}FALHOU${NC} (deveria ser válido)"
                failed=$((failed + 1))
            fi
        else
            # Mapas inválidos devem retornar código de erro (!= 0)
            if [ $exit_code -ne 0 ]; then
                echo -e "${GREEN}PASSOU${NC}"
                passed=$((passed + 1))
            else
                echo -e "${RED}FALHOU${NC} (deveria ser inválido)"
                failed=$((failed + 1))
            fi
        fi
    done
    
    echo
    echo -e "${BLUE}--- Resumo $test_type ---${NC}"
    echo -e "Total: $total | Passou: ${GREEN}$passed${NC} | Falhou: ${RED}$failed${NC}"
    echo
    
    # Retornar número de falhas para o total geral
    return $failed
}

# Início do script
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}      TESTADOR COMPLETO - cub3D         ${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Verificar se o programa existe
if [ ! -f "$PROGRAM" ]; then
    echo -e "${RED}Erro: Programa $PROGRAM não encontrado!${NC}"
    echo -e "${YELLOW}Execute 'make' para compilar o programa.${NC}"
    exit 1
fi

total_failures=0

# Testar mapas válidos
if [ "$test_valid" = true ]; then
    test_maps "$VALID_MAPS_DIR" 0 "VÁLIDOS"
    total_failures=$((total_failures + $?))
fi

# Testar mapas inválidos  
if [ "$test_invalid" = true ]; then
    test_maps "$INVALID_MAPS_DIR" 1 "INVÁLIDOS"
    total_failures=$((total_failures + $?))
fi

# Resultado final
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           RESULTADO FINAL              ${NC}"
echo -e "${BLUE}========================================${NC}"

if [ $total_failures -eq 0 ]; then
    echo -e "${GREEN}✓ Todos os testes passaram!${NC}"
    echo -e "${GREEN}O programa está funcionando corretamente.${NC}"
    exit 0
else
    echo -e "${RED}✗ $total_failures teste(s) falharam.${NC}"
    echo -e "${RED}Verifique a validação do programa.${NC}"
    exit 1
fi
