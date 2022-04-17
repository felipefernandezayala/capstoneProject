#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <thread>
#include <mutex>
#include "SDL.h"


class Snake : public std::enable_shared_from_this<Snake>
{
public:
  enum class Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight
  };

  Snake();
  Snake(int grid_widthV, int grid_heightV)
      : head_x(grid_widthV / 2),
        head_y(grid_heightV / 2),
        grid_width(grid_widthV),
        grid_height(grid_heightV){};
  
  void initialize(int grid_widthV, int grid_heightV)
  {
    head_x = grid_widthV / 2;
    head_y = grid_heightV / 2;
    grid_width = grid_widthV;
    grid_height =grid_heightV;

  };
  
  Snake & operator=(Snake const &otherSnake)
  {
    speed = otherSnake.speed;
    size = otherSnake.size;
    alive = otherSnake.alive;
    head_x = otherSnake.head_x;
    head_y = otherSnake.head_y;
    body = otherSnake.body;
    growing = otherSnake.growing;
    grid_width = otherSnake.grid_width;
    grid_height = otherSnake.grid_height;
    direction = otherSnake.direction;
    return *this;
  };

  void Update(std::vector<SDL_Point> const &items);
  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

  // miscellaneous
  std::shared_ptr<Snake> get_shared_this() { return shared_from_this(); }

  void checkIsAliveWithOwnBody(SDL_Point const &myCell);
  static std::mutex _mtx;           // mutex shared by all objects for protecting cout 
private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, std::vector<SDL_Point> const &items);
  void checkIsAlive(SDL_Point const &current_head_cell, std::vector<SDL_Point> const &vecItems);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif