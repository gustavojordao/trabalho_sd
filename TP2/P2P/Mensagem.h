/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mensagem.h
 * Author: gustavo
 *
 * Created on October 2, 2017, 9:53 AM
 */

#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <string>

using namespace std;

class Mensagem {
public:
    Mensagem();
    Mensagem(const Mensagem& orig);
    virtual ~Mensagem();
private:

    int codigo;
    string key;
    string value;
    int 
    
};

#endif /* MENSAGEM_H */

