#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <chrono>
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
            body.x = _id*grid_widthV/5;
            body.y = grid_heightV/4;
            
        };
    ~FieldObject();

    // getter and setter
    int getID() { return _id; }
    ObjectType getType() { return _type; }

    float speed{0.f};

    // typical behaviour methods
    void simulate();
    SDL_Point body;      // object body
    
    // miscellaneous
    std::shared_ptr<FieldObject> get_shared_this() { return shared_from_this(); }

    void setSnake(std::shared_ptr<Snake> &theSnake) 
    { 
        mySnake = theSnake; 
    };

protected:
    ObjectType _type;                 // identifies the class type
    int _id;                          // every object has its own unique id
    
    std::vector<std::thread> threads; // holds all threads that have been launched within this object
    static std::mutex _mtx;           // mutex shared by all objects for protecting cout 

    
    
private:
    // typical behaviour methods
    void doThings();
    static int _idCnt; // global variable for counting object ids
    int grid_width;
    int grid_height;

    std::shared_ptr<Snake> mySnake;
};





#endif