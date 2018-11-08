
#pragma once 
struct Drctype{
unsigned char sA1;
unsigned char D1;
unsigned char dA1;
unsigned char sA2;
unsigned char D2;
unsigned char dA2;
};
struct Drc{

      int Connects;
	  Drctype DoorConnects[0xFF];
};




