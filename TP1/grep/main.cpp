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

#define NOME_ARQUIVO "maquina[.].*[.]log"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 2){
        printf("Parâmetros insuficientes: entre com o texto a ser buscado.\n");
        printf("%s <TEXTO>\n", argv[0]);
        printf("Exemplo: %s \"falha\"\n", argv[0]);
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
    }
    else{
        Cliente* cliente = new Cliente(8001);
        
        cliente->conectar();
        
        // Recebe solicitação de execução de comando grep local
        cliente->receber();
        // Executa comando grep local
        execv("grep", [argv, NOME_ARQUIVO]);
        // Envia resultado do comando grep local
        cliente->enviar();
        
        // Recebe resultado de grep distribuído
        cliente->receber();
        // Exibe resultado de grep distribuído        
        printf("");
        
        cliente->encerrar();
        
    }
    
    return 0;
}

