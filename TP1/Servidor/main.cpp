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

#include "Servidor.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 2){
        printf("Parâmetros insuficientes: entre com a porta do servidor.\n");
        printf("%s <PORTA>\n", argv[0]);
        printf("Exemplo: %s 8000\n", argv[0]);
        //return 1;
    }
    
    int porta = 8000;//atoi(argv[1]);
    
    Servidor* servidor = new Servidor(porta);
    
    printf("Iniciando servidor na porta %d...", porta);
    fflush(stdout);
    
    servidor->iniciar();
    
    printf("Ok\n");
    fflush(stdout);
    
    int filho = 0;

    while(true){
        
        printf("\nAguardando cliente...");
        fflush(stdout);
                
        int indice_cliente = servidor->aceitarCliente();
        
        //printf("Pressione qualquer tecla para continuar...");
        //getchar();
        
        if(indice_cliente >= 0){
        
            printf("\nCliente conectado...Ok");
            fflush(stdout);

            if((filho = fork()) < 0){
                perror("Ocorreu um erro criando nova instância no servidor - fork");        
            }
            else{
                // Processo filho
                if(filho == 0){
                    servidor->encerrarServidor();

                    //--------------------------------------------
                    //Receber mensagem de solicitação de grep distribuído - 1
                    Mensagem* m = servidor->receber(indice_cliente);

                    //--------------------------------------------
                    //Enviar mensagem de solicitação de grep - 2                
                    m->setCodigo(2);
                    servidor->enviarTodos(m);

                    //--------------------------------------------
                    //Receber mensagem de resposta de solicitação de grep - 3 [Resposta de 2]                
                    vector<Mensagem*> mensagens = servidor->receberTodos();

                    //--------------------------------------------
                    //Agrupa conteúdos das mensagens
                    m = servidor->agruparMensagens(mensagens);

                    //--------------------------------------------
                    //Enviar mensagem de resposta de solicitação de grep distribuído - 4 [Resposta de 1]
                    m->setCodigo(4);
                    servidor->enviar(indice_cliente, m);
                }
            }
        }
        else{
            perror("Não foi possível realizar conexão com o cliente. - aceitarCliente");
        }

        servidor->encerrarCliente();
    }
    return 0;
}

