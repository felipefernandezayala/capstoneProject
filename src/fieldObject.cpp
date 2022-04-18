#include "fieldObject.h"

// init static variable
int FieldObject::_idCnt = 0;

std::mutex FieldObject::_mtx;

FieldObject::FieldObject()
{
    _type = ObjectType::objectEgg; // first they appear as eggs
    _id = _idCnt++;
}

FieldObject::~FieldObject()
{
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Field Object #" << _id << " is safely destroyed " << std::endl;
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
    float deltaX = mySnake->head_x - static_cast<float>(objectBody.at(0).x);
    float deltaY = mySnake->head_y - static_cast<float>(objectBody.at(0).y);
    float deltaZ = sqrt(deltaX*deltaX + deltaY*deltaY);

    objectBody.at(0).x += static_cast<int>(2.*deltaX/deltaZ);
    objectBody.at(0).y += static_cast<int>(2.*deltaY/deltaZ);

    // Wrap the Snake around to the beginning if going off of the screen.
    objectBody.at(0).x = fmod(objectBody.at(0).x + grid_width, grid_width);
    objectBody.at(0).y = fmod(objectBody.at(0).y + grid_height, grid_height);

}

void FieldObject::isSnakeCaught()
{
    // Check if the snake has died againts chicken
    for(auto cell : objectBody )
    {
        mySnake->checkIsAliveWithOwnBody(cell);

        // check with head
        if(static_cast<int>(mySnake->head_x)==cell.x && static_cast<int>(mySnake->head_y)==cell.y)
            mySnake->alive = false;
    }

    
    /*
    // print id of the current thread
    std::unique_lock<std::mutex> lck(_mtx);
    if(!mySnake->alive)
    {
        std::cout << "Field Object #" << _id << ":: Caught the Snake: thread id = " << std::this_thread::get_id() << std::endl;
        std::cout << "Game Over: Snake is Dead" << std::endl;
    }
    lck.unlock();
    */
}

// virtual function which is executed in a thread
void FieldObject::doThings()
{
    // print id of the current thread
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Field Object #" << _id << ":: do Things: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();
    
    // initalize variables
    double cycleDuration = 500./static_cast<double>(speed); // duration of a single simulation cycle in ms
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
            moveAround();

            //lck.lock();
            //std::cout << "Field Object #" << _id << ":: move around: thread id = " << std::this_thread::get_id() << std::endl;
            //lck.unlock();

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
            
        }
    } // eof simulation loop
}
