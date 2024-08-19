#!/bin/bash
INPUT=input
OUTPUT=output
NAME=./builtins
NUM=1

mkdir -p "$OUTPUT"

while IFS= read -r line; do
	IFS=' ' read -r -a args <<< "$line"
    echo "test case ${NUM}: $line" > ${OUTPUT}/${NUM}
	echo >> ${OUTPUT}/${NUM}
	bash "${args}" >> ${OUTPUT}/${NUM}
	bash export >> ${OUTPUT}/${NUM}
	echo >> ${OUTPUT}/${NUM}
	${NAME} "${args}" >> ${OUTPUT}/${NUM}
	(( NUM++ ))
done < "$INPUT"