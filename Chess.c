#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "AI.h"
#include "Chess.h"
#include "DataTypes.h"
#include "AI.h"
#include "GUI.h"
#include "ChessLog.h"

void PrintMenu();
void PrintTrojan();

int MAX = 300;
int arraynum = 0;

int main(){
	int User = 0;
	int GameAI = 0;
	int mainmenu = 0;
	int invalidprompt = 0;
	char input[2];
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int difficulty = 1;
	int check = 0;
	int PvP, AvA, PvA;
	int turn;
	t_board InitialBoard;
	BLIST *boardlist;
	t_castle Castle = {0,0,0,0,0,0};
	int Black, White; /* 1 = human player 0 = AI player */
	int end = 0;
	int i;
	int *wN = malloc(sizeof(int));
	int *wB = malloc(sizeof(int));
	int *bN = malloc(sizeof(int));
	int *bB = malloc(sizeof(int));
	int piece[MAX], capture[MAX], special[MAX];
	char START[MAX][3], END[MAX][3];
	
	*wN = 0;
	*wB = 0;
	*bN = 0;
	*bB = 0;
	while (choice1 != 4){
		system("clear");
		PrintTrojan();
		
		if(invalidprompt == 1)
		{
			invalidprompt = 0;
			printf("Invalid selection");
		}
		else if(GameAI == 1)
		{
			GameAI = 0;
			printf("Please Select a Game mode fire before starting the game");
		}
		
		PrintMenu();
		printf("Select an option: ");
		scanf("%s", input);
		getchar();
		/* choice1: Game Menu Check */
		if(input[0] > 52 || input [0] < 49)
		{
			invalidprompt = 1;
			continue;/* go back to game menu */
		}
		else
		{
			choice1 = input[0] - 48;
			memset(input, 0, 2);
		}/* end input check:Game menu */
		
		while (choice1 != 3){ /* while loop to take in all menu choice before starting the game */
		
			switch (choice1){
				case 1:
					while (choice2 != 1 && choice2 != 2 && choice2 != 3){
					
						system("clear");
						PrintTrojan();
						if(invalidprompt == 1)
						{
							invalidprompt = 0;
							printf("Invalid Selection");
						}
						
						printf("\n<========================>{ Game Mode }<==========================>\n"); 
						printf("                       1: Player vs Player                         \n");
						printf("                       2: Player vs AI                             \n");
						printf("                       3: AI vs AI                                 \n");
						
						printf("Choose a game mode: ");
						scanf("%s", input);
						/*choice2:Game mode check*/
						if(input[0] > 51 || input[0] < 49)
						{
							invalidprompt = 1;
							continue;/*go back to game mode menu*/
						}
						else
						{
							choice2 = input[0] - 48;
							memset(input, 0, 2);
						}/*end input check:Game mode*/
						
						if (choice2 == 1){
							PvP = 1;
							PvA = 0;
							AvA = 0;
							White = 1;
							Black = 1;
						}	
					
						else if (choice2 == 2){
							while (choice3 != 1 && choice3 != 2){
								system("clear");
								PrintTrojan();
								if(invalidprompt == 1)
								{
									invalidprompt = 0;
									printf("Invalid Selection");
								}
								printf("\n<=========================>{ AI Piece }<==========================>\n");
								printf("                             1: White                              \n");
								printf("                             2: Black                              \n");
								
								printf("What color is the AI: ");
								scanf("%s", input);
								/*choice3:AI color check*/
								if(input[0] > 50 || input[0] < 49){
									invalidprompt = 1;
									continue;/*go back to AI Piece menu*/
								}
								else{
									choice3 = input[0] - 48;
									memset(input, 0, 2);
								}/*end input check:AI color*/
					
								if (choice3 == 1){
									White = 0;
									Black = 1;
								}
							
								else if(choice3 == 2){
									White = 1;
									Black = 0;
								}
							}
							PvP = 0;
							PvA = 1;
							AvA = 0;
						}
					
						else if (choice2 == 3){
							PvP = 0;
							PvA = 0;
							AvA = 1;
							White = 0;
							Black = 0;
						}				
					}
					choice2 = 0;
					choice3 = 0;
					break;
				case 2:
					while (choice2 != 1 && choice2 != 2 && choice2 != 3 && choice2 != 4){
						system("clear");
						PrintTrojan();
						if(invalidprompt == 1){
							invalidprompt = 0;
							printf("Invalid Selection");
						}
					        printf("\n<=======================>{ AI Difficulty }<=======================>\n");
						printf(" 			    1: Easy                                 \n");
						printf("     			    2: Medium                               \n");
						printf("                            3: Hard                                 \n");
						printf("                            4: Master                               \n");
						
						printf("Please make a selection: ");
						scanf("%s", input);
						/*choice2:AI difficulty check*/
						if(input[0] > 52 || input[0] < 49){
							invalidprompt = 1;
							continue;/*go back to AI difficulty menu*/
						}
						else{
							choice2 = input[0] - 48;
							memset(input, 0, 2);
						}/*end input check:AI difficulty*/
					
						if (choice2 == 1)
							difficulty = 1;
						
						else if (choice2 == 2)
							difficulty = 2;
						
						else if (choice2 == 3)
							difficulty = 3;
						else if (choice2 == 4)
							difficulty = 4;
					}
					choice2 = 0;
					break;
				case 4:
					return 0;
				default:
					break;
			
			}/*end switch: choice1*/
			mainmenu = 1;
			break;
		}/*end while: choice1 != 3*/
		
		/*return to menu*/
		if(mainmenu == 1){
			mainmenu = 0;
			continue;
		}

		for(i = 0; i < 120; i++){
			InitialBoard[i] = 42;
		}

		InitialBoard[21] = -4;		/* Row 1...Rank 8 */
		InitialBoard[22] = -2;
		InitialBoard[23] = -3;
		InitialBoard[24] = -5;
		InitialBoard[25] = -6;
		InitialBoard[26] = -3;
		InitialBoard[27] = -2;
		InitialBoard[28] = -4;

		for(i = 31; i < 39; i++){	/* Row 2...Rank 7 */
			InitialBoard[i] = -1;
		}

		for(i = 41; i < 79; i++){	/* Row 3 thru 6...Ranks 6 thru 3 */
			InitialBoard[i] = 0;
		}

		InitialBoard[49] = 42;
		InitialBoard[50] = 42;
		InitialBoard[59] = 42;
		InitialBoard[60] = 42;
		InitialBoard[69] = 42;
		InitialBoard[70] = 42;
		InitialBoard[79] = 42;
		InitialBoard[80] = 42;

		for(i = 81; i < 89; i++){	/* Row 7...Rank 2 */
			InitialBoard[i] = 1;
		}

		InitialBoard[91] = 4;		/* Row 8...Rank 1 */
		InitialBoard[92] = 2;
		InitialBoard[93] = 3;
		InitialBoard[94] = 5;
		InitialBoard[95] = 6;
		InitialBoard[96] = 3;
		InitialBoard[97] = 2;
		InitialBoard[98] = 4;
	
		boardlist = NewBoardList();
		AddBoard(&InitialBoard, boardlist);
		system("clear");
		printboard(InitialBoard);
		
			if (PvP == 1){
				while (end != 1){
					turn = 1;
					User = UserTurn(InitialBoard, Castle, turn,piece, START,END, capture, special, wN, wB, bN, bB);
					arraynum++;
					if(User == 1){
						arraynum = 0;
						WriteLog(piece,START, END, capture,special);
						break;
					}
					system("clear");
					printboard(InitialBoard);
					turn = 0;
					check = CheckCheck(InitialBoard, turn, Castle);
					if (check == 1)
					{
						check = CheckCheckmate(InitialBoard, turn, Castle);
					}
					if (check == 2)
					{
						WriteLog(piece,START, END, capture,special);
						arraynum = 0;
						printf("White Wins!\n");
						printf("Press Enter to Continue");
						while(getchar() != '\n');
						end = 1;
					}
					if(end !=1){
						turn = 0;
						User = UserTurn(InitialBoard, Castle, turn,piece, START, END, capture, special, wN, wB, bN, bB);
						arraynum++;
						if(User == 1){
							WriteLog(piece,START, END, capture,special);
							arraynum = 0;
							break;
						}
						system("clear");
						printboard(InitialBoard);
						check = 0;
						turn = 1;
						check = CheckCheck(InitialBoard, turn,Castle);
						if(check == 1)
						{
							check = CheckCheckmate(InitialBoard, turn,Castle);
						}
						if(check == 2)
						{
							WriteLog(piece,START, END, capture,special);
							arraynum = 0;
							printf("Black Wins!\n");
							printf("Press Enter to Continue");
							while(getchar() != '\n');
							end = 1;
						}
					}
					else
						break;
				}
			}
			else if (PvA == 1 && White == 1){
				while (end != 1){
					turn = 1;
					User = UserTurn(InitialBoard, Castle, turn,piece, START,END, capture, special, wN, wB, bN, bB);
					arraynum++;
					if(User == 1){
						WriteLog(piece,START, END, capture,special);
						arraynum = 0;
						break;
					}
					system("clear");
					printboard(InitialBoard);
					turn = 0;
					check = CheckCheck(InitialBoard, turn, Castle);
					if (check == 1)
					{
						check = CheckCheckmate(InitialBoard, turn, Castle);
					}
					if (check == 2)
					{
						WriteLog(piece,START, END, capture,special);
						arraynum = 0;
						printf("White Wins!\n");
						printf("Press Enter to Continue");
						while(getchar() != '\n');
						end = 1;
					}
					if(end != 1){
						turn = 0;
						AITurn(InitialBoard, Castle, turn, difficulty, piece, START,END, capture, special, wN, wB, bN, bB);
						arraynum++;
						system("clear");
						printboard(InitialBoard);
						check = 0;
						turn = 1;
						check = CheckCheck(InitialBoard, turn,Castle);
						if(check == 1)
						{
							check = CheckCheckmate(InitialBoard, turn,Castle);
						}
						if(check == 2)
						{
							WriteLog(piece,START, END, capture,special);
							arraynum = 0;
							printf("Black Wins!\n");
							printf("Press Enter to Continue");
							while(getchar() != '\n');
							end = 1;
						}
					}	
					else
						break;
				}
			}
			else if (PvA == 1 && White == 0){
				while (end != 1){
					turn = 1;
					AITurn(InitialBoard,Castle, turn, difficulty,piece, START, END, capture, special, wN, wB, bN, bB);
					arraynum++;
					system("clear");
					printboard(InitialBoard);
					turn = 0;
					check = CheckCheck(InitialBoard, turn,Castle);
					if (check == 1)
					{
						check = CheckCheckmate(InitialBoard, turn, Castle);
					}
					if (check == 2)
					{
						WriteLog(piece,START, END, capture,special);
						arraynum = 0;
						printf("White Wins!\n");
						printf("Press Enter to Continue");
						while(getchar() != '\n');
						end = 1;
					}
					if(end != 1){
						turn = 0;
						User = UserTurn(InitialBoard, Castle, turn, piece, START,END, capture, special, wN, wB, bN, bB);
						arraynum++;
						if(User == 1){
							arraynum = 0;
							break;
						}
						system("clear");
						printboard(InitialBoard);
						check = 0;
						turn = 1;
						check = CheckCheck(InitialBoard, turn,Castle);
						if(check == 1)
						{
							check = CheckCheckmate(InitialBoard, turn, Castle);
						}
						if(check == 2)
						{
							WriteLog(piece,START, END, capture,special);
							arraynum = 0;
							printf("Black Wins!\n");
							printf("Press Enter to Continue");
							while(getchar() != '\n');
							end = 1;
						}
					}
					else 
						break;
				}
			}
			else if (AvA == 1){
				while (end != 1){
					turn = 1;
					AITurn(InitialBoard,Castle, turn, difficulty,piece, START,END, capture, special, wN, wB, bN, bB);
					arraynum++;
					system("clear");
					printboard(InitialBoard);
					turn = 0;
					check = CheckCheck(InitialBoard, turn,Castle);
					if (check == 1)
					{
						check = CheckCheckmate(InitialBoard, turn, Castle);
					}
					if (check == 2)
					{
						WriteLog(piece,START, END, capture,special);
						arraynum = 0;
						printf("White Wins!\n");
						printf("Press Enter to Continue");
						while(getchar() != '\n');
						end = 1;
					}
					if(end != 1){
						turn = 0;
						AITurn(InitialBoard,Castle,  turn, difficulty,piece, START,END, capture, special, wN, wB, bN, bB);
						arraynum++;
						system("clear");
						printboard(InitialBoard);
						turn = 1;

						check = CheckCheck(InitialBoard, turn,Castle);
						if(check == 1)
						{
							check = CheckCheckmate(InitialBoard, turn, Castle);
						}
						if(check == 2)
						{
							WriteLog(piece,START, END, capture,special);
							arraynum = 0;
							printf("Black Wins!\n");
							printf("Press Enter to Continue");
							while(getchar() != '\n');
							end = 1;
						}
					}
					else
						break;
				}
			}
			else{
				GameAI = 1;
			}
			end  = 0;
			check = 0;
	}
system("clear");
return 0;
}
	
	

	
void PrintMenu(){ /* Need to expand this menu probably */

	printf("\n<======================>{ Chess Game Menu }<======================>\n");
        printf("                      1: Select Game Mode                          \n");
        printf("                      2: Choose AI Difficulty                      \n");
        printf("                      3: Start Game                                \n");
        printf("                      4: Quit    								   \n");              
}


