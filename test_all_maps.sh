#!/bin/bash

# Script completo para testar o programa cub3D
# Uso: ./test_all_maps.sh [opções]
# Opções: 
#   -v, --valid     : Testar apenas mapas válidos
#   -i, --invalid   : Testar apenas mapas inválidos
#   --valgrind      : Executar testes com Valgrind (detecta memory leaks)
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
SUPPRESSION_FILE="./suppression.sup"
LOG_DIR="./logs"
VALGRIND_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=$SUPPRESSION_FILE"

# Flags
test_valid=true
test_invalid=true
use_valgrind=false

# Função de ajuda
show_help() {
    echo -e "${BLUE}Uso: $0 [opções]${NC}"
    echo
    echo -e "${YELLOW}Opções:${NC}"
    echo -e "  -v, --valid     Testar apenas mapas válidos"
    echo -e "  -i, --invalid   Testar apenas mapas inválidos"
    echo -e "  --valgrind      Executar testes com Valgrind (detecta memory leaks)"
    echo -e "  -h, --help      Mostrar esta ajuda"
    echo
    echo -e "${YELLOW}Exemplos:${NC}"
    echo -e "  $0              # Testa todos os mapas"
    echo -e "  $0 -v           # Testa apenas mapas válidos"
    echo -e "  $0 -i           # Testa apenas mapas inválidos"
    echo -e "  $0 --valgrind   # Testa todos os mapas com Valgrind"
    echo -e "  $0 -i --valgrind # Testa mapas inválidos com Valgrind"
    echo
    echo -e "${YELLOW}Nota:${NC} Testes com Valgrind são mais lentos mas detectam vazamentos de memória."
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
        --valgrind)
            use_valgrind=true
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

# Função para verificar vazamentos reais no Valgrind
check_real_leaks() {
    local valgrind_file=$1
    
    # Extrair valores numéricos dos vazamentos (apenas bytes, não o número do processo)
    local definitely_lost=$(grep "definitely lost:" "$valgrind_file" | sed 's/.*definitely lost: \([0-9]*\) bytes.*/\1/')
    local indirectly_lost=$(grep "indirectly lost:" "$valgrind_file" | sed 's/.*indirectly lost: \([0-9]*\) bytes.*/\1/')
    local possibly_lost=$(grep "possibly lost:" "$valgrind_file" | sed 's/.*possibly lost: \([0-9]*\) bytes.*/\1/')
    
    # Definir valores padrão como 0 se não encontrados ou se a substituição falhou
    definitely_lost=${definitely_lost:-0}
    indirectly_lost=${indirectly_lost:-0}
    possibly_lost=${possibly_lost:-0}
    
    # Verificar se são números válidos
    [[ "$definitely_lost" =~ ^[0-9]+$ ]] || definitely_lost=0
    [[ "$indirectly_lost" =~ ^[0-9]+$ ]] || indirectly_lost=0
    [[ "$possibly_lost" =~ ^[0-9]+$ ]] || possibly_lost=0
    
    # Retornar 0 se há vazamentos, 1 se não há
    if [ $definitely_lost -gt 0 ] || [ $indirectly_lost -gt 0 ] || [ $possibly_lost -gt 0 ]; then
        return 0  # Há vazamentos
    else
        return 1  # Não há vazamentos
    fi
}

