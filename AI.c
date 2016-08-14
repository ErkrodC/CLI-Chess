#include "AI.h"
#include "DataTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GUI.h"

void CopyBoard(t_board Board1, t_board Board2){
	int i;

	for (i = 0; i < 120; i++){
		Board1[i] = Board2[i];
	}
}

double Max(double a, double b){
	if( a >= b)
		return a;
	else
		return b;
}

double Min(double a, double b){
	if (a <= b)
		return a;
	else
		return b;
}

/* Function to find the best moves */
t_move GetBestMove(t_board board,t_castle Castle, int turn, int depth, int* wn, int* wb, int* bn, int* bb){
	t_coin BestMove;
	t_node top;
	double alpha = -99999;
	double beta = 99999;
	int i;

	top.Depth = -1;
	for (i = 0; i < 120; i++)
		top.Board[i] = board[i];

	if (turn == 0){		/* Black's move */
		BestMove = MinValue(top,Castle, alpha, beta, depth, turn, wn, wb, bn, bb);
	}else{			/* White's move */
		BestMove = MaxValue(top,Castle, alpha, beta, depth, turn, wn, wb, bn, bb);
	}

	return BestMove.Move;
}

t_coin MaxValue(t_node state,t_castle Castle, double alpha, double beta, int depth, int turn, int* WN, int* WB, int* BN, int* BB){
	t_coin poss, MinC;
	t_move_array Moves;
	t_node child = state;
	double score;
	int MoveID = 0;
	double oldAlpha = alpha;
	int i;

	state.Depth++;
	child = state;
	Moves = ExhaustMoves(state.Board, turn, Castle);
	if (state.Depth == depth){
		poss.Value = EvaluatePosition(state.Board, Moves, turn, Castle);
		return poss;
	}


	for (i = 0; i < Moves.Length; i++){
		child = state;
		MovePiece(child.Board, Moves.Moves[i], Castle);
		MinC = MinValue(child,Castle, alpha, beta, depth, turn == 1 ? 0 : 1, WN, WB, BN, BB);
		score = MinC.Value;
		score += EvaluateMove(state.Board, Moves.Moves[i], WN, WB, BN, BB, turn);
		alpha = Max(alpha, score);
/*		if (state.Depth == 0)
			printf("Checking Move %d\n\tValue: %lf\n\tStart: Rank: %d File: %d | End: Rank: %d File: %d | PT: %d\n", i+1, score, Moves.Moves[i].Start.Rank, Moves.Moves[i].Start.File, Moves.Moves[i].End.Rank, Moves.Moves[i].End.File, Moves.Moves[i].PieceType);
*/		if (score > oldAlpha){
			MoveID = i;
			oldAlpha = score;
		}
		if (alpha >= beta){
			poss.Value = alpha;
			poss.Move = Moves.Moves[i];
			return poss;
		}
	}
	poss.Value = alpha;
/*	if (MoveID == -1)
		MoveID = MoveID.Length/2;
*/	poss.Move = Moves.Moves[MoveID];
/*	printf("maxv BestMove : RFS -> RFE: %d %d -> %d %d | D: %d | MID: %d\n", poss.Move.Start.Rank, poss.Move.Start.File, poss.Move.End.Rank, poss.Move.End.File, state.Depth, MoveID);
*/	return poss;
}		

t_coin MinValue(t_node state, t_castle Castle, double alpha, double beta, int depth, int turn, int* WN, int* WB, int* BN, int* BB){
	t_coin poss, MaxC;
	t_move_array Moves;
	t_node child = state;
	double score;
	int MoveID = 0;
	double oldBeta = beta;
	int i;

	state.Depth++;
	child = state;
	Moves = ExhaustMoves(state.Board, turn, Castle);
	if (state.Depth == depth){
		poss.Value = EvaluatePosition(state.Board, Moves, turn, Castle);
		return poss;
	}

	for (i = 0; i < Moves.Length; i++){
		child = state;
		MovePiece(child.Board, Moves.Moves[i], Castle);
		MaxC = MaxValue(child,Castle, alpha, beta, depth, turn == 0 ? 1 : 0, WN, WB, BN, BB);
		score = MaxC.Value;
		score += EvaluateMove(state.Board, Moves.Moves[i], WN, WB, BN, BB, turn);
		beta = Min(beta, score);
/*		if (state.Depth == 1)
			printf("Checking Move %d\n\tValue: %lf\n\tStart: Rank: %d File: %d | End: Rank: %d File: %d | PT: %d\n", i+1, score, Moves.Moves[i].Start.Rank, Moves.Moves[i].Start.File, Moves.Moves[i].End.Rank, Moves.Moves[i].End.File, Moves.Moves[i].PieceType);
*/		if (score < oldBeta){
			MoveID = i;
			oldBeta = score;
		}
		if (beta <= alpha){
			poss.Value = beta;
			poss.Move = Moves.Moves[i];
			return poss;
		}
	}
	poss.Value = beta;
/*	if (MoveID == -1)
		MoveID = Moves.Length/2;
*/	poss.Move = Moves.Moves[MoveID];
/*	printf(" MINV BestMove : RFS -> RFE: %d %d -> %d %d | D: %d | MID: %d\n", poss.Move.Start.Rank, poss.Move.Start.File, poss.Move.End.Rank, poss.Move.End.File, state.Depth, MoveID);
*/	return poss;
}

/* Checks if the move is legal and returns a certain value depending on the result */
int CheckLegalMove(t_board board);

