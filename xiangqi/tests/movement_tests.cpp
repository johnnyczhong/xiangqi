#include "catch.hpp"
#include "xiangqi/Board.cpp"

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

	// === Passing Movement Tests ===
	
	//a bit redundant due to same tests in collision
	REQUIRE( nb.cannon_move(NE_CANNON, NE_CANNON + 5*LEFT) == true );
	REQUIRE( nb.cannon_move(NW_CANNON, NW_CANNON + 5*RIGHT) == true );
	REQUIRE( nb.cannon_move(SE_CANNON, SE_CANNON + 4*UP) == true );
	REQUIRE( nb.cannon_move(NW_CANNON, NW_CANNON + 4*DOWN) == true );

	//make dummy cannon in between enemy pawns 3 and 4
	int dummy_cannon_pos = 72;
	nb.make_piece(dummy_cannon_pos, -CANNON);

	// === Passing Attacking Tests ===

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

	
	// === Failing Movement Tests ===

	//a bit redundant, but just keep since it's already written
	REQUIRE( nb.cannon_move(SE_CANNON, (SE_CANNON + 4*LEFT + 2*UP)) == false );
	REQUIRE( nb.cannon_move(SE_CANNON, (SE_CANNON + 3*UP + RIGHT)) == false );
	REQUIRE( nb.cannon_move(NE_CANNON, (NE_CANNON + 3*DOWN + LEFT)) == false );
	REQUIRE( nb.cannon_move(NE_CANNON, (NE_CANNON + 3*DOWN + RIGHT)) == false );

	// === Failing Attack Tests ===

	//attack enemy without leapfrogging
	REQUIRE( nb.cannon_move(SW_CANNON, NW_CANNON) == false );

	//attack ally without leapfrogging
	REQUIRE( nb.cannon_move(SW_CANNON, SW_HORSE) == false );

	//attack diagonally
	REQUIRE( nb.cannon_move(SE_CANNON, n_pawn_pos[4]) == false );
}

//cart tests significantly simpler
//due move and attack being the same action
TEST_CASE( "Cart Move or Attack Test", "cart_move" )
{
	Board nb;

	nb.make_piece(s_pawn_pos[4] + LEFT, -CART);
	//attack enemy piece
	REQUIRE( nb.cart_move(s_pawn_pos[4] + LEFT, NE_CANNON) == true );
	//fail to attack over obstruction
	REQUIRE( nb.cart_move(SE_CART, n_pawn_pos[4]) == false );
}

TEST_CASE( "Horse Movement and Collision", "horse_move" )
{
	Board nb;
	//valid positions are initial + (+/-27, +/-25, +/-11, +/-15)
	//absolute then relative

	//up, up, left: -25
	REQUIRE( nb.horse_move(SW_HORSE, SW_CANNON + LEFT) == true );
	REQUIRE( nb.horse_move(SW_HORSE, SW_HORSE + 2*UP + LEFT) == true );
	//up, up, right: -27
	REQUIRE( nb.horse_move(SE_HORSE, SE_CANNON + RIGHT) == true );
	REQUIRE( nb.horse_move(SE_HORSE, SE_HORSE + 2*UP + RIGHT) == true );
	//down, down, left: +25
	REQUIRE( nb.horse_move(NE_HORSE, NE_CANNON + LEFT) == true );
	REQUIRE( nb.horse_move(NE_HORSE, NE_HORSE + 2*DOWN + LEFT) == true );
	//down, down, right: +27
	REQUIRE( nb.horse_move(NW_HORSE, NW_CANNON + RIGHT) == true );
	REQUIRE( nb.horse_move(NW_HORSE, NW_HORSE + 2*DOWN + RIGHT) == true );
	
	//make a dummy horse for these. replace an existing pawn.
	int test_horse_pos = n_pawn_pos[3];
	nb.make_piece(test_horse_pos, HORSE);

	//right, right, down: +15
	REQUIRE( nb.horse_move(test_horse_pos, n_pawn_pos[4] + DOWN) == true );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*RIGHT + DOWN) == true );
	//right, right, up: -11
	REQUIRE( nb.horse_move(test_horse_pos, n_pawn_pos[4] + UP) == true );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*RIGHT + UP) == true );
	//left, left, up: -15
	REQUIRE( nb.horse_move(test_horse_pos, n_pawn_pos[2] + UP) == true );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*LEFT + UP) == true );
	//left, left, down: +11
	REQUIRE( nb.horse_move(test_horse_pos, n_pawn_pos[2] + DOWN) == true );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*LEFT + DOWN) == true );

	//horses follow obstruction on 1st space moved along the long path
	//  ie. if the horse moves up, up, right the first space up must be empty

	//blocking upward movement
	nb.make_piece(test_horse_pos + UP, PAWN);
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*UP + RIGHT) == false );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*UP + LEFT) == false );	

	//blocking downward movement
	nb.make_piece(test_horse_pos + DOWN, PAWN);
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*DOWN + RIGHT) == false );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*DOWN + LEFT) == false );

	//block left movement
	nb.make_piece(test_horse_pos + LEFT, PAWN);
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*LEFT + UP) == false );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*LEFT + DOWN) == false );

	//block right movement
	nb.make_piece(test_horse_pos + RIGHT, PAWN);
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*RIGHT + UP) == false );
	REQUIRE( nb.horse_move(test_horse_pos, test_horse_pos + 2*RIGHT + DOWN) == false );
}

