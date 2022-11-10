/*
* Name: Brandon Xu
* Project: Battleship Game
* File: functions.c
* Date: 11/9/22
* Description: Header file containing full function definitions to be used throughout the project
*/

#include "header.h"

void putsCenter(char str[]) {
	//console width is 120 characters
	int width = 120;
	int length = sizeof(str) - 1;  //discount the terminal '\0'
	int pad = (length >= width) ? 0 : (width - length) / 2;

	//print padded string
	printf("%*.*s%s\n", pad, pad, " ", str);

}

void displayMenu(void) {
	//menu text display
	puts("**** MENU ****");
	puts("--------------");
	puts("[1] display rules");
	puts("[2] start new game");
	puts("[3] quit");
}

void displayRules(void) {
	//print rules to console
	putsCenter("**** RULES ****");
	puts("------------------------------------------------------------------------------------------------------------------------");
	puts("| The objective of Battleship is to try and sink all of the other player's before they sink all of your ships. All of  |");
	puts("| the other player's ships are somewhere on his/her board. You try and hit them by calling out the coordinates of one  |");
	puts("| of the squares on the board. The other player also tries to hit your ships by calling out coordinates. Neither you n |");
	puts("| or the other player can see the other's board so you must try to guess where they are. Each board in the physical ga |");
	puts("| me has two grids: the lower (horizontal) section for the player's ships and the upper part (vertical during play) fo |");
	puts("| r recording the player's guesses.                                                                                    |");
	puts("|                                                                                                                      |");
	puts("| Each player places the 5 ships somewhere on their board. The ships can only be placed vertically or horizontally. Di |");
	puts("| agonal placement is not allowed. No part of a ship may hang off the edge of the board. Ships may not overlap each ot |");
	puts("| her. No ships may be placed on another ship. Once the guessing begins, the players may not move the ships.           |");
	puts("|                                                                                                                      |");
	puts("| The 5 ships are :                                                                                                    |");
	puts("|     - Carrier (occupies 5 spaces)                                                                                    |");
	puts("|     - Battleship (4)                                                                                                 |");
	puts("|     - Cruiser (3)                                                                                                    |");
	puts("|     - Submarine(3)                                                                                                   |");
	puts("|     - Destroyer(2)                                                                                                   |");

	puts("");
	system("pause");
}

int getMenuInput(void) {
	//return variable initialization 
	int input = 0;

	//prompt user 
	displayMenu();

	//scanner for user input
	printf("\nPlease input an option: ");
	scanf("%d", &input);

	//invalid input check
	while (input < 1 || input > 3) {
		system("cls");
		displayMenu();

		printf("\nInvalid input detected! Please try again: ");
		scanf("%d", &input);
	}

	return input;

}

void fillBoard(Cell board[10][10]) {
	//loop through entire board
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			//initialize water cell and place in board
			Cell cell = { i, k, 0, 0, 'w', '~'};
			board[i][k] = cell;
		}
	}
}

void obscureBoard(Cell board[10][10]) {
	//run through entire board
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			//set displayed symbol to water symbol
			board[i][k].symbol = '~';
		}
	}
}


