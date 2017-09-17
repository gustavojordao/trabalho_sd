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

#include "Cliente.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 3){
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR>", argv[0]);
        printf("Exemplo: %s 192.168.10.1 8000", argv[0]);
        return 1;
    }
    
    char* ip_servidor = argv[0];
    int porta = atoi(argv[1]);
    
    Cliente* cliente = new Cliente(ip_servidor, porta);
    
    cliente->conectarAoServidor();

    cliente->iniciarGrep();
    
    while(true){
        
        cliente->aceitarGrep();
        cliente->receberDoGrep();
        
        cliente->enviarAoServidor();        
        cliente->receberDoServidor();
        
        cliente->enviarAoGrep();
        cliente->encerrarGrep();
    }
    
    cliente->encerrarServidor();
    
    return 0;
}

