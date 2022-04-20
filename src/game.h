#ifndef GAME_H
#define GAME_H

#include <random>
#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <fstream>
using namespace std::placeholders;

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include <vector>
#include "snake.h"
#include "fieldObject.h"

class Game
{
  std::mutex m_mutex;
  std::condition_variable m_condVar;
  bool areFieldObjectsLoaded;

public:
  Game(std::size_t grid_width, std::size_t grid_height);
  Game(std::size_t grid_width, std::size_t grid_height, std::vector<std::shared_ptr<FieldObject>> &theObjects, std::shared_ptr<Snake> &theSnake);
  void updateFieldObjects();
  bool isDataLoaded()
  {
    return areFieldObjectsLoaded;
  }
  void updateSnake()
  {
    //std::cout<<"Do Some Handshaking"<<std::endl;
    // Acquire the lock
    std::unique_lock<std::mutex> mlock(m_mutex);
    // Start waiting for the Condition Variable to get signaled
    // Wait() will internally release the lock and make the thread to block
    // As soon as condition variable get signaled, resume the thread and
    // again acquire the lock. Then check if condition is met or not
    // If condition is met then continue else again go in wait.
    m_condVar.wait(mlock, std::bind(&Game::isDataLoaded, this));
    //std::cout<<"Do Processing On loaded Data"<<std::endl;
    mySnake->Update(items);
  }



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
  void writeScoresToFile();

private:
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