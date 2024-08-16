#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'
BOLD='\033[1m'

DIR=../../
MSH=./minishell
INPUT=brackets.txt 
REDIR=redirect.txt
SYNTAX=syntax.txt
OUTPUT=output
NUM=1
LOG=log/case_

function print_line() {
    local number="$1"
    local status="$2"
    local command="$3"
    local color=""

    if [ "${status}" == "[ KO ]" ]; then
        color=${RED}
    else
        color=${GREEN}
    fi
    printf "${color}%-3s %-6s: %s\n${RESET}" "${number}" "${status}" "${command}"
}

function log_command() {
  local command="$1"
  local logfile="$2"
  {
    eval "$command"
  } >> "$logfile" 2>&1
}

mkdir -p "$OUTPUT"

if [ "$1" == "-re" ]; then
    rm -f minishell &> /dev/null
    make -C "$DIR" re clean
    if [ -f "${DIR}/minishell" ]; then
        mv "${DIR}/minishell" ./
    else
        echo "Make error: Could not create minishell."
        exit 1
    fi
fi

if [ "$1" == "clear" ]; then
    rm -f "$OUTPUT"/*
    exit 0
fi

if [ ! -f "minishell" ]; then
    echo "Error: minishell not found. Use the -re flag to build minishell."
    exit 1
fi

rm -f ${OUTPUT}/*
rm -f ${LOG}/*

echo -e "\n${BOLD}BASE TEST${RESET}\n"

while IFS= read -r line; do

    if diff $(eval "$line") $(./minishell "$line") > /dev/null 2>&1; then
        print_line "${NUM}" "[ KO ]" "${line}"
    else
        print_line "${NUM}" "[ OK ]" "${line}"
    fi
    (( NUM++ ))
done < "$INPUT"

echo -e "\n${BOLD}SYNTAX TEST${RESET}\n"

while IFS= read -r line; do
    bash_out=$(bash "$line" 2>&1 | awk '/syntax/{flag=1} flag; /'\''/{flag=0}')
    msh_out=$(./minishell "$line" 2>&1 | awk '/syntax/{flag=1} flag; /'\''/{flag=0}')
    if [ "$bash_out" = "$msh_out" ]; then
        print_line "${NUM}" "[ OK ]" "${line}"
    else
        print_line "${NUM}" "[ KO ]" "${line}"
        echo "Difference detected:"
        echo "Bash output: $bash_out"
        echo "Minishell output: $msh_out"
    fi
    (( NUM++ ))
done < "$SYNTAX"

# echo -e "\n${BOLD}REDIRECT TEST${RESET}\n"

# while IFS= read -r line; do
#     bash -c "$line" 2> /dev/null > out_bash
#     ./minishell "$line" 2> /dev/null > out_msh
#     if ! diff out_bash out_msh >/dev/null; then
#         print_line "${NUM}" "[ KO ]" "${line}"
#     else
#         print_line "${NUM}" "[ OK ]" "${line}"
#     fi
#     rm -f out_bash out_msh
#     (( NUM++ ))
# done < "$REDIR"

rm -rf  test_main.o output