//board.cpp
//implementation file
#include <cmath>
#include <iostream>
#include "Board.h"
#include "Board_Defaults.h"

//purpose: validate elephant movement
//returns bool representing valid/invalid movement
bool Board::elephant_move(int i, int f)
{
  int movement = f - i;
  bool valid = false;
  //up-right
  if (movement == (2*UP + 2*RIGHT))
  {
    valid = true;
  }
  //up-left
  else if (movement == (2*UP + 2*LEFT))
  {
    valid = true;
  }
  //down-right
  else if (movement == (2*DOWN + 2*RIGHT))
  {
    valid = true;
  }
  //down-left
  else if (movement == (2*DOWN + 2*LEFT))
  {
    valid = true;
  }

  return valid;
}

//purpose: validate horse movement.
//returns bool representing valid/invalid movement
bool Board::horse_move(int i, int f)
{
  int movement = f - i;
  bool valid = false;
  //up
  if (movement == (2*UP + RIGHT) || movement == (2*UP + LEFT))
  {
    valid = (ia_grid[i + UP] == 0);
  }
  //down
  else if (movement == (2*DOWN + LEFT) || movement == (2*DOWN + RIGHT))
  {
    valid = (ia_grid[i + DOWN] == 0);
  }
  //left
  else if (movement == (2*LEFT + UP) || movement == (2*LEFT + DOWN))
  {
    valid = (ia_grid[i + LEFT] == 0);
  }
  //right
  else if (movement == (2*RIGHT + UP)|| movement == (2*RIGHT + DOWN))
  {
    valid = (ia_grid[i + RIGHT] == 0);
  }

  return valid;
}

//purpose: check for straight line, unobstructed movement
//  or attack
//returns: bool representing valid/invalid move
bool Board::cart_move(int i, int f)
{
  bool valid = false;
  if (straight_collision_check(i, f) == 0)
  {
    valid = true;
  }
  return valid;
}

//purpose: check for unit collision for cannons/carts
//returns: the index where there has been a collision or 0
int Board::straight_collision_check(int i, int f)
{
  int straight_path = false; //horizontal or vertical movement
  int collision = 0; //assume path is clear unless checks say otherwise
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
          collision = next; //collided
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
          collision = next; //collided
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
          collision = next;
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
          collision = next; 
          break;
        }
      }
    }
    straight_path = true;
  }

  //indicate that path is not straight
  if (!straight_path)
  {
    collision = -1;
  }

  return collision;
}

//purpose: evaluate cannon movement
//  cannons move like rooks/carts
//  determines movement by seeing if there is any
//    collision between the initial and final position
//    and that the final position is empty.
//  determines attack by seeing if there is
//    only 1 piece between the initial and final position
//    and that the final position is occupied by another piece
//returns: true for valid move, false for invalid move
bool Board::cannon_move(int i, int f)
{

  int collided = straight_collision_check(i, f);
  int rev_collided = straight_collision_check(f, i);
  bool move = ((collided == 0) && (ia_grid[f] == 0));

  bool attack = false;
  if ((ia_grid[f] != 0) && (collided > 0))
  {
    attack = (collided == rev_collided);
  }

  return (attack || move); //attacking or moving
}


//purpose: evaluate pawn movement
//pawns only move forward
//if the pawn has crossed the river, 
//  they can move left and right
//returns: true if valid move, false if invalid
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

//make new pieces for testing
void Board::make_piece(int pos, int piece)
{
  ia_grid[pos] = piece;
}

//remove piece from board
void Board::remove_piece(int pos)
{
  ia_grid[pos] = 0;
}

int Board::check_pos(int pos)
{
  return ia_grid[pos];
}