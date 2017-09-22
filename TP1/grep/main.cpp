/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 16, 2017, 6:31 PM
 */

#include <cstdlib>

#include "Cliente.h"

#include <string.h>

//#define NOME_ARQUIVO "maquina[.][0-9]*[.]log"
#define NOME_ARQUIVO "maquina.1.log"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

/*    if(argc < 2){
        printf("Parâmetros insuficientes: entre com o texto a ser buscado.\n");
        //printf("%s <TEXTO>\n", argv[0]);
        //printf("Exemplo: %s \"falha\"\n", argv[0]);
        //return 1;
    }
    else if(strcmp(argv[1], "--help")){
        printf("Busca de texto: \n");
        printf("%s <TEXTO>\n", argv[0]);
        printf("Exemplo: %s \"falha\"\n\n", argv[0]);
        
        printf("Busca de expressão regular: \n");
        printf("%s -e <EXPRESSÃO>\n", argv[0]);
        printf("Exemplo: %s -e \"[0-9]*\"\n", argv[0]);
        printf("Exemplo acima busca números.");
    }
    else*/{
        char argumentos[1000];
        
        sprintf(argumentos, "grep");
        
        for(int i=1; i<argc; i++){
            sprintf(argumentos, "%s %s", argumentos, argv[i]);
        }
        
        
        string str_argumentos = argumentos;
                
        Cliente* cliente = new Cliente(8001);
        printf("\nConectar...");       
        fflush(stdout);
        cliente->conectar();
        printf("Ok\n");       
        fflush(stdout);
        
        //--------------------------------------------
        //Enviar mensagem ao cliente solicitando grep distribuído - 0
        printf("\nEnviar...");       
        fflush(stdout);
        cliente->enviar(new Mensagem(0, str_argumentos));
        printf("Ok\n");
        fflush(stdout);
        
        printf("\nReceber...");       
        fflush(stdout);
        //--------------------------------------------
        //Receber mensagem do cliente de resposta de solicitação ao grep distribuído - 5 [Resposta de 0]
        Mensagem* m = cliente->receber();
        // Exibe resultado de grep distribuído        
        printf("Ok\n");
        fflush(stdout);
        
        char msg[255]; 
        m->toChar(msg);///
        printf("Mensagem: %s", msg);
        fflush(stdout);
        
        cliente->encerrar();
 
        delete cliente;
    }
    
    return 0;
}

