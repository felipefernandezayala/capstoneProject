#ifndef GENERAL_H
#define GENERAL_H

#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "SDL.h"

enum ObjectType
{
    noObject,
    objectRooster,
    objectChicken,
};

class GeneralObject
{
public:
  GeneralObject();
  ~GeneralObject();
  int getID() { return _id; }
  ObjectType & getType(){return _type;}
  void setType(ObjectType const &myType){ _type=myType;}
  void setGridSize(int const &grid_widthV, int const &grid_heightV);
  int & getGridWidth(){return grid_width;}
  int & getGridHeight(){return grid_height;}
  float & getSpeed(){return speed;}
  void setSpeed(float const & mySpeed){speed=mySpeed;}
  void wrapAroundGrid(SDL_Point & cell);
  void wrapAroundGrid(int & x, int & y);

protected:
  int _id;          // every object has its own unique id

private:
  static int _idCounter; // global variable for counting object ids
  ObjectType _type; // identifies the class type
  int grid_width;
  int grid_height;
  float speed{.015f};
};

#endif