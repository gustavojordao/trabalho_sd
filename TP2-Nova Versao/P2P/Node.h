/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.h
 * Author: gustavo
 *
 * Created on October 5, 2017, 9:28 PM
 */

#ifndef NODE_H
#define NODE_H

#include "Servidor.h"
#include "Cliente.h"

#include <math.h>
#include <vector>
using namespace std;

class Node
{
private:
	int indice;
        int num_nodes;
	vector<Pair*> pares;

	Cliente* antecessor;
	Servidor* sucessor;
	
public:
	Node();
	Node(int indice, string ip_antecessor, int porta_antecessor, int porta_sucessor);
	~Node();

	void setIndice(int indice);
	int getIndice();
	
	void incNumNodes();
	void setNumNodes(int numNodes);
	int getNumNodes();
	
	// Retorna índice do par no vetor
	// Retorna -1 caso deve estar no antecessor
	// Retorna -2 caso deve estar no sucessor
	// Retorna -3 caso deveria estar neste nó mas não está
	int findPar(string value);
	void storePar(Pair* pair);
	vector<Pair*> getPares();

	Cliente* getAntecessor();
	Servidor* getSucessor();
	
	int getEnderecoInicial();
	int getEnderecoFinal();
	
};


#endif /* NODE_H */

