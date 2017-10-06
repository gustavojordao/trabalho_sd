/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.cpp
 * Author: gustavo
 * 
 * Created on October 5, 2017, 9:28 PM
 */

#include "Node.h"

Node::Node() {
    this->num_nodes = 0;
}

Node::~Node() {
}

Node::Node(int indice, string ip_antecessor, int porta) {
    this->indice = indice;
    this->antecessor = new Cliente(ip_antecessor, porta);
    this->sucessor = new Servidor(porta);
    this->pares = *new vector<Pair*>();
    this->num_nodes = 0;
}

int Node::findPar(string value) {
    for(int i=0; i<pares.size(); i++){
        if(pares.at(i)->getKey().compare(value)){
            return i;
        }
    }
    
    // TODO: Pensar daqui para baixo
    int num_letras = 'z'-'a'+1;
    float num_nodes_f = num_nodes;
    float letras_por_node = num_letras/num_nodes_f;
    
    int ultima_letra = (int) ceil(indice*letras_por_node);
    return (int) ceil(indice*letras_por_node);
}

Cliente* Node::getAntecessor() {
    return antecessor;
}

int Node::getIndice() {
    return indice;
}

vector<Pair*> Node::getPares() {
    return pares;
}

Servidor* Node::getSucessor() {
    return sucessor;
}

void Node::setIndice(int indice) {
    this->indice = indice;
}

void Node::storePar(Pair* pair) {
    // TODO: Planejar
}
