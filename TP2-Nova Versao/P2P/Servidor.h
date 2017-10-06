/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Servidor.h
 * Author: gustavo
 *
 * Created on October 5, 2017, 9:30 PM
 */

#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "Mensagem.h"

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Servidor
{
private:
	string ip_cliente;
	int porta;

	int conexao;
	int conexaoCliente;
        int conexaoCliente_temp;
public:
	Servidor(int porta);
	~Servidor();
	
	void iniciar();
	void aceitar();
	
	void setConexao(int conexao);

	int enviar(Mensagem* m);
	Mensagem* receber();

	string getIpCliente();

	int getPorta();
	void setPorta(int porta);
        
        int getConexaoCliente();
};


#endif /* SERVIDOR_H */

