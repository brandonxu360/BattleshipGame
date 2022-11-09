#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct Cell {
	int x;
	int y;
	int alive;
	int hit;
	char type;
	char symbol;
} Cell;

typedef struct Stats {
	int hits;
	int misses;
	int shots;
	double accuracy;
	int wins;

} Stats;

void putsCenter(char str[]);
void displayMenu(void);
void displayRules(void);
int getMenuInput(void);
void fillBoard(Cell board[10][10]);
void obscureBoard(Cell board[10][10]);
int checkPlacement(int size, int count, int x, int y, Cell board[10][10], char type);
void place1Ship(int size, char type, Cell board[10][10], int player);
void manuallyPlaceShips(Cell board[10][10], int player);
void printBoard(Cell board[10][10]);
void randomlyPlace1Ship(Cell board[10][10], int size, int player, char type);
void randomlyPlaceShips(Cell board[10][10], int player);
int checkHit(int x, int y, Cell board[10][10]);
void playerTurn(int player, Cell board[10][10], Stats *stats);
void computerTurn(Cell board[10][10], Stats *stats);
int checkWin(Cell board[10][10]);
int getSetupChoice(int player);
int getPlayerChoice(int player);
