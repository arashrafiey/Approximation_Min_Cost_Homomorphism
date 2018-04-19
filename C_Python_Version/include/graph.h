#include <stdio.h>  
#include <stdlib.h>
#include <string.h> 
#include <glpk.h>


//Set bit y (0-indexed) of x to '1' by generating a a mask with a '1' in the proper bit location and ORing x with the mask.
#define SET_BIT(X,Y) X |= (1 << Y)

//Set bit y (0-indexed) of x to '0' by generating a mask with a '0' in the y position and 1's elsewhere then ANDing the mask with x.
#define CLEAR_BIT(X,Y) X &= ~(1<< Y)

//Return '1' if the bit value at position y within x is '1' and '0' if it's 0 
#define READ_BIT(X,Y) ((0u == (X & (1<<Y)))?0u:1u)

#define GET_BYTE_POSITION(byte_position,size_h,pair_hash) byte_position =  size_h - 1 - (pair_hash/8) // from right to left
#define GET_BIT_POSITION(bit_position,size_h,pair_hash,byte_position) bit_position = pair_hash - ( ((size_h - byte_position -1)*8) )
#define KEY_HASH(KEY,X,Y,N) KEY = (X*N)+Y;


typedef 
	unsigned char
uchar;

typedef struct graphdata {
  uchar ** list_homom_matrix; // matrix of G x H that saves each vertice in H available to G
	uchar ** pairs_matrix; // Matrix of G squared +G X H squared. Saves(by bit set) each pair(hash key) available for that pair in G(also a key)
	double ** cost_g_h_matrix;

	uchar ** graph_g;
	uchar ** graph_h;

	int num_bytes; // to save how many bytes in to make pairs with H
	int num_vert_G;
	int num_vert_H;
	int size_g_pairs; // use to create pairs_matrix and to acess it
	int size_h_pairs;

} GRAPH;

typedef struct LPdata {
	int ** index_saving_matrix;
	int *ia; // save the index i of the constraint matrix
	int *ja; // save the index j of the constraint matrix
	double *ar; // save the value/coeficient of the constraing at [i,j]
	int num_constraints;
	glp_prob *mip;

	int allocation_size; // size to realocate memory
	int variable_type;

} LP_MIN_COST;

typedef struct userdata {
  char *graph_g;  // Graph G file name
  char *graph_h;
  char *list_cost;
  char *full_list;
  char *list_homom;
  char *list_pairs;

} USER_PARAMS;

void makegraph(GRAPH *op, USER_PARAMS *ip);
void pair_consistency(GRAPH *op, USER_PARAMS * ip);
void construct_LP(GRAPH *op, LP_MIN_COST * lp);
