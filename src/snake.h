#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "SDL.h"
#include "generalObject.h"

class Snake : public GeneralObject , public std::enable_shared_from_this<Snake>
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
        head_y(grid_heightV / 2){ 
          setGridSize(grid_widthV, grid_heightV);
          setSpeed(0.1f);
        };

  void initialize(int grid_widthV, int grid_heightV)
  {
    head_x = grid_widthV / 2;
    head_y = grid_heightV / 2;
    setGridSize(grid_widthV, grid_heightV);
  };

  Snake &operator=(Snake &otherSnake)
  {
    size = otherSnake.size;
    alive = otherSnake.alive;
    head_x = otherSnake.head_x;
    head_y = otherSnake.head_y;
    body = otherSnake.body;
    growing = otherSnake.growing;
    setGridSize(otherSnake.getGridWidth(), otherSnake.getGridHeight());
    setSpeed(otherSnake.getSpeed());
    direction = otherSnake.direction;
    return *this;
  };

  void Update(std::vector<SDL_Point> const &items);
  void checkBody(std::promise<bool> &&prms, const int &x, const int &y);
  bool checkFirstHalfBody(const int &x, const int &y);
  void checkLastHalfBody(std::promise<bool> &&prms, const int &x, const int &y);
  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  std::shared_ptr<Snake> get_shared_this() { return shared_from_this(); }
  void checkIsAliveWithOwnBody(SDL_Point const &myCell);
  static std::mutex _mtx; // mutex shared by all objects for protecting cout
private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, std::vector<SDL_Point> const &items);
  void checkIsAlive(SDL_Point const &current_head_cell, std::vector<SDL_Point> const &vecItems);
  bool growing{false};
};

#endif