/* MovePiece func */
void MovePiece(t_board Board, t_move Move, t_castle Castle){
	if(Move.PieceType == 6)
		Castle[0] = 1;
	else if (Move.PieceType == -6)
		Castle[1] = 1;
	else if (Move.PieceType == 4){
		if ((two2one(Move.Start)) == 91)
			Castle[4] = 1;
		else if ((two2one(Move.Start)) == 98)
			Castle[5] = 1; 
	}
	else if (Move.PieceType == -4){
		if ((two2one(Move.Start)) == 21)
			Castle[2] = 1;
		else if ((two2one(Move.Start)) == 28)
			Castle[3] = 1;
	}
	Board[two2one(Move.Start)] = 0;
	Board[two2one(Move.End)] = Move.PieceType;

}


/* PromotePiece Func */
void PromotePiece(t_board board, int promotingSquare){
	char input[2];
	int decision;
	int validInput;
	
	while (1)
	{
		if(validInput == 1)
		{
			validInput = 0;
			system("clear");
			printboard(board);
			printf("\nInvalid Selection!");
		}
	
		printf("\n<=========================>{ Promote Piece }<========================>\n");
		printf("                              1. Knight                               \n");
		printf("                              2. Bishop                               \n");
		printf("                              3. Rook                                 \n");	
		printf("                              4. Queen                                \n");	
		printf("Make your decision: ");
		scanf("%s", input);
		if(input[0] > 52 || input [0] < 49)
		{
			validInput = 1;
			continue;
		}
		else
		{
			decision = input[0] - 47;
			memset(input, 0, 2);
			if(promotingSquare > 90 && promotingSquare < 99)
			{
				decision = decision * (-1);
			}
			board[promotingSquare] = decision;
		}
		
		break;
	}
}


/* 1 to 2D conversion */
int two2one(t_square Square){ /* ranks and files are 1-8, not 0-7 */
	int one;

	one = ((10 - Square.Rank) * 10) + Square.File;

	return one;
}

/* 2 to 1D conversion */
t_square one2two(int oneDindex){
	t_square interSquare;

	interSquare.File = oneDindex % 10;
	interSquare.Rank = 9 - ((oneDindex - interSquare.File)/10 - 1);

	return interSquare;
}

t_move_array ExhaustMoves(t_board board, int turn, t_castle Castle){
	t_move_array All_Moves;		/* MUST REMEMBER TO FREE THIS LATER */
	int num_moves = 0;
	t_square start;
	t_move Move;
	int i, j;

	for (i = 21; i < 99; i++){
		if (board[i] != 42 && board[i] != 0){
			if (turn == 0){	 	/* Black's turn */
				if (board[i] < 0){
					start = one2two(i);
					Move.Start = start;
					Move.PieceType = board[i];
					switch(board[i]){
						case -6:
							Move.End.Rank = Move.Start.Rank + 1;
							Move.End.File = Move.Start.File - 1;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.Rank--;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File -= 2;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.Rank--;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							break;
						case -5:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case -4:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case -3:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case -2:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case -1:

							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
					}
				}
			}else{				/* White's turn */
				if (board[i] > 0){
					start = one2two(i);
					Move.Start = start;
					Move.PieceType = board[i];
					switch(board[i]){
						case 6:
							Move.End.Rank = Move.Start.Rank + 1;
							Move.End.File = Move.Start.File - 1;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.Rank--;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File -= 2;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.Rank--;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							Move.End.File++;
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}

						case 5:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case 4:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case 3:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case 2:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
						case 1:
							for (j = 21; j < 99; j++){
								Move.End = one2two(j);
								if (IsLegal(board, Move, turn, Castle) == 1){
									All_Moves.Moves[num_moves] = Move;
									num_moves++;
								}
							}
							break;
					}
				}
			}
		}
	}

	All_Moves.Length = num_moves;
	return All_Moves;
}
unsigned char GamePhase(t_board board, int turn){
	unsigned char phase = 0;
	unsigned char count = 0;
	
	switch(turn){
		case 0:
			count += 0 - board[22] - board[23] - board[26] - board[27];
			if (count <= 6)
				phase = 1;
			break;
		case 1:
			count += board[92] + board[93] + board[96] + board[97];
			if (count <= 6)
				phase = 1;
			break;
	}
	return phase;
}

double EvaluateMove(t_board board, t_move Move, int* whN, int* whB, int* blN, int* blB, int turn){
	double score = 0;

	if (GamePhase(board, turn) == 0){
		switch(Move.PieceType){
			case 2:
				if (*whN == 1 && Move.Start.Rank != 1)
					score -= 1.5;
				break;
			case -2:
				if (*blN == 1 && Move.Start.Rank != 8)
					score += 1.5;
				break;
			case 3:
				if (*whB == 1 && Move.Start.Rank != 1)
					score -= 1.5;
				break;
			case -3:
				if (*blB == 1 && Move.Start.Rank != 8)
					score += 1.5;
				break;
		}
	}
	return score;
}


