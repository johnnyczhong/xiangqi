//board.cpp
//implementation file
#include <cmath>
#include <iostream>
#include "Board.h"

#define DEBUG 1



bool Board::get_in_check(int g)
{
  return (m_in_check[g]);
}

//end of turn declaration
//determines if the move declared would 
//  put the current player in check (self-check)
//  and if the opponent would be in check
//  if the opponent would be in check, looks for
//    valid moves or ways to leave check.
//    if none are found, declare checkmate

//generals are in check if under threat
//determine threat by making mock attacks against other pieces
//if mock attack results in finding a piece that could make an identical
//  attack, then the general is under threat (in-check)
void Board::set_in_check()
{
  m_in_check[NORTH] = evaluate_threat(m_general_pos[NORTH], NORTH);
  m_in_check[SOUTH] = evaluate_threat(m_general_pos[SOUTH], SOUTH);
}

bool Board::evaluate_threat(int gen_pos, int player)
{
  //make mock attacks against opposing pieces using the gen_pos
  //  as origin point
  return (pawn_threat(gen_pos, player) || 
    straight_path_threat(gen_pos, player) || 
    horse_threat(gen_pos, player) ||
    general_threat(gen_pos, player));
}

bool Board::general_threat(int gen_pos, int player)
{
  int x_offset = (gen_pos - ORIGIN) % DOWN;
  bool general_threat = false;

  if (player == 1) //NORTH
  {
    general_threat = 
      (ia_grid[straight_collision_check(gen_pos, ORIGIN_SOUTH + x_offset)] 
        == -GENERAL);
  }
  else
  {
    general_threat = 
      (ia_grid[straight_collision_check(gen_pos, ORIGIN_NORTH + x_offset)] 
        == GENERAL);    
  }
  return general_threat;
}

bool Board::horse_threat(int gen_pos, int player)
{
  //make mock attacks in the horse pattern
  // find possible threat, determine if it's blocked.
  //if the space 1 diagonal away is not empty (0),
  //  then the threat from the horse is blocked
  //starting from top-left, running clockwise
  bool horse_threat = false;
  int enemy_horse = player * -HORSE;

  int blockers[] = {UP+LEFT, UP+RIGHT, DOWN+RIGHT, DOWN+LEFT};
  int horse_attack_vectors[] = 
  {
    (2*LEFT + UP), (2*UP + LEFT), // blockers[0]
    (2*UP + RIGHT), (2*RIGHT + UP), // blockers[1]
    (2*RIGHT + DOWN), (2*DOWN + RIGHT), // blockers[2]
    (2*DOWN + LEFT), (2*LEFT + DOWN) // blockers[3]
  };

  int n = sizeof(blockers)/sizeof(int);
  int blocker_check;
  int horse_pos0;
  int horse_pos1;

  for (int i = 0; i < n; i++)
  {
    blocker_check = gen_pos + blockers[i];
    if (ia_grid[abs(blocker_check)] == UNOCCUPIED) // no blocker
    {
      horse_pos0 = horse_attack_vectors[2*i] + gen_pos;
      horse_pos1 = horse_attack_vectors[(2*i) + 1] + gen_pos;
      if (ia_grid[horse_pos0] == enemy_horse 
        || ia_grid[horse_pos1] == enemy_horse) 
        //determine if there's a horse in the threat spots
      {
        horse_threat = true;
        break;
      }
    }
  }

  return (horse_threat);
}

//checks for cart and cannon threats
bool Board::straight_path_threat(int gen_pos, int player)
{
  bool cart_threat = false;
  bool cannon_threat = false;

  int offset = gen_pos - ORIGIN; //calculate relative position from top-left corner
  int x_offset = offset % DOWN; //how far to the right
  int y_offset = offset / DOWN; //how far down
  int west_limit = ORIGIN_WEST + (y_offset * DOWN);
  int east_limit = ORIGIN_EAST + (y_offset * DOWN);
  int north_limit = ORIGIN_NORTH + x_offset;
  int south_limit = ORIGIN_SOUTH + x_offset;
  int four_directions[] = {west_limit, east_limit, north_limit, south_limit};
  int n = sizeof(four_directions)/sizeof(int);

  int collided_position;
  int next_collision;

  for (int i = 0; i < n; i++)
  {
    collided_position = straight_collision_check(gen_pos, four_directions[i]);

    if (ia_grid[collided_position] == (player * -CART))
    {
      //carts represent immediate threat
      cart_threat = true;
      break;
    }
    else if (ia_grid[collided_position] != 0)
    {
      //cannons need something to jump over
      next_collision = straight_collision_check(collided_position, four_directions[i]);
      if (ia_grid[next_collision] == (player * -CANNON))
      {
        cannon_threat = true;
        break;
      }
    }
  }

  return (cart_threat || cannon_threat);
}

bool Board::pawn_threat(int gen_pos, int player)
{
  bool pawn_threat = false;
  int forward = player * DOWN; //forward relative to general/player perspective
  int pawn_threat_pos[] = {LEFT, RIGHT, forward};
  int n = sizeof(pawn_threat_pos)/sizeof(int);
  for (int i = 0; i < n; i++)
  {
    if (ia_grid[gen_pos + i] == (player * -PAWN))
    {
      pawn_threat = true;
      break;
    }
  }

  return pawn_threat;
}

