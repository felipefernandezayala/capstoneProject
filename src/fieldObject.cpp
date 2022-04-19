#include "fieldObject.h"

std::mutex FieldObject::_mtx;

FieldObject::FieldObject()
{
    setType( ObjectType::objectChicken); // all chickens otherwise stated
}

void FieldObject::initialize(int const &grid_widthV, int const &grid_heightV)
{
    setGridSize(grid_widthV, grid_heightV);
    objectBody.resize(4);
    objectBody.at(0).x = _id * getGridWidth() / 7;
    objectBody.at(0).y = _id * getGridHeight() / 7;
    objectBody.at(1).x = objectBody.at(0).x - 1;
    objectBody.at(1).y = objectBody.at(0).y - 1;
    objectBody.at(2).x = objectBody.at(0).x - 1;
    objectBody.at(2).y = objectBody.at(0).y;
    objectBody.at(3).x = objectBody.at(0).x;
    objectBody.at(3).y = objectBody.at(0).y - 1;

    egg.x = getGridWidth() / 2;
    egg.y = getGridHeight() / 2;
}

FieldObject::~FieldObject()
{
    std::unique_lock<std::mutex> lck(_mtx);
    if (isChicken())
        std::cout << "Field Object #" << _id << " Chicken is safely destroyed and thread is being joined." << std::endl;
    else
        std::cout << "Field Object #" << _id << " Rooster is safely destroyed and thread is being joined." << std::endl;
    lck.unlock();

    // set up thread barrier before this object is destroyed
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
    
}

void FieldObject::simulate()
{
     // launch drive function in a thread
    threads.emplace_back(std::thread(&FieldObject::doThings, this));
    
}

void FieldObject::wrapAroundGrid(SDL_Point & cell)
{
    cell.x = fmod(cell.x + getGridWidth(),  getGridWidth());
    cell.y = fmod(cell.y + getGridHeight(), getGridHeight());
}

void FieldObject::moveAround()
{
    // move around the egg
    float deltaX = egg.x - static_cast<float>(objectBody.at(0).x);
    float deltaY = egg.y - static_cast<float>(objectBody.at(0).y);
    float deltaZ = sqrt(deltaX*deltaX + deltaY*deltaY);
    float Radius = 5.;
    // go to the egg directly
    int dx = static_cast<int>(2.*deltaX/deltaZ);
    int dy = static_cast<int>(2.*deltaY/deltaZ);


    // check on the snake
    if (deltaZ<Radius)
    {
        deltaX = mySnake->head_x - static_cast<float>(objectBody.at(0).x);
        deltaY = mySnake->head_y - static_cast<float>(objectBody.at(0).y);
        deltaZ = sqrt(deltaX*deltaX + deltaY*deltaY);

        if (deltaZ < 2*Radius)
        {   
            // snake is close and chase it
            dx = static_cast<int>(3.*deltaX/deltaZ);
            dy = static_cast<int>(3.*deltaY/deltaZ);
        }
    }
      
    // Wrap the Object around to the beginning if going off of the screen.
    for (auto &cell: objectBody)
    {
        cell.x += dx;
        cell.y += dy;
        wrapAroundGrid(cell);
    }
}

void FieldObject::chaseSnake()
{
    float deltaX = mySnake->head_x - static_cast<float>(objectBody.at(0).x);
    float deltaY = mySnake->head_y - static_cast<float>(objectBody.at(0).y);
    float deltaZ = sqrt(deltaX*deltaX + deltaY*deltaY);

    int dx = static_cast<int>(2.*deltaX/deltaZ);
    int dy = static_cast<int>(2.*deltaY/deltaZ);

    // Wrap the Object around to the beginning if going off of the screen.
    for (auto &cell: objectBody)
    {
        cell.x += dx;
        cell.y += dy;
        wrapAroundGrid(cell);
    }
}

void FieldObject::isSnakeCaught()
{
    
    // Check if the snake has died againts chicken
    for(auto cell : objectBody )
    {
        // check with snake body
        mySnake->checkIsAliveWithOwnBody(cell);

        // check with head
        if(static_cast<int>(mySnake->head_x)==cell.x && static_cast<int>(mySnake->head_y)==cell.y)
            mySnake->alive = false;

    }
}

void FieldObject::whereIsEgg(SDL_Point &eggLoc)
{
    egg = eggLoc;
}

// virtual function which is executed in a thread
void FieldObject::doThings()
{
    // print id of the current thread
    std::unique_lock<std::mutex> lck(_mtx);
    if (isChicken())
        std::cout << "Field Object #" << _id << " Chicken is running to  save  the  Egg:: thread id = " << std::this_thread::get_id() << std::endl;
    else
        std::cout << "Field Object #" << _id << " Rooster is running to catch the Snake:: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();
    
    // initalize variables
    double cycleDuration = 10./static_cast<double>(getSpeed()); // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();
    
    
    while (mySnake->alive)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {            
            if (getType() == ObjectType::objectChicken)
            {
                moveAround();
            }
            else
            {
                // Rooster
                chaseSnake();
            }

            //lck.lock();
            //std::cout << "Field Object #" << _id << ":: move around: thread id = " << std::this_thread::get_id() << std::endl;
            //lck.unlock();

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
            
        }
    } // eof simulation loop
}
