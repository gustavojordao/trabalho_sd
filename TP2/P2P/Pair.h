/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pair.h
 * Author: gustavo
 *
 * Created on October 1, 2017, 3:11 PM
 */

#ifndef PAIR_H
#define PAIR_H

#include <string>

using namespace std;

class Pair {
public:
    Pair();
    Pair(const Pair& orig);
    Pair(string key, string value);
    virtual ~Pair();
    
    void setKey(string key);
    string getKey();
    
    void setValue(string value);
    string getValue();
    
private:

    string key;
    string value;
    
};

#endif /* PAIR_H */