# Função para testar mapas
test_maps() {
    local maps_dir=$1
    local expected_result=$2  # 0 para válidos, 1 para inválidos
    local test_type=$3
    
    if [ "$use_valgrind" = true ]; then
        echo -e "${PURPLE}Testando mapas $test_type com Valgrind...${NC}"
        echo -e "${YELLOW}(Isso pode demorar mais tempo)${NC}"
    else
        echo -e "${PURPLE}Testando mapas $test_type...${NC}"
    fi
    echo
    
    local total=0
    local passed=0
    local failed=0
    local memory_errors=0
    
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
        
        # Preparar comando e arquivos temporários para Valgrind
        if [ "$use_valgrind" = true ]; then
            local valgrind_output="/tmp/valgrind_output_$$_$total.txt"
            local individual_log="$LOG_DIR/valgrind_${test_type,,}_${map_name%.*}_$TIMESTAMP.log"
            
            # Executar com Valgrind e capturar saída
            $VALGRIND_CMD "$PROGRAM" "$map_file" > /dev/null 2>"$valgrind_output"
            exit_code=$?
            
            # Verificar se há vazamentos de memória usando função auxiliar
            if check_real_leaks "$valgrind_output"; then
                local has_memory_leak=true
                memory_errors=$((memory_errors + 1))
                
                # Extrair valores para logging
                local definitely_lost=$(grep "definitely lost:" "$valgrind_output" | sed 's/.*definitely lost: \([0-9]*\) bytes.*/\1/')
                local indirectly_lost=$(grep "indirectly lost:" "$valgrind_output" | sed 's/.*indirectly lost: \([0-9]*\) bytes.*/\1/')
                local possibly_lost=$(grep "possibly lost:" "$valgrind_output" | sed 's/.*possibly lost: \([0-9]*\) bytes.*/\1/')
                definitely_lost=${definitely_lost:-0}
                indirectly_lost=${indirectly_lost:-0}
                possibly_lost=${possibly_lost:-0}
            else
                local has_memory_leak=false
            fi
            
            # Verificar se há erros de file descriptors
            if grep -q "Open file descriptor" "$valgrind_output"; then
                local has_fd_leak=true
            else
                local has_fd_leak=false
            fi
            
            # Salvar log individual se houver problemas ou se solicitado
            if [ "$has_memory_leak" = true ] || [ "$has_fd_leak" = true ]; then
                echo "======================================" > "$individual_log"
                echo "VALGRIND LOG - $map_name" >> "$individual_log"
                echo "Data: $(date)" >> "$individual_log"
                echo "Comando: $VALGRIND_CMD $PROGRAM $map_file" >> "$individual_log"
                echo "Exit code: $exit_code" >> "$individual_log"
                echo "======================================" >> "$individual_log"
                echo >> "$individual_log"
                cat "$valgrind_output" >> "$individual_log"
                
                # Adicionar ao log principal
                echo "PROBLEMA DETECTADO: $map_name" >> "$MAIN_LOG"
                echo "Exit code: $exit_code" >> "$MAIN_LOG"
                if [ "$has_memory_leak" = true ]; then
                    echo "- Memory leak detectado:" >> "$MAIN_LOG"
                    echo "  definitely lost: $definitely_lost bytes" >> "$MAIN_LOG"
                    echo "  indirectly lost: $indirectly_lost bytes" >> "$MAIN_LOG"
                    echo "  possibly lost: $possibly_lost bytes" >> "$MAIN_LOG"
                    grep -A 5 -B 2 "definitely lost\|indirectly lost\|possibly lost" "$valgrind_output" >> "$MAIN_LOG"
                fi
                if [ "$has_fd_leak" = true ]; then
                    echo "- File descriptor leak detectado" >> "$MAIN_LOG"
                    grep -A 3 -B 1 "Open file descriptor" "$valgrind_output" >> "$MAIN_LOG"
                fi
                echo "Log detalhado: $individual_log" >> "$MAIN_LOG"
                echo "----------------------------------------" >> "$MAIN_LOG"
                echo >> "$MAIN_LOG"
            else
                # Apenas adicionar ao log principal se não houver problemas
                echo "OK: $map_name (sem leaks detectados)" >> "$MAIN_LOG"
            fi
            
            # Limpar arquivo temporário
            rm -f "$valgrind_output"
        else
            # Executar normalmente sem Valgrind
            "$PROGRAM" "$map_file" > /dev/null 2>&1
            exit_code=$?
            local has_memory_leak=false
            local has_fd_leak=false
        fi
        
        # Verificar resultado baseado no tipo de teste
        local test_passed=false
        if [ $expected_result -eq 0 ]; then
            # Mapas válidos devem retornar 0
            if [ $exit_code -eq 0 ]; then
                test_passed=true
            fi
        else
            # Mapas inválidos devem retornar código de erro (!= 0)
            if [ $exit_code -ne 0 ]; then
                test_passed=true
            fi
        fi
        
        # Exibir resultado
        if [ "$test_passed" = true ]; then
            if [ "$use_valgrind" = true ]; then
                if [ "$has_memory_leak" = true ] || [ "$has_fd_leak" = true ]; then
                    echo -e "${YELLOW}PASSOU/LEAK${NC}"
                    if [ "$has_memory_leak" = true ]; then
                        echo -e "    ${YELLOW}⚠ Memory leak detectado${NC}"
                    fi
                    if [ "$has_fd_leak" = true ]; then
                        echo -e "    ${YELLOW}⚠ File descriptor leak detectado${NC}"
                    fi
                else
                    echo -e "${GREEN}PASSOU${NC}"
                fi
            else
                echo -e "${GREEN}PASSOU${NC}"
            fi
            passed=$((passed + 1))
        else
            if [ $expected_result -eq 0 ]; then
                echo -e "${RED}FALHOU${NC} (deveria ser válido)"
            else
                echo -e "${RED}FALHOU${NC} (deveria ser inválido)"
            fi
            failed=$((failed + 1))
        fi
    done
    
    echo
    echo -e "${BLUE}--- Resumo $test_type ---${NC}"
    echo -e "Total: $total | Passou: ${GREEN}$passed${NC} | Falhou: ${RED}$failed${NC}"
    if [ "$use_valgrind" = true ] && [ $memory_errors -gt 0 ]; then
        echo -e "Memory/FD leaks detectados: ${YELLOW}$memory_errors${NC}"
    fi
    echo
    
    # Retornar número de falhas para o total geral
    return $failed
}

