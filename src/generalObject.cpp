#include "generalObject.h"

// init static variable
int GeneralObject::_idCnt = 0;

GeneralObject::GeneralObject()
{
    _id = _idCnt++;
}

GeneralObject::~GeneralObject()
{
    
}