/* func to give a value to every position */
double EvaluatePosition(t_board Board, t_move_array Moves, int turn, t_castle Castle){
	double score = 0;
	double open_game = 1 - ((.125 * PawnsOnBoard(Board))/2); /* Level of openness of the board */
	int i;
/*	turn = turn == 1 ? 0 : 1;
*/

	if (GamePhase(Board, turn) == 0)
		score += 5.0 * QueenIsOut(Board);
	turn = 0;
	score += 99999 * (turn == 1 ? 1 : -1) * (CheckCheckmate(Board, turn, Castle) == 2 ? 1 : 0);
	score += (5 * CenterControl(Board) + .125 * CenterControl2(Board, Moves, turn, Castle));
	score += 2 * BishopPair(Board);
	score += IsCastled(Board,turn);
	for (i = 21; i < 99; i++){
		if(i % 10 == 9){	/* This is used so that only legal squares are considered */
			i++;
			continue;
		}else{
/*			if (Board[i] == 6){
				score += IsCastled(Board, i);
			}else if (Board[i] == -6){
				score += IsCastled(Board, i);
			}

*/			switch(Board[i]){	/* Adding up to see which side has more pieces, both in terms of quantity and quality of those pieces */
				case -5: /* Black Queen */
					score -= 18;
					break;
				case -4: /* Black Rook */
					score -= 10 + open_game;
					break;
				case -3: /* Black Bishop */
					score -= 6 + open_game - 1.5* IsUndeveloped(Board, i);
					break;
				case -2: /* Black Knight */
					score -= 6 - open_game - (.5 * KnightOnRim(i)) - 1.5 * IsUndeveloped(Board, i);
					break;
				case -1: /* Black Pawn */
					score -= 2 - .1 * (IsDoubled(Board, i)) + IsPassed(Board, i) + 9 * CanPromote(Board, i) - 1.5 * IsUndeveloped(Board, i);
					break;
				case 1:	 /* White Pawn */
					score += 2 - .1 * (IsDoubled(Board, i)) + IsPassed(Board, i) + 9 * CanPromote(Board, i) - 1.5 * IsUndeveloped(Board, i);
					break;
				case 2:	 /* White Knight */
					score += 6 - open_game - (.5 * KnightOnRim(i)) - 1.5 * IsUndeveloped(Board, i);
					break;
				case 3:	 /* White Bishop */
					score += 6 + open_game - 1.5 * IsUndeveloped(Board, i);
					break;
				case 4:	 /* White Rook */
					score += 10 + open_game;
					break;
				case 5:	 /* White Queen */
					score += 18;
					break;
				default:
					score += 0;
			}
		}
	}

	return score;

}

int PawnsOnBoard(t_board Board){
	int pawns = 0;
	int i;
	int piece;

	for (i = 21; i < 99; i++){
		if(i % 10 == 9){
			i++;
			continue;
		}else{
			piece = Board[i];
			if (piece * piece == 1){
				pawns++;
			}	
		}
	}
	return pawns;
}

int IsDoubled(t_board Board, int index){
	int doubled = 0;
	int file = GetFile(index);
	int i;

	for (i = file + 20; i < 99; i += 10){
		if( (Board[i] == Board[index]) && (i != index) ){
			doubled = 1;
			break;
		}
	}

	return doubled;
}

int GetFile(int index){
	t_square ret;

	ret = one2two(index);

	return ret.File;
}

int GetRank(int index){
	t_square ret;

	ret = one2two(index);

	return ret.Rank;
}

