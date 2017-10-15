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

Node::Node(int porta_sucessor) {
    this->porta_sucessor = porta_sucessor;
    
    this->indice = 0;
    this->antecessor = NULL;
    this->sucessor = new Servidor(porta_sucessor);
    this->pares = *new vector<Pair*>();
    this->num_nodes = 1;
}

Node::Node(string ip_antecessor, int porta_antecessor, int porta_sucessor) {
    this->ip_antecessor = ip_antecessor;
    this->porta_antecessor = porta_antecessor;
    this->porta_sucessor = porta_sucessor;
    
    this->indice = 0;
    this->antecessor = new Cliente(ip_antecessor, porta_antecessor);
    this->sucessor = new Servidor(porta_sucessor);
    this->pares = *new vector<Pair*>();
    this->num_nodes = 1;
}

int Node::findPar(string key) {
    for(int i=0; i<pares.size(); i++){
        if(pares.at(i)->getKey().compare(key) == 0){
            return i;
        }
    }
    
    // TODO: Pensar daqui para baixo
    if(key.at(0) < getEnderecoInicial()){
        return -1;
    }
    else if(key.at(0) > getEnderecoFinal()){
        return -2;
    }
	
    return -3;
}

void Node::setAntecessor(string ip, int porta) {
    antecessor = new Cliente(ip, porta);
}

Cliente* Node::getAntecessor() {
    return antecessor;
}

int Node::getIndice() {
    return indice;
}

void Node::incNumNodes(){
	num_nodes++;
}

void Node::setNumNodes(int numNodes){
	this->num_nodes = numNodes;
}

int Node::getNumNodes(){
	return num_nodes;
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
    for(int i=0; i<pares.size(); i++){
        if(pares.at(i)->getKey().compare(pair->getKey()) == 0){
            pares.at(i) = pair;
            return;
        }
    }

    pares.push_back(pair);
}

void Node::removePar(Pair* pair) {
    int ind = findPar(pair->getKey());
    
    if(ind >= 0)
        pares.erase(pares.begin()+ind);
}


int Node::getEnderecoInicial(){
	//return 65 + (indice*26/num_nodes) + (indice < 26%num_nodes ? 1 : 0);
    int end = 65 + (indice*ceil(26.0/num_nodes));    
    printf("\nEnd. inicial: %d", end);
    fflush(stdout);
    return end;
}

int Node::getEnderecoFinal(){
	//return 65 + ((indice+1)*26/num_nodes) + (indice+1 < 26%num_nodes ? 1 : 0)-1;
    int end = min((int) (65 + ((indice+1)*ceil(26.0/num_nodes))-1),90);
    printf("\nEnd. final: %d", end);
    fflush(stdout);
    return end;
}

string Node::toString() {
    stringstream ss;
    ss << "Node: " << indice << "\n" << "Num. nodes: " << num_nodes << "\n" 
            << "Endereço inicial: " << getEnderecoInicial() << "\n"
            << "Endereço final: " << getEnderecoFinal() << "\n"
            << "Pares (" << pares.size() << "):\n";
    for(int i=0; i<pares.size(); i++){
        ss << "[" << pares.at(i)->getKey() << "] = " << pares.at(i)->getValue() << "\n";
    }
    
    return ss.str();

}