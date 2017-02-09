//board.h
//header file
#include <map>
#include "Board_Defaults.h"

#ifndef BOARD_H
#define BOARD_H

class Board
{
  private:
  	bool b_north_turn = true; //north starts first
    int turn = 1; //north: 1, south: -1

    std::map<int, bool> m_in_check =
    {
      {1, false}, {-1, false}
    };

    std::map<int, int> m_general_pos = 
    {
      {1, N_GENERAL}, {-1, S_GENERAL}
    };

    // std::map<int, int> m_camp_center =
    // {
    //   {1, NORTH_CAMP_CENTER}, {-1, SOUTH_CAMP_CENTER}
    // };
  
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
  		//prompt player specify intial position
    	//	determine if is valid, goto turn_start if not
    	//prompt player specify final position
    	//	determine if is valid: goto turn_start if not
      //if current player is attempting to finish move in-check, goto turn_start
      //  being in-check doesn't matter, ending turn in-check matters
    	//execute move
    	//determine ramifications (evaluate check, remove pieces from board, etc.)
      //if this move would put the opposing player in-check,
      // determine if there would be a valid move that would allow the opposing player to
      // escape the check. if not, then declare checkmate
      // valid moves: move general, remove threatening piece, block threatening piece
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
  	bool get_in_check(int);
    void set_in_check();
      bool evaluate_threat(int, int);
      bool pawn_threat(int, int);
      bool straight_path_threat(int, int);
      bool horse_threat(int, int);

    void make_piece(int, int); //position, piece id
    void remove_piece(int); //position
    void move_piece(int, int); //initial, final positions
    void update_general_pos(int, int);
    void flip_turn(); //change turn from N to S and vice versa
    int get_general_pos(int);

    int check_pos(int); //returns piece at given position
    bool n_camp_box_check(int); //given a position, determines if it's in the box
    bool s_camp_box_check(int); 
};

#endif