int checkPlacement(int size, int count, int x, int y, Cell board[10][10], char type) {
	// !!! Each allcaps statement labels the section of code meant to catch a specific situation !!!

	//OUT OF RANGE
	if (x > 9 || y > 9 || x < 0 || y < 0) return 0;

	//ALREADY OCCUPIED
	if (board[y][x].type != 'w') return 0;

	//DOES NOT FORM A CONNECTED SHIP (excluding first coordinate for a ship)
	char up = 'w', down = 'w', right = 'w', left = 'w';

	//get adjacent cell types if not out of bounds
	if (y - 1 < 10) up = board[y - 1][x].type;
	if (y + 1 > -1) down = board[y + 1][x].type;
	if (x + 1 < 10) right = board[y][x + 1].type; 
	if (x - 1 > -1) left = board[y][x - 1].type;

	if (size != count && 
		(
		(type != right) &&
		(type != up) &&
		(type != left) &&
		(type != down)
		)) return 0;

	//SPACE FOR SHIP FOR DIRECTION COMMIT (2nd coordinate selection) (NOT WORKING)
	if (size - 1 == count) {
		int spaceCount = 0;
		
		//vertical ship
		if (up == type || down == type) {
			int index = y;
			//spaces above
			while (index > -1 && (board[index][x].type == 'w' || board[index][x].type == type)) {
				if (board[index][x].type == 'w') spaceCount++;

				index--;
			}
			index = y;
			//spaces below
			while (index < 10 && (board[index][x].type == 'w' || board[index][x].type == type)) {
				if (board[index][x].type == 'w') spaceCount++;

				index++;
			}
			
		}

		//horizontal ship
		else if (right == type || left == type) {
			int index = x;
			//spaces left
			while (index > -1 && (board[y][index].type == 'w' || board[y][index].type == type)) {
				if (board[y][index].type == 'w') spaceCount++;

				index--;
			}
			index = x;
			//spaces right
			while (index < 10 && (board[y][index].type == 'w' || board[y][index].type == type)) {
				if (board[y][index].type == 'w') spaceCount++;

				index++;
			}
			
		}
		if (size > spaceCount + 1) return 0;
	}

	//NOT A STRAIGHT SHIP

	int shipPieceCount = 0;

	//vertical ship
	if (up == type || down == type) {
		int index = y;
		//# of segments above
		while (index > -1 && (board[index][x].type == 'w' || board[index][x].type == type)) {
			if (board[index][x].type == type) shipPieceCount++;

			index--;
		}
		index = y;
		//# of segments below
		while (index < 10 && (board[index][x].type == 'w' || board[index][x].type == type)) {
			if (board[index][x].type == type) shipPieceCount++;

			index++;
		}

	}

	//horizontal ship
	else if (right == type || left == type) {
		int index = x;
		//# of segments to the left
		while (index > -1 && (board[y][index].type == 'w' || board[y][index].type == type)) {
			if (board[y][index].type == type) shipPieceCount++;

			index--;
		}
		index = x;
		//# of segments to the right
		while (index < 10 && (board[y][index].type == 'w' || board[y][index].type == type)) {
			if (board[y][index].type == type) shipPieceCount++;

			index++;
		}

	} 

	//the number of already placed ship segments must equal the nuimber of ship segments in the indented direction
	if (count != size - shipPieceCount) return 0;

	//FIRST COORDINATE SURROUNDED (NOT WORKING)
	if (size == count &&
			((right != 'w') &&
			(left != 'w') &&
			(up != 'w') &&
			(down != 'w'))
		) return 0;

	//NO INVALIDATING CONDITIONS MET
	return 1;
}


void place1Ship(int size, char type, Cell board[10][10], int player) {
	int x = 0;
	int y = 0;

	//loop for to place the number of segments outlined by int size
	for (int i = size; i > 0; i--) {
		//coordinate prompt
		printf("PLAYER %d SETUP\n\n", player);
		printBoard(board, player, 0);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		printf("Currently placing ship type: %c", type);
		printf("\nPlease input your desired coordinates (%d coordinates left): ", i);

		//player input
		printf("\nx: ");
		scanf("%d", &x);
		printf("y: ");
		scanf("%d", &y);

		//invalid coordinate input loop
		while (checkPlacement(size, i, x, y, board, type) != 1) {
			printf("Invalid coordinates detected! Please try again: ");
			printf("\nx: ");
			scanf("%d", &x);
			printf("y: ");
			scanf("%d", &y);
		}

		//changes to Cell to reflect ship occupation
		board[y][x].symbol = type;
		board[y][x].type = type;
		board[y][x].alive = 1;

		system("cls");
	}
}

void manuallyPlaceShips(Cell board[10][10], int player) {
	place1Ship(5, 'c', board, player); //place carrier
	place1Ship(4, 'b', board, player); //place battleship
	place1Ship(3, 'r', board, player); //place cruiser
	place1Ship(3, 's', board, player); //place submarine
	place1Ship(2, 'd', board, player); //place destroyer
}


void printBoard(Cell board[10][10], int player, int turn) {
	//board labels
	printf("TURN %d\n\n", turn);
	printf("PLAYER %d'S BOARD\n", player);
	puts("  0 1 2 3 4 5 6 7 8 9 X");

	//iterate through entire board for each Cell
	for (int i = 0; i < 10; i++) {
		printf("%d", i);
		for (int k = 0; k < 10; k++) {
			//print each Cell symbol
			printf(" %c", board[i][k].symbol);
		}

		if (i == 9) printf("\nY");
			
		printf("\n");
	}
}

