/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pair.cpp
 * Author: gustavo
 * 
 * Created on October 1, 2017, 3:11 PM
 */

#include "Pair.h"

Pair::Pair() {
    this->key = "";
    this->value = "";
}

Pair::Pair(const Pair& orig) {
}

Pair::~Pair() {
}

Pair::Pair(string key, string value) {
    this->key = key;
    this->value = value;
}

