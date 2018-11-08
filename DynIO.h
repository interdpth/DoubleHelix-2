#pragma once
#include <stdio.h>
#include "MemFile.h"
#include "Logger.h"
enum ObjType 
{

};
class DynIO
{
public:
	DynIO(FILE*);
	DynIO(MemFile*);
	~DynIO();
};

