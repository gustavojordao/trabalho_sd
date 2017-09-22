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
#include <string.h>
#include <vector>
#include "Mensagem.h"

using namespace std;

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
    
    // Envia mensagem ao cliente
    int enviar(int maquina, Mensagem* mensagem);
    // Recebe mensagem do cliente
    Mensagem* receber(int maquina);

    void setConexao(int conexao);
    
    void enviarTodos(Mensagem* mensagem);
    
    vector<Mensagem*> receberTodos();

    vector<Mensagem*> enviarReceberTodos(Mensagem* mensagem);
    
    Mensagem* agruparMensagens(vector<Mensagem*> mensagens);
    
    int getCliente(int i);
    
    void setCliente(int i, int cliente);
    
protected:
    // Insere cliente no array
    int insereCliente(int cliente);
    // Remove cliente do array
    void removeCliente(int i);
    
    // Retorna o índice do próximo cliente a ser adicionado ou -1 quando não houver vaga
    int proximoCliente();
    
private:
    int conexao;
    int conexaoCliente;
    
    int porta;
    
    // Array de clientes: A posição no vetor corresponde ao índice da máquina
    int clientes[MAX_CLIENTES];
    
    
};

#endif /* SERVIDOR_H */

