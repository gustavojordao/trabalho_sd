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

bool Servidor::sendingBusy = false;

Servidor::~Servidor() {
}

void Servidor::aceitar() {
    /*
    printf("\nAguardando aceite...");
    fflush(stdout);
     */
    struct sockaddr_in cliente_addr;
    socklen_t addrlen = sizeof (cliente_addr);

    conexaoCliente_temp = accept(conexao, (struct sockaddr *) &cliente_addr, &addrlen);

    struct sockaddr_in cliente_addr_aux;
    socklen_t addrlen_aux = sizeof (cliente_addr_aux);
    getsockname(conexaoCliente_temp, (struct sockaddr *) &cliente_addr_aux, &addrlen_aux);

    ip_novo_cliente = inet_ntoa(cliente_addr_aux.sin_addr);
    porta = ntohs(cliente_addr_aux.sin_port);

    if (conexaoCliente <= 0) {
        conexaoCliente = conexaoCliente_temp;
        ip_cliente = ip_novo_cliente;
    }

    if (conexaoCliente_temp <= 0) {
        perror("\nNão foi possível aceitar conexão. - accept");
    }

    /*
    printf("Aceitou(%s:%d)\n", ip_novo_cliente.c_str(), porta);
    fflush(stdout);
     */
}

int Servidor::enviar(Mensagem* m) {
    sleep(1);
    while (sendingBusy);

    sendingBusy = true;

    /*
    printf("\nEnviando mensagem do servidor...");
    fflush(stdout);
     */

    stringstream ss;
    int numBytes = 0;

    ss << m->getCodigo() << ss.str() << "|" << m->getTexto() << "@";

    string str = ss.str();

    numBytes = ::send(conexaoCliente, str.c_str(), str.size(), 0);
    fsync(conexao);

    /*
    if (numBytes <= 0) {
        perror("\nNão foi possível enviar mensagem. - send");
    } else {
        printf("Enviou(%s)\n", str.c_str());
        fflush(stdout);
    }
     */

    sendingBusy = false;

    return numBytes;
}

int Servidor::enviarParaNovoCliente(Mensagem* m) {
    sleep(1);
    while (sendingBusy);

    sendingBusy = true;

    /*
    printf("\nEnviando mensagem do servidor...");
    fflush(stdout);
     */

    stringstream ss;
    int numBytes = 0;

    ss << m->getCodigo() << ss.str() << "|" << m->getTexto() << "@";

    string str = ss.str();

    numBytes = ::send(conexaoCliente_temp, str.c_str(), str.size(), 0);
    fsync(conexao);

    /*
    if (numBytes <= 0) {
        perror("\nNão foi possível enviar mensagem. - send");
    } else {
        printf("Enviou(%s)\n", str.c_str());
        fflush(stdout);
    }
     */

    sendingBusy = false;

    return numBytes;
}


Mensagem* Servidor::receber() {
    /*
    printf("\nRecebendo mensagem no servidor...");
    fflush(stdout);
     */
    Mensagem* m;

    int numBytes = 0;
    char msg[1000];
    numBytes = ::recv(conexaoCliente, msg, 1000, 0);

    if (numBytes > 0) {
        stringstream ss;
        ss << msg;
        string str = ss.str().substr(0, ss.str().find_first_of('@'));

        /*
        printf("Recebeu(%s)\n", str.c_str());
        fflush(stdout);
         */
        return new Mensagem(str);
    } else {
        //perror("\nNão foi possível receber mensagem. - recv");
        return NULL;
    }
}

Mensagem* Servidor::receberDoNovoCliente() {
    /*
    printf("\nRecebendo mensagem no servidor...");
    fflush(stdout);
     */
    Mensagem* m;

    int numBytes = 0;
    char msg[1000];
    numBytes = ::recv(conexaoCliente_temp, msg, 1000, 0);

    if (numBytes > 0) {
        stringstream ss;
        ss << msg;
        string str = ss.str().substr(0, ss.str().find_first_of('@'));

        /*
        printf("RecebeuNovo(%s)\n", str.c_str());
        fflush(stdout);
         */
        return new Mensagem(str);
    } else {
        //perror("\nNão foi possível receber mensagem. - recv");
        return NULL;
    }
}

int Servidor::getConexaoCliente() {
    return conexaoCliente;
}

string Servidor::getIpCliente() {
    return ip_cliente;
}

string Servidor::getIpNovoCliente() {
    return ip_novo_cliente;
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
    this->conexaoCliente = -1;
    this->conexaoCliente_temp = -1;
}

int Servidor::getNovaConexaoCliente() {
    return this->conexaoCliente_temp;
}

bool Servidor::isSendingBusy() {
    return sendingBusy;
}
