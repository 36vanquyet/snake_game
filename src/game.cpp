#include "game.h"
#include <iostream>
#include "SDL.h"
#include "manager_db.h"
#include "parser_string.h"

/**
 * @brief Constructs a Game object with the specified grid dimensions and level.
 *
 * This constructor initializes a Game object with the specified grid dimensions and level. It creates a Snake object with the given grid dimensions, initializes the engine, and sets up the random number generators for the x and y coordinates. Depending on the level, it places food and/or obstacles in the game grid.
 *
 * @param grid_width The width of the game grid.
 * @param grid_height The height of the game grid.
 * @param level The level of the game.
 */
Game::Game(std::size_t grid_width, std::size_t grid_height, int level)
    : snake(std::make_unique<Snake>(grid_width, grid_height)),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      level(level) {
  PlaceFood();
  if (level == 2) {
    PlaceObstacles();
  } else if (level == 3) {
    StartObstacleThread();
  }
}

/**
 * @brief Assigns a Game object by moving its contents.
 *
 * This function moves the contents of the given Game object into the current object. This is useful when you want to replace the current object with the given one without copying its contents.
 *
 * @param[in] other The Game object to move its contents into the current object.
 *
 * @return A reference to the current object after the move operation.
 */
Game::Game(Game&& other) noexcept
    : snake(std::move(other.snake)),
      food(other.food),
      obstacles(std::move(other.obstacles)),
      engine(std::move(other.engine)),
      random_w(std::move(other.random_w)),
      random_h(std::move(other.random_h)),
      score(other.score),
      level(other.level),
      running(other.running) {}

/**
 * @brief Assigns a Game object by moving its contents.
 *
 * This function moves the contents of the given Game object into the current object. This is useful when you want to replace the current object with the given one without copying its contents.
 *
 * @param[in] other The Game object to move its contents into the current object.
 *
 * @return A reference to the current object after the move operation.
 */
Game& Game::operator=(Game&& other) noexcept {
  if (this != &other) {
    snake = std::move(other.snake);
    food = other.food;
    obstacles = std::move(other.obstacles);
    engine = std::move(other.engine);
    random_w = std::move(other.random_w);
    random_h = std::move(other.random_h);
    score = other.score;
    level = other.level;
    running = other.running;
  }
  return *this;
}

/**
 * @brief Runs the game loop.
 *
 * This function runs the main game loop, handling input, updating the game state, and rendering the game.
 *
 * @param controller The controller object responsible for handling user input.
 * @param renderer The renderer object responsible for rendering the game.
 * @param target_frame_duration The target duration for each frame in milliseconds.
 */
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake);
    Update();
    renderer.Render(*snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  if (level == 3) {
    StopObstacleThread();
  }
}

/**
 * @brief Places food in the game grid.
 *
 * This function populates the game grid with food. The food is placed at a random location that is not occupied by a snake item or obstacle.
 *
 * @param None
 *
 * @return None
 */
void Game::PlaceFood(void) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item or obstacle before placing food.
    bool valid = true;
    for (const auto& obstacle : obstacles) {
      if (obstacle.x == x && obstacle.y == y) {
        valid = false;
        break;
      }
    }
    if (valid && !snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

/**
 * @brief Updates the game state.
 *
 * This function updates the game state by moving the snake, checking for collisions with food and obstacles, and updating the score.
 *
 * @param None
 *
 * @return None
 */
void Game::Update(void) {
  if (!snake->alive) return;

  snake->Update();

  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }

  // Check if the snake has collided with an obstacle
  for (const auto& obstacle : obstacles) {
    if (obstacle.x == new_x && obstacle.y == new_y) {
      snake->alive = false;
      break;
    }
  }
}

/**
 * @brief Returns the player's score.
 *
 * This function retrieves the player's score from the game state.
 *
 * @return The player's score.
 */
int Game::GetScore(void) const { return score; }

/**
 * @brief Returns the size of the snake.
 *
 * This function returns the size of the snake, which is the number of body segments it has.
 *
 * @return The size of the snake.
 */
int Game::GetSize(void) const { return snake->size; }

/**
 * @brief Saves the game state to the database.
 *
 * This function saves the game state, including the player's score and size, to the database.
 *
 * @param db_path The path to the database file.
 * @param info The player's information, including their name, level, score, and size.
 */
void Game::SaveGame(const std::string &db_path, const PlayerInfo_t &info) {
  std::lock_guard<std::mutex> lock(mtx);
  ManagerDBJson db(db_path);
  json info_data = db.ReadJsonFile(info.name);
  info_data[Parser::LevelToString(info.level)]["Score"].push_back(info.score);
  info_data[Parser::LevelToString(info.level)]["Size"].push_back(info.size);
  db.UpdateJsonFile(info.name, info_data);
  db.SaveJsonFile();
}

/**
 * @brief Places obstacles in the game grid.
 *
 * This function populates the game grid with obstacles. The obstacles are placed at the edges of the grid.
 *
 * @param None
 *
 * @return None
 */
void Game::PlaceObstacles(void) {
  for (int i = 0; i < static_cast<int>(snake->GetGridWidth()); ++i) {
    obstacles.push_back(SDL_Point{i, 0});
    obstacles.push_back(SDL_Point{i, static_cast<int>(snake->GetGridHeight()) - 1});
  }
  for (int i = 0; i < static_cast<int>(snake->GetGridHeight()); ++i) {
    obstacles.push_back(SDL_Point{0, i});
    obstacles.push_back(SDL_Point{static_cast<int>(snake->GetGridWidth()) - 1, i});
  }
}

/**
 * @brief Starts a separate thread to change the obstacles in the game.
 * 
 * @param None
 *
 * @return None
 */
void Game::ChangeObstacles(void) {
  std::unique_lock<std::mutex> lock(mtx);
  while (running) {
    cv.wait_for(lock, std::chrono::seconds(5), [this] { return !running; });
    if (!running) break;

    // Clear existing obstacles
    obstacles.clear();

    // Add new random obstacles
    int num_obstacles = random_w(engine) % 10 + 5;  // Random number of obstacles between 5 and 15
    for (int i = 0; i < num_obstacles; ++i) {
      SDL_Point obstacle;
      do {
        obstacle.x = random_w(engine);
        obstacle.y = random_h(engine);
      } while (snake->SnakeCell(obstacle.x, obstacle.y) || (food.x == obstacle.x && food.y == obstacle.y));
      obstacles.push_back(obstacle);
    }
  }
}

/**
 * @brief Starts a separate thread to change the obstacles in the game.
 * 
 * @param None
 *
 * @return None
 */
void Game::StartObstacleThread(void) {
  obstacle_thread = std::thread(&Game::ChangeObstacles, this);
}

/**
 * @brief Stops the obstacle thread and cleans up resources.
 * 
 * @param None
 *
 * @return None
 */
void Game::StopObstacleThread(void) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    running = false;
  }
  cv.notify_all();
  if (obstacle_thread.joinable()) {
    obstacle_thread.join();
  }
}
