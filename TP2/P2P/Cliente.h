/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.h
 * Author: gustavo
 *
 * Created on October 1, 2017, 10:22 PM
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Cliente {
public:
    Cliente(char* ip_servidor, int portaServidor);
    Cliente(const Cliente& orig);
    virtual ~Cliente();
    
    // Conectar ao servidor
    void conectarAoServidor();
    
    // Encerra conexao com servidor
    void encerrarServidor();
    
    
private:

    int conexao;
    int conexaoServidor;
    
    char ip_servidor[20];
    int portaServidor;
    
};

#endif /* CLIENTE_H */

