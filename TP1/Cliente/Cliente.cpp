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
    printf("Servidor %s", ip_servidor);
    
    this->portaServidor = portaServidor;
    this->portaGrep = portaGrep;
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
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(serv_addr);
        
    acceptGrep = accept(conexaoGrep, (struct sockaddr *) &serv_addr, &addrlen);
    
    if(acceptGrep <= 0){
        perror("\nNão foi possível aceitar conexão. - accept");
    }
    
    return acceptGrep;
}

void Cliente::encerrarGrep(){
    close(acceptGrep);
}

int Cliente::enviarAoGrep(Mensagem* mensagem){
    int numBytes = 0;
    char msg[255]; 
    mensagem->toChar(msg);
    numBytes = ::send(acceptGrep, msg, strlen(msg), 0);
    
    if(numBytes <= 0){
        perror("\nNão foi possível enviar mensagem. - send");
    }
    
    return numBytes;
}

Mensagem* Cliente::receberDoGrep(){
    sleep(1);
    int numBytes = 0;
    char msg[255];
    
    numBytes = recv(acceptGrep, msg, 255, 0);
    
    if(numBytes > 0)
        return new Mensagem(msg);
    else{
        perror("\nNão foi possível receber mensagem. - recv");
        return NULL;
    }
}

int Cliente::enviarAoServidor(Mensagem* mensagem) {
    int numBytes = 0;
    char msg[255]; 
    mensagem->toChar(msg);
    numBytes = ::send(conexaoServidor, msg, strlen(msg), 0);
    
    if(numBytes <= 0){
        perror("\nNão foi possível enviar mensagem. - send");
    }
    
    return numBytes;
}

    
Mensagem* Cliente::receberDoServidor(){
    int numBytes = 0;
    char msg[255]; 
    numBytes = ::recv(conexaoServidor, msg, 255, 0);
    
    if(numBytes > 0)
        return new Mensagem(msg);
    else{
        perror("\nNão foi possível receber mensagem. - recv");
        return NULL;
    }
}

void Cliente::encerrarServidor(){
    close(conexaoServidor);
}