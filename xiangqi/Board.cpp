//board.cpp
//implementation file
#include <cmath>
#include <iostream>
#include "Board.h"

//purpose: check for unit collision for cannons/carts
//
bool Board::collision_check(int i, int f)
{
  bool straight_path = false; //horizontal or vertical movement
  bool path_clear = true; //assume path is clear unless checks say otherwise
  int next; //next space for piece
  //move: can only move horizontally or vertically
  //each space moved through must be empty
  //cannot exceed limits of board
  if (std::abs(f - i) <= 9) //horizontal movement
  {
    if (i < f) //left to right movement
    {
      next = i + 1;
      for (next; next < f; next++) //check each space from left to right
      {
        if (ia_grid[next] != 0) 
        {
          path_clear = false; //collided
          break; //break if space is not empty
        }
      }
    }
    else //right to left movement
    {
      next = i - 1;
      for (next; next > f; next--) //reverse check direction
      {
        if (ia_grid[next] != 0) 
        {
          path_clear = false;
          break;
        }
      }
    }
    straight_path = true;
  }
  else if (std::abs(f - i) % 13 == 0) //validate vertical movement
  {
    if (i < f) //southward/down movement
    {
      next = i + 13;
      for (next; next < f; next += 13) //movement downwards is 13 spaces
      {
        if (ia_grid[next] != 0)
        {
          path_clear = false;
          break;
        }
      }
    }
    else //northward/up movement
    {
      next = i - 13;
      for (next; next > f; next -= 13)
      {
        if (ia_grid[next] != 0)
        {
          path_clear = false; 
          break;
        }
      }
    }
    straight_path = true;
  }

  return (straight_path && path_clear);
}


//purpose: evaluate cannon movement
//cannons move like rooks/carts
//  but attack by leapfrogging over another piece
//initial cannon positions
//  N: 55, 61
//  S: 120, 126
bool Board::cannon_move(int i, int f)
{
  //is the final position an enemy piece?
  //if so, is there only one piece that separates the cannon
  //  from the enemy piece?
  bool attack = false;
  bool valid_path = collision_check(i, f);
  int cannon = ia_grid[i];

  return (attack || valid_path);
}


//purpose: evaluate pawn movement
//pawns only move forward
//if the pawn has crossed the river, 
//  they can move left and right
//initial pawn positions:
//N: 67, 69, 71, 73, 75
//S: 114, 112, 110, 108, 106

bool Board::pawn_move(int i, int f)
{
  bool valid = false;
  int pawn = ia_grid[i]; //tells you who's pawn via +/-

  if (pawn > 0) //north pawn
  {
    //forward movement is always valid || 
    //crossed river and moving left/right
    if ((f == i + 13) || 
      (((f == i + 1) || (f == i - 1)) && (i > 90)))
    {
      valid = true;
    }
  }
  else //south pawn
  {
    if ((f == i - 13) || 
      (((f == i + 1) || (f == i - 1)) && (i < 90)))
    {
      valid = true;
    }
  }

  return valid;

}

//make/delete new pieces for testing
void Board::make_piece(int pos, int piece)
{
  ia_grid[pos] = piece;
}