#ifndef GENERAL_H
#define GENERAL_H

#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
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
  void setType(ObjectType const &theType){ _type=theType;}
  void setGridSize(int const &grid_widthV, int const &grid_heightV);
  int & getGridWidth(){return grid_width;}
  int & getGridHeight(){return grid_height;}
  float & getSpeed(){return speed;}
  void setSpeed(float const & mySpeed){speed=mySpeed;}
  void wrapAroundGrid(SDL_Point & cell);

  template <class myTemplateType>
  void wrapAroundGrid(myTemplateType & x, myTemplateType & y)
  {
      x = fmod(x + static_cast<myTemplateType>(getGridWidth()),  static_cast<myTemplateType>(getGridWidth()));
      y = fmod(y + static_cast<myTemplateType>(getGridHeight()), static_cast<myTemplateType>(getGridHeight()));
  }
  
  virtual void simulate(){ std::cout<< "object do not move."<<std::endl;}

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