TEST_CASE( "Elephant Movement", "elephant_move" )
{
	Board nb;

	//elephants only move 2 spaces diagonally
	//4 directions, upleft, upright, downleft, downright
	//cannot cross river
	//both elephants have 7 possible positions

	//2 down, 2 right
	REQUIRE( nb.elephant_move(NE_ELEPHANT, NE_ELEPHANT + (2*DOWN + 2*RIGHT)) == true );
	//2 down, 2 left
	REQUIRE( nb.elephant_move(NE_ELEPHANT, NE_ELEPHANT + (2*DOWN + 2*LEFT)) == true );
	//2 up, 2 left
	REQUIRE( nb.elephant_move(SE_ELEPHANT, SE_ELEPHANT + (2*UP + 2* LEFT)) == true );
	//2 up, 2 right
	REQUIRE( nb.elephant_move(SE_ELEPHANT, SE_ELEPHANT + (2*UP + 2*RIGHT)) == true );
}

TEST_CASE( "Elephant Collision", "elephant_move")
{
	Board nb;
	//blocking with a piece in the middle
	//make a piece, then test, then delete intervening piece

	//2 down, 2 right
	nb.make_piece(NE_ELEPHANT + DOWN + RIGHT, PAWN);
	REQUIRE( nb.elephant_move(NE_ELEPHANT, NE_ELEPHANT + (2*DOWN + 2*RIGHT)) == false );
	nb.remove_piece(NE_ELEPHANT + DOWN + RIGHT);

	//2 down, 2 left
	nb.make_piece(NE_ELEPHANT + DOWN + LEFT, PAWN);
	REQUIRE( nb.elephant_move(NE_ELEPHANT, NE_ELEPHANT + (2*DOWN + 2*LEFT)) == false );
	nb.remove_piece(NE_ELEPHANT + DOWN + LEFT);

	//2 up, 2 left
	nb.make_piece(SE_ELEPHANT + UP + LEFT, PAWN);
	REQUIRE( nb.elephant_move(SE_ELEPHANT, SE_ELEPHANT + (2*UP + 2* LEFT)) == false );
	nb.remove_piece(NE_ELEPHANT + UP + LEFT);

	//2 up, 2 right
	nb.make_piece(SE_ELEPHANT + UP + RIGHT, PAWN);
	REQUIRE( nb.elephant_move(SE_ELEPHANT, SE_ELEPHANT + (2*UP + 2*RIGHT)) == false );
	nb.remove_piece(NE_ELEPHANT + UP + RIGHT);
}

TEST_CASE( "Blocking Elephant River Crossing", "elephant_move")
{
	Board nb;

	//make elephant, put it in proper locations, 
	// and put it on a riverbank
	int ne = n_pawn_pos[1] + LEFT; //north elephant
	nb.make_piece(ne, ELEPHANT);
	//attempt to cross river
	REQUIRE( nb.elephant_move(ne, ne + (2*DOWN + 2*RIGHT)) == false );
	nb.remove_piece(ne); //clean-up

	int se = s_pawn_pos[1] + LEFT;
	nb.make_piece(se, -ELEPHANT);
	REQUIRE( nb.elephant_move(se, se + (2*UP + 2*RIGHT)) == false );
	nb.remove_piece(se);
}

