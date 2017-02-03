//board.h
//header file
#include "Board_Defaults.h"

#ifndef BOARD_H
#define BOARD_H

class Board
{
  private:
  	bool b_north_turn = true; //north starts first
  	bool b_north_check = false;
  	bool b_south_check = false;
    int i_north_general_pos = N_GENERAL;
    int i_south_general_pos = S_GENERAL;
  
  	// starting board
    //computer-view of board 14 (L) by 13 (W)
  	int ia_grid[182] = 
    {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //12
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //25
     -1, -1,  6,  5,  4,  7,  8,  7,  4,  5,  6, -1, -1, //38
     -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, //51
     -1, -1,  0,  3,  0,  0,  0,  0,  0,  3,  0, -1, -1, //64
     -1, -1,  2,  0,  2,  0,  2,  0,  2,  0,  2, -1, -1, //77
     -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, //riverbank 90
     -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, //riverbank 103
     -1, -1, -2,  0, -2,  0, -2,  0, -2,  0, -2, -1, -1, //116
     -1, -1,  0, -3,  0,  0,  0,  0,  0, -3,  0, -1, -1, //129
     -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, //142
     -1, -1, -6, -5, -4, -7, -8, -7, -4, -5, -6, -1, -1, //155
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //168
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  //181
    };
  
  
  public:

    //function prototypes

  	void play(); 
  	//TODO:
  	//start game
  	//turn_start
  	//determine is "in check" - adds more checks to validate moves
		//prompt player specify intial position
  	//	determine if is valid, goto turn_start if not
  	//prompt player specify final position
  	//	determine if is valid: goto turn_start if not
  	//if "in check", determine if move would remove check condition
  	//	if not, goto turn_start
  	//execute move
  	//determine ramifications (flip check status, remove pieces from board, etc.)
  	//next player turn
    
  	int determine_position(int, int); //given set of x, y find corresponding array position
    	
  	//args: initial and final position of piece 
    //pieces cannot occupy the same position as an allied piece
    //pieces cannot move off the board (into -1 territory)
  	//return: false if illegal move, true if valid
  	bool eval_move(int, int); 
    bool eval_dest(int, int);

    //check unit collision for cannons and carts
    int straight_collision_check(int, int);

    //helper functions per different piece (should check rules of a particular piece)
  	//args: initial position, final position
  	//returns: true if valid, false if not
  	bool pawn_move(int, int);
  	bool cannon_move(int, int);
    bool cart_move(int, int);
    bool horse_move(int, int);
    bool elephant_move(int, int);
    bool guard_move(int, int);
    bool general_move(int, int);
    bool obstructed_generals(int);

  	//call from eval_move if general is threatened
  	//flips north_check or south_check from false to true and vice versa
  	void get_in_check(); 

    void make_piece(int, int); //position, piece id
    void remove_piece(int); //position
    void update_general_pos(int);
    void flip_turn(); //change turn from N to S and vice versa
    int get_general_pos(int);

    int check_pos(int); //returns piece at given position
    bool n_camp_box_check(int); //given a position, determines if it's in the box
    bool s_camp_box_check(int); 
};

#endif