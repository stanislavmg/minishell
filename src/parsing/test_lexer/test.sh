#!/bin/bash

INPUT=test_case.txt
OUTPUT=output
NUM=1

while IFS= read -r line; do
    echo "test case ${NUM}: $line" > ${OUTPUT}/${NUM}
	./lexer "${line}" >> ${OUTPUT}/${NUM}
	(( NUM++ ))
done < "$INPUT"