TEST_CASE( "Guard Diagonal Movement", "guard_move" )
{
	Board nb;

	//guards can only move/attack diagonally
	//  1 space, confined within the box
	//only have 5 possible positions

	//up-left
	REQUIRE( nb.guard_move(SE_GUARD, SE_GUARD + UP + LEFT) == true );
	//up-right
	REQUIRE( nb.guard_move(SW_GUARD, SW_GUARD + UP + RIGHT) == true );
	//down-left
	REQUIRE( nb.guard_move(NE_GUARD, NE_GUARD + DOWN + LEFT) == true );
	//down-right
	REQUIRE( nb.guard_move(NW_GUARD, NW_GUARD + DOWN + RIGHT) == true );

	//move up, down, left, right
	//make guard
	int new_guard = N_GENERAL + DOWN;
	nb.make_piece(new_guard, GUARD);

	//these should fail
	REQUIRE( nb.guard_move(new_guard, new_guard + UP) == false );
	REQUIRE( nb.guard_move(new_guard, new_guard + DOWN) == false );
	REQUIRE( nb.guard_move(new_guard, new_guard + RIGHT) == false );
	REQUIRE( nb.guard_move(new_guard, new_guard + LEFT) == false );
}

TEST_CASE( "Camp Boundaries Check", "n_camp_box_check and s_camp_box_check")
{
	Board nb;

	// === Successful Tests ===
	//check if functions confirm that the positions given are valid
	REQUIRE( nb.n_camp_box_check(NE_GUARD) == true );
	REQUIRE( nb.n_camp_box_check(N_GENERAL) == true );
	REQUIRE( nb.n_camp_box_check(NE_GUARD + 2*DOWN) == true );
	REQUIRE( nb.s_camp_box_check(S_GENERAL + 2*UP) == true);

	// === Failing Tests ===
	//check if functions confirm that the positions given are invalid
	REQUIRE( nb.s_camp_box_check(S_GENERAL + 3*UP) == false );
	REQUIRE( nb.s_camp_box_check(SE_GUARD + RIGHT) == false );
}

TEST_CASE( "Guard Constricted Movement Tests", "guard_move" )
{
	Board nb;

	//confine within 3x3 box
	//check 4 corners of box exit
	REQUIRE( nb.guard_move(SE_GUARD, SE_GUARD + UP + RIGHT) == false );
	REQUIRE( nb.guard_move(SW_GUARD, SW_GUARD + UP + LEFT) == false );

	//make guards on the top corners
	int top_right_guard = SE_GUARD + 2*UP;
	int top_left_guard = SW_GUARD + 2*UP;
	nb.make_piece(top_right_guard, GUARD);
	nb.make_piece(top_left_guard, GUARD);

	//check exit from these corners
	//block exit
	REQUIRE( nb.guard_move(top_right_guard, top_right_guard + UP + RIGHT) == false );
	REQUIRE( nb.guard_move(top_left_guard, top_left_guard + UP + LEFT) == false );

	// GUARDS CAN'T FLY TO THE OTHER ENCAMPMENT!
	REQUIRE( nb.guard_move(SE_GUARD, NE_GUARD + DOWN) == false );
}

TEST_CASE( "General Movement Tests", "general_move" )
{
	// generals can only move 1 square: up, down, left, or right
	// they can only stay within their encampment, like guards

	Board nb;

	//move a general into the middle empty space
	int general_pos = N_GENERAL + DOWN;
	nb.make_piece(general_pos, GENERAL);
	nb.remove_piece(N_GENERAL);

	REQUIRE( nb.general_move(general_pos, general_pos + UP) == true );
	REQUIRE( nb.general_move(general_pos, general_pos + DOWN) == true );
	REQUIRE( nb.general_move(general_pos, general_pos + RIGHT) == true );
	REQUIRE( nb.general_move(general_pos, general_pos + LEFT) == true );

	//move more than 1 space
	REQUIRE( nb.general_move(general_pos, general_pos + 2*UP) == false );
	REQUIRE( nb.general_move(general_pos, general_pos + 2*LEFT) == false );

	//move the general down another space
	//move out of encampment box
	nb.remove_piece(general_pos);
	general_pos += DOWN; //down 1 more space
	nb.make_piece(general_pos, GENERAL); //spawn at the border
	REQUIRE( nb.general_move(general_pos, general_pos + DOWN) == false ); //attempt to move out

}

//might be better to load this onto the post-move check
//allow player to attempt any move
//run a check after the move is declared
//  and invalidate the move if it's declared illegal.
//technically any move could put the general into the "check" status,
//  so all moves must be checked

