/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 16, 2017, 12:09 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

#include "Servidor.h"

using namespace std;

void* thread_cliente(void* arg);

void* thread_conexao(void* arg);

pthread_t threads[MAX_CLIENTES];
pthread_t thread_reinicia;

Servidor* servidor = NULL;

Mensagem* mensagens[MAX_CLIENTES];

int cliente = -1;

int qtd_clientes = 0;

bool finalizado = false;

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

    servidor = new Servidor(porta);

    printf("Iniciando servidor na porta %d...", porta);
    fflush(stdout);

    servidor->iniciar();

    printf("Ok\n");
    fflush(stdout);

    for(int i=0; i<MAX_CLIENTES; i++){
        mensagens[i] = NULL;
    }
    
    pthread_create(&(thread_reinicia), NULL, thread_conexao, NULL);
                
    while(true){
        
//        if(cliente == -1){
        
            printf("\nAguardando cliente...Pid=%d",getpid());
            fflush(stdout);

            int indice_cliente = servidor->aceitarCliente();

            if(indice_cliente >= 0){
                qtd_clientes++;
                pthread_create(&(threads[indice_cliente]), NULL, thread_cliente, &indice_cliente);
            }
//        }
    }
    
    return 0;
}

void* thread_conexao(void* arg){

    int s;
    
    if(finalizado){
        pthread_cancel(threads[cliente]); /////////////////////////////////////////////
        for(int i=0; i<qtd_clientes; i++){
            pthread_join(threads[i], NULL);
        }
        
        printf("\nRecria threads");
        fflush(stdout);
        
        for(int i=0; i<qtd_clientes; i++){
            // if(cliente != i){
                s = pthread_create(&(threads[i]), NULL, thread_cliente, &i);
                if (s != 0)
                   perror("pthread_create");
            // }
            
            finalizado = false;

        }
    }
}

void* thread_cliente(void* arg){

    int indice = *((int*) arg);
    
    void* res;
    int s;
//    if(cliente == -1 || cliente == indice)
    
    Mensagem* m;
    
    while(true){

        m = servidor->receber(indice);

        if(m != NULL){
            /*for(int i=0; i<qtd_clientes; i++){
                if(indice != i){
                    s = pthread_cancel(threads[i]);
                    if (s != 0)
                       perror("pthread_cancel");
                }
            }*/
           
            if(cliente == -1){
            
                cliente = indice;

                printf("\nCliente %d fez requisição", indice);
                fflush(stdout);

                printf("\nBroadcast de requisição");
                fflush(stdout);
                m->setCodigo(2);
                //vector<Mensagem*> mensagens = servidor->enviarReceberTodos(m);
                
                // Não dá para colocar essa parte na classe porque depende do array
                // de mensagens
                vector<Mensagem*>* v_mensagens = new vector<Mensagem*>();
                char str[50];
                Mensagem* mensagem;
                for(int i=0; i<MAX_CLIENTES; i++){
                    if(servidor->getCliente(i) != -1){
                        printf("\nEnviando para Cliente %d...", i);
                        fflush(stdout);
                        servidor->enviar(i, m);
                        printf("Ok\n", i);
                        fflush(stdout);
                        mensagem = NULL;
                        while(mensagem == NULL){
                            mensagem = mensagens[i];
                        }
                        //m = receber(i);
                        mensagens[i] = NULL;
                        sprintf(str, "----------\nMaquina %d\n----------\n", i);
                        mensagem->setTexto(str + mensagem->getTexto());
                        v_mensagens->push_back(mensagem);

                    }
                }
                
                
                m = servidor->agruparMensagens(*v_mensagens);

                printf("\nResposta ao cliente %d", indice);
                fflush(stdout);
                m->setCodigo(4);
                servidor->enviar(indice, m);
                
                cliente = -1;
            }
            else{
                mensagens[indice] = m;
            }
        }
        
        finalizado = true;
    }
}


