/*
* Name: Brandon Xu
* Project: Battleship Game
* File: header.h
* Date: 11/9/22
* Description: Header file containing function declarations and nessessary project libraries
* configs for the rest of the project
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
* STRUCT
* Name: Cell
* Fields: int x, int y, int alive, int hit, char type, char symbol
* Description: Used in a 2D array to represent the boards on which the players will play 
* battleship. Includes important fields that communicate if a ship is occupying a location,
* if the location has been fired on, what type of ship occupies the location, etc.
* 
* int x and y - location in the array (not used in this project yet)
* int alive - 1 if the Cell contains a ship that has not been hit, 0 if it does not contain a ship or the ship
* has been hit
* int hit - 1 if the Cell has been fired on, 0 if not
* char type - type of Cell occupation (c for carrier, b for battleship, r for cruiser, s for submarine, d for destroyer
* char symbol - symbol used to represent the char on the board that is printed out for the users
*/
typedef struct Cell {
	int x;
	int y;
	int alive;
	int hit;
	char type;
	char symbol;
} Cell;

/*
* STRUCT
* Name: Stats
* Fields: int hits, int misses, int shots, double accuracy, int wins
* Description: Struct to store the statistics about each player and print to console and output file 
* at the end of the game and the termination of the program.
*/
typedef struct Stats {
	int hits;
	int misses;
	int shots;
	double accuracy;
	int wins;

} Stats;

/*
* FUNCTION
* Name: putsCenter
* Inputs: string(array of char)
* Outputs: void (print string to console)
* Descriptoin: Creates padding based on the width of the console and the length of the inputted string
* and prints the string at the center of the console. This function is rarely used in the project (only 
* in the title for the menu display)
*/
void putsCenter(char str[]);

/*
* FUNCTION
* Name: displayMenu
* Inputs: void
* Outputs: void (prints strings to console)
* Description: Prints the menu prompt and options to console. Used in get getMenuInput to prompt the user
*/
void displayMenu(void);

/*
* FUNCTION
* Name: displayRules
* Inputs: void
* Outputs: void (prints strings to console)
* Description: Prints the rules of battleship to console
*/
void displayRules(void);

/*
* FUNCTION
* Name: getMenuInput
* Inputs: void
* Outputs: int
* Description: Uses displayMenu function to display menu options, then prompts user for input, scans input,
* and returns the user int input
*/
int getMenuInput(void);

/*
* FUNCTION
* Name: fillBoard
* Inputs: Cell board[10][10] (10x10 array of Cells)
* Outputs:void
* Description: Takes a 2D 10x10 array of Cells and runs through every index of the array, initializing 
* water Cells and assigning them to the array
*/
void fillBoard(Cell board[10][10]);

/*
* FUNCTION
* Name: obscureBoard
* Inputs: Cell board[10][10] (10x10 array of Cells)
* Outputs: void
* Description: Takes a 2D 10x10 array of Cells and runs through every index of the array, assigning every
* Cell's symbol to tilde, the water symbol. This hides the location of enemy ships
*/
void obscureBoard(Cell board[10][10]);

/*
* FUNCTION
* Name: checkPlacement
* Inputs: int size, int count, int x, int y, Cell board[10][10], char type
* Outputs: int (1 or 0)
* Description: Takes size of a ship, count of # of ship segments left to place for that ship, 
* int x and y location to check, the board to place the ship segment, and the type of ship to be placed.
* These values are used to check if the location (int x and y) is a valid location to place the ship
* segment. 
* The conditions that currently work are:
*	- Can not place at location outside the bounds of the board
*	- Can not place on location already occupied by ship segment
*	- If at least one segment has already been placed, can not place
*     another segment if it is not adjacent to another segment (not diagonal) 
*	- If at least 2 segments have been placed, can not place a segment that does not
*	  line up with the existing segments
* 
* These conditions satisfy the requirements set in the project description, however future changes intend
* to predict whether a early ship segment placement will result in a lack of valid locations to place 
* the rest of the segments (ex. surrounded by other ships)
* 
* Returns 1 if the coordinate (x and y) is valid. Otherwise, returns 0.
*/
int checkPlacement(int size, int count, int x, int y, Cell board[10][10], char type);

/*
* FUNCTION
* Name: place1Ship
* Inputs: int size, int count, int x, int y, Cell board[10][10], char type
* Outputs: void
* Description: Prompts user for coordinates to place all the segments of a single ship. This is 
* part of the manual method of placing ships for human players. The ship is of size
* int size and of type char type, to be placed on the board passed in. This function uses 
* the function checkPlacement to ensure a legal ship placement is achieved. The symbol, type, and alive
* fields of the Cell selected will be changed to reflect the occupation of the location by the ship segment.
*/
void place1Ship(int size, char type, Cell board[10][10], int player);

/*
* FUNCTION
* Name: manuallyPlaceShips
* Inputs: Cell board[10][10], int player
* Outputs: void
* Description: calls place1Ship function and feeds the appropriate inputs 5 times to place 5 ships as
* required for battlefield. This is part of the manual method of placing ships for human players.
*/
void manuallyPlaceShips(Cell board[10][10], int player);

/*
* FUNCTION
* Name: printBoard
* Inputs: Cell board[10][10], int player, int turn
* Outputs: void
* Description: Runs through the whole 2D array of Cells that represents the board and prints the
* symbols of each cell such that the board is displayed as a grid
*/
void printBoard(Cell board[10][10], int player, int turn);

