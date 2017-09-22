/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 16, 2017, 4:09 PM
 */

#include <cstdlib>
#include <unistd.h>

#include "Cliente.h"
#include "Mensagem.h"
#include <fstream>
#include <iostream>
#include <signal.h>

#include <pthread.h>

#define NOME_ARQUIVO "maquina.log"

using namespace std;

void* thread_grep(void* arg);
void* thread_conexao(void* arg);

pthread_t thread;
pthread_t thread_reinicia;

Cliente* cliente = NULL;

int qtd_clientes = 0;

bool finalizado = false;

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 3){
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR>", argv[0]);
        //printf("Exemplo: %s 192.168.10.1 8000", argv[0]);
        return 1;
    }

    Mensagem* m;
    
    char ip_servidor[255];
    strcpy(ip_servidor, argv[1]);
    int porta_servidor = atoi(argv[2]);
    int porta_aplicacao = atoi(argv[3]);
    cliente = new Cliente(ip_servidor, porta_servidor, porta_aplicacao);

    printf("Conectar ao servidor...");
    fflush(stdout);
    cliente->conectarAoServidor();
    printf("Ok\n");
    fflush(stdout);

    printf("Iniciar serviço background...");
    fflush(stdout);
    cliente->iniciarGrep();
    printf("Ok\n");
    fflush(stdout);

    pthread_create(&(thread), NULL, thread_grep, NULL);
    pthread_create(&(thread_reinicia), NULL, thread_conexao, NULL);
                
    while(true){
        
//        if(cliente == -1){
        
            printf("\nAguardando cliente...Pid=%d",getpid());
            fflush(stdout);

            int indice_cliente = cliente->aceitarGrep();

            if(indice_cliente >= 0){
                
                //--------------------------------------------
                //Receber mensagem de solicitação de grep distribuído - 0

                printf("\nReceber.pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoGrep();
                printf("Ok\n");
                fflush(stdout);

                char msg[255];
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem ao servidor solicitando grep distribuído - 1
                printf("\nEnviar.");
                fflush(stdout);
                m->setCodigo(1);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                fflush(stdout);
                
            }
//        }
    }
    
    return 0;
}

void* thread_grep(void* arg){

    if(!finalizado){
    
        Mensagem* m;
        char msg[1000];

        //--------------------------------------------
        //Receber mensagem do servidor solicitando grep local - 2
        printf("\nReceber...pid=%d ",getpid());
        fflush(stdout);
        m = cliente->receberDoServidor();
        m->toChar(msg); ///
        printf("Mensagem: %s", msg);
        fflush(stdout);

        //--------------------------------------------
        //Executar grep local
        //string comando = m->getTexto() + " " + NOME_ARQUIVO + " > log.temp";
        string comando = "cat maquina.log | grep erro > log.temp";
        system(comando.c_str());
        std::ifstream arq("log.temp");
        string buffer;
        string result;
        while (getline(arq, buffer)) {
            result += buffer;
        }

        //--------------------------------------------
        //Enviar mensagem de resposta da solicitação de grep local - 3 [Resposta de 2]
        printf("\nEnviar...pid=%d ",getpid());
        fflush(stdout);
        m = new Mensagem(3, result.data());
        cliente->enviarAoServidor(m);
        printf("Ok\n");
        m->toChar(msg); ///
        printf("Mensagem: %s", msg);
        fflush(stdout);

        //--------------------------------------------
        //Receber mensagem do servidor com resposta de grep distribuído - 4 [Resposta de 1]
        printf("\nReceber...pid=%d ",getpid());
        fflush(stdout);
        m = cliente->receberDoServidor();
        m->toChar(msg);
        printf("Mensagem: %s", msg);
        fflush(stdout);

        //--------------------------------------------
        //Enviar mensagem à aplicação com resposta de grep distribuído - 5 [Resposta de 0]
        printf("\nEnviar...pid=%d ",getpid());
        fflush(stdout);
        m->setCodigo(5);
        m->toChar(msg); ///
        printf("Mensagem: %s", msg);
        cliente->enviarAoGrep(m);
        printf("Ok\n");
        fflush(stdout);
        cliente->encerrarGrep();

        finalizado = true;
    
    }
}

