#include "header.h"

int main(void) {
	srand(time(NULL));
	int menuChoice = 0;
	int setup = 0;
	FILE* outputfile = fopen("output.txt", "w");

	Stats player1stats = { 0, 0, 0, 0.0, 0 };
	Stats player2stats = { 0, 0, 0, 0.0, 0 };

	do {
		menuChoice = getMenuInput();

		system("cls");

		if (menuChoice == 1) displayRules(); //display rules

		else if (menuChoice == 2) { //start new game
			int playing = 1;
			int winner = 0;
			int player1identity = 0;
			int player2identity = 0;

			//board initialization
			Cell board1[10][10], board2[10][10];

			//fill empty board
			fillBoard(board1);
			fillBoard(board2);

			//get player identities (CPU or human)
			player1identity = getPlayerChoice(1);

			system("cls");

			player2identity = getPlayerChoice(2);

			system("cls");

			//set up ships
			if (player1identity == 1) {
				setup = getSetupChoice(1);
				if (setup == 1) manuallyPlaceShips(board1, 1);
				else randomlyPlaceShips(board1, 1);
			}
			else randomlyPlaceShips(board1, 1);

			system("cls");

			if (player2identity == 1) {
				setup = getSetupChoice(2);
				if (setup == 1) manuallyPlaceShips(board2, 2);
				else randomlyPlaceShips(board2, 2);
			}
			else randomlyPlaceShips(board2, 2);
			
			system("cls");

			//hide boards
			obscureBoard(board1);
			obscureBoard(board2);
			
			//play loop
			while (playing == 1) {
				//player 1 turn
				if (player1identity == 1) playerTurn(1, board2, &player1stats);
				else computerTurn(board2, &player1stats);
				player1stats.shots++;

				//check if player 1 won
				if (checkWin(board2) == 1) {
					winner = 1;
					player1stats.wins++;
					break;
				}

				//player 2 turn
				if (player2identity == 1) playerTurn(2, board1, &player2stats);
				else computerTurn(board1, &player2stats);
				player2stats.shots++;

				//check if player 2 won
				if (checkWin(board1) == 1) {
					winner = 2;
					player2stats.wins++;
					break;
				}
			}

			printf("Player %d wins\n\n", winner);
			system("pause");

		}

		system("cls");

	} while (menuChoice != 3);

	player1stats.accuracy = (double)(player1stats.hits) / (double)(player1stats.shots);
	player2stats.accuracy = (double)(player2stats.hits) / (double)(player2stats.shots);

	fprintf(outputfile, "Player 1");
	fprintf(outputfile, "\nHits: %d", player1stats.hits);
	fprintf(outputfile, "\nMisses: %d", player1stats.misses);
	fprintf(outputfile, "\nShots: %d", player1stats.shots);
	fprintf(outputfile, "\nAccuracy: %lf", player1stats.accuracy);
	fprintf(outputfile, "\nWins: %d", player1stats.wins);

	fprintf(outputfile, "\n\nPlayer 2");
	fprintf(outputfile, "\nHits: %d", player2stats.hits);
	fprintf(outputfile, "\nMisses: %d", player2stats.misses);
	fprintf(outputfile, "\nShots: %d", player2stats.shots);
	fprintf(outputfile, "\nAccuracy: %lf", player2stats.accuracy);
	fprintf(outputfile, "\nWins: %d", player2stats.wins);

	fclose(outputfile);
	puts("Goodbye!");
	
	return 0;
}