void Board::flip_turn()
{
  turn *= -1; 
}

int Board::get_general_pos(int player)
{
  return m_general_pos[player];
}

void Board::update_general_pos(int player, int pos)
{
  m_general_pos[player] = pos;
}

bool Board::eval_dest(int i, int f)
{
  bool valid;
  if (ia_grid[i] > 0) //north
  {
    //extra check here because -1 is an invalid space
    valid = (ia_grid[f] <= 0) && (ia_grid[f] != -1);
  }
  else
  {
    valid = (ia_grid[f] >= 0);
  }
  return valid;
}

bool Board::eval_move(int i, int f)
{
  //see if piece collides with allied piece
  // or leaves board
  int piece = ia_grid[i];
  bool valid_dest = eval_dest(i, f);

  //determine what the piece is 
  //and determine if the move is valid
  bool valid_path;
  switch (piece)
  {
    case PAWN: valid_path = pawn_move(i, f);
               break;
    case CANNON: valid_path = cannon_move(i, f);
                 break;
    case CART: valid_path = cart_move(i, f);
               break;
    case HORSE: valid_path = horse_move(i, f);
                break;
    case ELEPHANT: valid_path = elephant_move(i, f);
                   break;
    case GUARD: valid_path = guard_move(i, f);
                break;
    case GENERAL: valid_path = general_move(i, f);
                  break;
  }

  return (valid_path && valid_dest);
}

bool Board::general_move(int i, int f)
{
  int movement = f - i;
  int general = ia_grid[i];

  bool not_gvg = obstructed_generals(f);
  bool in_boundary = false;
  bool valid = 
  (
    (movement == UP) || (movement == DOWN) ||
    (movement == RIGHT) || (movement == LEFT)
  );

  if (general > 0)
  {
    in_boundary = n_camp_box_check(f);
  }
  else
  {
    in_boundary = s_camp_box_check(f);
  }

  return (in_boundary && valid && not_gvg);
}

//determines if the position the general
//is moving to will initiate a "check" status
bool Board::obstructed_generals(int f)
{
  int e_pos;
  if (turn) //north's turn
  {
    e_pos = get_general_pos(-1); //get south general position
  }
  else
  {
    e_pos = get_general_pos(1);
  }

  //if there is a collision (before hitting the other general)
  //or if the path is not straight, return true
  int collision = straight_collision_check(f, e_pos);

  return (collision != 0);
}

bool Board::guard_move(int i, int f)
{
  int movement = f - i;
  int guard = ia_grid[i];
  bool in_boundary = false;
  //movement allowed: up-right, up-left, down-right, down-left
  bool valid = 
  (
    (movement == UP + RIGHT) || (movement == UP + LEFT) ||
    (movement == DOWN + RIGHT) || (movement == DOWN + LEFT)
  );

  if (guard > 0) //north guard
  {
    in_boundary = n_camp_box_check(f);
  }
  else //south guard
  {
    in_boundary = s_camp_box_check(f);
  }

  return (in_boundary && valid);
}

bool Board::n_camp_box_check(int f)
{
  bool first_row = (f >= NW_GUARD && f <= NE_GUARD);
  bool second_row = (f >= (NW_GUARD + DOWN) && f <= (NE_GUARD + DOWN));
  bool third_row = (f >= (NW_GUARD + 2*DOWN) && f <= (NE_GUARD + 2*DOWN));
  return (first_row || second_row || third_row);
}

bool Board::s_camp_box_check(int f)
{
  bool first_row = (f >= SW_GUARD && f <= SE_GUARD);
  bool second_row = (f >= (SW_GUARD + UP) && f <= (SE_GUARD + UP));
  bool third_row = (f >= (SW_GUARD + 2*UP) && f <= (SE_GUARD + 2*UP));
  return (first_row || second_row || third_row);
}


//purpose: validate elephant movement
//returns bool representing valid/invalid movement
bool Board::elephant_move(int i, int f)
{
  int elephant = ia_grid[i]; //identify elephant side
  int movement = f - i;
  bool valid = false;

  //elephant must stay on its own side
  if ((elephant > 0 && f < 90) || (elephant < 0 && f > 90))
  {
    //up-right
    if (movement == (2*UP + 2*RIGHT))
    {
      valid = (ia_grid[i + UP + RIGHT] == 0);
    }
    //up-left
    else if (movement == (2*UP + 2*LEFT))
    {
      valid = (ia_grid[i + UP + LEFT] == 0);
    }
    //down-right
    else if (movement == (2*DOWN + 2*RIGHT))
    {
      valid = (ia_grid[i + DOWN + RIGHT] == 0);
    }
    //down-left
    else if (movement == (2*DOWN + 2*LEFT))
    {
      valid = (ia_grid[i + DOWN + LEFT] == 0);
    }
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

void Board::move_piece(int i, int f)
{
  int p = check_pos(i);
  make_piece(f, p);
  remove_piece(i);

  if (abs(p) == GENERAL)
  {
    int player = p/GENERAL;
    update_general_pos(player, f);
  }
}

int Board::check_pos(int pos)
{
  return ia_grid[pos];
}