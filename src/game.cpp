#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))
{
  areFieldObjectsLoaded = false;
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, mySnake);
    Update();
    renderer.Render(food, items, _objectsInTheField, mySnake);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  mySnake->alive = false; // kill snake so the other field object threads stop
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!mySnake->SnakeCell( x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceItems()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!mySnake->SnakeCell(x, y))
    {
      SDL_Point newfood;
      newfood.x = x;
      newfood.y = y;
      items.emplace_back(newfood);
      return;
    }
  }
}


void Game::updateFieldObjects()
  {
   // Make This Thread sleep for 1 milisecond
   std::this_thread::sleep_for(std::chrono::milliseconds(1));
   //std::cout<<"Loading Data from Field Objects"<<std::endl;
   // Lock The Data structure
   std::lock_guard<std::mutex> guard(m_mutex);

   std::for_each(_objectsInTheField.begin(), _objectsInTheField.end(), [&](std::shared_ptr<FieldObject> &thisObject) {
        thisObject->isSnakeCaught();
        thisObject->whereIsEgg(food);
    });

    

   // Set the flag to true, means data is loaded
   areFieldObjectsLoaded = true;
   // Notify the condition variable
   m_condVar.notify_one();
  }

void Game::writeScoresToFile()
{
  std::ofstream myfile;
  myfile.open ("score.txt");
  myfile << "Game has terminated successfully!\n";
  myfile << "Score: " << GetScore() << "\n";
  myfile << "Size: " << GetSize() << "\n";
  myfile.close();
}

void Game::Update()
{
  if (!mySnake->alive)
  {
    return;
  }

  //using condition variable to check if field objects are loaded before we continue updating our snake
  areFieldObjectsLoaded = false;
  std::thread thread_1(&Game::updateSnake,this);
  std::thread thread_2(&Game::updateFieldObjects,this);
  thread_2.join();
  thread_1.join();

  int new_x = static_cast<int>(mySnake->head_x);
  int new_y = static_cast<int>(mySnake->head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    //PlaceItems();
    // Grow mySnake and increase speed.
    mySnake->GrowBody();
    mySnake->speed += 0.005;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return mySnake->size; }