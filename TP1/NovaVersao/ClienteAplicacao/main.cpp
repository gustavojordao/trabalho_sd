/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 26, 2017, 12:07 AM
 */

#include <cstdlib>

#include "Cliente.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 2){
        printf("Parâmetros insuficientes: entre com o texto a ser buscado.\n");
        printf("%s <TEXTO>\n", argv[0]);
        printf("Exemplo: %s \"falha\"", argv[0]);
        
        return 1;
    }
    else if(strcmp(argv[1], "--help")){
        printf("Busca de texto: \n");
        printf("%s <TEXTO>\n", argv[0]);
        printf("Exemplo: %s \"falha\"\n\n", argv[0]);
        
        printf("Busca de expressão regular: \n");
        printf("%s -e <EXPRESSÃO>\n", argv[0]);
        printf("Exemplo: %s -e \"[0-9]*\"\n", argv[0]);
        printf("Exemplo acima busca números.");
        
        return 1;
    }
    else{
    
        char argumentos[1000];
        sprintf(argumentos, "grep");
        for(int i=1; i<argc; i++){
            sprintf(argumentos, "%s %s", argumentos, argv[i]);
        }
        string str_argumentos = argumentos;

        Cliente* cliente = new Cliente(8001);
        cliente->conectar();
        
        // Envia mensagem 00
        cliente->enviar(new Mensagem(0, str_argumentos));
        // Recebe mensagem 05
        Mensagem* m = cliente->receber();
        
        cliente->encerrar();
        
        delete cliente;
    }
    
    return 0;
}

