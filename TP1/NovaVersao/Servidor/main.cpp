/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 25, 2017, 9:25 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTES 10

#include "Servidor.h"

using namespace std;

void* thread_recv(void *arg);
void* thread_accept(void* arg);

pthread_t thread_r[MAX_CLIENTES];
pthread_t thread_a;

Servidor* servidor;

Mensagem* mensagens[MAX_CLIENTES];

int qtd_clientes = 0;

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Parâmetros insuficientes: entre com a porta do servidor.\n");
        printf("%s <PORTA>\n", argv[0]);
        printf("Exemplo: %s 8000\n", argv[0]);
        //return 1;
    }

    int porta = atoi(argv[1]);
    
    for(int i=0; i<MAX_CLIENTES; i++){
        mensagens[i] = NULL;
    }

    pthread_create(&(thread_a), NULL, thread_accept, NULL);
    
    servidor = new Servidor(porta);
    
    while(true);
    
    return 0;
}

void* thread_recv(void *arg){
    
    int ind = *((int *) arg);
    
    while(true){
        Mensagem* m = servidor->receber(ind);
        Mensagem* m2;
        Mensagem* m5;
        
        bool faltamMensagens = false;
        string mensagem_agrupada = "";
                
        switch(m->getCodigo()){
            case 0:
                // Não se aplica
                break;
            case 1:
                // Envia mensagem 02
                m2 = new Mensagem(m->getCodigo()+1, m->getTexto());
                servidor->enviar(ind, m2);
                break;
            case 2:
                // Não se aplica
                break;
            case 3:
                // Verifica se todas as mensagens já foram obtidas
                faltamMensagens = false;
                mensagens[ind] = m;
                for(int i=0; i<qtd_clientes; i++){
                    if(mensagens[i] == NULL){
                        faltamMensagens = true;
                        break;
                    }
                }
                
                if(!faltamMensagens){
                    // Agrupa mensagens para enviar
                    mensagem_agrupada = "";
                    for(int i=0; i<qtd_clientes; i++){
                        mensagem_agrupada += "\n\nMáquina ";
                        mensagem_agrupada += i + "\n" + mensagens[i]->getTexto();
                        mensagens[i] = NULL;
                    }
                    
                    // Envia mensagem 05
                    m5 = new Mensagem(m->getCodigo()+1, mensagem_agrupada);
                    servidor->enviar(ind, m5);
                }
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
    
        delete m2;
        delete m5;
        delete m;
    }
    
}

void* thread_accept(void* arg){
    
    while(true){
        int ind = servidor->aceitarCliente();
        qtd_clientes++;
        pthread_create(&(thread_r[ind]), NULL, thread_recv, &ind);
    }
    
}
