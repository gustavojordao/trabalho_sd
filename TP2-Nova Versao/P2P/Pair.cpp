/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pair.cpp
 * Author: gustavo
 * 
 * Created on October 5, 2017, 9:29 PM
 */

#include "Pair.h"

Pair::Pair() {
    key = "";
    value = "";
}

Pair::~Pair() {
}

Pair::Pair(string key, string value) {
    this->key = key;
    this->value = value;
}

string Pair::getKey() {
    return key;
}

string Pair::getValue() {
    return value;
}

void Pair::setPair(string key, string value) {
    this->key = key;
    this->value = value;
}

