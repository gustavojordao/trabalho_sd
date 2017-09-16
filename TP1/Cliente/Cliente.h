/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.h
 * Author: gustavo
 *
 * Created on September 16, 2017, 4:44 PM
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

class Cliente {
public:
    Cliente(char* ip_servidor, int porta);
    Cliente(const Cliente& orig);
    virtual ~Cliente();
    
    // Conectar ao servidor
    void conectar();
    
    // Encerra servidor
    void encerrar();
    
    // Envia mensagem ao cliente
    void enviar();
    // Recebe mensagem do cliente
    void receber();
private:

    int conexao;
    
    char* ip_servidor;
    int porta;
    
};

#endif /* CLIENTE_H */