void AddBoard(t_board *board, BLIST *list){

	BENTRY *newentry;
	
	assert(list);
	
	newentry = (BENTRY *)malloc (sizeof(BENTRY));
	
	if (newentry != NULL){
		newentry->List = list;
		newentry->Next = NULL;
		newentry->Prev = NULL;
		newentry->Turn = list->Turn;
		newentry->Boards = board;
		
		if(!list->First){
			list->First = list->Last = newentry;
		}
		
		else{
			list->Last->Next = newentry;
			newentry->Prev = list->Last;
			list->Last = newentry;
		}
		list->Turn ++;
	}
		
}


void DeleteBoard(BLIST *list){
	
	BENTRY *entry;
	
	assert(list);
	assert(list->Last);
	
	entry = list->Last;
	list->Last = entry->Prev;
	DeleteBoard(entry->List); /* need to adjust this code */
	entry->Boards = NULL;
	free(entry);
	list->Turn --;
	
}


BLIST *NewBoardList(void){

	BLIST *newlist = (BLIST *)malloc(sizeof(BLIST));
	
	if (newlist != NULL)
	{
		newlist->First = NULL;
		newlist->Last = NULL;
		newlist->Turn = 0;
	}
	
	return newlist;
}


void DeleteBoardList(BLIST *list){

	BENTRY *curr, *succ;
	
	assert(list);
	curr = list->First;
	while(curr){
		succ = curr->Next;
		assert(curr->List == list);
		DeleteBoard(curr->List); /* need to adjust this code */
		curr->Boards = NULL;
		free(curr);
		curr = succ;
	}
	
	free (list);
}



