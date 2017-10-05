/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.cpp
 * Author: gustavo
 * 
 * Created on October 1, 2017, 10:22 PM
 */

#include "Cliente.h"

Cliente::Cliente(char* ip_servidor, int portaServidor, int portaGrep) {
    printf("Servidor %s", ip_servidor);
    
    this->portaServidor = portaServidor;
    sprintf(this->ip_servidor, ip_servidor);
}

Cliente::Cliente(const Cliente& orig) {
}

Cliente::~Cliente() {
}

void Cliente::conectarAoServidor(){
    struct sockaddr_in serv_addr;
    socklen_t * addrlen;
    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr(ip_servidor); 
    serv_addr.sin_port = htons(portaServidor);
    
    conexaoServidor = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(conexaoServidor, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Ocorreu um erro na conexão - connect");
        exit(1);
    }
}

void Cliente::encerrarServidor(){
    close(conexaoServidor);
}