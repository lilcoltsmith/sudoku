#!/bin/bash

Sudoku: Sudoku.cpp
	g++ -std=c++11 -o Sudoku Sudoku.cpp

clean:
	rm Sudoku
