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

Mensagem::Mensagem(int codigo, string texto) {
    this->codigo = codigo;
    this->texto = texto;
}

Mensagem::Mensagem(string full_msg) {
    int index = full_msg.find_first_of('|');
    this->codigo = atoi(full_msg.substr(0, index).c_str());
    this->texto = full_msg.substr(index + 1);
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

Mensagem* Mensagem::criarMensagemStore(int solicitante, Pair* pair) {
    stringstream ss;
    ss << solicitante << "|" << pair->getKey() << "|" << pair->getValue();
    return new Mensagem(Mensagem::STORE, ss.str());
}

Mensagem* Mensagem::criarMensagemAtualizacaoIndice(int indice) {
    stringstream ss;
    ss << indice;
    return new Mensagem(Mensagem::ATUALIZACAO_INDICE, ss.str());
}

Mensagem* Mensagem::criarMensagemAtualizacaoNodeAnt(int inicial, int num_nodes, vector<Pair*> pares) {
    stringstream ss;
    ss << inicial << "|" << num_nodes;
    for (int i = 0; i < pares.size(); i++) {
        ss << "|" << pares.at(i)->getKey() << "|" << pares.at(i)->getValue();
    }
    return new Mensagem(Mensagem::ATUALIZACAO_NODE_ANT, ss.str());
}

Mensagem* Mensagem::criarMensagemAtualizacaoNodeSuc(int inicial, int num_nodes, vector<Pair*> pares) {
    stringstream ss;
    ss << inicial << "|" << num_nodes;

    for (int i = 0; i < pares.size(); i++) {
        ss << "|" << pares.at(i)->getKey() << "|" << pares.at(i)->getValue();
    }

    return new Mensagem(Mensagem::ATUALIZACAO_NODE_SUC, ss.str());
}

Mensagem* Mensagem::criarMensagemAtualizacaoListaNodes(int inicial, vector<string> nodes) {
    stringstream ss;
    ss << inicial;

    for (int i = 0; i < nodes.size(); i++) {
        ss << "|" << nodes.at(i);
    }

    return new Mensagem(Mensagem::ATUALIZACAO_LISTA_NODES, ss.str());

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

Mensagem* Mensagem::criarMensagemRespostaFindNaoEncontrado(int solicitante, string key) {
    stringstream ss;
    ss << solicitante << "|" << key;
    return new Mensagem(Mensagem::RESPOSTA_FIND, ss.str());
}

Mensagem* Mensagem::criarMensagemNotificacaoFalha(int indice_informante, int indice_falha) {
    stringstream ss;
    ss << indice_informante << "|" << indice_falha;
    return new Mensagem(Mensagem::NOTIFICACAO_FALHA, ss.str());
}

Mensagem* Mensagem::criarMensagemPing() {
    return new Mensagem(Mensagem::PING, "");
}

Mensagem* Mensagem::criarMensagemAtualizacaoIndiceAposFalha(int inicial, int indice_falha, int num_nodes) {
    stringstream ss;
    ss << inicial << "|" << indice_falha << "|" << num_nodes;
    return new Mensagem(Mensagem::ATUALIZACAO_INDICE_APOS_FALHA, ss.str());
}

Mensagem* Mensagem::criarMensagemRespostaAtualizacaoIndice()
{
    return new Mensagem(Mensagem::RESPOSTA_ATUALIZACAO_INDICE, "");
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

