/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mensagem.cpp
 * Author: gustavo
 * 
 * Created on September 16, 2017, 1:28 PM
 */

#include "Mensagem.h"

Mensagem::Mensagem() {
}

Mensagem::Mensagem(char* mensagem){
    char txt[255];
    sscanf(mensagem, "%d|%s", &this->codigo, txt);
    this->texto = *(new string(txt));
}

Mensagem::Mensagem(const Mensagem& orig) {
}

Mensagem::~Mensagem() {
}

void Mensagem::toChar(char* resultado){
    sprintf(resultado, "%d|%s", codigo, texto.data()); 
}