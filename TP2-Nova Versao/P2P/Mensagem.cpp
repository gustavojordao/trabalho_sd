/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mensagem.cpp
 * Author: gustavo
 * 
 * Created on October 5, 2017, 9:28 PM
 */

#include <stdio.h>

#include "Mensagem.h"

Mensagem::Mensagem() {
    this->codigo = 0;
    this->texto = "";
}

Mensagem::Mensagem(int codigo, string texto){
    this->codigo = codigo;
    this->texto = texto;
}

Mensagem::Mensagem(string full_msg){
    int index = full_msg.find_first_of('|');
    this->codigo = atoi(full_msg.substr(0, index).c_str());
    this->texto = full_msg.substr(index+1);    
}

Mensagem::~Mensagem() {
}

Mensagem* Mensagem::criarMensagemFind(int solicitante, string key) {
    stringstream ss;
    ss << solicitante << "|" << key;
    return new Mensagem(Mensagem::FIND, ss.str());
}

Mensagem* Mensagem::criarMensagemNovoNode(string ipNovoNode, int portaNovoNode) {
    stringstream ss;
    ss << ipNovoNode << "|" << portaNovoNode;
    return new Mensagem(Mensagem::NOVO_NODE, ss.str());
}

Mensagem* Mensagem::criarMensagemRespostaFind(int solicitante, int detentor, Pair* pair) {
    stringstream ss;
    ss << solicitante << "|" << detentor << "|" << pair->getKey() << "|" << pair->getValue();
    return new Mensagem(Mensagem::RESPOSTA_FIND, ss.str());
}

Mensagem* Mensagem::criarMensagemRespostaSolicitacaoIndice(int indice) {
    stringstream ss;
    ss << indice;
    return new Mensagem(Mensagem::RESPOSTA_SOLICITACAO_INDICE, ss.str());
}

Mensagem* Mensagem::criarMensagemSolicitacaoIndice() {
    stringstream ss;
    ss << "";
    return new Mensagem(Mensagem::SOLICITACAO_INDICE, ss.str());
}

Mensagem* Mensagem::criarMensagemStore(int solicitante, Pair* pair) {
    stringstream ss;
    ss << solicitante << "|" << pair->getKey() << "|" << pair->getValue();
    return new Mensagem(Mensagem::STORE, ss.str());
}

Mensagem* Mensagem::criarMensagemAtualizacaoNodeAnt(int inicial, int num_nodes, vector<Pair*> pares) {
    // TODO: Mensagem de atualização de nós
    // Nó sucessor recebe pares (key, value) de acordo com a mudança de nó
    // Pensar no cálculo: deve seguir mesmo cálculo que estabelecimento de próximo índice
    stringstream ss;
    ss << inicial << "|" << num_nodes << "|";
	for(int i=0; i<pares.size(); i++){
            ss << pares.at(i)->getKey() << "|" << pares.at(i)->getValue();
	}
	
    return new Mensagem(Mensagem::ATUALIZACAO_NODE_ANT, ss.str());
}

Mensagem* Mensagem::criarMensagemAtualizacaoNodeSuc(int inicial, int num_nodes, vector<Pair*> pares) {
    // TODO: Mensagem de atualização de nós
    // Nó sucessor recebe pares (key, value) de acordo com a mudança de nó
    // Pensar no cálculo: deve seguir mesmo cálculo que estabelecimento de próximo índice
    stringstream ss;
    ss << inicial << "|" << num_nodes << "|";
	for(int i=0; i<pares.size(); i++){
		ss << pares.at(i)->getKey() << "|" << pares.at(i)->getValue();
	}
	
    return new Mensagem(Mensagem::ATUALIZACAO_NODE_SUC, ss.str());
}

Mensagem* Mensagem::criarMensagemSolicitacaoPorta() {
    stringstream ss;
    ss << "";
    return new Mensagem(SOLICITACAO_PORTA, ss.str());
}

Mensagem* Mensagem::criarRespostaMensagemSolicitacaoPorta(int porta) {
    stringstream ss;
    ss << porta;
    return new Mensagem(RESPOSTA_SOLICITACAO_PORTA, ss.str());
}


int Mensagem::getCodigo() {
    return codigo;
}

vector<string> Mensagem::getPartes() {
    stringstream ss(texto);
    string item;
    vector<string> tokens;
    while (getline(ss, item, '|')) {
        tokens.push_back(item);
    }
    return tokens;
}

string Mensagem::getTexto() {
    return texto;
}

