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
    
    this->codigo = atoi(full_msg.substr(0, index-1).c_str());
    this->texto = full_msg.substr(index);    
}

Mensagem::~Mensagem() {
}

Mensagem* Mensagem::criarMensagemFind(int solicitante, string key) {
    string str = "";
    str += solicitante + "|" + key;
    return new Mensagem(Mensagem::FIND, str);
}

Mensagem* Mensagem::criarMensagemNovoNode(string ipNovoNode, int portaNovoNode) {
    string str = "";
    str += ipNovoNode;
    str += "|";
    str += portaNovoNode;
    return new Mensagem(Mensagem::NOVO_NODE, str);
}

Mensagem* Mensagem::criarMensagemRespostaFind(int solicitante, int detentor, Pair* pair) {
    string str = "";
    str += solicitante;
    str += "|";
    str += detentor;
    str += "|";
    str += pair->getKey();
    str += "|";
    str += pair->getValue();
    return new Mensagem(Mensagem::RESPOSTA_FIND, str);
}

Mensagem* Mensagem::criarMensagemRespostaSolicitacaoIndice(int indice) {
    string str = "" + indice;
    return new Mensagem(Mensagem::RESPOSTA_SOLICITACAO_INDICE, str);
}

Mensagem* Mensagem::criarMensagemSolicitacaoIndice() {
    string str = "";
    return new Mensagem(Mensagem::SOLICITACAO_INDICE, str);
}

Mensagem* Mensagem::criarMensagemStore(int solicitante, Pair* pair) {
    string str = solicitante + "|" + pair->getKey() + "|" + pair->getValue();
    return new Mensagem(Mensagem::STORE, str);
}

Mensagem* Mensagem::criarMensagemAtualizacaoNodeAnt(int inicial, int num_nodes, vector<Pair*> pares) {
    // TODO: Mensagem de atualização de nós
    // Nó sucessor recebe pares (key, value) de acordo com a mudança de nó
    // Pensar no cálculo: deve seguir mesmo cálculo que estabelecimento de próximo índice
    string str = inicial + "|" + num_nodes + "|";
	for(int i=0; i<pares.size(); i++){
		str += pares.at(i).getKey() + "|" + pares.at(i).getValue();
	}
	
    return new Mensagem(Mensagem::ATUALIZACAO_NODE_ANT, str);
}

Mensagem* Mensagem::criarMensagemAtualizacaoNodeSuc(int inicial, int num_nodes, vector<Pair*> pares) {
    // TODO: Mensagem de atualização de nós
    // Nó sucessor recebe pares (key, value) de acordo com a mudança de nó
    // Pensar no cálculo: deve seguir mesmo cálculo que estabelecimento de próximo índice
    string str = inicial + "|" + num_nodes + "|";
	for(int i=0; i<pares.size(); i++){
		str += pares.at(i).getKey() + "|" + pares.at(i).getValue();
	}
	
    return new Mensagem(Mensagem::ATUALIZACAO_NODE_SUC, str);
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