void randomlyPlace1Ship(Cell board[10][10], int size, int player, char type) {
	int x = 0;
	int y = 0;

	//loop for each segment of ship of inputted size
	for (int i = size; i > 0; i--) {

		do { //find a suitable coordinate (loop until one is found)
			x = rand() % 10;
			y = rand() % 10;
		} while (checkPlacement(size, i, x, y, board, type) != 1);

		//changes to Cell to reflect ship occupation
		board[y][x].symbol = type;
		board[y][x].type = type;
		board[y][x].alive = 1;
	}
}

void randomlyPlaceShips(Cell board[10][10], int player) {
	randomlyPlace1Ship(board, 5, player, 'c'); //place carrier
	randomlyPlace1Ship(board, 4, player, 'b'); //place battleship
	randomlyPlace1Ship(board, 3, player, 'r'); //place cruiser
	randomlyPlace1Ship(board, 3, player, 's'); //place submarine
	randomlyPlace1Ship(board, 2, player, 'd'); //place destroyer
}

int checkHit(int x, int y, Cell board[10][10]) { //checks hit for a specified location in board
	//if Cell at location is occupied by ship segment, return 1 for hit
	if (board[y][x].type != 'w') return 1;

	else return 0; //otherwise return 0 for miss;
}

void playerTurn(int player, int playerboard, Cell board[10][10], Stats *stats, FILE* outputfile, int sunkships[5], int turn) {
	int xInput = 0;
	int yInput = 0;
	//print board for user to see
	printBoard(board, playerboard, turn);

	//prompt and scan for user coordinate input
	puts("\n------------------------------------------------------------------------------------------------------------------------");
	printf("TARGET SELECTION: PLAYER %d TURN", player);
	printf("\nInput x value: ");
	scanf("%d", &xInput);
	printf("Input y value: ");
	scanf("%d", &yInput);

	//invalid shot location loop
	while (validShot(yInput, xInput) != 1) {
		puts("\nInvalid input detected! Try again.");
		printf("Input x value: ");
		scanf("%d", &xInput);
		printf("Input y value: ");
		scanf("%d", &yInput);
	}

	system("cls");
	//checks if the shot was a hit or miss
	if (checkHit(xInput, yInput, board) == 1) { //hit

		//updates Cell to reflect a hit
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = 'x';

		//add a miss to player's stats struct
		stats->hits++;

		//prints to let player know they got a hit
		printBoard(board, playerboard, turn);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		puts("Direct hit!");

		//writes hit to output file
		fprintf(outputfile, "%d. PLAYER %d GUESS: [%d, %d] is a hit\n", turn, player, xInput, yInput);

		//updates sunkships array
		checkSunkShips(board, xInput, yInput, sunkships, outputfile);

		system("pause");
	}

	else { //miss

		//updates Cell to reflect a miss at the location
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = ' ';

		//add a miss to player's stats struct
		stats->misses++;

		//prints to let player know they missed
		printBoard(board, playerboard, turn);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		puts("Miss!");

		//writes miss to output file
		fprintf(outputfile, "%d. PLAYER %d GUESS: [%d, %d] is a miss\n", turn, player, xInput, yInput);

		system("pause");
	}

	system("cls");
}

void computerTurn(Cell board[10][10], Stats *stats, FILE* outputfile, int sunkships[5], int player, int playerboard, int turn) {
	int xInput = 0;
	int yInput = 0;

	do { //loop until a location that has not been shot at by the player is reached
		xInput = rand() % 10;
		yInput = rand() % 10;
	} while (board[yInput][xInput].hit == 1);

	system("cls");

	//checks if the shot was a hit or miss
	if (checkHit(xInput, yInput, board) == 1) { //hit

		//updates Cell to reflect a hit at the location
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = 'x';
		board[yInput][xInput].hit = 1;

		//add a hit to computer player's stats struct
		stats->hits++;
		
		//prints to let player know the computer got a hit
		printBoard(board, playerboard, turn);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		printf("COMPUTER %d GUESS: [%d, %d]", player, xInput, yInput);
		puts("\nDirect hit!");

		//writes hit to output file
		fprintf(outputfile, "%d. COMPUTER %d GUESS: [%d, %d] is a hit\n", turn, player, xInput, yInput);

		//updates sunkships array
		checkSunkShips(board, xInput, yInput, sunkships, outputfile);

		system("pause");
	}

	else { //miss

		//updates Cell to reflect a miss at the location
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = ' ';
		board[yInput][xInput].hit = 1;

		//add a miss to computer player's stats struct
		stats->misses++;

		//prints to let player know the computer missed
		printBoard(board, playerboard, turn);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		printf("COMPUTER %d GUESS: [%d, %d]", player, xInput, yInput);
		puts("\nMiss!");

		//writes miss to output file
		fprintf(outputfile, "%d. COMPUTER %d GUESS: [%d, %d] is a miss\n", turn, player, xInput, yInput);

		system("pause");
	}

	system("cls");

}

