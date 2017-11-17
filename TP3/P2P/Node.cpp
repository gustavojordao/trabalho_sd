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
    
    stringstream ss;
    ss << "127.0.0.1:" << porta_sucessor;
    this->nodes = *new vector<string>();
    this->nodes.push_back(ss.str());
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
    this->nodes = *new vector<string>();
}

int Node::findPar(string key) {
    for (int i = 0; i < pares.size(); i++) {
        if (Pair::compareStrings(pares.at(i)->getKey(), key, false)) {
            return i;
        }
    }

    if (Pair::compareChars(key.at(0), getEnderecoInicial(), false) == -1) {
        return -1;
    }
    if (Pair::compareChars(key.at(0), getEnderecoFinal(), false) == 1) {
        return -2;
    }

    return -3;
}

void Node::setAntecessor(string ip, int porta) {
    this->ip_antecessor = ip;
    this->porta_antecessor = porta;
        
    antecessor = new Cliente(ip, porta);
}

Cliente* Node::getAntecessor() {
    return antecessor;
}

void Node::desconectarAntecessor(){
    this->ip_antecessor = "";
    this->porta_antecessor = 0;
        
    antecessor = NULL;
}

int Node::getIndice() {
    return indice;
}

void Node::incNumNodes() {
    num_nodes++;
}

void Node::decNumNodes() {
    num_nodes--;
}


void Node::setNumNodes(int numNodes) {
    this->num_nodes = numNodes;
}

int Node::getNumNodes() {
    return num_nodes;
}

vector<Pair*> Node::getPares() {
    return pares;
}

void Node::addNode(string node) {
    nodes.push_back(node);
}

vector<string> Node::getNodes() {
    return nodes;
}

void Node::removeNode(string node) {
    for(int i=0; i<nodes.size(); i++){
        if(node.compare(nodes.at(i)) == 0){
            nodes.erase(nodes.begin()+i);
        }
    }
}

void Node::removeNode(int indice) {
    nodes.erase(nodes.begin()+indice);
}

void Node::setNodes(vector<string> nodes) {
    this->nodes = nodes;
}

Servidor* Node::getSucessor() {
    return sucessor;
}

void Node::setIndice(int indice) {
    this->indice = indice;
}

void Node::storePar(Pair* pair) {
    for (int i = 0; i < pares.size(); i++) {
        if (Pair::compareStrings(pares.at(i)->getKey(), pair->getKey(), false)) {
            pares.at(i) = pair;
            return;
        }
    }

    pares.push_back(pair);
}

void Node::removePar(Pair* pair) {
    int ind = findPar(pair->getKey());

    if (ind >= 0)
        pares.erase(pares.begin() + ind);
}

int Node::getEnderecoInicial() {
    int end = 65 + (indice * ceil(26.0 / num_nodes));
    return end;
}

int Node::getEnderecoFinal() {
    int end = min((int) (65 + ((indice + 1) * ceil(26.0 / num_nodes)) - 1), 90);
    return end;
}

string Node::getEnderecoAntecessor() {
    stringstream ss;
    
    ss << ip_antecessor << ":" << porta_antecessor;
    
    return  ss.str();
}

int Node::getPortaAntecessor() {
    return porta_antecessor;
}

int Node::getPortaSucessor() {
    return porta_sucessor;
}


string Node::toString() {
    stringstream ss;
    ss << "Node: " << indice << "\n" << "Num. nodes: " << num_nodes << "\n"
            << "Endereço inicial: " << getEnderecoInicial() << " (" << (char) getEnderecoInicial() << ")" << "\n"
            << "Endereço final: " << getEnderecoFinal() << " (" << (char) getEnderecoFinal() << ")" << "\n"
            << "Pares (" << pares.size() << "):\n";
    for (int i = 0; i < pares.size(); i++) {
        ss << "[" << pares.at(i)->getKey() << "] = " << pares.at(i)->getValue() << "\n";
    }
    ss << "\n\nLista de Nodes:\n";
    for (int i = 0; i < nodes.size(); i++) {
        ss << nodes.at(i) << "\n";
    }
    
    return ss.str();

}