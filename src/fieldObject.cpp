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

// virtual function which is executed in a thread
void FieldObject::doThings()
{
    // print id of the current thread
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Field Object #" << _id << ":: do Things: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();

    // initalize variables
    double cycleDuration = 100; // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {
            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
            lck.lock();
                std::cout << "Field Object #" << _id << ":: do Things: thread id = " << std::this_thread::get_id() << std::endl;
            lck.unlock();
        }
    } // eof simulation loop
}
