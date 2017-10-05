/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Servidor.h
 * Author: gustavo
 *
 * Created on October 1, 2017, 9:36 PM
 */

#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class Servidor {
public:
    Servidor(int porta);
    Servidor(const Servidor& orig);
    virtual ~Servidor();
    
    // Inicia servidor
    void iniciar();
    // Aceitar conexão de cliente
    int aceitarCliente();
    // Encerra servidor
    void encerrarServidor();
    // Encerra servidor
    void encerrarCliente();

private:
    int conexao;
    int conexaoCliente;
    
    int porta;

};

#endif /* SERVIDOR_H */