int KnightOnRim(int index){
	int onRim = 0;
	int file = GetFile(index);

	if ( (file == 1) || (file == 8) ){
		onRim = 1;
	}

	return onRim;
}
/* this is the IsLegal() function that DOES check to verify if a move would put that player in check	*/
int IsLegal(t_board Board, t_move Move, int turn, t_castle Castle){
	int value = 0;
	int i = 0;
	int start_index = two2one(Move.Start);
	int end_index = two2one(Move.End);
	int difference = end_index - start_index;
	int space = 0;
	int direction = 0;

	if (Board[end_index] != 42){
		switch (Move.PieceType) {
			case -6:	/*black king*/
				if( turn == 1)
					{
					value = 0;
					break;
					}
				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
					if (difference == -11 || difference == -10 || difference == -9 || difference == -1 || difference == 1 || difference == 9 || difference == 10 || difference == 11){
						value = 1; 
					}
					else if (start_index == 25 && end_index == 27){
						if (Board[26] == 0 && Board[27] == 0 && Board[28] == -4){
							if (Castle[1] == 1 || Castle[3]== 1)
								break;
							else {
								Board[26] = -4;
								Board[28] = 0;
								value = 1;
							}	
						}
					}
					
					else if (start_index == 25 && end_index == 23){
						if (Board[22] == 0 && Board[23] == 0 && Board[24] == 0 && Board[21] == -4){
							if (Castle[1] == 1 || Castle[2] == 1)
								break;
							else {
								Board[24] = -4;
								Board[21] = 0;
								value = 1;
							}  	
						}
					}
				}
				break;
			case -5:  	/*black queen*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
						if(Move.End.File == Move.Start.File){
                                        	        if (difference > 0)
                                                	        direction = 10;
	                                                else	
        	                                                direction = -10;
                	                        }
                        	                else if(Move.End.Rank ==  Move.Start.Rank){
                                	                if (difference > 0)
                                        	                direction = 1;
	                                                else
        	                                                direction = -1;
                	                        }
						else if(difference % 11 == 0){
                	                                if (difference > 0)
                        	                                direction = 11;
                                	                else
                                        	                direction = -11;
	                                        }
        	                                else if(difference % 9 == 0){
                	                                if (difference > 0)
                        	                                direction = 9;
                                	                else
	                                       	                direction = -9;
        	                                }
				
						if(direction == 0)
						{
							value = 0;
							break;
						}
						else
						{
							space = difference / direction;
						}
	
						if (space == 1)
							value = 1;
						else {
							for(i = 1; i < space; i++){
	                	                        	        if(Board[start_index + (direction * i)] != 0){
	                        	                        	        value = 0;
		                                	                        break;
	        	                                	        }else{
	                	                                	        value = 1;
									}
		 	                               }
						}
				}
				break;
			case -4:	/*black rook*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
					if(Move.End.File == Move.Start.File || Move.End.Rank == Move.Start.Rank){
						if(Move.End.File == Move.Start.File){
							if (difference > 0)
								direction = 10;
							else
								direction = -10;
						}
						else if(Move.End.Rank ==  Move.Start.Rank){
							if (difference > 0)
								direction = 1;
							else
								direction = -1;
						}		
						
						space = difference / direction;
					
						if (space == 1)
							value = 1;
						else {
							for(i = 1; i < space; i++){
								if(Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}
				break;
			case -3:	/*black bishop*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){ 
					if (difference % 11 == 0 || difference % 9 == 0){
						if(difference % 11 == 0){
							if (difference > 0)
								direction = 11;
							else
								direction = -11;
						}
						else if(difference % 9 == 0){
							if (difference > 0)
								direction = 9;
							else
								direction = -9;			
						}
					
						space = difference / direction;

						if (space == 1)
							value = 1;
						else {
							for (i = 1; i < space; i++){
								if (Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}			
				break;
			case -2:	/*black knight*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
					if(difference == -21 || difference == -19 || difference == -12 || difference == -8 || difference  == 8 || difference == 12 || difference == 19 || difference == 21){
						value = 1;
					}
				}
				break;
			case -1:	/*black pawn*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index  > 20){
					if (Move.Start.Rank == 7 && Board[end_index] == 0){
						if (difference == 10){
							value = 1;
						}
						else if(difference == 20 && Board[end_index] == 0){
							if (Board[start_index + 10] != 0)
								break;
							else
								value = 1;
						}
						else if((difference == 11 && Board[end_index] > 0) || (difference == 9 && Board[end_index] > 0)){
							value =1;
						}
					}
					else{
						if (difference == 10 && Board[end_index] == 0)
							value = 1;
						else if ((difference == 11 && Board[end_index] > 0) || (difference == 9 && Board[end_index] > 0)){
							value = 1;
						}
						
					}
				}

				break;
			case 1:		/*white pawn*/
				if( turn == 0)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index  > 20){
					if (Move.Start.Rank == 2){
						if (difference == -10 && Board[end_index] == 0)
							value = 1;
						else if(difference == -20 && Board[end_index] == 0){
							if (Board[start_index - 10] != 0)
								break;
							else
								value = 1;
						}
						else if((difference == -11 && Board[end_index] < 0) || (difference == -9 && Board[end_index] < 0)){
							value = 1;
						}
					}
							
					else {
						if (difference == -10 && Board[end_index] == 0)
							value = 1;
						else if ((difference == -11 && Board[end_index] < 0) || (difference == -9 && Board[end_index] < 0)){
							value = 1;
						}
						
					}
				}

				break;
			case 2:		/*white knight*/
				if( turn == 0)
				{
				value = 0;
				break;
				}


				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
					if(difference == -21 || difference == -19 || difference == -12 || difference == -8 || difference  == 8 || difference == 12 || difference == 19 || difference == 21){
					value = 1;
					}
				}

				break;
			case 3:		/*white bishop*/
				if( turn == 0)
					{
					value = 0;
					break;
					}


				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){ 
					if (difference % 11 == 0 || difference % 9 == 0){
						if(difference % 11 == 0){
							if (difference > 0)
								direction = 11;
							else
								direction = -11;
						}
						else if(difference % 9 == 0){
							if (difference > 0)
								direction = 9;
							else
								direction = -9;			
						}
				
						space = difference / direction;

						if (space == 1)
							value = 1;
						else{
							for (i = 1; i < space; i++){
								if (Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}			
				break;
			case 4:		/*white rook*/
				if( turn == 0)
					{
					value = 0;
					break;
					}


				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
					if(Move.End.File == Move.Start.File || Move.End.Rank == Move.Start.Rank){
						if(Move.End.File == Move.Start.File){
							if (difference > 0)
								direction = 10;
							else
								direction = -10;
						}
						else if(Move.End.Rank ==  Move.Start.Rank){
							if (difference > 0)
								direction = 1;
							else
								direction = -1;
						}		
						
						space = difference / direction;

						if (space == 1)
							value = 1;
						else {
							for(i = 1; i < space; i++){
								if(Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}
				break;
			case 5:		/*white queen*/
				if( turn == 0)
					{
					value = 0;
					break;
					}


				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
						if(Move.End.File == Move.Start.File){
                                	                if (difference > 0)
                                        	                direction = 10;
	                                                else
        	                                                direction = -10;
						}
        	                                else if(Move.End.Rank ==  Move.Start.Rank){
                	                                if (difference > 0)
                        	                                direction = 1;
                                	                else
                                        	                direction = -1;
	                                        }
						else if(difference % 11 == 0){
                        	                        if (difference > 0)
                                	                        direction = 11;
                                        	        else
                                                	        direction = -11;
	                                        }
        	                                else if(difference % 9 == 0){
                	                                if (difference > 0)
                        	                                direction = 9;
                                	                else
                                        	                direction = -9;
						}	
						if(direction == 0)
						{
							value = 0;
							break;
						}
						else
						{
							space = difference / direction;
						}
						if (space == 1)
							value = 1;					
						else {
							for(i = 1; i < space; i++){
	                	                        	        if(Board[start_index + (direction * i)] != 0){
	                        	                        	        value = 0;
	                                	                	        break;
		                                        	        }else{
	        	                                        	        value = 1;
		        	                                        }
	        	        	                }
						}
				}

				break;
			case 6:		/* white king */
				if( turn == 0)
					{
					value = 0;
					break;
					}


				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
					if (difference == -11 || difference == -10 || difference == -9 || difference == -1 || difference == 1 || difference == 9 || difference == 10 || difference == 11){
						value = 1; 
					}
					else if (start_index == 95 && end_index == 97){
						if (Board[96] == 0 && Board[97] == 0 && Board[98] == 4){
							if (Castle[0] == 1 || Castle[5] == 1)
								break;
							else {	
								Board[96] = 4;
								Board[98] = 0;	
								value = 1;
							}    
							
						}
					}
					
					else if (start_index == 95 && end_index == 93){
						if (Board[92] == 0 && Board[93] == 0 && Board[94] == 0 && Board[91] == 4){
							if (Castle[0] == 1 || Castle[4] == 1)
								break;
							else {
								Board[94] = 4;
								Board[91] = 0;
								value = 1;	
							}
						}
					}
					
				}
				break;
			}
		}
	if (value == 1)
		if(AmIinCheck(Board, Move, turn, Castle) == 1)
			value = 0;
	return value;
}


/* Determines is a move is legal */
/* This func take a t_board that represents the board BEFORE the MOVE is made */
/* this version of isLegal() does not contain a call to AmIinCheck() as such checking is not needed seeing as we are simply making sure if we make a move that our opponent could not capture us on his very next move
	which does not require us to check to see if his move would put him in check because if he takes our king he would have won and such checks are both irrelevant and woudl result in an infinite loop */
int IsLegal_Check(t_board Board, t_move Move, int turn, t_castle Castle){
	int value = 0;
	int i = 0;
	int start_index = two2one(Move.Start);
	int end_index = two2one(Move.End);
	int difference = end_index - start_index;
	int space = 0;
	int direction = 0;
	
	if (Board[end_index] != 42){
		switch (Move.PieceType) {
			case -6:	/*black king*/
				if( turn == 0)
					{
					value = 0;
					break;
					}

	
				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
					if (difference == -11 || difference == -10 || difference == -9 || difference == -1 || difference == 1 || difference == 9 || difference == 10 || difference == 11){
						value = 1; 
					}
					else if (start_index == 25 && end_index == 27){
						if (Board[26] == 0 && Board[27] == 0 && Board[28] == -4){
							if (Castle[1] == 1 || Castle[3]== 1)
								break;
							else {
								Board[26] = -4;
								Board[28] = 0;
								value = 1;
							}	
						}
					}
					
					else if (start_index == 25 && end_index == 23){
						if (Board[22] == 0 && Board[23] == 0 && Board[24] == 0 && Board[21] == -4){
							if (Castle[1] == 1 || Castle[2] == 1)
								break;
							else {
								Board[24] = -4;
								Board[21] = 0;
								value = 1;
							}  	
						}
					}
				}
				break;
			case -5:  	/*black queen*/
				if( turn == 0)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
						if(Move.End.File == Move.Start.File){
                                        	        if (difference > 0)
                                                	        direction = 10;
	                                                else	
        	                                                direction = -10;
                	                        }
                        	                else if(Move.End.Rank ==  Move.Start.Rank){
                                	                if (difference > 0)
                                        	                direction = 1;
	                                                else
        	                                                direction = -1;
                	                        }
						if(difference % 11 == 0){
                	                                if (difference > 0)
                        	                                direction = 11;
                                	                else
                                        	                direction = -11;
	                                        }
        	                                else if(difference % 9 == 0){
                	                                if (difference > 0)
                        	                                direction = 9;
                                	                else
	                                       	                direction = -9;
        	                                }
						if(direction == 0)
						{
							value = 0;
							break;
						}
						else
						{
							space = difference / direction;
						}
						if (space == 1)
							value = 1;
						else {
							for(i = 1; i < space; i++){
	                	                        	        if(Board[start_index + (direction * i)] != 0){
	                        	                        	        value = 0;
		                                	                        break;
	        	                                	        }else{
	                	                                	        value = 1;
									}
		 	                               }
						}
				}
				break;
			case -4:	/*black rook*/	
				if( turn == 0)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
					if(Move.End.File == Move.Start.File || Move.End.Rank == Move.Start.Rank){
						if(Move.End.File == Move.Start.File){
							if (difference > 0)
								direction = 10;
							else
								direction = -10;
						}
						else if(Move.End.Rank ==  Move.Start.Rank){
							if (difference > 0)
								direction = 1;
							else
								direction = -1;
						}		
						
						space = difference / direction;
					
						if (space == 1)
							value = 1;
						else {
							for(i = 1; i < space; i++){
								if(Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}
				break;
			case -3:	/*black bishop*/
				if( turn == 0)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){ 
					if (difference % 11 == 0 || difference % 9 == 0){
						if(difference % 11 == 0){
							if (difference > 0)
								direction = 11;
							else
								direction = -11;
						}
						else if(difference % 9 == 0){
							if (difference > 0)
								direction = 9;
							else
								direction = -9;			
						}
					
						space = difference / direction;

						if (space == 1)
							value = 1;
						else {
							for (i = 1; i < space; i++){
								if (Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}			
				break;
			case -2:	/*black knight*/
				if( turn == 0)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] >= 0){
					if(difference == -21 || difference == -19 || difference == -12 || difference == -8 || difference  == 8 || difference == 12 || difference == 19 || difference == 21){
						value = 1;
					}
				}
				break;
			case -1:	/*black pawn*/
				if( turn == 0)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index  > 20){
					if (Move.Start.Rank == 7 && Board[end_index] == 0){
						if (difference == 10){
							value = 1;
						}
						else if(difference == 20 && Board[end_index] == 0){
							if (Board[start_index + 10] != 0)
								break;
							else
								value = 1;
						}
						else if((difference == 11 && Board[end_index] > 0) || (difference == 9 && Board[end_index] > 0)){
							value =1;
						}
					}
					else{
						if (difference == 10 && Board[end_index] == 0)
							value = 1;
						else if ((difference == 11 && Board[end_index] > 0) || (difference == 9 && Board[end_index] > 0)){
							value = 1;
						}
						
					}
				}

				break;
			case 1:		/*white pawn*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index  > 20){
					if (Move.Start.Rank == 2){
						if (difference == -10 && Board[end_index] == 0)
							value = 1;
						else if(difference == -20 && Board[end_index] == 0){
							if (Board[start_index - 10] != 0)
								break;
							else
								value = 1;
						}
						else if((difference == -11 && Board[end_index] < 0) || (difference == -9 && Board[end_index] < 0)){
							value = 1;
						}
					}
							
					else {
						if (difference == -10 && Board[end_index] == 0)
							value = 1;
						else if ((difference == -11 && Board[end_index] < 0) || (difference == -9 && Board[end_index] < 0)){
							value = 1;
						}
						
					}
				}

				break;
			case 2:		/*white knight*/
				if( turn == 1)
				{
				value = 0;
				break;
				}

				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
					if(difference == -21 || difference == -19 || difference == -12 || difference == -8 || difference  == 8 || difference == 12 || difference == 19 || difference == 21){
					value = 1;
					}
				}

				break;
			case 3:		/*white bishop*/
				if( turn == 1)
				{
				value = 0;
				break;
				}

				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){ 
					if (difference % 11 == 0 || difference % 9 == 0){
						if(difference % 11 == 0){
							if (difference > 0)
								direction = 11;
							else
								direction = -11;
						}
						else if(difference % 9 == 0){
							if (difference > 0)
								direction = 9;
							else
								direction = -9;			
						}
				
						space = difference / direction;

						if (space == 1)
							value = 1;
						else{
							for (i = 1; i < space; i++){
								if (Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}			
				break;
			case 4:		/*white rook*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
					if(Move.End.File == Move.Start.File || Move.End.Rank == Move.Start.Rank){
						if(Move.End.File == Move.Start.File){
							if (difference > 0)
								direction = 10;
							else
								direction = -10;
						}
						else if(Move.End.Rank ==  Move.Start.Rank){
							if (difference > 0)
								direction = 1;
							else
								direction = -1;
						}		
						
						space = difference / direction;

						if (space == 1)
							value = 1;
						else {
							for(i = 1; i < space; i++){
								if(Board[start_index + (direction * i)] != 0){
									value = 0;
									break;
								}else{
									value = 1;
								}
							}
						}
					}
				}
				break;
			case 5:		/*white queen*/
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
						if(Move.End.File == Move.Start.File){
                                	                if (difference > 0)
                                        	                direction = 10;
	                                                else
        	                                                direction = -10;
						}
        	                                else if(Move.End.Rank ==  Move.Start.Rank){
                	                                if (difference > 0)
                        	                                direction = 1;
                                	                else
                                        	                direction = -1;
	                                        }
						else if(difference % 11 == 0){
                        	                        if (difference > 0)
                                	                        direction = 11;
                                        	        else
                                                	        direction = -11;
	                                        }
        	                                else if(difference % 9 == 0){
                	                                if (difference > 0)
                        	                                direction = 9;
                                	                else
                                        	                direction = -9;
						}	
						if(direction == 0)
						{
							value = 0;
							break;
						}
						else
						{
							space = difference / direction;
						}
						if (space == 1)
							value = 1;					
						else {
							for(i = 1; i < space; i++){
	                	                        	        if(Board[start_index + (direction * i)] != 0){
	                        	                        	        value = 0;
	                                	                	        break;
		                                        	        }else{
	        	                                        	        value = 1;
		        	                                        }
	        	        	                }
						}
				}

				break;
			case 6:		/* white king */
				if( turn == 1)
					{
					value = 0;
					break;
					}

				if (end_index < 99 && end_index > 20 && Board[end_index] <= 0){
					if (difference == -11 || difference == -10 || difference == -9 || difference == -1 || difference == 1 || difference == 9 || difference == 10 || difference == 11){
						value = 1;
					}
					else if (start_index == 95 && end_index == 97){
						if (Board[96] == 0 && Board[97] == 0 && Board[98] == 4){
							if (Castle[0] == 1 || Castle[5] == 1)
								break;
							else {	
								Board[96] = 4;
								Board[98] = 0;	
								value = 1;
							}    
							
						}
					}
					
					else if (start_index == 95 && end_index == 93){
						if (Board[92] == 0 && Board[93] == 0 && Board[94] == 0 && Board[91] == 4){
							if (Castle[0] == 1 || Castle[4] == 1)
								break;
							else {
								Board[94] = 4;
								Board[91] = 0;
								value = 1;	
							}
						}
					}
				}
				break;
			}
		}

	return value;
}

double IsCastled(t_board Board,int turn){
	int Castled = 0;

	switch (turn){
		case 1:
			if(Board[24] == 6 || Board[25] == 6 || Board[34] == 6 || Board[35] == 6 || Board[44] == 6 || Board[45] == 6 || Board[54]==6 || Board[55] == 6 || Board[64] == 6 || Board[65] ==6 || Board[74] == 6 || Board[75] == 6 || Board[84] == 6 || Board[85] == 6 || Board[94] ==6 || Board[95] == 6){
	Castled = 0;
	}
	else{
		Castled = 5;	
	}
	break;
		case 0:
		if(Board[24] == -6 || Board[25] == -6 || Board[34] == -6 || Board[35] == -6 || Board[44] == -6 || Board[45] == -6 || Board[54]== -6 || Board[55] == -6 || Board[64] == -6 || Board[65] == -6 || Board[74] == -6 || Board[75] == -6 || Board[84] == -6 || Board[85] == -6 || Board[94] == -6 || Board[95] == -6){
	Castled = 0;
	}
	else{
		Castled = -5;	
	}

			break;
	}

	return Castled;
}

int AmIinCheck(t_board Board, t_move Move, int turn, t_castle Castle){
	int check1;
	int i,j;
	t_board Board_Copy;
	t_move MoveTest;
	t_square Nsquare, Ksquare;
	for(i = 0; i<120; i++)
	{
		Board_Copy[i] = Board[i];
	}
	MovePiece(Board_Copy, Move, Castle);
	if( turn == 1) /* this is white's turn */
		{
		/*loop through to locate white's king  */
		for(i = 1; i <9 ; i++)
		{
			for(j = 1; j < 9; j++)
				{
				Ksquare.Rank = i;
				Ksquare.File = j;	
				check1 = two2one(Ksquare);
				if( Board_Copy[check1] == 6)
					break;
				}
			if (Board_Copy[check1] == 6)
				break;
		}
		/* now loop through black's pieces to see fi they have a legal move to white's king */
		for( i = 1; i < 9 ; i++)
			for( j = 1; j < 9 ; j++)
				{
				Nsquare.Rank = i;
				Nsquare.File = j;
				check1 = two2one(Nsquare);
				/*looking for black pieces */
				if(Board_Copy[check1] < 0)
					{
					MoveTest.Start = Nsquare;
					MoveTest.End = Ksquare;
					MoveTest.PieceType = Board_Copy[check1];
					if(IsLegal_Check(Board_Copy, MoveTest, turn, Castle) == 1)
						return 1;
					}							
				}
			
			return 0;
		}
	else /* this is blacks turn */		
		{		
		/*loop through to locate blacks's king  */
		for(i = 1; i <9 ; i++)
		{
			for(j = 1; j < 9; j++)
				{
				Ksquare.Rank = i;
				Ksquare.File = j;	
				check1 = two2one(Ksquare);
				if( Board_Copy[check1] == -6)
					break;
				}
			if (Board_Copy[check1] == -6)
				break;
		}
		/* now loop through white's pieces to see fi they have a legal move to black's king */
		for( i = 1; i < 9 ; i++)
			for( j = 1; j < 9 ; j++)
				{
				Nsquare.Rank = i;
				Nsquare.File = j;
				check1 = two2one(Nsquare);
				/*looking for black pieces */
				if(Board_Copy[check1] > 0 && Board_Copy[check1] != 42)
					{
					MoveTest.Start = Nsquare;
					MoveTest.End = Ksquare;
					MoveTest.PieceType = Board_Copy[check1];
					if(IsLegal_Check(Board_Copy, MoveTest, turn, Castle) == 1)
						return 1;
					}							
				}
			
			return 0;
			}
		}/* eof	*/

/****************************************************************************************************************************************************/
/****************************************************************************************************************************************************/
/*This function is used to see if after a move was made, that the move put the opposing side in check.						*/
int CheckCheck(t_board Board, int turn,t_castle Castle){
	int check1;
	int i,j;
	t_move MoveTest;
	t_square Nsquare, Ksquare;

	if( turn == 1) /* Black has just finished their move, see if a black piece can move to white's king. */
		{
		/*loop through to locate white's king  */
		for(i = 1; i <9 ; i++)
		{
			for(j = 1; j < 9; j++)
				{
				Ksquare.Rank = i;
				Ksquare.File = j;	
				check1 = two2one(Ksquare);
				if( Board[check1] == 6)
					break;
				}
			if (Board[check1] == 6)
				break;
		}
		/* now loop through black's pieces to see if they have a legal move to white's king */
		for( i = 1; i < 9 ; i++)
			for( j = 1; j < 9 ; j++)
				{
				Nsquare.Rank = i;
				Nsquare.File = j;
				check1 = two2one(Nsquare);
				/*looking for black pieces */
				if(Board[check1] < 0)
					{
					MoveTest.Start = Nsquare;
					MoveTest.End = Ksquare;
					MoveTest.PieceType = Board[check1];
					if(IsLegal_Check(Board, MoveTest, turn, Castle) == 1)
						return 1;
					}							
				}
			
			return 0;
		}
	else /* White has just made its move, see if a white piece could move to black king */		
		{		
		/*loop through to locate blacks's king  */
		for(i = 1; i <9 ; i++)
		{
			for(j = 1; j < 9; j++)
				{
				Ksquare.Rank = i;
				Ksquare.File = j;	
				check1 = two2one(Ksquare);
				if( Board[check1] == -6)
					break;
				}
			if (Board[check1] == -6)
				break;
		}
		/* now loop through white's pieces to see fi they have a legal move to black's king */
		for( i = 1; i < 9 ; i++)
			for( j = 1; j < 9 ; j++)
				{
				Nsquare.Rank = i;
				Nsquare.File = j;
				check1 = two2one(Nsquare);
				/*looking for black pieces */
				if(Board[check1] > 0 && Board[check1] != 42)
					{
					MoveTest.Start = Nsquare;
					MoveTest.End = Ksquare;
					MoveTest.PieceType = Board[check1];
					if(IsLegal_Check(Board, MoveTest, turn, Castle) == 1)
						return 1;
					}							
				}
			
			return 0;
			}
		}/* eof	*/







/*this will verify after a move is made if that move put the opposing side in CheckMate	*/
int CheckCheckmate(t_board Board, int turn, t_castle Castle){
	int check1;
	int i,j;
	int a,b;
/*	t_board Board_Copy;	*/
	t_move MoveTest;
	t_square Ssquare, Esquare;
	/* it was white turns, so see if black now has a valid move... using turn = 0 due to data flow in chess.c	*/
	if( turn == 0)
	{
	for( i = 1; i < 9 ; i++)
	{
		for( j = 1; j < 9; j++)
		{
			Ssquare.Rank = i;
			Ssquare.File = j; 
			check1 = two2one(Ssquare);
			if(Board[check1] < 0 && Board[check1] != 42)
				{
					for( a = 1; a < 9 ; a++)
					{
						for( b = 1; b < 9 ; b++)
						{
							Esquare.Rank = a;
							Esquare.File = b;
							MoveTest.Start = Ssquare;
							MoveTest.End = Esquare;
							MoveTest.PieceType = Board[check1];
							if( IsLegal(Board, MoveTest, turn, Castle) == 1)
								return 1;
							else
								continue;
						}
					}
				}
			else  /*if board[check1] isn't a black piece*/
				continue;	
		
		} /* end of j loop */
	} /* end of i loop */
	} /* enf of if it was white's turn */
	/* so here it was a black move made, checking to see if black put white in checkmate. */
	else
	{
	for( i = 1; i < 9 ; i++)
	{
		for( j = 1; j < 9; j++)
		{
			Ssquare.Rank = i;
			Ssquare.File = j; 
			check1 = two2one(Ssquare);
			if(Board[check1] > 0 && Board[check1] != 42 )
				{
					for( a = 1; a < 9 ; a++)
					{
						for( b = 1; b < 9 ; b++)
						{
							Esquare.Rank = a;
							Esquare.File = b;
							MoveTest.Start = Ssquare;
							MoveTest.End = Esquare;
							MoveTest.PieceType = Board[check1];
							if( IsLegal(Board, MoveTest, turn, Castle) == 1)
								return 1;
							else
								continue;
						}
					}
				}
			else /* if board[check1] isn't a white piece */
				continue;	
		
		} /* end of j loop */
	} /* end of i loop */
	} /* enf of if it was black's turn */
	/* so if at no point did the function find a legal move, we return 1 to show that a checkmate occured. */
	return 2;
} /*eof */

int CanPromote(t_board board, int index){
	int can = 0;
	t_square loc = one2two(index);

	switch(board[index]){
		case 1:
			if (loc.Rank == 7)
				can = 1;
			break;
		case -1:
			if (loc.Rank == 2)
				can = 1;
			break;
	}
	return can;
}

int IsPassed(t_board board, int index){
	int passed = 1;
	t_square loc = one2two(index);
	int i = loc.Rank;

	switch(board[index]){
		case 1:
			for (i++; i < 9; i++){
				loc.Rank = i;
				if (board[two2one(loc)] == -1){
					passed = 0;
					break;
				}
			}
			break;
		case -1:
			for (i--; i > -1; i--){
				loc.Rank = i;
				if (board[two2one(loc)] == 1){
					passed = 0;
					break;
				}
			}
			break;
	}
	return passed;
}

int BishopPair(t_board board){
	int pair = 0;
	int i;

	for (i = 20; i < 99; i++){
		if (board[i] == 3)
			pair++;
		else if (board[i] == -3)
			pair--;
		
	}
	pair = pair > 0 ? 1 : pair < 0 ? -1 : 0;
	return pair;

}

int IsUndeveloped(t_board board, int index){
	int isnot = 0;
	t_square loc = one2two(index);

	if (board[index] == 1){
		if (loc.Rank == 2)
			isnot = 1;
	}else if (board[index] == -1){
		if (loc.Rank == 7)
			isnot = 1;
	}else{
		if (board[index] > 0){
			if (loc.Rank == 1)
				isnot = 1;
		}else{
			if (loc.Rank == 8)
				isnot = 1;
		}
	}
	return isnot;
}

int CenterControl(t_board board){
	int center;
	double score = 0;
	int i;

	for (i = 43; i < 47; i++){
		if (board[i] == -1)
			score -= .2;
	}

	for (i = 53; i < 57; i += 3){
		if (board[i] == -1){
			score -= .1;
			break;
		}
	}

	for (i = 54; i < 56; i++){
		if (board[i] == -1)
			score -= .5;
	}

	for (i = 73; i < 77; i++){
		if (board[i] == 1)
			score += .2;
	}

	for (i = 63; i < 67; i += 3){
		if (board[i] == 1){
			score += .1;
			break;
		}
	}

	for (i = 64; i < 66; i++){
		if (board[i] == 1)
			score += .5;
	}

	center = score > 0 ? 1 : score < 0 ? -1 : 0;
	return center;

}

char CenterControl2(t_board board, t_move_array current_turn, int turn, t_castle Castle){
	char score;
	int i;
	t_square End;
	t_move_array next_turn = ExhaustMoves(board, turn == 1 ? 0 : 1, Castle);

	for (i = 0; i < current_turn.Length; i++){
		End = current_turn.Moves[i].End;
		if ((End.File == 4 || End.File == 5) && (End.Rank == 4 || End.Rank == 5)){
			if (turn == 1)
				if (current_turn.Moves[i].PieceType == 1)
					score += 2;
				else
					score++;
			else
				if (current_turn.Moves[i].PieceType == -1)
					score -= 2;
				else
					score--;
		}
	}

	for (i = 0; i <next_turn.Length; i++){
		End = next_turn.Moves[i].End;
		if ((End.File == 4 || End.File == 5) && (End.Rank == 4 || End.Rank == 5)){
			if (turn == 1)
				if (next_turn.Moves[i].PieceType == -1)
					score -= 2;
				else
					score--;
			else
				if (next_turn.Moves[i].PieceType == 1)
					score += 2;
				else
					score++;
		}
	}

	score = score > 0 ? 1 : score < 0 ? -1 : 0;
	return score;
}

void UpdateHasMoved(t_board board, int* wNi, int* wBi, int* bNi, int* bBi){
	
	if (board[22] == 0 || board[27] == 0)
		*bNi = 1;
	if (board[23] == 0 || board[26] == 0)
		*bBi = 1;
	if (board[92] == 0 || board[97] == 0)
		*wNi = 1;
	if (board[93] == 0 || board[96] == 0)
		*wBi = 1;
}

char QueenIsOut(t_board board){
	char out = 0;
	unsigned int i;

	for (i = 21; i < 99; i++){
		if (board[i] == 5 && one2two(i).Rank > 2)
			out++;
		else if (board[i] == -5 && one2two(i).Rank < 7)
			out--;
	}

	return out;
}
