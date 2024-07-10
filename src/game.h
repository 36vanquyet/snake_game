#ifndef GAME_H
#define GAME_H

#include <random>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

typedef struct PlayerInfo {
  std::string name;
  int level;
  int score;
  int size;
} PlayerInfo_t;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, int level);

  // Move constructor
  Game(Game&& other) noexcept;

  // Move assignment operator
  Game& operator=(Game&& other) noexcept;

  // Deleted copy constructor and copy assignment operator
  Game(const Game& other) = delete;
  Game& operator=(const Game& other) = delete;

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore(void) const;
  int GetSize(void) const;

  void SaveGame(const std::string &db_path, const PlayerInfo_t &info);

 private:
  std::unique_ptr<Snake> snake;
  SDL_Point food;
  std::vector<SDL_Point> obstacles;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int level;

  void PlaceFood(void);
  void Update(void);

  // Threading and synchronization
  std::thread game_thread;
  std::thread input_thread;
  std::thread obstacle_thread;
  std::mutex mtx;
  std::condition_variable cv;
  bool running{true};

  void PlaceObstacles(void);
  void ChangeObstacles(void);
  void StartObstacleThread(void);
  void StopObstacleThread(void);
};

#endif
