#!/bin/bash

DIR=../../
MSH=./minishell
INPUT=brackets.txt 
REDIR=redirect.txt
SYNTAX=syntax.txt
OUTPUT=output
LOG=log
NUM=1

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
    fi
    (( NUM++ ))
done < "$INPUT"

while IFS= read -r line; do
    bash -c "$line"
    mv out out_bash
    ./minishell "$line"
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
    fi
    rm -f out_bash out_msh
    (( NUM++ ))
done < "$REDIR"

rm -f "$OUTPUT"/*