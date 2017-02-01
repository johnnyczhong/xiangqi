#include "catch.hpp"
#include "xiangqi/Board.h"
#include "xiangqi/Board.cpp"



TEST_CASE( "Pawn Movement Test", "pawn_move" )
{
  Board newboard;
  
  //north-pawn south/forward movement
  REQUIRE( newboard.pawn_move(67, 80) == true );
  //south-pawn north/forward movement
  REQUIRE( newboard.pawn_move(106, 93) == true );

	int invalid_np_moves[] = {1, -1, -13, 30, -45};
	int invalid_sp_moves[] = {1, -1, 13, 30, -45};
  int n = sizeof(invalid_np_moves)/sizeof(int);
  for (int i = 0; i < n; i++)
  {
  	REQUIRE( newboard.pawn_move(67, 
  		67 + invalid_np_moves[i]) == false );
  	REQUIRE( newboard.pawn_move(106,
  	 106 + invalid_sp_moves[i]) == false );
  }

  //cross river
  int valid_cross_river_np_moves[] = {1, -1, 13};
  int valid_cross_river_sp_moves[] = {1, -1, -13};
  n = sizeof(valid_cross_river_np_moves)/sizeof(int);

  //make new pawns
  int new_np_pos = 124;
  newboard.make_piece(new_np_pos, 2);
  int new_sp_pos = 59;
  newboard.make_piece(new_sp_pos, -2);
  for (int i = 0; i < n; i++)
  {
  	REQUIRE( newboard.pawn_move(new_np_pos,
  	  new_np_pos + valid_cross_river_np_moves[i]) == true);
  	REQUIRE( newboard.pawn_move(new_sp_pos,
  		new_sp_pos + valid_cross_river_sp_moves[i]) == true);
  }

  //remove pawns
  newboard.make_piece(new_np_pos, 0);
  newboard.make_piece(new_sp_pos, 0);

}

/* TODO:
TEST_CASE( "Collision Check Test", "collision_check" )
{
	Board newboard;

}
*/