/* Defines the data and function flow for a human player's turn */
/*returns 1 to return to main menu or 0 to conitnue game in control flow, 2 prompt undo*/
int UserTurn(t_board Board,t_castle Castle, int turn, int piece[], char start[][3], char end[][3], int capture[], int special[], int* Wn, int* Wb, int* Bn, int* Bb)
{
	char start_rank, end_rank; 
	t_move Move;
	char start_file, end_file;
	int validInput = 0;
	int decision;
	int i;
	char input[2];	
	char startin[5], endin[3];
	static int chessboard1 = 0, chessboard2 = 0, legal = 0;

	/*user choice menu prompt*/
	while(1){
		/*prompt board again within players move*/
		if(chessboard1 != 0){
			system("clear");
			printboard(Board);
		}
		
		chessboard1++;
/*		if(undoprompt == 2){
			undoprompt = 0;
			printf("Cannot Undo");
		}
*/		if(validInput == 1){
			validInput = 0;
			printf("Invalid Selection");
		}
		printf("\n<=========================>{ Your Move }<========================>\n");
		printf("                          1. Make Move                            \n");
		printf("                          2. Undo Move                            \n");
		printf("                          3. Quit to Menu                         \n");
		
		printf("Make your decision: ");
		scanf("%s", input);
		/*decision check*/
		if(input[0] > 51 || input[0] < 49){
			validInput = 1;
			/*printf("Invalid Selection\n\n");*/
			continue;/*go back to move menu*/
		}
		else{
			decision = input[0] - 48;
			memset(input, 0, 2);
		}/*end input check:AI difficulty*/
		
		/*make a move*/
		if(decision == 1){
			/*for file and rank check*/
			while(1){
				system("clear");
				printboard(Board);
				if(chessboard2 != 0){
					if(legal == 1){
						printf("\nThat move is not legal");
						legal = 0;
					}
					else if(validInput != 0){
						
						validInput = 0;
						printf("\nInvalid entry. Make sure File is a letter and Rank is a number");
					}
				}
				
				chessboard2++;
				printf("\nEnter Move(FileRank NewFileRank): "); 
				scanf("%s", startin);
				if(startin[3]){
					start_file = startin[0];
					start_rank = startin[1];
					end_file = startin[2];
					end_rank = startin[3]; 
					/*logfile*/
					start[arraynum][0] = startin[0];
					start[arraynum][1] = startin[1];
					end[arraynum][0] = startin[2];
					end[arraynum][1] = startin[3];
				}
				else{
					scanf("%s", endin);
					start_file = startin[0]; 
					start_rank = startin[1];
					end_file = endin[0];
					end_rank = endin[1];
					/*logfile*/
					start[arraynum][0] = startin[0];
					start[arraynum][1] = startin[1];
					end[arraynum][0] = endin[0];
					end[arraynum][1] = endin[1];
				}
				getchar();
				
				/* clear start and end */
				memset(startin, 0, 5);
				memset(endin, 0, 3);
				
				/*FILE CHECKS => allows capital or lowercase*/
				/*check start file*/
				if(start_file >= 65 && start_file <= 72)
					start_file -= 64;
				else if(start_file >= 97 && start_file <= 104)
					start_file -= 96;
				else{
					/*printf("\nInvalid start file.");*/
					validInput++;
				}/*end Start File check*/

				if(end_file >= 65 && end_file <= 72)
					end_file -= 64;
				else if(end_file >= 97 && end_file <= 104)
					end_file -= 96;
				else{
					/*printf("\nInvalid end file.");*/
					validInput++;
				}/*end End File check*/
	
				/*RANK CHECKS*/
				if(start_rank > 56 || start_rank < 49){
					/*printf("\nInvalid start rank");*/
					validInput++;
				}
				else{
					start_rank -= 48;
				}/*end start rank check*/

				if(end_rank > 56 || end_rank < 49){
					/*printf("\nInvalid end rank");*/
					validInput++;
				}
				else{
					end_rank -= 48;
				}/*end end rank check*/

				if(validInput != 0){
                                        continue;
                                }/*end if*/

                        	Move.Start.File = start_file;
                       		Move.Start.Rank = start_rank;
                        	Move.End.File = end_file;
                        	Move.End.Rank = end_rank;
                        	Move.PieceType = Board[two2one(Move.Start)];
				
				/*logfile*/
				piece[arraynum] = Move.PieceType;
				capture[arraynum] = Board[two2one(Move.End)];
				special[arraynum] = 0; /*no special move*/
					
                        	if (IsLegal(Board, Move, turn,Castle) == 1){
                        	        MovePiece(Board, Move,Castle);
					UpdateHasMoved(Board, Wn, Wb, Bn, Bb);
					if (turn == 1)
					{
						for ( i = 21; i < 29; i++)
						{	
							if(Board[i] == 1)
							{	
								PromotePiece(Board, i);
							}
						}
					}
					else
					{
						for ( i = 91; i < 99; i++)
						{
							if(Board[i] == -1)
							{
								PromotePiece(Board, i);
							}
						}
					}
					
                        	}
                        	else{
                        	       /* printf("That move is not legal!\n");*/ /* Raising illegal move flag */
					legal = 1;
					continue;
					/*UserTurn(Board, turn);*/ /*Loop through UserTurn until a legal move is made*/
                        	}
				break;
			}/*end while F/R CHECK*/ 
                        break;/*gets out of user decision menu*/
                }/*end if: make move*/

                else if (decision == 2){/*undo choice*/
/*           		undoprompt = UndoBoard(boardlist, Board);
			if(turn == 1)
				turn = 0;
			else
				turn = 1;
			printboard(Board);
			return undoprompt;
*/                }/*end else if: undo*/

                else if (decision == 3){/*Quit game choice*/
			chessboard1 = 0;
/*			DeleteBoardList(boardlist);
*/			return 1;
		}/*end else if:quit out of game*/
		
	}/*end while: user decision*/
	chessboard1 = 0;
	return 0;
}

