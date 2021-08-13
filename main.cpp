/*Copyright (C) 2019 Gabriel Kaszewski - All Rights Reserved
* You may use, distribute and modify this code under the 
* terms of the XYZ license, which unfortunately won't be
* written for another century. Hehe
*/

#include "Game.hpp"
#include "States/EditorState.hpp"
#include "States/BetterEditor.hpp"
#include "States/TestState.hpp"


int main() {
	Game game;
	//game.pushState<EditorState>(game, "Brick Engine - Level editor");
	//game.pushState<TestState>(game, "Brick Engine - Test scene (Physics)");
	game.pushState<BetterEditor>(game, "Brick Engine - Editor");
	game.run();
}
