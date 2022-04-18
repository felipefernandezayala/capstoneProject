#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <future>
#include <queue>
#include "SDL.h"
#include "snake.h"

enum ObjectType
{
    noObject,
    objectSnake,
    objectEgg,
    objectBaby,
    objectChicken,
};

class FieldObject : public std::enable_shared_from_this<FieldObject>
{
public:
    // constructor / desctructor
    FieldObject();
    FieldObject(int grid_widthV, int grid_heightV)
      : grid_width(grid_widthV),
        grid_height(grid_heightV){
            _type = ObjectType::objectEgg; // first they appear as eggs
            _id = _idCnt++;
            objectBody.resize(1);
            objectBody.at(0).x = _id*grid_widthV/10;
            objectBody.at(0).y = _id*grid_heightV/10;
            
        };
    ~FieldObject();

    // getter and setter
    int getID() { return _id; }
    ObjectType getType() { return _type; }

    float speed{1.f};

    // typical behaviour methods
    void simulate();
    void isSnakeCaught();
    std::vector<SDL_Point> objectBody;
    
    // miscellaneous
    std::shared_ptr<FieldObject> get_shared_this() { return shared_from_this(); }

    void setSnake(std::shared_ptr<Snake> &theSnake) 
    { 
        mySnake = theSnake; 
    };

    // from c++ example
    std::mutex m;
    std::condition_variable cv;
    std::string data;
    bool ready;
    bool processed;


protected:
    ObjectType _type;                 // identifies the class type
    int _id;                          // every object has its own unique id
    
    std::vector<std::thread> threads; // holds all threads that have been launched within this object
    static std::mutex _mtx;           // mutex shared by all objects for protecting cout 
    std::vector<std::promise<void>> _promises; // list of associated promises
    std::condition_variable _cond; //condition variable    
    
private:
    // typical behaviour methods
    void doThings();
    void moveAround();
    static int _idCnt; // global variable for counting object ids
    int grid_width;
    int grid_height;

    std::shared_ptr<Snake> mySnake;
};





#endif