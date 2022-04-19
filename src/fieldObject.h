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
#include <math.h> 
#include "SDL.h"
#include "snake.h"

enum ObjectType
{
    noObject,
    objectRooster,
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
            _type = ObjectType::objectChicken; // first they appear as eggs
            _id = _idCnt++;
            objectBody.resize(4);
            objectBody.at(0).x = _id*grid_widthV/7;
            objectBody.at(0).y = _id*grid_heightV/7;
            objectBody.at(1).x = objectBody.at(0).x-1;
            objectBody.at(1).y = objectBody.at(0).y-1;
            objectBody.at(2).x = objectBody.at(0).x-1;
            objectBody.at(2).y = objectBody.at(0).y;
            objectBody.at(3).x = objectBody.at(0).x;
            objectBody.at(3).y = objectBody.at(0).y-1;

            egg.x = grid_widthV/2;
            egg.y = grid_heightV/2;
            
        };
    ~FieldObject();

    // getter and setter
    int getID() { return _id; }
    ObjectType getType() { return _type; }
 
    float speed{.02f};

    // typical behaviour methods
    void simulate();
    void isSnakeCaught();
    void makeItRooster(){_type = ObjectType::objectRooster;};
    bool isChicken(){return _type==ObjectType::objectChicken;}
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

    void whereIsEgg(SDL_Point &eggLoc);


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
    void chaseSnake();
    static int _idCnt; // global variable for counting object ids
    int grid_width;
    int grid_height;
    SDL_Point egg;

    std::shared_ptr<Snake> mySnake;

    
};





#endif