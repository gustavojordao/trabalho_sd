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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class Cliente {
public:
    Cliente(char* ip_servidor, int portaServidor, int portaGrep);
    Cliente(const Cliente& orig);
    virtual ~Cliente();
    
    // Iniciar servidor do grep
    void iniciarGrep();
    
    // Aceita conexão do grep
    int aceitarGrep();
    
    // Receber do grep
    void receberDoGrep();
    
    // Enviar ao grep
    void enviarAoGrep();
    
    // Encerra conexao com servidor
    void encerrarGrep();


    // Conectar ao servidor
    void conectarAoServidor();
    
    // Envia mensagem ao cliente
    void enviarAoServidor();
    
    // Recebe mensagem do cliente
    void receberDoServidor();
    
    // Encerra conexao com servidor
    void encerrarServidor();

private:

    int conexaoGrep;
    int conexaoServidor;
    
    char* ip_servidor;
    int portaServidor;

    int portaGrep;
    
};

#endif /* CLIENTE_H */

