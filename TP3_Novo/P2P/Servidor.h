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
#include <string>
#include <iostream>
using namespace std;

class Servidor
{
private:
	string ip_cliente;
	string ip_novo_cliente;
	int porta;

	int conexao;
	int conexaoCliente;
        int conexaoCliente_temp;
        
        static bool sendingBusy;
public:
	Servidor(int porta);
	~Servidor();
	
	void iniciar();
	void aceitar();
	void aceitarAposFalha();
        
	void setConexao(int conexao);

	int enviar(Mensagem* m);
	int enviarParaNovoCliente(Mensagem* m);
        Mensagem* receber();
        Mensagem* receberDoNovoCliente();

	string getIpCliente();
	string getIpNovoCliente();

	int getPorta();
	void setPorta(int porta);
        
        int getConexaoCliente();
        int getNovaConexaoCliente();
        
        static bool isSendingBusy();
};


#endif /* SERVIDOR_H */

