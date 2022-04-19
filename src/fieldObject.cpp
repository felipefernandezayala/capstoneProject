#include "fieldObject.h"

// init static variable
int FieldObject::_idCnt = 0;

std::mutex FieldObject::_mtx;

FieldObject::FieldObject()
{
    _type = ObjectType::objectChicken; // all chickens otherwise stated
    _id = _idCnt++;
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
        cell.x = fmod(cell.x + grid_width, grid_width);
        cell.y = fmod(cell.y + grid_height, grid_height);
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
        cell.x = fmod(cell.x + grid_width, grid_width);
        cell.y = fmod(cell.y + grid_height, grid_height);
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
    double cycleDuration = 10./static_cast<double>(speed); // duration of a single simulation cycle in ms
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
            if (_type == ObjectType::objectChicken)
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
