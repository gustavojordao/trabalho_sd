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

bool Cliente::sendingBusy = false;

Cliente::~Cliente() {
}

Cliente::Cliente(string ip, int porta) {
    this->ip = ip;
    this->porta = porta;
}

int Cliente::conectar() {
    struct sockaddr_in serv_addr;
    socklen_t * addrlen;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(porta);

    conexao = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(conexao, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("Ocorreu um erro na conexão - connect");
        //exit(1);
        return -1;
    }
    
    else{
        printf("Conectou(%s:%d)\n", ip.c_str(), porta);
        return 1;
    }
    
}

int Cliente::conectarAposFalha() {
    struct sockaddr_in serv_addr;
    socklen_t * addrlen;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(porta);

    conexao = socket(AF_INET, SOCK_STREAM, 0);
    // Retorna negativo quando não conseguiu conectar
    return connect(conexao, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
}


void Cliente::desconectar() {
    close(conexao);
}

int Cliente::enviar(Mensagem* m) {
    sleep(1);
    while (sendingBusy);

    sendingBusy = true;

    /*
    printf("\nEnviando mensagem do cliente...");
    fflush(stdout);
     */

    int numBytes = 0;
    stringstream ss;
    ss << m->getCodigo();
    
    if(m->getTexto().compare("") != 0){
        ss << "|" << m->getTexto();
    }
    
    ss << "@";
    string str = ss.str();

    numBytes = ::send(conexao, str.c_str(), str.size(), 0);
    fsync(conexao);

    
    if (numBytes <= 0) {
        perror("\nNão foi possível enviar mensagem. - send");
        printf(" -> %s", str.c_str());
        fflush(stdout);
    } else {
        if(m->getCodigo() != Mensagem::PING){        
            printf("Enviou(%s)\n", str.c_str());
            fflush(stdout);
        }
    }
     

    sendingBusy = false;
    return numBytes;
}

Mensagem* Cliente::receber() {
    /*
    printf("\nRecebendo mensagem no cliente...");
    fflush(stdout);
     */
    Mensagem* m;

    int numBytes = 0;
    char msg[1000];
    numBytes = ::recv(conexao, msg, 1000, 0);

    if (numBytes > 0) {
        stringstream ss;
        ss << msg;
        string str = ss.str().substr(0, ss.str().find_first_of('@'));
        
        if(str.substr(0, 2).compare("12") != 0){
            printf("Recebeu(%s)\n", str.c_str());
            fflush(stdout);
        }
        return new Mensagem(str);
    } else {
        //perror("\nNão foi possível receber mensagem. - recv");
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

bool Cliente::isSendingBusy() {
    return sendingBusy;
}
