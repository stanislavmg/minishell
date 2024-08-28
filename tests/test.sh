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
NUM=1
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
    # Run the command with Valgrind and check for leaks
    if [ $? -eq 42 ]; then
            print_line "${NUM}" "[ LEAK ]" "$command"
			echo $vg > $log_file
    else
            print_line "${NUM}" "[ OK ]" "$command"
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
# while IFS= read -r line; do
#     if diff $(eval "$line") $(echo "$line" | ./minishell) 2> /dev/null; then
#         print_line "${NUM}" "[ KO ]" "${line}"
#     else
#         print_line "${NUM}" "[ OK ]" "${line}"
#     fi
#     check_leaks "echo ${line} | ./minishell" "${LOG}/valgrind_${NUM}.log"
#     (( NUM++ ))
# done < "$INPUT"

function run_all_tests_in_one_process() {
    local input_file="$1"
    local valgrind_log="$2"
    local expected_output_file="expected_output.txt"
    local minishell_output_file="minishell_output.txt"

    # Prepare expected outputs by evaluating all lines with the system shell
    while IFS= read -r line; do
        eval "$line" 2> /dev/null
    done < "$input_file" > "$expected_output_file"

    # Run all commands through minishell in one process
    $VG ./minishell < "$input_file" > "$minishell_output_file" 2> "$valgrind_log"
    sed -i '/minishell\$>/d' $minishell_output_file
    # Compare outputs line by line
    local line_num=1
    while IFS= read -r expected_line && IFS= read -r minishell_line <&3; do
        if [ "$expected_line" = "$minishell_line" ]; then
            print_line "$line_num" "[ OK ]" "Test case $line_num"
        else
            print_line "$line_num" "[ KO ]" "Test case $line_num"
            echo "Expected: $expected_line"
            echo "Got: $minishell_line"
        fi
        (( line_num++ ))
    done < "$expected_output_file" 3< "$minishell_output_file"

    # Check for leaks reported by Valgrind
    if grep -q "definitely lost: 0 bytes" "$valgrind_log"; then
        echo -e "${GREEN}No leaks detected.${RESET}"
    else
        echo -e "${RED}Memory leaks detected. Check $valgrind_log for details.${RESET}"
    fi

    # Cleanup temporary files
//    rm -f "$expected_output_file" "$minishell_output_file"
}

run_all_tests_in_one_process "$INPUT" "${LOG}/valgrind_batch.log"

echo -e "\n${BOLD}SYNTAX TEST${RESET}\n"

while IFS= read -r line; do
    bash_out=$(bash -c "$line" 2>&1)
    echo $bash_out
    msh_out=$(echo "$line" | ./minishell 2>&1)
    bash_trimmed=$(echo "$bash_out" | sed -n "s/.*\(syntax.*\`[^']*'\).*/\1/p")
    msh_trimmed=$(echo "$msh_out" | cut -d ' ' -f2-)

    if [ "$bash_trimmed" = "$msh_trimmed" ]; then
        print_line "${NUM}" "[ OK ]" "${line}"
    else
        print_line "${NUM}" "[ KO ]" "${line}"
        echo "Bash output: $bash_trimmed"
        echo "Minishell output: $msh_trimmed"
    fi
#    check_leaks "echo ${line} | ./minishell" "${LOG}/valgrind_${NUM}.log
    (( NUM++ ))
done < "$SYNTAX"

echo -e "\n${BOLD}REDIRECT TEST${RESET}\n"

while IFS= read -r line; do
    bash -c "$line" 2> /dev/null > out_bash
    echo "$line" | ./minishell 2> /dev/null > out_msh
    if ! diff out_bash out_msh >/dev/null; then
        print_line "${NUM}" "[ KO ]" "${line}"
    else
        print_line "${NUM}" "[ OK ]" "${line}"
    fi

    rm -f out_bash out_msh
    (( NUM++ ))
done < "$REDIR"

rm -rf  test_main.o output