#include "header.h"

void putsCenter(char str[]) {
	//console width is 120 characters
	int width = 120;
	int length = sizeof(str) - 1;  //discount the terminal '\0'
	int pad = (length >= width) ? 0 : (width - length) / 2;

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

	displayMenu();

	//scanner
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
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			Cell cell = { i, k, 0, 0, 'w', '~'};
			board[i][k] = cell;
		}
	}
}

void obscureBoard(Cell board[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			board[i][k].symbol = '~';
		}
	}
}


int checkPlacement(int size, int count, int x, int y, Cell board[10][10], char type) {
	
	//OUT OF RANGE
	if (x > 9 || y > 9 || x < 0 || y < 0) return 0;

	//ALREADY OCCUPIED
	if (board[y][x].type != 'w') return 0;

	//NOT CONNECTED SHIP (excluding first coordinate for a ship)
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

	//SPACE FOR SHIP FOR DIRECTION COMMIT (2nd coordinate selection)
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

	//NOT STRAIGHT SHIP

	int shipPieceCount = 0;

	//vertical ship
	if (up == type || down == type) {
		int index = y;
		//spaces above
		while (index > -1 && (board[index][x].type == 'w' || board[index][x].type == type)) {
			if (board[index][x].type == type) shipPieceCount++;

			index--;
		}
		index = y;
		//spaces below
		while (index < 10 && (board[index][x].type == 'w' || board[index][x].type == type)) {
			if (board[index][x].type == type) shipPieceCount++;

			index++;
		}

	}

	//horizontal ship
	else if (right == type || left == type) {
		int index = x;
		//spaces left
		while (index > -1 && (board[y][index].type == 'w' || board[y][index].type == type)) {
			if (board[y][index].type == type) shipPieceCount++;

			index--;
		}
		index = x;
		//spaces right
		while (index < 10 && (board[y][index].type == 'w' || board[y][index].type == type)) {
			if (board[y][index].type == type) shipPieceCount++;

			index++;
		}

	} 
	if (count != size - shipPieceCount) return 0;

	//FIRST COORDINATE SURROUNDED
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

	for (int i = size; i > 0; i--) {
		printf("PLAYER %d SETUP\n\n", player);
		printBoard(board);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		printf("Currently placing ship type: %c", type);
		printf("\nPlease input your desired coordinates (%d coordinates left): ", i);
		printf("\nx: ");
		scanf("%d", &x);
		printf("y: ");
		scanf("%d", &y);

		while (checkPlacement(size, i, x, y, board, type) != 1) {
			printf("Invalid coordinates detected! Please try again: ");
			printf("\nx: ");
			scanf("%d", &x);
			printf("y: ");
			scanf("%d", &y);
		}

		board[y][x].symbol = type;
		board[y][x].type = type;
		board[y][x].alive = 1;

		system("cls");
	}
}

void manuallyPlaceShips(Cell board[10][10], int player) {
	place1Ship(5, 'c', board, player);
	place1Ship(4, 'b', board, player);
	place1Ship(3, 'r', board, player);
	place1Ship(3, 's', board, player);
	place1Ship(2, 'd', board, player);
}


void printBoard(Cell board[10][10]) {
	puts("  0 1 2 3 4 5 6 7 8 9 X");
	for (int i = 0; i < 10; i++) {
		printf("%d", i);
		for (int k = 0; k < 10; k++) {
			printf(" %c", board[i][k].symbol);
		}

		if (i == 9) printf("\nY");
			
		printf("\n");
	}
}

void randomlyPlace1Ship(Cell board[10][10], int size, int player, char type) {
	int x = 0;
	int y = 0;

	for (int i = size; i > 0; i--) {

		do { //find a suitable coordinate
			x = rand() % 10;
			y = rand() % 10;
		} while (checkPlacement(size, i, x, y, board, type) != 1);

		//assign ship to coordinate
		board[y][x].symbol = type;
		board[y][x].type = type;
		board[y][x].alive = 1;
	}
}

void randomlyPlaceShips(Cell board[10][10], int player) {
	randomlyPlace1Ship(board, 5, player, 'c');
	randomlyPlace1Ship(board, 4, player, 'b');
	randomlyPlace1Ship(board, 3, player, 'r');
	randomlyPlace1Ship(board, 3, player, 's');
	randomlyPlace1Ship(board, 2, player, 'd');
}

int checkHit(int x, int y, Cell board[10][10]) {
	if (board[y][x].type != 'w') return 1;

	else return 0;
}

void playerTurn(int player, Cell board[10][10], Stats *stats) {
	int xInput = 0;
	int yInput = 0;
	printBoard(board);
	puts("\n------------------------------------------------------------------------------------------------------------------------");
	printf("TARGET SELECTION: PLAYER %d TURN", player);
	printf("\nInput x value: ");
	scanf("%d", &xInput);
	printf("Input y value: ");
	scanf("%d", &yInput);

	system("cls");
	if (checkHit(xInput, yInput, board) == 1) {
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = 'x';
		stats->hits++;

		printBoard(board);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		puts("Direct hit!");
		system("pause");
	}

	else {
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = ' ';
		stats->misses++;

		printBoard(board);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		puts("Miss!");
		system("pause");
	}

	system("cls");
}

void computerTurn(Cell board[10][10], Stats *stats) {
	int xInput = 0;
	int yInput = 0;

	do {
		xInput = rand() % 10;
		yInput = rand() % 10;
	} while (board[yInput][xInput].hit == 1);

	system("cls");

	if (checkHit(xInput, yInput, board) == 1) {
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = 'x';
		board[yInput][xInput].hit = 1;
		stats->hits++;
		

		printBoard(board);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		printf("COMPUTER GUESS: [%d, %d]", xInput, yInput);
		puts("\nDirect hit!");
		system("pause");
	}

	else {
		board[yInput][xInput].alive = 0;
		board[yInput][xInput].symbol = ' ';
		board[yInput][xInput].hit = 1;
		stats->misses++;

		printBoard(board);
		puts("\n------------------------------------------------------------------------------------------------------------------------");
		printf("COMPUTER GUESS: [%d, %d]", xInput, yInput);
		puts("\nMiss!");
		system("pause");
	}

	system("cls");

}

int checkWin(Cell board[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			if (board[i][k].alive == 1) return 0;
		}
	}

	return 1;
}

int getSetupChoice(int player) {
	int input = 0;

	printf("**** PLAYER %d SETUP ****", player);
	puts("--------------");
	puts("[1] manual setup");
	puts("[2] randomized setup");

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

	printf("**** PLAYER %d ****", player);
	puts("--------------");
	puts("[1] human");
	puts("[2] computer");

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