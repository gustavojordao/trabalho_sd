/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pair.h
 * Author: gustavo
 *
 * Created on October 5, 2017, 9:29 PM
 */

#ifndef PAIR_H
#define PAIR_H

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
        
        string getKey();
        string getValue();
};

#endif /* PAIR_H */