int _main(int argc, char** argv) {

    if (argc < 2) {
        printf("Parâmetros insuficientes: entre com a porta do servidor.\n");
        printf("%s <PORTA>\n", argv[0]);
        printf("Exemplo: %s 8000\n", argv[0]);
        //return 1;
    }

    int porta = atoi(argv[1]);

    Servidor* servidor = new Servidor(porta);

    printf("Iniciando servidor na porta %d...", porta);
    fflush(stdout);

    servidor->iniciar();

    printf("Ok\n");
    fflush(stdout);

    int filhoNovaConexao = 0;
    int filhoAceitaRecebe = 0;

    int indice_cliente = -1;
    int parent_pid = getpid();
    
    int fd[2];
    while (true) {

            if (getpid() == parent_pid){
                printf("\nAguardando cliente...Pid=%d",getpid());
                fflush(stdout);

                indice_cliente = servidor->aceitarCliente();

                if (indice_cliente >= 0) {

                    printf("\nCliente conectado...Ok..indice=%d",indice_cliente);
                    fflush(stdout);
                    
                    pipe(fd);

                    if ((filhoNovaConexao = fork()) < 0) {
                        perror("Ocorreu um erro criando nova instância no servidor - fork");
                    } else {
                        // Processo filho
                        if (filhoNovaConexao == 0) {
                            close(fd[0]);
                            servidor->encerrarServidor();
                            servidor->setConexao(indice_cliente);
                            int dados[2];
                            dados[0] = indice_cliente;
                            dados[1] = servidor->getCliente(indice_cliente);
                            write(fd[1], &dados, 2*sizeof(int));
printf("\n\n\nDados: %d %d\n\n\n", dados[0], dados[1]);
                        }
                        else{
                            close(fd[1]);
                            int dados_recebidos[2];
                            int bytes_recebidos = read(fd[0], &dados_recebidos, 2*sizeof(int));
                            
                            if(bytes_recebidos > 0){
printf("\n\n\nSodad: %d %d\n\n\n", dados_recebidos[0], dados_recebidos[1]);

                                servidor->setCliente(dados_recebidos[0], dados_recebidos[1]);
                            }
                        }
                    }
                } else {
                    perror("Não foi possível realizar conexão com o cliente. - aceitarCliente");
                }

                // servidor->encerrarCliente();
            }// Recebe nova mensagem
            else {
                if(indice_cliente < 0)
                    return 0;
                
                //--------------------------------------------
                //Receber mensagem de solicitação de grep distribuído - 1
                printf("\nReceber...Pid=%d",getpid());
                printf("\nIndice cliente = %d",indice_cliente);
                fflush(stdout);
                Mensagem* m = servidor->receber(indice_cliente);
                if(m->getCodigo()!=1){
                    break;
                }
                char msg[255];
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                printf("Ok\n");
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem de solicitação de grep - 2                
                 /*printf("\nEnviar...");
                 fflush(stdout);
                 m->setCodigo(2);
                 servidor->enviarTodos(m);
                 // servidor->enviar(indice_cliente, m);
                 m->toChar(msg); ///
                 printf("Mensagem: %s", msg);
                 printf("Ok\n");
                 fflush(stdout);*/

                //--------------------------------------------
                //Receber mensagem de resposta de solicitação de grep - 3 [Resposta de 2]                
                printf("\nEnviar e receber de todos");
                fflush(stdout);
                m->setCodigo(2);
                vector<Mensagem*> mensagens = servidor->enviarReceberTodos(m);
                mensagens.at(0)->toChar(msg);
                // m = servidor->receber(indice_cliente);
                printf("%s", msg);
                printf("Ok\n");
                fflush(stdout);

                //--------------------------------------------
                //Agrupa conteúdos das mensagens
                m = servidor->agruparMensagens(mensagens);
                m = mensagens.at(0);
                m->toChar(msg);
                printf("Mensagem: %s", msg);

                //--------------------------------------------
                //Enviar mensagem de resposta de solicitação de grep distribuído - 4 [Resposta de 1]
                printf("\nEnviar");
                fflush(stdout);
                m->setCodigo(4);
                servidor->enviar(indice_cliente, m);
                printf("Ok fim\n");
                fflush(stdout);
                
            }
        // }


    }
    return 0;
}

