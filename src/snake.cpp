#include "snake.h"
#include <cmath>
#include <iostream>

std::mutex Snake::_mtx;


Snake::Snake()
{
  head_x = 0;
  head_y = 0;
  grid_width = 0;
  grid_height =0;
}


void Snake::Update(std::vector<SDL_Point> const &items)
{
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)}; // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)}; // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
  {
    UpdateBody(current_cell, prev_cell, items);
  }
}

void Snake::UpdateHead()
{
  switch (direction)
  {
  case Direction::kUp:
    head_y -= speed;
    break;

  case Direction::kDown:
    head_y += speed;
    break;

  case Direction::kLeft:
    head_x -= speed;
    break;

  case Direction::kRight:
    head_x += speed;
    break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::checkIsAliveWithOwnBody(SDL_Point const &myCell)
{
  checkIsAlive(myCell, body);
}

void Snake::checkIsAlive(SDL_Point const &current_head_cell, std::vector<SDL_Point> const &vecItems)
{
  for (auto const &cellOfItems : vecItems)
  {
    if (current_head_cell.x == cellOfItems.x && current_head_cell.y == cellOfItems.y)
    {
      alive = false;
    }
  }
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell, std::vector<SDL_Point> const &items)
{
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing)
  {
    // Remove the tail from the vector.
    body.erase(body.begin());
  }
  else
  {
    growing = false;
    size++;
  }

  // Check if the snake has died againts its own body
  checkIsAliveWithOwnBody(current_head_cell);

  // print id of the current thread
  /*
    if(!alive)
    {
        std::unique_lock<std::mutex> lck(_mtx);
        std::cout << "Snake ate itself!! #" << std::this_thread::get_id() << std::endl;
        std::cout << "Game Over: Snake is Dead" << std::endl;
        lck.unlock();
    }
  */
  // Check if the snake has died againts due to collide to items
  checkIsAlive(current_head_cell, items);
}

void Snake::GrowBody() { growing = true; }


void Snake::checkBody(std::promise<bool> &&prms, const int &x,const  int &y)
{
  for (auto const &item : body)
  {
    if (x == item.x && y == item.y)
    {
      prms.set_value(true);
      break;
    }
  }
  prms.set_value(false);
}

bool Snake::checkFirstHalfBody(const int &x,const  int &y)
{
  for (long unsigned int i=0;i<(body.size()/2);i++)
  {
    if (x == body.at(i).x && y == body.at(i).y)
    {
      return true;
    }
  }
  return false;
}

void Snake::checkLastHalfBody(std::promise<bool> &&prms, const int &x,const  int &y)
{
  for (long unsigned int i=(body.size()/2);i<body.size();i++)
  {
    if (x == body.at(i).x && y == body.at(i).y)
    {
      prms.set_value(true);
      break;
    }
  }
  prms.set_value(false);
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y)
{
  // create promise and future
  std::promise<bool> prms;
  std::future<bool> ftr = prms.get_future();
  // start thread and pass promise as argument
  std::thread t(&Snake::checkLastHalfBody, this, std::move(prms), x, y);

  bool status = checkFirstHalfBody(x,y);

  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y))
  {
    status = true;
  }

 status = status||ftr.get();
 t.join();
  
  return status;
}