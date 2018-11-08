#pragma once
class DHProject
{
private:
	char* projectName; 
	unsigned long freeSpaceStart;

public:
	const char fileExtension[32] = "DHProj";
	DHProject();
	~DHProject();
};

