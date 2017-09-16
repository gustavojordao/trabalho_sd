/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mensagem.h
 * Author: gustavo
 *
 * Created on September 16, 2017, 1:28 PM
 */

#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <iostream>
#include <string>
using namespace std;

class Mensagem {
public:
    Mensagem();
    Mensagem(const Mensagem& orig);
    virtual ~Mensagem();
private:

    int codigo;
    // 1 - Cliente solicita log ao servidor
    // 2 - Servidor solicita log ao cliente - uma chamada para cada cliente
    // 3 - Cliente envia log ao servidor - Resposta de 2
    // 4 - Servidor envia log ao cliente - Resposta de 1    
    
    string texto;
        
    // Formato
    // <codigo_mensagem>|<texto>
};

#endif /* MENSAGEM_H */

