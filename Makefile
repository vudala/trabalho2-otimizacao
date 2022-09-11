#!/bin/bash

CC=g++
INPUT=elenco.cpp
OUTPUT=elenco
FLAGS=-std=c++17

all:
	$(CC) $(FLAGS) $(INPUT) -o $(OUTPUT)

clean:
	rm $(OUTPUT)

purge: clean