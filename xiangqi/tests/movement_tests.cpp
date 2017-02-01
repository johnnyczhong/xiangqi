#include "catch.hpp"
#include "xiangqi/Board.h"
#include "xiangqi/Board.cpp"
#include "xiangqi/Board_Defaults.h"

TEST_CASE( "Make and Remove Pieces", "make_piece remove_piece" )
{
	Board nb;

	//make piece, confirm
	int new_piece_pos = 80;
	nb.make_piece(new_piece_pos, PAWN);
	REQUIRE( (nb.check_pos(new_piece_pos) == PAWN) == true );

	//remove piece, confirm
	nb.remove_piece(new_piece_pos);
	REQUIRE( (nb.check_pos(new_piece_pos) == UNOCCUPIED) == true);

}


TEST_CASE( "Pawn Movement Test", "pawn_move" )
{
  Board nb;
  
  //north-pawn down movement
  REQUIRE( nb.pawn_move(n_pawn_pos[0],  n_pawn_pos[0] + DOWN) == true );
  //south-pawn up movement
  REQUIRE( nb.pawn_move(s_pawn_pos[0], s_pawn_pos[0] + UP) == true );

  int invalid_np_moves[] = {RIGHT, LEFT, UP, 30, -45};
  int invalid_sp_moves[] = {RIGHT, LEFT, DOWN, 30, -45};
  int n = sizeof(invalid_np_moves)/sizeof(int);
  for (int i = 0; i < n; i++)
  {
  	REQUIRE( nb.pawn_move(67, 
  		67 + invalid_np_moves[i]) == false );
  	REQUIRE( nb.pawn_move(106,
  	 106 + invalid_sp_moves[i]) == false );
  }

  //cross river
  int valid_cross_river_np_moves[] = {RIGHT, LEFT, DOWN};
  int valid_cross_river_sp_moves[] = {RIGHT, LEFT, UP};
  n = sizeof(valid_cross_river_np_moves)/sizeof(int);

  //make new pawns
  int new_np_pos = 124;
  nb.make_piece(new_np_pos, PAWN);
  int new_sp_pos = 59;
  nb.make_piece(new_sp_pos, -PAWN);
  for (int i = 0; i < n; i++)
  {
  	REQUIRE( nb.pawn_move(new_np_pos,
  	  new_np_pos + valid_cross_river_np_moves[i]) == true);
  	REQUIRE( nb.pawn_move(new_sp_pos,
  		new_sp_pos + valid_cross_river_sp_moves[i]) == true);
  }

  //remove pawns
  nb.remove_piece(new_np_pos);
  nb.remove_piece(new_sp_pos);

}


TEST_CASE( "Collision Check Test", "straight_collision_check" )
{
	Board nb;

	//cannon as example piece. mobile and in an easy starting position
	//right, north-left cannon to right
	REQUIRE( (nb.straight_collision_check(NW_CANNON, NW_CANNON + 4*RIGHT) == 0) == true );
	//left, north-right cannon to left
	REQUIRE( (nb.straight_collision_check(NE_CANNON, NE_CANNON + 4*LEFT) == 0) == true );
	//up, south-right cannon to up
	REQUIRE( (nb.straight_collision_check(SE_CANNON, SE_CANNON + 4*UP) == 0) == true );
	//down, north-left cannon to down
	REQUIRE( (nb.straight_collision_check(NW_CANNON, NW_CANNON + 4*DOWN) == 0) == true );

	//failing tests
	//right-ish
	REQUIRE( (nb.straight_collision_check(NW_CANNON, NW_CANNON + 4*RIGHT + DOWN) == -1) == true );
	//left-ish
	REQUIRE( (nb.straight_collision_check(NE_CANNON, NE_CANNON + 4*LEFT + UP) == -1) == true );
	//up-ish
	REQUIRE( (nb.straight_collision_check(SE_CANNON, SE_CANNON + 3*UP + LEFT) == -1) == true );
	//down-ish
	REQUIRE( (nb.straight_collision_check(NE_CANNON, NE_CANNON + 3*DOWN + LEFT) == -1) == true );
	//attempt to move past another piece, up
	REQUIRE( (nb.straight_collision_check(SE_CANNON, NE_HORSE) == NE_CANNON) == true );
	//attempt to move past another piece, down
	REQUIRE( (nb.straight_collision_check(NE_CANNON, SE_HORSE) == SE_CANNON) == true );
	//attempt to move past another piece, right
	REQUIRE( (nb.straight_collision_check(NW_CANNON, NE_CANNON + RIGHT) == NE_CANNON) == true );
	//attempt to move past another piece, left
	REQUIRE( (nb.straight_collision_check(NE_CANNON, NW_CANNON + LEFT) == NW_CANNON) == true );

}

TEST_CASE( "Cannon Move or Attack Test", "cannon_move" )
{
	Board nb;

	/*
	=== Passing Tests ===
	*/

	//move left
	REQUIRE( nb.cannon_move(NE_CANNON, NE_CANNON + 5*LEFT) == true );
	//move right
	REQUIRE( nb.cannon_move(NW_CANNON, NW_CANNON + 5*RIGHT) == true );
	//move up
	REQUIRE( nb.cannon_move(SE_CANNON, SE_CANNON + 4*UP) == true );
	//move down
	REQUIRE( nb.cannon_move(NW_CANNON, NW_CANNON + 4*DOWN) == true );

	//make dummy cannon in between enemy pawns 3 and 4
	int dummy_cannon_pos = 72;
	nb.make_piece(dummy_cannon_pos, -CANNON);

	//=== ATTACKS ===

	//attack enemy piece left
	REQUIRE( nb.cannon_move(dummy_cannon_pos, n_pawn_pos[1]) == true );
	//attack enemy piece right
	REQUIRE( nb.cannon_move(dummy_cannon_pos, n_pawn_pos[4]) == true );

	//remove dummy cannnon
	nb.remove_piece(dummy_cannon_pos);

	//attack enemy piece up
	REQUIRE( nb.cannon_move(SE_CANNON, NE_HORSE) == true );
	//attack enemy piece down
	REQUIRE( nb.cannon_move(NE_CANNON, SE_HORSE) == true );

	/*
	=== Failing Tests ===
	*/

	//move diagonally up-left
	REQUIRE( nb.cannon_move(SE_CANNON, (SE_CANNON + 4*LEFT + 2*UP)) == false );

	//move diagonally up-right
	REQUIRE( nb.cannon_move(SE_CANNON, (SE_CANNON + 3*UP + RIGHT)) == false );

	//move diagonally down-left
	REQUIRE( nb.cannon_move(NE_CANNON, (NE_CANNON + 3*DOWN + LEFT)) == false );
	
	//move diagonally down-right
	REQUIRE( nb.cannon_move(NE_CANNON, (NE_CANNON + 3*DOWN + RIGHT)) == false );

	int new_s_cannon_pos = SE_CANNON + UP; 
	nb.make_piece(new_s_cannon_pos, -CANNON);
	//attack ally
	REQUIRE( nb.cannon_move(new_s_cannon_pos, s_pawn_pos[2]) == false );
	nb.remove_piece(new_s_cannon_pos);

	//attack enemy without leapfrogging
	REQUIRE( nb.cannon_move(SW_CANNON, NW_CANNON) == false );

	//attack ally without leapfrogging
	REQUIRE( nb.cannon_move(SW_CANNON, SW_HORSE) == false );

	//attack diagonally
	REQUIRE( nb.cannon_move(SE_CANNON, n_pawn_pos[4]) == false );
}
