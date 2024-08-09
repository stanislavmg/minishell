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
LOG=log
NUM=1

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
    printf "${color}%-3s | %-6s: %s\n${RESET}" "${number}" "${status}" "${command}"
}

mkdir -p "$OUTPUT"
mkdir -p "$LOG"

if [ "$1" == "-re" ]; then
    rm -f minishell &> /dev/null
    make -C "$DIR" MAIN=tests/test_ast/test_main.c re clean
    if [ -f "${DIR}/minishell" ]; then
        mv "${DIR}/minishell" ./
    else
        echo "Make error: Could not create minishell."
        exit 1
    fi
fi

if [ "$1" == "-clear" ]; then
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
    bash -c "$line" > "${OUTPUT}/${NUM}_bash"
    ./minishell "$line" > "${OUTPUT}/${NUM}_msh"
    if ! diff "${OUTPUT}/${NUM}_bash" "${OUTPUT}/${NUM}_msh" >/dev/null 2>&1; then
        echo "Test case ${NUM}: $line" > "${LOG}/${NUM}_log"
        echo -n "Bash output: " >> "${LOG}/${NUM}_log"
		echo >> "${LOG}/${NUM}_log"
        cat "${OUTPUT}/${NUM}_bash" >> "${LOG}/${NUM}_log"
		echo >> "${LOG}/${NUM}_log"
        echo -n "Minishell output: " >> "${LOG}/${NUM}_log"
		echo >> "${LOG}/${NUM}_log"
        cat "${OUTPUT}/${NUM}_msh" >> "${LOG}/${NUM}_log"
        print_line "${NUM}" "[ KO ]" "${line}"
    else
        print_line "${NUM}" "[ OK ]" "${line}"
    fi
    (( NUM++ ))
done < "$INPUT"

echo -e "\n${BOLD}SYNTAX TEST${RESET}\n"
while IFS= read -r line; do
    if ./minishell "$line" | grep "syntax error" >& /dev/null; then
        print_line "${NUM}" "[ OK ]" "${line}"
    else
        print_line "${NUM}" "[ KO ]" "${line}"
    fi
    (( NUM++ ))
done < "$SYNTAX"

echo -e "\n${BOLD}REDIRECT TEST${RESET}\n"
while IFS= read -r line; do
    bash -c "$line" 2> /dev/null > out
    mv out out_bash
    ./minishell "$line" 2> /dev/null > out
    mv out out_msh
    if ! diff out_bash out_msh >/dev/null; then
        echo "Test case ${NUM}: $line" > "${LOG}/${NUM}_log"
        echo -n "Bash output: " >> "${LOG}/${NUM}_log"
		echo >> "${LOG}/${NUM}_log"
        cat "out_bash" >> "${LOG}/${NUM}_log"
		echo >> "${LOG}/${NUM}_log"
        echo -n "Minishell output: " >> "${LOG}/${NUM}_log"
		echo >> "${LOG}/${NUM}_log"
        cat "out_msh" >> "${LOG}/${NUM}_log"
        print_line "${NUM}" "[ KO ]" "${line}"
    else
        print_line "${NUM}" "[ OK ]" "${line}"
    fi
    rm -f out_bash out_msh
    (( NUM++ ))
done < "$REDIR"

rm -f "$OUTPUT"/* test_main.o