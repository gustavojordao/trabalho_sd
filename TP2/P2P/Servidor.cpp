/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Servidor.cpp
 * Author: gustavo
 * 
 * Created on October 1, 2017, 9:36 PM
 */

#include "Servidor.h"

Servidor::Servidor(int porta) {
    this->porta = porta;
}

Servidor::Servidor(const Servidor& orig) {
}

Servidor::~Servidor() {
}

void Servidor::iniciar(){
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(porta);
    
    socklen_t * addrlen;
    
    conexao = socket(AF_INET, SOCK_STREAM, 0);
    if(bind(conexao, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Ocorreu um erro na conexão - bind");
        ::exit(1);
    }
    
    if(listen(conexao, 10) != 0){
        perror("Ocorreu um erro na conexão - listen");
        ::exit(1);
    }
}

int Servidor::aceitarCliente(){
    struct sockaddr_in cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    
    conexaoCliente = accept(conexao,(struct sockaddr *) &cli_addr, &addrlen);

    if(conexaoCliente <= 0){
        perror("\nNão foi possível aceitar conexão. - accept");
    }
    
    return 1;
}

void Servidor::encerrarServidor(){
    ::close(conexao);
}

void Servidor::encerrarCliente(){
    ::close(conexaoCliente);
}