TEST_CASE( "General Check Status", "set_in_check and get_in_check" )
{
  Board nb;

  //after every move, pretend that the general is every offensive piece
  //  ie. pawn, horse, cart, cannon
  //then make mock attacks against those spaces and determine 
  //if there is that piece is at the destination
  //except horses. horses have a blocking mechanic that only works one way.
  //  make an unobstructed mock attack in all directions
  //  if there's a horse there, then determine if the horse could make a valid attack

  //starting positions
  REQUIRE( nb.get_in_check(NORTH) == false );
  REQUIRE( nb.get_in_check(SOUTH) == false );

  //move general forward/down 1 space
  int n = nb.get_general_pos(1);
  nb.make_piece(n + DOWN, GENERAL);
  nb.remove_piece(n);
  nb.update_general_pos(NORTH, n + DOWN);
  n = nb.get_general_pos(NORTH);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false );
  

  // === START PAWN TEST ===

  //spawn enemy pawn in threat position
  int ep = n + RIGHT;
  nb.make_piece(ep, -PAWN);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == true );
  REQUIRE( nb.get_in_check(SOUTH) == false );

  //remove enemy pawn, recheck
  nb.remove_piece(ep);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false );

  // === END PAWN TEST ===

  // === BEGIN CART TEST ===

  //spawn enemy cart/rook left-horizontal of the general's position
  nb.make_piece(n + 3*LEFT, -CART);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == true );
  //remove cart
  nb.remove_piece(n + 3*LEFT);

  //spawn enemy cart/rook north of south general position
  int nc = S_GENERAL + 2*UP;
  nb.make_piece(nc, CART);

  //recheck check status
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false );
  REQUIRE( nb.get_in_check(SOUTH) == true ); 

  nb.remove_piece(nc);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(SOUTH) == false );

  // === END CART TEST ===


  // === BEGIN CANNON TEST ===

  //spawn enemy cannon in front of central pawn
  int ec = n_pawn_pos[2] + DOWN;
  nb.make_piece(ec, -CANNON);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == true );
  nb.remove_piece(ec);

  //recheck check status
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false );

  // === END CANNON TEST === 
}


TEST_CASE ( "Horse Threat Test", "horse_threat" )
{

  Board nb;

  // === START HORSE TEST ===

  //spawn enemy horse in threat position
  int h = nb.get_general_pos(NORTH) + 2*DOWN + RIGHT;
  nb.make_piece(h, -HORSE);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == true );

  //block enemy horse with allied pawn
  int p = h + UP;
  nb.make_piece(p, PAWN);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false );

  //remove enemy horse
  nb.remove_piece(h);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false);

  //remove pawn
  nb.remove_piece(p);
  nb.set_in_check();
  REQUIRE( nb.get_in_check(NORTH) == false);

  // === END HORSE TEST ===
}

TEST_CASE( "Piece movement", "move_piece" )
{
  Board b;

  //confirm cart is in correct location
  REQUIRE( (b.check_pos(NE_CART) == CART) == true );

  //move cart DOWN 2 spaces
  b.move_piece(NE_CART, NE_CART + 2*DOWN);

  //make sure the cart is in the correct position
  REQUIRE( (b.check_pos(NE_CART + 2*DOWN) == CART) == true );

  //make sure the old position is now empty
  REQUIRE( (b.check_pos(NE_CART) == UNOCCUPIED) == true );
}

TEST_CASE( "General vs General", "general_threat" )
{
  Board b;

  //confirm that the generals are not currently in check
  b.set_in_check();
  REQUIRE( b.get_in_check(NORTH) == false );
  REQUIRE( b.get_in_check(SOUTH) == false );

  //moving generals into a threatening position
  b.move_piece(S_GENERAL, S_GENERAL + UP + RIGHT);
  b.move_piece(N_GENERAL, N_GENERAL + DOWN + RIGHT);

  //both generals should be in check
  b.set_in_check();
  REQUIRE( b.get_in_check(NORTH) == true );
  REQUIRE( b.get_in_check(SOUTH) == true );

  //introduce a blocking piece
  b.move_piece(SW_GUARD, SW_GUARD + 2*UP + 2*RIGHT);

  //confirm that the generals are no longer in check
  b.set_in_check();
  REQUIRE( b.get_in_check(NORTH) == false );
  REQUIRE( b.get_in_check(SOUTH) == false );

}


TEST_CASE( "Movement Validation", "eval_move" )
{
	Board nb;
	//goal here to perform movement checking on all pieces
	//no piece can occupy the same space as an allied piece
	REQUIRE( nb.eval_move(NE_CART, NE_HORSE) == false );
	REQUIRE( nb.eval_move(NE_CANNON, NW_CANNON) == false );

	//no piece can move off the board (-1 spaces)
	int off_map_cannon = 63;
	int off_map_horse = 10;
	//cannon attempts to run off the map
	REQUIRE( nb.eval_move(NE_CANNON, off_map_cannon) == false );
	//horse attempts to leap off the map
	REQUIRE( nb.eval_move(NE_HORSE, off_map_horse) == false );
}







