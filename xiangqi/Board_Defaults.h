//Board_Defaults.h

//piece list
#define UNOCCUPIED 0
#define INVALID -1
#define PAWN 2
#define CANNON 3
#define ELEPHANT 4
#define HORSE 5
#define CART 6
#define GUARD 7
#define GENERAL 8

//players
#define NORTH 1
#define SOUTH -1

//directions
#define UP -13
#define DOWN 13
#define LEFT -1
#define RIGHT 1

//default piece positions
#define NW_CANNON 55
#define NE_CANNON 61
#define SW_CANNON 120
#define SE_CANNON 126

#define NW_ELEPHANT 30
#define NE_ELEPHANT 34
#define SW_ELEPHANT 147
#define SE_ELEPHANT 151

#define NW_HORSE 29
#define NE_HORSE 35
#define SW_HORSE 146
#define SE_HORSE 152

#define NW_CART 28
#define NE_CART 36
#define SW_CART 145
#define SE_CART 153

#define NW_GUARD 31
#define NE_GUARD 33
#define SW_GUARD 148
#define SE_GUARD 150

#define N_GENERAL 32
#define S_GENERAL 149

//left to right designations
int n_pawn_pos[] = {67, 69, 71, 73, 75};
int s_pawn_pos[] = {106, 108, 110, 112, 114};
