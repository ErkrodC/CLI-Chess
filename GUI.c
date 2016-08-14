#include "GUI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*printboard*/                                                                                              
void printboard(t_board board)                                                                              
{                                                                                                           
        int i, j;
	int boardnum = 21;                                                                                           
        int line = 8;                                                                                       
        char piecetype[3];                                                                                  
        
	printf("\n");                                                                                                    
        /* number => piecetype*/                                                                            
        for(i = 0; i < 18; i++){ /*18 = num of lines on board*/                                                                           
                if(i == 17){                                                                                
                        printf("                   a    b    c    d    e    f    g    h   \n");                          
                }                                                                                           
                else if(i == 0 || i % 2 == 0){                                                        
			printf("                +----+----+----+----+----+----+----+----+\n");                           
                }                                                                                           
                else if(i % 2 != 0){                                                                        
                        printf("              %d |", line);                                                              
                                                                                                            
                        for(j = boardnum; j < (boardnum + 8); j++){                                                             
                                convertPieceType(board[j], piecetype);                    
                                printf(" %s |", piecetype);                         
                        }/*end for j*/                                                                      
                        
			boardnum += 10;                                                                                    
                        printf("\n");                                                                       
                        line--;                                                                             
                }                                                                                           
        }/*end for i*/                                                                                      
}/*end of printboard*/ 

/*convert number to piece type*/                                                                            
void convertPieceType(int num, char piece[3])                                                               
{                                                                                                           
                                                                                                            
        switch(num){                                                                                        
                case(1):{                                                                                   
                        strcpy(piece, "wP");                                                                
                        break;                                                                              
                }                                                                                           
                case(-1):{                                                                                  
                        strcpy(piece, "bP");                                                                
                        break;                                                                              
                }                                                                                           
                case(2):{                                                                                   
                        strcpy(piece, "wN");                                                                
                        break;                                                                              
                }                                                                                           
                case(-2):{                                                                                  
                        strcpy(piece, "bN");                                                                
                        break;                                                                              
                }                                                                                           
                case(3):{                                                                                   
                        strcpy(piece, "wB");                                                                
                        break;                                                                              
                }                                                                                           
                case(-3):{                                                                                  
                        strcpy(piece, "bB");                                                                
                        break;                                                                              
                }                                                                                           
                case(4):{                                                                                   
                        strcpy(piece, "wR");                                                                
                        break;                                                                              
                }                                                                                           
                case(-4):{                                                                                  
                        strcpy(piece, "bR");                                                                
                        break;                                                                              
                }                                                                                           
                case(5):{                                                                                   
                        strcpy(piece, "wQ");                                                                
                        break;                                                                              
                }                                                                                           
                case(-5):{                                                                                  
                        strcpy(piece, "bQ");                                                                
                        break;                                                                              
                }                                                                                           
                case(6):{                                                                                   
                        strcpy(piece, "wK");                                                                
                        break;                                                                              
                }                                                                                           
                case(-6):{                                                                                  
                        strcpy(piece, "bK");                                                                
                        break;                                                                              
                }                                                                                           
                default:{                                                                                   
                        strcpy(piece, "  ");                                                                
                        break;                                                                              
                }                                                                                           
        }                                                                                                   
}/*end conversion*/ 
 
