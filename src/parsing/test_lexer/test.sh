#!/bin/bash
INPUT=test_case.txt 
OUTPUT=output
NUM=1


if [ "$1" == "-re" ]; then
	rm lex &> /dev/null
	make -C ../ lexer clean
	if [ -f ../lex ]; then
		mv ../lex ./
	else
		echo "Make error"
		exit
	fi
fi

if [ "$1" == "-clear" ]; then
	rm output/*
	rm lex
	exit 0
fi

if [ ! -f "lex" ]; then
	echo "lexer not found. Use -re flag for make lexer"
	exit 1
fi

while IFS= read -r line; do
    echo "test case ${NUM}: $line" > ${OUTPUT}/${NUM}
	./lex "${line}" >> ${OUTPUT}/${NUM}
	(( NUM++ ))
done < "$INPUT"