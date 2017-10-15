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

bool Pair::compareStrings(string str1, string str2, bool sensitive) {
    if (sensitive) {
        return str1.compare(str2);
    } else {
        if (str1.length() == str2.length()) {
            for (int i = 0; i < str1.length(); i++) {
                if (Pair::compareChars(str1.at(i), str2.at(i), sensitive) != 0) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }        
    }
}

int Pair::compareChars(char c1, char c2, bool sensitive) {
    if (sensitive) {
        return (c1 == c2) ? 0 : (c1 > c2 ? 1 : -1);
    } else {
        if (c1 != c2) {
            if (c1 >= 'a' && c1 <= 'z' && c2 >= 'A' && c2 <= 'Z') {
                return (c1 == c2 + 32) ? 0 : 
                    (c1 > c2 + 32 ? 1 : -1); 
            } 
            else if (c1 >= 'A' && c1 <= 'Z' && c2 >= 'a' && c2 <= 'z') {
                return (c1 == c2 - 32) ? 0 : 
                    (c1 > c2 - 32 ? 1 : -1); 
            }
            else{
                return c1 > c2 ? 1 : -1;
            }
        }
        else{
            return 0;
        }
    }
}
