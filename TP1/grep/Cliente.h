/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.h
 * Author: gustavo
 *
 * Created on September 16, 2017, 9:13 PM
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

class Cliente {
public:
    Cliente(int porta);
    Cliente(const Cliente& orig);
    virtual ~Cliente();
    
    // Conectar ao daemon cliente
    void conectar();
    // Encerra conexão com o daemon cliente
    void encerrar();
    
    // Envia mensagem ao daemon cliente
    void enviar();
    // Recebe mensagem do daemon cliente
    void receber();

private:
    int porta;    
    int conexao;
};

#endif /* CLIENTE_H */

