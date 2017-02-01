#include "catch.hpp"
#include "xiangqi/Board.h"
#include "xiangqi/Board.cpp"



TEST_CASE( "Pawn Movement Test", "pawn_move" )
{
  Board * nb_ptr;
  nb_ptr = new Board;
  
  //north-pawn south/forward movement
  REQUIRE( nb_ptr->pawn_move(67, 80) == true );
  //south-pawn north/forward movement
  REQUIRE( nb_ptr->pawn_move(106, 93) == true );

	int invalid_np_moves[] = {1, -1, -13, 30, -45};
	int invalid_sp_moves[] = {1, -1, 13, 30, -45};
  int n = sizeof(invalid_np_moves)/sizeof(int);
  for (int i = 0; i < n; i++)
  {
  	REQUIRE( nb_ptr->pawn_move(67, 
  		67 + invalid_np_moves[i]) == false );
  	REQUIRE( nb_ptr->pawn_move(106,
  	 106 + invalid_sp_moves[i]) == false );
  }

  //cross river
  int valid_cross_river_np_moves[] = {1, -1, 13};
  int valid_cross_river_sp_moves[] = {1, -1, -13};
  n = sizeof(valid_cross_river_np_moves)/sizeof(int);

  //make new pawns
  int new_np_pos = 124;
  nb_ptr->make_piece(new_np_pos, 2);
  int new_sp_pos = 59;
  nb_ptr->make_piece(new_sp_pos, -2);
  for (int i = 0; i < n; i++)
  {
  	REQUIRE( nb_ptr->pawn_move(new_np_pos,
  	  new_np_pos + valid_cross_river_np_moves[i]) == true);
  	REQUIRE( nb_ptr->pawn_move(new_sp_pos,
  		new_sp_pos + valid_cross_river_sp_moves[i]) == true);
  }

  //remove pawns
  nb_ptr->make_piece(new_np_pos, 0);
  nb_ptr->make_piece(new_sp_pos, 0);

  delete[] nb_ptr;

}


TEST_CASE( "Collision Check Test", "collision_check" )
{
	Board * nb_ptr;
	nb_ptr = new Board;

	//cannon as example piece. mobile and in an easy starting position
	//right, north-left cannon to right
	REQUIRE( nb_ptr->collision_check(55, 60) == true );
	//left, north-right cannon to left
	REQUIRE( nb_ptr->collision_check(60, 56) == true );
	//up, south-right cannon to up
	REQUIRE( nb_ptr->collision_check(126, 74) == true );
	//down, north-left cannon to down
	REQUIRE( nb_ptr->collision_check(55, 107) == true );

	//failing tests
	//right-ish
	REQUIRE( nb_ptr->collision_check(55, 65) == false );
	//left-ish
	REQUIRE( nb_ptr->collision_check(61, 71) == false );
	//up-ish
	REQUIRE( nb_ptr->collision_check(126, 54) == false );
	//down-ish
	REQUIRE( nb_ptr->collision_check(61, 125) == false );
	//attempt to move past another piece, up
	REQUIRE( nb_ptr->collision_check(126, 48) == false );
	//attempt to move past another piece, down
	REQUIRE( nb_ptr->collision_check(61, 139) == false );
	//attempt to move past another piece, right
	REQUIRE( nb_ptr->collision_check(55, 62) == false );
	//attempt to move past another piece, left
	REQUIRE( nb_ptr->collision_check(61, 54) == false );

	delete[] nb_ptr;

}