/* Defines the data and function flow for a AI player's turn */
void AITurn(t_board Board, t_castle Castle, int turn, int difficulty, int piece[], char start[][3], char end[][3], int capture[], int special[], int* Wn, int* Wb, int* Bn, int* Bb)
{
	t_move Best = GetBestMove(Board,Castle, turn, difficulty, Wn, Wb, Bn, Bb);
	
	/*logfile*/
	piece[arraynum] = Best.PieceType;
	switch(Best.Start.File){
		case 1:
			start[arraynum][0] = 'a';
			break;
		case 2:
			start[arraynum][0] = 'b';
			break;
		case 3:
			start[arraynum][0] = 'c';
			break;
		case 4:
			start[arraynum][0] = 'd';
			break; 
		case 5:
			start[arraynum][0] = 'e';
			break; 
		case 6:
			start[arraynum][0] = 'f';
			break; 
		case 7:
			start[arraynum][0] = 'g';
			break; 
 		case 8:
			start[arraynum][0] = 'h';
			break; 
		default:
			start[arraynum][0] = 'X';
	}/*end switch: start file*/
	switch(Best.Start.Rank){                                                                                               
                case 1:                                                                                                        
                        start[arraynum][1] = '1';                                                                              
                        break;                                                                                                 
                case 2:                                                                                                        
                        start[arraynum][1] = '2';                                                                              
                        break;                                                                                                 
                case 3:                                                                                                        
                        start[arraynum][1] = '3';                                                                              
                        break;                                                                                                 
                case 4:                                                                                                        
                        start[arraynum][1] = '4';                                                                              
                        break;                                                                                                 
                case 5:                                                                                                        
                        start[arraynum][1] = '5';                                                                              
                        break;                                                                                                 
                case 6:                                                                                                        
                        start[arraynum][1] = '6';                                                                              
                        break;                                                                                                 
                case 7:                                                                                                        
                        start[arraynum][1] = '7';                                                                              
                        break;                                                                                                 
                case 8:                                                                                                        
                        start[arraynum][1] = '8';                                                                              
                        break;                                                                                                 
                default:
			start[arraynum][1] = 'X';                                                                                                       
        }/*end switch: start file*/
	switch(Best.End.File){                                                                                               
                case 1:                                                                                                        
                        end[arraynum][0] = 'a';                                                                              
                        break;                                                                                                 
                case 2:                                                                                                        
                        end[arraynum][0] = 'b';                                                                              
                        break;                                                                                                 
                case 3:                                                                                                        
                        end[arraynum][0] = 'c';                                                                              
                        break;                                                                                                 
                case 4:                                                                                                        
                        end[arraynum][0] = 'd';                                                                              
                        break;                                                                                                 
                case 5:                                                                                                        
                        end[arraynum][0] = 'e';                                                                              
                        break;                                                                                                 
                case 6:                                                                                                        
                        end[arraynum][0] = 'f';                                                                              
                        break;                                                                                                 
                case 7:                                                                                                        
                        end[arraynum][0] = 'g';                                                                              
                        break;                                                                                                 
                case 8:                                                                                                        
                        end[arraynum][0] = 'h';                                                                              
                        break;                                                                                                 
                default:
			end[arraynum][0] = 'X';                                                                                                       
        }/*end switch: start file*/                                                                                            
        switch(Best.End.Rank){                                                                                               
                case 1:                                                                                                        
                        end[arraynum][1] = '1';                                                                              
                        break;                                                                                                 
                case 2:                                                                                                        
                        end[arraynum][1] = '2';                                                                              
                        break;                                                                                                 
                case 3:                                                                                                        
                        end[arraynum][1] = '3';                                                                              
                        break;                                                                                                 
                case 4:                                                                                                        
                        end[arraynum][1] = '4';                                                                              
                        break;                                                                                                 
                case 5:                                                                                                        
                        end[arraynum][1] = '5';                                                                              
                        break;                                                                                                 
                case 6:                                                                                                        
                        end[arraynum][1] = '6';                                                                              
                        break;                                                                                                 
                case 7:                                                                                                        
                        end[arraynum][1] = '7';                                                                              
                        break;                                                                                                 
                case 8:                                                                                                        
                        end[arraynum][1] = '8';                                                                              
                        break;                                                                                                 
                default:
			end[arraynum][1] = 'X';                                                                                   
        }/*end switch: start file*/ 
	capture[arraynum] = Board[two2one(Best.End)];
	special[arraynum] = 0;/*for special move*/

/*	sleep(2);
*/	MovePiece(Board, Best,Castle);
	UpdateHasMoved(Board, Wn, Wb, Bn, Bb);

}
	
