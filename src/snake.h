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
      { 
          setGridSize(grid_widthV, grid_heightV);
          setSpeed(0.1f);
          setHeadX(grid_widthV / 2);
          setHeadY(grid_heightV / 2);
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

  bool & isAlive(){return alive;}
  void snakeIsDead(){alive=false;}
  float& getHeadX(){return head_x;}
  void setHeadX(float const &theHeadX){head_x=theHeadX;}
  float& getHeadY(){return head_y;}
  void setHeadY(float const &theHeadY){head_y=theHeadY;}
  Direction& getDirection(){return direction;}
  void setDirection(Direction const &theDirection){direction=theDirection;}
  int & getSize(){return size;}
  std::vector<SDL_Point> & getBody(){return body;}

  void Update(std::vector<SDL_Point> const &items);
  void checkBody(std::promise<bool> &&prms, const int &x, const int &y);
  bool checkFirstHalfBody(const int &x, const int &y);
  void checkLastHalfBody(std::promise<bool> &&prms, const int &x, const int &y);
  void GrowBody();
  bool SnakeCell(int x, int y);
  std::shared_ptr<Snake> get_shared_this() { return shared_from_this(); }
  void checkIsAliveWithOwnBody(SDL_Point const &myCell);

private:
  std::vector<SDL_Point> body;
  int size{1};
  Direction direction = Direction::kUp;
  float head_x{0};
  float head_y{0};
  bool alive{true};
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, std::vector<SDL_Point> const &items);
  void checkIsAlive(SDL_Point const &current_head_cell, std::vector<SDL_Point> const &vecItems);
  bool growing{false};
  
};

#endif