/*
* FUNCTION
* Name: randomlyPlace1Ship
* Inputs: Cell board[10][10], int size, int player, char type
* Outputs: void
* Description: This function is part of the random method of placing ships that is selected automatically 
* for computer players and offered to players as a choice. THe function randomly selects x and y coordinates
* until a suitable location is found (using the checkPlacement function). Then the Cell's symbol, type, and 
* alive values are changed to reflect the occupation of a ship segment.
*/
void randomlyPlace1Ship(Cell board[10][10], int size, int player, char type);

/*
* FUNCTION
* Name: randomlyPlaceShips
* Inputs: Cell board[10][10], int player
* Outputs: void
* Description: This function is part of the random method of placing ships. It calls the function
* randomlyPlace1Ship and inputs the appropriate values 5 times such that 5 ships are placed that 
* fulfill the requirements for battleship. 
*/
void randomlyPlaceShips(Cell board[10][10], int player);

/*
* FUNCTION
* Name: checkHit
* Inputs: int x, int y, Cell board[10][10]
* Outputs: int
* Description: checks if the inputted location (x and y) is occupied by an enemy ship. If yes, returns 1. 
* Otherwise 0 is returned for a miss.
*/
int checkHit(int x, int y, Cell board[10][10]);

/*
* FUNCTION
* Name: PlayerTurn
* Inputs: int player, int playerboard, Cell board[10][10], Stats *stats, FILE *outputfile, int sunkships[5], int turn
* Outputs: void
* Description: handles a human player's turn. Prints the enemy board and prompts the a player for a 
* location to fire on (loops until a valid location is inputted). Checks if an enemy ship is occupying the 
* inputted location and if one is, changes the fields of that Cell to reflect a hit. Also prints to the user 
* that they hit an enemy ship and writes to the output file. If the Cell is a water Cell, the Cell fields are 
* changed to reflect a miss and prints to the user that they missed. The miss is also written to the output file.
*/
void playerTurn(int player, int playerboard, Cell board[10][10], Stats *stats, FILE *outputfile, int sunkships[5], int turn);

/*
* FUNCTION
* Name: computerTurn
* Inputs: Cell board[10][10], Stats *stats, FILE* outputfile, int sunkships[5], int player, int playerboard, int turn
* Outputs: void
* Description: handles a human player's turn. keeps selecting random x and y values between 0 and 9 (looping) until
* a location that has not been fired on by the computer player is chosen. Then checks if an enemy ship is occupying the 
* inputted location and if one is, changes the fields of that Cell to reflect a hit and the hit is written to the output file. 
* If the Cell is a water Cell, the Cell fields are changed to reflect a miss and the miss is written to the output file.
*/
void computerTurn(Cell board[10][10], Stats *stats, FILE* outputfile, int sunkships[5], int player, int playerboard, int turn);

/*
* FUNCTION
* Name: checkWin
* Inputs: Cell board[10][10]
* Outputs: int
* Description: loops through the entire board and if any Cells have an alive value of 1, returns 0 for no win 
* achieved yet. Otherwise, return 1 because no ships that have not been hit exist
*/
int checkWin(Cell board[10][10]);

/*
* FUNCTION
* Name: getSetupChoice
* Inputs: int player
* Outputs: int
* Description: displays the setup options (1 for manual and 2 for randomized) and prompts the user for input
* of 1 or 2 (loops until a valid input is inputted). Then returns the user input.
*/
int getSetupChoice(int player);

/*
* FUNCTION
* Name: getPlayerChoice
* Inputs: int player
* Outputs: int
* Description: displays player options (1 for human player and 2 for computer player) and prompts the user
* for an input of 1 or 2 (loops until a valid input is inputted). Then returns the user input. Used for assigning
* player identities for computer or human. 
*/
int getPlayerChoice(int player);

/*
* FUNCTION
* Name: writeStats
* Inputs: int player, Stats stats, FILE *outputfile
* Outputs: void
* Description: Prints the player number, hit count, miss count, shot count, the accuracy as a decimal, 
* and 1 if the player won and 0 if not. Also writes all of these statistics to the output file on the 
* termination of the program. 
* 
*/
void writeStats(int player, Stats stats, FILE *outputfile);

/*
* FUNCTION
* Name: checkSunkShips
* Inputs: Cell board[10][10], int x, int y, int sunkships[5], FILE* outfile
* Outputs: void
* Description: takes in sunkships array of length 5 that stores the number of each ship's remaining (unhit) 
* segments and updates the array based on which ship is hit. Then checks if any index of the array contains
* 0 (meaning no segments of that ship remain unhit). If so, this function indicates that the ship was sunk
* by printing to the console and writing to the output file which ship was sunk.
*/
void checkSunkShips(Cell board[10][10], int x, int y, int sunkships[5], FILE* outfile);

/*
* FUNCTION
* Name: validShot
* Inputs: int y, int x
* Outputs: int
* Description: returns 1 if the inputed x and y are both between 0 and 9 inclusive. Otherwise, return 9 for invalid 
*/
int validShot(int y, int x);

/*
* FUNCTION
* Name: startFirst
* Inputs: void
* Outputs: int
* Description: returns a random number between 1 and 2 inclusive for the purpose of randomly decieding which player goes first
*/
int startFirst(void);