# Início do script
echo -e "${BLUE}========================================${NC}"
if [ "$use_valgrind" = true ]; then
    echo -e "${BLUE}   TESTADOR COMPLETO - cub3D (Valgrind)  ${NC}"
else
    echo -e "${BLUE}      TESTADOR COMPLETO - cub3D         ${NC}"
fi
echo -e "${BLUE}========================================${NC}"
echo

# Verificar se o programa existe
if [ ! -f "$PROGRAM" ]; then
    echo -e "${RED}Erro: Programa $PROGRAM não encontrado!${NC}"
    echo -e "${YELLOW}Execute 'make' para compilar o programa.${NC}"
    exit 1
fi

# Verificações específicas para Valgrind
if [ "$use_valgrind" = true ]; then
    # Verificar se o Valgrind está instalado
    if ! command -v valgrind &> /dev/null; then
        echo -e "${RED}Erro: Valgrind não está instalado!${NC}"
        echo -e "${YELLOW}Instale com: sudo apt-get install valgrind${NC}"
        exit 1
    fi
    
    # Criar diretório de logs
    mkdir -p "$LOG_DIR"
    
    # Verificar se o arquivo de supressão existe
    if [ ! -f "$SUPPRESSION_FILE" ]; then
        echo -e "${YELLOW}Aviso: Arquivo de supressão $SUPPRESSION_FILE não encontrado.${NC}"
        echo -e "${YELLOW}Executando sem supressões...${NC}"
        VALGRIND_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes"
    fi
    
    # Criar arquivo de log principal
    TIMESTAMP=$(date '+%Y%m%d_%H%M%S')
    MAIN_LOG="$LOG_DIR/valgrind_test_$TIMESTAMP.log"
    
    echo -e "${YELLOW}Modo Valgrind ativado - os testes serão mais lentos mas detectarão vazamentos de memória.${NC}"
    echo -e "${YELLOW}Logs serão salvos em: $LOG_DIR/${NC}"
    echo -e "${YELLOW}Log principal: $MAIN_LOG${NC}"
    echo
    
    # Inicializar arquivo de log
    echo "======================================" > "$MAIN_LOG"
    echo "TESTE VALGRIND - cub3D" >> "$MAIN_LOG"
    echo "Data: $(date)" >> "$MAIN_LOG"
    echo "======================================" >> "$MAIN_LOG"
    echo >> "$MAIN_LOG"
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
    exit_code_final=0
else
    echo -e "${RED}✗ $total_failures teste(s) falharam.${NC}"
    echo -e "${RED}Verifique a validação do programa.${NC}"
    exit_code_final=1
fi

# Informações sobre logs do Valgrind
if [ "$use_valgrind" = true ]; then
    echo
    echo -e "${YELLOW}📄 LOGS DO VALGRIND:${NC}"
    echo -e "Log principal: ${BLUE}$MAIN_LOG${NC}"
    
    # Contar arquivos de log individuais
    individual_logs=$(find "$LOG_DIR" -name "valgrind_*_$TIMESTAMP.log" -not -name "valgrind_test_$TIMESTAMP.log" 2>/dev/null | wc -l)
    if [ $individual_logs -gt 0 ]; then
        echo -e "Logs individuais de problemas: ${YELLOW}$individual_logs arquivo(s)${NC}"
        echo -e "Diretório de logs: ${BLUE}$LOG_DIR/${NC}"
        echo
        echo -e "${YELLOW}Para ver problemas específicos:${NC}"
        echo -e "  cat $MAIN_LOG"
        echo -e "  ls $LOG_DIR/valgrind_*_$TIMESTAMP.log"
    else
        echo -e "${GREEN}Nenhum problema detectado - sem logs individuais criados.${NC}"
    fi
fi

exit $exit_code_final
