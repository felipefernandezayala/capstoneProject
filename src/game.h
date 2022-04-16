#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include <vector>
#include "snake.h"
#include "fieldObject.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void setFieldObjects(std::vector<std::shared_ptr<FieldObject>> &theObjects) { _objectsInTheField = theObjects; };
  void setSnake(std::shared_ptr<Snake> &theSnake) 
  { 
    mySnake = theSnake; 
    PlaceFood();
  };

 private:
  Snake snake;
  SDL_Point food;
  std::vector<SDL_Point> items;
  std::vector<std::shared_ptr<FieldObject>> _objectsInTheField;
  std::shared_ptr<Snake> mySnake;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void PlaceItems();
  void Update();
};

#endif