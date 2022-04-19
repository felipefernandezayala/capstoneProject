#ifndef GENERAL_H
#define GENERAL_H

#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "SDL.h"

class GeneralObject
{
  GeneralObject();
  ~GeneralObject();
  int getID() { return _id; }

protected:
  int _id;          // every object has its own unique id

private:
  
};

#endif