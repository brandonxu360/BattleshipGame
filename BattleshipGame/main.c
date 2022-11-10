/*
* Name: Brandon Xu
* Project: Battleship Game
* File: main.c
* Date: 11/9/22
* Description: Main file for execution of the program. Contains the main game loops and overarching logic.
*/

#include "header.h"

int main(void) {
	//rand config
	srand(time(NULL));

	//variable initialization
	int menuChoice = 0;
	int setup = 0;

	//open output file and assign to pointer outputfile
	FILE* outputfile = fopen("output.txt", "w");

	//player Stats structs initializations
	Stats player1stats = { 0, 0, 0, 0.0, 0 };
	Stats player2stats = { 0, 0, 0, 0.0, 0 };

	do { //loop until the user quits (inputs 3 to menu prompt)

		//get user menu choice
		menuChoice = getMenuInput();

		system("cls");

		if (menuChoice == 1) displayRules(); //display rules

		else if (menuChoice == 2) { //start new game

			//varaible initializations
			int playing = 1;
			int turn = 1;
			int startingPlayer = 0;
			int winner = 0;
			int player1identity = 0;
			int player2identity = 0;

			//sunk ships array initializations (represents the # of unhit segments remaining for each ship)
			int p1sunkships[5] = { 5,4,3,3,2 }, p2sunkships[5] = { 5,4,3,3,2 };

			//board initializations
			Cell board1[10][10], board2[10][10];

			//fill empty boards
			fillBoard(board1);
			fillBoard(board2);

			//get player 1 identity (CPU or human)
			player1identity = getPlayerChoice(1);

			system("cls");

			//get player 2 identity (CPU or human)
			player2identity = getPlayerChoice(2);

			system("cls");

			//SET UP SHIPS FOR PLAYER 1
			if (player1identity == 1) { //if player 1 is a human
				
				setup = getSetupChoice(1); //get setup choice (manual or randomized)

				system("cls");

				if (setup == 1) manuallyPlaceShips(board1, 1, turn); //manual
				else randomlyPlaceShips(board1, 1, turn); //random
			}
			else randomlyPlaceShips(board1, 1, turn); //player 1 is computer (default to random)

			system("cls");

			//SET UP SHIPS FOR PLAYER 2
			if (player2identity == 1) { //if player 1 is a human

				setup = getSetupChoice(2); //get setup choice (manual or randomized)

				system("cls");

				if (setup == 1) manuallyPlaceShips(board2, 2, turn);//manual
				else randomlyPlaceShips(board2, 2, turn); //random
			}
			else randomlyPlaceShips(board2, 2, turn); //player 2 is computer (default to random)
			
			system("cls");

			//hide boards (every Cell symbol represents water - tilde)
			obscureBoard(board1);
			obscureBoard(board2);
			
			//selects random player to start first
			puts("RANDOMIZED STARTING PLAYER");
			startingPlayer = startFirst(); //1 or 2
			printf("Player %d will start first!\n\n", startingPlayer);
			system("pause");

			//player 2 turn (if decieded that player 2 goes first)
			if (startingPlayer == 2) {
				if (player2identity == 1) playerTurn(2, 1, board1, &player2stats, outputfile, p1sunkships, turn);
				else computerTurn(board1, &player2stats, outputfile, p1sunkships, 2, 1, turn);
				player2stats.shots++;
			}

			//play loop
			while (playing == 1) {
				//player 1 turn
				if (player1identity == 1) playerTurn(1, 2, board2, &player1stats, outputfile, p2sunkships, turn); //if player 1 is a human
				else computerTurn(board2, &player1stats, outputfile, p2sunkships, 1, 2, turn); //if player 1 is a computer
				player1stats.shots++; 

				//check if player 1 won
				if (checkWin(board2) == 1) {
					winner = 1;
					player1stats.wins++; //assign win to player 1 Stat struct
					break; //break if win is achieved
				}

				if (startingPlayer == 2) turn++; // if player 2 starts, increment turn # after player 1 goes

				//player 2 turn
				if (player2identity == 1) playerTurn(2, 1, board1, &player2stats, outputfile, p1sunkships, turn); //if player 2 is a human
				else computerTurn(board1, &player2stats, outputfile, p1sunkships, 2, 1, turn); //if player 2 is a computer
				player2stats.shots++;

				//check if player 2 won
				if (checkWin(board1) == 1) {
					winner = 2;
					player2stats.wins++; //assign win to player 2 Stat struct
					break; //break if win is achieved
				}

				if (startingPlayer == 1) turn++; // if player 1 starts, increment turn # after player 2 goes
			}

			printf("Player %d wins on turn %d\n", winner, turn);
			
			//calculate accuracy and assign to player Stat structs
			player1stats.accuracy = (double)(player1stats.hits) / (double)(player1stats.shots);
			player2stats.accuracy = (double)(player2stats.hits) / (double)(player2stats.shots);

			//write stats to output file
			writeStats(1, player1stats, outputfile);
			writeStats(2, player2stats, outputfile);

			printf("\n");
			system("pause");


		}

		system("cls");

	} while (menuChoice != 3);

	//close output file
	fclose(outputfile);

	//goodbye message for quit
	puts("Goodbye!");
	
	return 0;
}