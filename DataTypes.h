#ifndef DATA_TYPES_H
#define DATA_TYPES_H

/* Initial Board Setup */
typedef int t_board[120];

typedef int t_castle[6];


/* Square Struct Definition */
typedef struct {          /* This is needed to convert from 1-D to 2-D */
		int Rank;
		int File;
	} t_square;

/* Link list for boards */
typedef struct BoardList BLIST;
typedef struct BoardEntry BENTRY;

struct BoardEntry{
	BLIST *List;
	BENTRY *Next;
	BENTRY *Prev;
	int Turn;
	t_board *Boards;
	};
	
struct BoardList{
	BENTRY *First;
	BENTRY *Last;
	int Turn;
	};
	
/* I think this struct would make it easy to connect GUI and AI -Eric */
typedef struct {
	t_square Start;
	t_square End;
	int PieceType;
	} t_move;

/* type for an array of moves */
typedef struct {
	t_move Moves[100];
	int Length;
	} t_move_array;

typedef struct {
	t_board Board;
	int Depth;
	} t_node;

typedef struct {
	t_move Move;
	double Value;
	} t_coin;
#endif
