/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.h
 * Author: gustavo
 *
 * Created on October 5, 2017, 9:27 PM
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include "Mensagem.h"
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;

class Cliente
{
private:
	string ip;
	int porta;

        int conexao;
public:
	Cliente(string ip, int porta);
	~Cliente();
	
	void conectar();
	void desconectar();

	int enviar(Mensagem* m);
	Mensagem* receber();

	void setEndereco(string ip, int porta);
        
        string getIp();
};


#endif /* CLIENTE_H */