int checkWin(Cell board[10][10]) {
	//loop through every Cell in the array
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			if (board[i][k].alive == 1) return 0; //if any Cell contains a ship segment that has not been hit, return 0 for no win

		}
	}

	return 1; //otherwise return 1 for win achieved
}

int getSetupChoice(int player) {
	int input = 0;

	//display options
	printf("**** PLAYER %d SETUP ****", player);
	puts("--------------");
	puts("[1] manual setup");
	puts("[2] randomized setup");
	
	//prompt and take user input
	printf("\nPlease input an option: ");
	scanf("%d", &input);

	//invalid input check
	while (input < 1 || input > 2) {
		system("cls");
		displayMenu();

		printf("\nInvalid input detected! Please try again: ");
		scanf("%d", &input);
	}

	return input;
}

int getPlayerChoice(int player) {
	int input = 0;

	//display options
	printf("**** PLAYER %d ****", player);
	puts("--------------");
	puts("[1] human");
	puts("[2] computer");

	//prompt and take user input
	printf("\nPlease input an option: ");
	scanf("%d", &input);

	//invalid input check
	while (input < 1 || input > 2) {
		system("cls");
		displayMenu();

		printf("\nInvalid input detected! Please try again: ");
		scanf("%d", &input);
	}

	return input;
}

void writeStats(int player, Stats stats, FILE *outputfile) {
	//prints statistics to console
	printf("\nPlayer %d", player);
	printf("\nHits: %d", stats.hits);
	printf("\nMisses: %d", stats.misses);
	printf("\nShots: %d", stats.shots);
	printf("\nAccuracy: %lf", stats.accuracy);
	printf("\nWins: %d\n", stats.wins);

	//writes statistics to output file
	fprintf(outputfile, "\nPlayer %d", player);
	fprintf(outputfile, "\nHits: %d", stats.hits);
	fprintf(outputfile, "\nMisses: %d", stats.misses);
	fprintf(outputfile, "\nShots: %d", stats.shots);
	fprintf(outputfile, "\nAccuracy: %lf", stats.accuracy);
	fprintf(outputfile, "\nWins: %d\n", stats.wins);
}

//updates sunkships after each hit
void checkSunkShips(Cell board[10][10], int x, int y, int sunkships[5], FILE* outfile) {
	//decrements # of living ship segments each time one is hit
	switch (board[y][x].type) {
	case 'c': sunkships[0]--; break; //carrier seg is hit
	case 'b': sunkships[1]--; break; //battleship seg is hit
	case 'r': sunkships[2]--; break; //cruiser seg is hit
	case 's': sunkships[3]--; break; //submarine seg is hit
	case 'd': sunkships[4]--; break; // destroyer seg is hit
	}
	if (sunkships[0] == 0) { //no carrier segments that have not been hit remain
		printf("You sunk the Carrier!\n\n");
		fprintf(outfile, "Sunk the Carrier.\n");
		sunkships[0]--;
	}
	if (sunkships[1] == 0) { //no battleship segments that have not been hit remain
		printf("You sunk the Battleship!\n\n");
		fprintf(outfile, "Sunk the Battleship.\n");
		sunkships[1]--;
	}
	if (sunkships[2] == 0) { //no cruiser segments that have not been hit remain
		printf("You sunk the Cruiser!\n\n");
		fprintf(outfile, "Sunk the Cruiser.\n");
		sunkships[2]--;
	}
	if (sunkships[3] == 0) { //no submarine segments that have not been hit remain
		printf("You sunk the Submarine!\n\n");
		fprintf(outfile, "Sunk the Submarine.\n");
		sunkships[3]--;
	}
	if (sunkships[4] == 0) {  //no destroyer segments that have not been hit remain
		printf("You sunk the Destroyer!\n\n");
		fprintf(outfile, "Sunk the Destroyer.\n");
		sunkships[4]--;
	}
}

int validShot(int y, int x) {
	if (y <= 9 && y >= 0) { //if y is between 0 and 9
		if (x <= 9 && x >= 0) // if x is between 0 and 9
			return 1; //is valid
	}
	return 0; //else is not valid
}

int startFirst(void) {
	//return random number (1 or 2)
	int first = 0;
	first = rand() % 2 + 1;
	return first;
}