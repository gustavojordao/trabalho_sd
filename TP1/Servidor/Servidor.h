/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Servidor.h
 * Author: gustavo
 *
 * Created on September 16, 2017, 12:12 PM
 */

#ifndef SERVIDOR_H
#define SERVIDOR_H

#define MAX_CLIENTES 10

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    void encerrar();
    
    // Envia mensagem ao cliente
    void enviar();
    // Recebe mensagem do cliente
    void receber();

protected:
    // Insere cliente no array
    int insereCliente(int cliente);
    // Remove cliente do array
    void removeCliente(int i);
    
    // Retorna o índice do próximo cliente a ser adicionado ou -1 quando não houver vaga
    int proximoCliente();
    
private:
    int conexao;
    
    int porta;
    
    // Array de clientes: A posição no vetor corresponde ao índice da máquina
    int clientes[MAX_CLIENTES];
    
    
};

#endif /* SERVIDOR_H */