void* thread_conexao(void* arg){
    
    int s;
    
    if(finalizado){

        s = pthread_cancel(thread);
        if (s != 0)
            perror("pthread_cancel");

        s = pthread_join(thread, NULL);
        if (s != 0)
            perror("pthread_join");
        
        finalizado = false;
        
        pthread_create(&(thread), NULL, thread_grep, NULL);

    }
}

/*
 * 
 */
int _main(int argc, char** argv) {

    if(argc < 3){
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR>", argv[0]);
        //printf("Exemplo: %s 192.168.10.1 8000", argv[0]);
        return 1;
    }

    char ip_servidor[255];
    strcpy(ip_servidor, argv[1]);
    int porta_servidor = atoi(argv[2]);
    int porta_aplicacao = atoi(argv[3]);
    Cliente* cliente = new Cliente(ip_servidor, porta_servidor, porta_aplicacao);

    printf("Conectar ao servidor...");
    fflush(stdout);
    cliente->conectarAoServidor();
    printf("Ok\n");
    fflush(stdout);

    printf("Iniciar serviço background...");
    fflush(stdout);
    cliente->iniciarGrep();
    printf("Ok\n");
    fflush(stdout);


    int conexaoGrep = -1;
    
    int parent_pid = getpid();
    int child_pid;
    if ((child_pid = fork()) < 0) {
        perror("Ocorreu um erro criando nova instância no servidor - fork");
    }

    while (true) {
            Mensagem* m = new Mensagem();
            if (getpid() == parent_pid){
                
                printf("\nAceitar.");
                fflush(stdout);
                conexaoGrep = cliente->aceitarGrep();
                kill(child_pid, SIGKILL);
                printf("Ok - Cliente %d\n", conexaoGrep);
                fflush(stdout);

                //--------------------------------------------
                //Receber mensagem de solicitação de grep distribuído - 0

                printf("\nReceber.pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoGrep();
                printf("Ok\n");
                fflush(stdout);

                char msg[255];
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem ao servidor solicitando grep distribuído - 1
                printf("\nEnviar.");
                fflush(stdout);
                m->setCodigo(1);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                fflush(stdout);

                
                //--------------------------------------------
                //Receber mensagem do servidor solicitando grep local - 2
                printf("\nReceber...pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoServidor();
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Executar grep local
                // string comando = m->getTexto() + " " + NOME_ARQUIVO + " > log.temp";
                string comando = "cat maquina.log | grep erro > log.temp";
                system(comando.data());
                std::ifstream arq("log.temp");
                string buffer;
                string result;
                while (getline(arq, buffer)) {
                    result += buffer;
                }

                //--------------------------------------------
                //Enviar mensagem de resposta da solicitação de grep local - 3 [Resposta de 2]
                printf("\nEnviar...pid=%d ",getpid());
                fflush(stdout);
                m = new Mensagem(3, result.data());
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Receber mensagem do servidor com resposta de grep distribuído - 4 [Resposta de 1]
                printf("\nReceber...pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoServidor();
                m->toChar(msg);
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem à aplicação com resposta de grep distribuído - 5 [Resposta de 0]
                printf("\nEnviar...pid=%d ",getpid());
                fflush(stdout);
                m->setCodigo(5);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                cliente->enviarAoGrep(m);
                printf("Ok\n");
                fflush(stdout);
                cliente->encerrarGrep();
                if ((child_pid = fork()) < 0) {
                    perror("Ocorreu um erro criando nova instância no servidor - fork");
                }
            }
            else {
                char msg[255];
                    
                // if(conexaoGrep >= 0)
                //     return 0;
                
                //--------------------------------------------
                //Receber mensagem do servidor solicitando grep local - 2
                printf("\nReceber...pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoServidor();
                if(m->getCodigo()!=2){
                    break;
                }
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Executar grep local
                printf("1\n");
                // string comando = m->getTexto() + " " + NOME_ARQUIVO + " > log.temp";
                string comando = "cat maquina.log | grep erro > log.temp";
                printf("2\n");
                system(comando.data());
                std::ifstream arq("log.temp");
                string buffer;
                string result;
                while (getline(arq, buffer)) {
                    result += buffer;
                }

                //--------------------------------------------
                //Enviar mensagem de resposta da solicitação de grep local - 3 [Resposta de 2]
                printf("\nEnviar...pid=%d ",getpid());
                fflush(stdout);
                m = new Mensagem(3, result.data());
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

            }
        
    }
    


    //cliente->encerrarServidor();

    //delete cliente;    

    return 0;
}

