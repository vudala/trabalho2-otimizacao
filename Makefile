#!/bin/bash

CC=g++
INPUT=elenco.cpp
OUTPUT=elenco
FLAGS=-g

all:
	$(CC) $(FLAGS) $(INPUT) -o $(OUTPUT)

run:
	./$(OUTPUT) < input.txt

clean:
	rm $(OUTPUT)