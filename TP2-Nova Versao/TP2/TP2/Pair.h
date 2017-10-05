#pragma once

#include <string>
using namespace std;

class Pair
{
private:
	string key;
	string value;

public:
	Pair();
	Pair(string key, string value);
	~Pair();

	void setPair(string key, string value);
};

