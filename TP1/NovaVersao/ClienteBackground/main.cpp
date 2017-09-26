/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 25, 2017, 11:11 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <fstream>

#define MAX_CLIENTES 10
#define NOME_ARQUIVO "maquina.log"

#include "Cliente.h"

using namespace std;

void* thread_recv_aplicacao(void *arg);
void* thread_recv_servidor(void *arg);
void* thread_accept(void* arg);

pthread_t thread_r;
pthread_t thread_a;

Cliente* cliente;

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 4) {
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR> <PORTA_APLICACAO>", argv[0]);
        printf("Exemplo: %s 192.168.10.1 8000 8001", argv[0]);
        return 1;
    }

    char* ip_servidor = argv[1];
    int porta_servidor = atoi(argv[2]);
    int porta_aplicacao = atoi(argv[3]);
        
    pthread_create(&(thread_a), NULL, thread_accept, NULL);
    
    cliente = new Cliente(ip_servidor, porta_servidor, porta_aplicacao);
    
    while(true);

    return 0;
}

void* thread_recv_aplicacao(void *arg){
    
    while(true){
        Mensagem* m = cliente->receberDoGrep();
        Mensagem* m0;
        
        switch(m->getCodigo()){
            case 0:
                // Recebe mensagem 00
                m0 = new Mensagem(m->getCodigo()+1, m->getTexto());
                cliente->enviarAoServidor(m0);
                break;
            case 1:
                // Não se aplica
                break;
            case 2:
                // Não se aplica
                break;
            case 3:
                // Não se aplica
                break;
            case 4:
                // Não se aplica
                break;
            case 5:
                // Não se aplica
                break;
            default:
                // Não se aplica
                break;
        }
    
    }
    
}

void* thread_recv_servidor(void *arg){
    
    while(true){
        Mensagem* m = cliente->receberDoServidor();
        std::ifstream arq;
        string comando;
        string buffer;
        string resultado_grep = "";
        Mensagem* m2;
        Mensagem* m4;
        
        switch(m->getCodigo()){
            case 0:
                // Não se aplica
                break;
            case 1:
                // Não se aplica
                break;
            case 2:
                // Processa grep
                comando = m->getTexto() + " " + NOME_ARQUIVO + " > log.temp";
                system(comando.c_str());
                arq.open("log.temp");
                while (getline(arq, buffer)) {
                    resultado_grep += buffer;
                }
                
                // Envia mensagem 02
                m2 = new Mensagem(m->getCodigo()+1, resultado_grep);
                cliente->enviarAoServidor(m2);
                break;
            case 3:
                // Não se aplica
                break;
            case 4:
                // Recebe mensagem 04
                m4 = new Mensagem(m->getCodigo()+1, m->getTexto());
                cliente->enviarAoGrep(m4);
                break;
            case 5:
                // Não se aplica
                break;
            default:
                // Não se aplica
                break;
        }
        
        delete m2;
        delete m4;
        delete m;    
    }
    
}

void* thread_accept(void* arg){
    
    while(true){
        int i = cliente->aceitarGrep();
        pthread_create(&(thread_r), NULL, thread_recv_aplicacao, &i);
    }
    
}