/*Prints out sexy menu header*/
void PrintTrojan()
{
printf("                     _________________________\n");
printf("                     -------------------------\n");
printf("                     |     Knights of the    |\n");
printf("                     |      Square Table     |\n");
printf("                     |        PRESENTS:      |\n");
printf("                     _________________________\n");
printf("                     -------------------------\n");
printf("                              ,....,          \n");
printf("                            ,::::::<          \n");
printf("                           ,::/^\\\"``.       \n");
printf("                          ,::/, `   e`.       \n");
printf("                         ,::; |        '.     \n");
printf("                         ,::|  \\___,-.  c)   \n");
printf("                         ;::|     \\   '-'    \n");
printf("                         ;::|      \\         \n");
printf("                         ;::|   _.=`\\        \n");
printf("                         `;:|.=` _.=`\\       \n");
printf("                           '|_.=`   __\\      \n");
printf("                           `\\_..==`` /       \n");
printf("                            .'.___.-'.        \n");
printf("                           /          \\      \n");
printf("                          ('--......--')      \n");
printf("                          /'--......--'\\     \n");
printf("                          `\"--......--\"`    \n");
printf("                     ________________________ \n");
printf("                     |       KNIGHT'S       | \n");
printf("                     |      CHESS GAME      | \n");
printf("                     |                      | \n");
printf("                     |       VER: 1.0       | \n");
printf("                     |______________________| \n\n\n");
}/*end print trojan*/

/*******************************/
/* HOW I AM SEEING THE BOARD */
/*		___________________________
 *  	8	| 21 22 23 24 25 26 27 28 |	BLACK
 *  	7	| 31 32 33 34 35 36 37 38 |
 *  R	6	| 41 42 43 44 45 46 47 48 |
 *  A	5	| 51 52 53 54 55 56 57 58 |
 *  N	4	| 61 62 63 64 65 66 67 68 |
 *  K	3	| 71 72 73 74 75 76 77 78 |
 *  	2	| 81 82 83 84 85 86 87 88 |
 *  	1	| 91 92 93 94 95 96 97 98 |	WHITE
 *  		__________________________
 *		  a  b  c  d  e  f  g  h
 *	          1  2  3  4  5  6  7  8
 *			   FILE
 *
 */
