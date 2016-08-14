#ifndef CHESS_H
#define CHESS_H

#include <assert.h>
#include "DataTypes.h"
#include "AI.h"

/* Prints a menu of options */
void PrintMenu();

void AddBoard(t_board *board, BLIST *list);

void DeleteBoard(BLIST *list);

BLIST *NewBoardList();

void DeleteBoardList(BLIST *list);

/* Defines the data and function flow for a human player's turn */
int UserTurn(t_board Board,t_castle Castle, int turn, int piece[], char start[][3], char end[][3], int capture[], int special[], int* Wn, int* Wb, int* Bn, int* Bb);

/* Defines the data and function flow for a AI player's turn */
void AITurn(t_board Board, t_castle Castle, int turn, int difficulty, int piece[], char start[][3], char end[][3], int capture[], int special[], int* Wn, int* Wb, int* Bn, int* Bb);

#endif
