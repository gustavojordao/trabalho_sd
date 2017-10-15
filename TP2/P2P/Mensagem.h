/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mensagem.h
 * Author: gustavo
 *
 * Created on October 5, 2017, 9:28 PM
 */

#ifndef MENSAGEM_H
#define MENSAGEM_H

#include "Pair.h"

#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
using namespace std;

class Mensagem
{
private:
	int codigo;
	string texto;

public:
	Mensagem();
	Mensagem(int codigo, string texto);
	Mensagem(string full_msg);
	~Mensagem();

	static Mensagem* criarMensagemFind(int solicitante, string key);
	static Mensagem* criarMensagemRespostaFind(int solicitante, int detentor, Pair* pair);
	static Mensagem* criarMensagemRespostaFindNaoEncontrado(int solicitante, string key);
        static Mensagem* criarMensagemStore(int solicitante, Pair* pair);
	
        static Mensagem* criarMensagemNovoNode(string ipNovoNode, int portaNovoNode);
	static Mensagem* criarMensagemAtualizacaoIndice(int indice);
        static Mensagem* criarMensagemAtualizacaoNodeAnt(int inicial, int num_nodes, vector<Pair*> pares);
        static Mensagem* criarMensagemAtualizacaoNodeSuc(int inicial, int num_nodes, vector<Pair*> pares);
        
        static Mensagem* criarMensagemSolicitacaoPorta();
        static Mensagem* criarRespostaMensagemSolicitacaoPorta(int porta);
        
	static const int FIND = 0;
	static const int RESPOSTA_FIND = 1;
	static const int RESPOSTA_FIND_NAO_ENCONTRADO = 2;
	static const int STORE = 3;
	
        static const int NOVO_NODE = 4;
	static const int ATUALIZACAO_INDICE = 5;
        static const int ATUALIZACAO_NODE_ANT = 6;
	static const int ATUALIZACAO_NODE_SUC = 7;
        
        static const int SOLICITACAO_PORTA = 8;
        static const int RESPOSTA_SOLICITACAO_PORTA = 9;
        
	int getCodigo();
	string getTexto();

	vector<string> getPartes();
};

#endif /* MENSAGEM_H */

