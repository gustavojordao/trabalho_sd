/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.h
 * Author: gustavo
 *
 * Created on October 1, 2017, 2:54 PM
 */

#ifndef NODE_H
#define NODE_H

#include "Pair.h"
#include "Servidor.h"
#include "Cliente.h"
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    Node();
    Node(const Node& orig);
    virtual ~Node();
    
    int getId();
    void setId(int id);
    
    Servidor* getAntecessor();
    Cliente* getSucessor();
    
    void setConexaoAntecessor(int conexaoAntecessor);
    void setConexaoSucessor(int conexaoSucessor);
    
    void desconectarAntecessor();
    void desconectarSucessor();
    
    void find();
    void store();
    
    void adicionarPair(Pair* p);
    
    // Retorna posição do Pair
    // Retorna -1 caso não exista no array e é caracterizado como antecessor
    // Retorna -2 caso não exista no array e é caracterizado como sucessor
    int localizarPair(Pair* p);
    
private:

    int id;
    
    int conexaoAntecessor;
    int conexaoSucessor;
    
    vector<Pair> pares;
    
    Servidor* antecessor;
    Cliente* sucessor;
};

#endif /* NODE_H */

