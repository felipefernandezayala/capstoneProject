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
#include <random>
#include "SDL.h"
#include "generalObject.h"
#include "snake.h"

class FieldObject : public GeneralObject, public std::enable_shared_from_this<FieldObject>
{
public:
    // constructor / desctructor
    FieldObject();
    void initialize(int const &grid_widthV, int const &grid_heightV);
    ~FieldObject();
    // typical behaviour methods
    void simulate();
    void isSnakeCaught();
    void makeItRooster() { setType( ObjectType::objectRooster); }
    bool isChicken() { return getType() == ObjectType::objectChicken;} 
    std::vector<SDL_Point> objectBody;
    std::shared_ptr<FieldObject> get_shared_this() { return shared_from_this(); }
    void setSnake(std::shared_ptr<Snake> &theSnake){ mySnake = theSnake; }
    void whereIsEgg(SDL_Point &eggLoc);
protected:
    std::vector<std::thread> threads;          // holds all threads that have been launched within this object
    static std::mutex _mtx;                    // mutex shared by all objects for protecting cout
private:
    // typical behaviour methods
    void eternalCycle();
    void saveEgg();
    void chaseSnake();
    // variables needed
    SDL_Point egg;
    std::shared_ptr<Snake> mySnake;
    
};

#endif