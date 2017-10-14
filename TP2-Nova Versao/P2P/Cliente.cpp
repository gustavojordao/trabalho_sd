/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.cpp
 * Author: gustavo
 * 
 * Created on October 5, 2017, 9:27 PM
 */

#include "Cliente.h"

Cliente::~Cliente() {
}

Cliente::Cliente(string ip, int porta) {
    this->ip = ip;
    this->porta = porta;
}

void Cliente::conectar() {
    struct sockaddr_in serv_addr;
    socklen_t * addrlen;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(porta);

    printf("Do you know where are you going to? %d %d", porta, serv_addr.sin_port);
    
    conexao = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(conexao, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("Ocorreu um erro na conexão - connect");
        exit(1);
    }
    else{
        printf("Conectou(%s:%d)\n", ip.c_str(), porta);
    }
}

void Cliente::desconectar() {
    close(conexao);
}

int Cliente::enviar(Mensagem* m) {
    printf("\nEnviando mensagem do cliente...");
    fflush(stdout);

    int numBytes = 0;
    stringstream ss;
    ss << m->getCodigo() << "|" << m->getTexto();
    string str = ss.str();
    
    numBytes = ::send(conexao, str.c_str(), str.size(), 0);

    if (numBytes <= 0) {
        perror("\nNão foi possível enviar mensagem. - send");
    } else {
        printf("Enviou(%s)\n", str.c_str());
        fflush(stdout);
    }

    return numBytes;
}

Mensagem* Cliente::receber() {
    printf("\nRecebendo mensagem no cliente...");
    fflush(stdout);

    Mensagem* m;

    int numBytes = 0;
    char msg[1000];
    numBytes = ::recv(conexao, msg, 1000, 0);

    if (numBytes > 0) {
        printf("Recebeu(%s)\n", msg);
        fflush(stdout);
        return new Mensagem(msg);
    } else {
        perror("\nNão foi possível receber mensagem. - recv");
        return NULL;
    }
}

void Cliente::setEndereco(string ip, int porta) {
    this->ip = ip;
    this->porta = porta;
}

string Cliente::getIp() {
    return ip;
}

int Cliente::getPorta() {
    return porta;
}
