#include <iostream>
#include <string>
#include "menu_choices.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"

#define SNAKE_GAME_DB "../src/game_db.json"

int main(int argc, char **argv) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  std::string player_name;
  
  // Create the menu game
  MenuChoice game_menu(SNAKE_GAME_DB);
  game_menu.MenuProcess(); 
  
  // Check if user choice run game then create object to run game
  if (game_menu.GetGameState()) {
    auto renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    auto controller = std::make_unique<Controller>();
    auto game = std::make_unique<Game>(kGridWidth, kGridHeight, game_menu.GetCurrentLevel());

    game->Run(*controller, *renderer, kMsPerFrame);

    std::cout << "Game has terminated successfully!" << std::endl;
  
    std::cout << "Enter your name to save score:" << std::endl;
    std::cin >> player_name;
    std::cout << "Score: " << game->GetScore() << "\n";
    std::cout << "Size: " << game->GetSize() << "\n";

    // Save player's score to game database with current level and score.
    PlayerInfo_t player{player_name, game_menu.GetCurrentLevel(), game->GetScore(), game->GetSize()};
    game->SaveGame(SNAKE_GAME_DB, player);
  }

  return 0;
}
