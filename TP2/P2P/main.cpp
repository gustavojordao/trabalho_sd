/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on October 1, 2017, 2:52 PM
 */

#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>

#include "Node.h"

using namespace std;

void* thread_recv_ant(void *arg);
void* thread_recv_suc(void *arg);
void* thread_accept(void* arg);

pthread_t thread_r_ant;
pthread_t thread_r_suc;
pthread_t thread_a;

Node* node;

/*
 * 
 */
int main(int argc, char** argv) {

    pthread_create(&(thread_a), NULL, thread_accept, NULL);
    pthread_create(&(thread_r_ant), NULL, thread_recv_ant, NULL);
    pthread_create(&(thread_r_suc), NULL, thread_recv_suc, NULL);

    char linha[500];
    
    string ip;
    int porta;
    
    if(argc > 2){
        ip = argv[1];
        porta = atoi(argv[2]);
    }
    else{
        perror("Parâmetros insuficientes. Entre com o ip e a porta do servidor.");
        return 1;
    }
    
    // Requisita accept
    node = new Node();
    node->getSucessor()->conectarAoServidor();
    
    node->getAntecessor()->aceitarCliente();
    
    
    while(true){
        // Aguarda opções find/store
        printf("Entre com o comando:\n");
        printf("find <KEY>\n");
        printf("store <KEY> <VALUE>\n>");
        //scanf("%[^\r]");
        fgets(linha, 499, stdin);
        printf("%s\n", linha);
        
        stringstream ss(linha);
        string str_linha = linha;
        string item;
        vector<string> tokens;
        while (getline(ss, item, ' ')) {
            tokens.push_back(item);
        }
        
        string key;
        string value;
        
        if(tokens.size() > 3){
            if(tokens.at(0).compare("store") == 0){
                key = tokens.at(1);
                int qtd_letras = tokens.at(0).length() + 1 + tokens.at(1).length();
                value = str_linha.substr(qtd_letras);
            }
            else{
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        }
        else if(tokens.size() == 2){
            if(tokens.at(0).compare("find") == 0){
                key = tokens.at(1);
            }
            else{
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        }
        else{
            perror("Erro de sintaxe. Tente novamente.\n");            
        }
    }
    
    
    return 0;
}

void* thread_recv_ant(void *arg){
    
    int ind = *((int *) arg);
        
    while(true){
        
    }
}

void* thread_recv_suc(void *arg){
    
    int ind = *((int *) arg);
        
    while(true){
        
    }
}

void* thread_accept(void* arg){
    
    int ind = *((int *) arg);
        
    while(true){
        
        int conexao = node->getAntecessor()->aceitarCliente();
        
        node->desconectarAntecessor();
        
        node->setConexaoAntecessor(conexao);
        
        
    }
}
