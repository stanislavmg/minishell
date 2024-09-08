#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'
BOLD='\033[1m'

DIR=../
MSH=./minishell
INPUT=brackets.txt 
REDIR=redirect.txt
SYNTAX=syntax.txt
TOTAL=1
SUCCESS=0
LOG=log/
VG="valgrind --leak-check=full --quiet --error-exitcode=42"

function print_line() {
    local number="$1"
    local status="$2"
    local command="$3"
    local color=""

    if [ "${status}" == "[ OK ]" ]; then
        color=${GREEN}
    else
        color=${RED}
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

function check_leaks() {
    local command="$1"
    local log_file="$2"
	local vg=$(${VG} $command)
    if [ $? -eq 42 ]; then
            print_line "${TOTAL}" "[ LEAK ]" "$command"
			echo $vg > $log_file
    else
            print_line "${TOTAL}" "[ OK ]" "$command"
    fi
}

mkdir -p "log"

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

rm -f ${LOG}/*

echo -e "\n${BOLD}BASE TEST${RESET}\n"
while IFS= read -r line; do
    echo "$line" | bash > b_out
    echo "$line" | ./minishell > m_out
    if ! diff b_out m_out > /dev/null; then
        print_line "${TOTAL}" "[ KO ]" "${line}"
        echo "msh out:"; cat m_out
        echo "bash out:"; cat b_out
    else
        print_line "${TOTAL}" "[ OK ]" "${line}"
        (( SUCCESS++ ))
    fi
    (( TOTAL++ ))
done < "$INPUT"

# echo -e "\n${BOLD}SYNTAX TEST${RESET}\n"

# while IFS= read -r line; do
#     bash_out=$(bash -c "$line" 2>&1)
#     echo $bash_out
#     msh_out=$(echo "$line" | ./minishell 2>&1)
#     bash_trimmed=$(echo "$bash_out" | sed -n "s/.*\(syntax.*\`[^']*'\).*/\1/p")
#     msh_trimmed=$(echo "$msh_out" | cut -d ' ' -f2-)

#     if [ "$bash_trimmed" = "$msh_trimmed" ]; then
#         print_line "${TOTAL}" "[ OK ]" "${line}"
#     else
#         print_line "${TOTAL}" "[ KO ]" "${line}"
#         echo "Bash output: $bash_trimmed"
#         echo "Minishell output: $msh_trimmed"
#     fi
#     (( TOTAL++ ))
# done < "$SYNTAX"

echo -e "\n${BOLD}REDIRECT TEST${RESET}\n"

while IFS= read -r line; do
    echo "$line" | bash > b_out
    echo "$line" | ./minishell > m_out
    if ! diff b_out m_out > /dev/null; then
        print_line "${TOTAL}" "[ KO ]" "${line}"
        echo "msh out:"; cat m_out
        echo "bash out:"; cat b_out
    else
        print_line "${TOTAL}" "[ OK ]" "${line}"
        (( SUCCESS++ ))
    fi
    rm -f out_bash out_msh
    (( TOTAL++ ))
done < "$REDIR"
echo "Passed ${SUCCESS}/${TOTAL}"
rm -rf output m_out b_out out4