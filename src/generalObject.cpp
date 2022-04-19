#include "generalObject.h"

int GeneralObject::_idCounter = 0;

GeneralObject::GeneralObject()
{
    _id = _idCounter++;
    _type = ObjectType::noObject;
}

GeneralObject::~GeneralObject()
{
    
}

void GeneralObject::setGridSize(int const &grid_widthV, int const &grid_heightV)
{
    grid_height = grid_heightV;
    grid_width = grid_widthV;
}

void GeneralObject::wrapAroundGrid(SDL_Point & cell)
{
    cell.x = fmod(cell.x + getGridWidth(),  getGridWidth());
    cell.y = fmod(cell.y + getGridHeight(), getGridHeight());
}

