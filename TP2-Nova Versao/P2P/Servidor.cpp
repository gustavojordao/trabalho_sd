/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Servidor.cpp
 * Author: gustavo
 * 
 * Created on October 5, 2017, 9:30 PM
 */

#include "Servidor.h"

Servidor::~Servidor() {
}

void Servidor::aceitar() {
    printf("\nAguardando aceite...");
    fflush(stdout);

    struct sockaddr_in cliente_addr;
    socklen_t addrlen = sizeof (cliente_addr);

    conexaoCliente = accept(conexao, (struct sockaddr *) &cliente_addr, &addrlen);

    if (conexaoCliente <= 0) {
        perror("\nNão foi possível aceitar conexão. - accept");
    }

    ip_cliente = inet_ntoa(cliente_addr.sin_addr);
    porta = (int) ntohs(cliente_addr.sin_port);

    printf("Ok\n");
    fflush(stdout);

}

int Servidor::enviar(Mensagem* m) {
    printf("\nEnviando mensagem do servidor...");
    fflush(stdout);

    int numBytes = 0;
    string str = m->getCodigo() + "|" + m->getTexto();
    numBytes = ::send(conexao, str.c_str(), str.size(), 0);

    if (numBytes <= 0) {
        perror("\nNão foi possível enviar mensagem. - send");
    } else {
        printf("Ok\n");
        fflush(stdout);
    }

    return numBytes;
}

Mensagem* Servidor::receber() {
    printf("\nRecebendo mensagem no servidor...");
    fflush(stdout);

    Mensagem* m;

    int numBytes = 0;
    char msg[1000];
    numBytes = ::recv(conexao, msg, 1000, 0);

    if (numBytes > 0) {
        printf("Ok\n");
        fflush(stdout);
        return new Mensagem(msg);
    } else {
        perror("\nNão foi possível receber mensagem. - recv");
        return NULL;
    }
}

int Servidor::getConexaoCliente() {
    return conexaoCliente;
}

string Servidor::getIpCliente() {
    return ip_cliente;
}

int Servidor::getPorta() {
    return porta;
}

void Servidor::iniciar() {
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(porta);

    socklen_t * addrlen;

    conexao = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(conexao, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("Ocorreu um erro na conexão - bind");
        ::exit(1);
    }

    if (listen(conexao, 2) != 0) {
        perror("Ocorreu um erro na conexão - listen");
        ::exit(1);
    }
}

void Servidor::setConexao(int conexao) {
    this->conexaoCliente = conexao;
}

void Servidor::setPorta(int porta) {
    this->porta = porta;
}

Servidor::Servidor(int porta) {
    this->porta = porta;
}

