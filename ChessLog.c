#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ChessLog.h"

void WriteLog(int piece[], char start[][3], char end[][3], int capture[], int special[])
{
	FILE *f;
	int j,i;
	int turn = 1;
    
	f = fopen("log.txt", "w+");
    
	if(f == NULL)
	{
		printf("Error creating file!");
		exit(10);
	}
    
	fprintf(f, "============================================================================================================\n");
	fprintf(f, "| Turn\t\tPlayer\t\tPiece\t\tStart\t\tEnd\t\tCapture\t\tSpecial    |\n|==========================================================================================================|\n");
    
	for(j = 0; j < 300; j++)
	{
		if(piece[j] == 0)
			break;
		if(j % 2 == 0 || j == 0)
		{
			if(j != 0)
			{
				fprintf(f, "|----------------------------------------------------------------------------------------------------------|\n");
			}
			fprintf(f, "| %d.\t\t", turn);
			turn++;
		}/*end if:seperation line*/
		else
			fprintf(f, "|\t\t");
        
		/* Prints out the player and the piece */
		switch(piece[j])
		{
			case 1:
				fprintf(f, "W\t\tPawn\t\t");
				break;
			case -1:
				fprintf(f, "B\t\tPawn\t\t");
				break;
			case 2:
				fprintf(f, "W\t\tKnight\t\t");
				break;
			case -2:
				fprintf(f, "B\t\tKnight\t\t");
				break;
			case 3:
				fprintf(f, "W\t\tBishop\t\t");
				break;
			case -3:
				fprintf(f, "B\t\tBishop\t\t");
				break;
			case 4:
				fprintf(f, "W\t\tRook\t\t");
				break;
			case -4:
				fprintf(f, "B\t\tRook\t\t");
				break;
			case 5:
				fprintf(f, "W\t\tQueen\t\t");
				break;
			case -5:
				fprintf(f, "B\t\tQueen\t\t");
				break;
			case 6:
				fprintf(f, "W\t\tKing\t\t");
				break;
			case -6:
				fprintf(f, "B\t\tKing\t\t");
				break;
			default:
				fprintf(f, "ERROR");
				break;
		}/*end switch:player piece*/
	
        
		/* prints the start and end positions */
		for(i = 0; i<4; i++){
			if(i < 2){
				fprintf(f, "%c", start[j][i]);
			}
			else{
				fprintf(f, "%c", end[j][i-2]);
			}
			if(i == 1 || i == 3)
				fprintf(f, "\t\t");
			/*fprintf(f, "%s\t\t%s\t\t", &start[j], &end[j]);*/
        	}
			
		/* prints out if a piece was captured */
		switch(capture[j])
		{
			case 0:
				fprintf(f, "\t\t");
				break;
			case 1:
			case -1:
				fprintf(f, "Pawn\t\t");
				break;
			case 2:
			case -2:
				fprintf(f, "Knight\t\t");
				break;
			case 3:
			case -3:
				fprintf(f, "Bishop\t\t");
				break;
			case 4:
			case -4:
				fprintf(f, "Rook\t\t");
				break;
			case 5:
			case -5:
				fprintf(f, "Queen\t\t");
				break;
			case 6:
			case -6:
				fprintf(f, "King\t\t");
				break;
			default:
				fprintf(f, "ERROR");
				break;
		}/*end switch: piece capture*/
        
		/* prints out the special moves */
		switch(special[j])
		{
			case 0:
				fprintf(f, "\t   |\n");	
				break;
			case 1:
				fprintf(f, "Castling   |\n");
				break;
			case 2:
				fprintf(f, "En Passant |\n");
				break;
			default:
				fprintf(f, "ERROR");
				break;
		}
	
	
	}/*end for:go through arrays*/

	fprintf(f, "============================================================================================================");    
	fclose(f);
}/*end Write Chess Log*/

/*Delete Lines for undo*/
void DeleteLines()
{

}/*end Delete line */


