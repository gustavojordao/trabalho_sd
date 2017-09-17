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
    
    servidor->iniciar();
    
    printf("Ok\n");
    
    int filho = 0;
    
    while(true){
        
        printf("\nAguardando cliente...");
        
        int cliente = servidor->aceitarCliente();
    
        printf("\nCliente conectado...Ok");
                
        if((filho = fork()) < 0){
            perror("Ocorreu um erro criando nova instância no servidor - fork");        
        }
        else{
            // Processo filho
            if(filho == 0){
                servidor->encerrar();

                servidor->setConexao(cliente);
                
                Mensagem* m = servidor->receber(0);
                
                servidor->enviar(0, m);
                
                m = servidor->receber(0);
                
                servidor->enviar(0, m);
                
                // Lógica do programa
                // Envia mensagem para as quatro máquinas solicitando grep
                // Une os resultados
                // Envia para o solicitante

            }
        }

        close(cliente);
    }
    return 0;
}

