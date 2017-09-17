/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.cpp
 * Author: gustavo
 * 
 * Created on September 16, 2017, 4:44 PM
 */

#include "Cliente.h"

Cliente::Cliente(char* ip_servidor, int portaServidor, int portaGrep) {
    strcpy(this->ip_servidor, ip_servidor);
    this->portaServidor = portaServidor;
    this->portaGrep = portaGrep;
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

void Cliente::iniciarGrep(){
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(portaGrep);
    
    socklen_t * addrlen;
    
    conexaoGrep = socket(AF_INET, SOCK_STREAM, 0);
    if(bind(conexaoGrep, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Ocorreu um erro na conexão - bind");
        exit(1);
    }
    
    if(listen(conexaoGrep, 1) != 0){
        perror("Ocorreu um erro na conexão - listen");
        exit(1);
    }
}

int Cliente::aceitarGrep(){
    struct sockaddr * addr;
    socklen_t * addrlen;
        
    int cliente = accept(conexaoGrep, addr, addrlen);
    
    return cliente;
}

void Cliente::encerrarGrep(){
    close(conexaoGrep);
}

int Cliente::enviarAoGrep(Mensagem* mensagem){
    int numBytes = 0;
    char msg[255]; 
    mensagem->toChar(msg);
    numBytes = ::send(conexaoGrep, msg, strlen(msg), 0);
    
    return numBytes;
}

Mensagem* Cliente::receberDoGrep(){
    int numBytes = 0;
    char msg[255]; 
    numBytes = ::recv(conexaoGrep, msg, 255, 0);
    
    if(numBytes > 0)
        return new Mensagem(msg);
    else
        return NULL;
}

int Cliente::enviarAoServidor(Mensagem* mensagem) {
    int numBytes = 0;
    char msg[255]; 
    mensagem->toChar(msg);
    numBytes = ::send(conexaoServidor, msg, strlen(msg), 0);
    
    return numBytes;
}

    
Mensagem* Cliente::receberDoServidor(){
    int numBytes = 0;
    char msg[255]; 
    numBytes = ::recv(conexaoServidor, msg, 255, 0);
    
    if(numBytes > 0)
        return new Mensagem(msg);
    else
        return NULL;
}

void Cliente::encerrarServidor(){
    close(conexaoServidor);
}