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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

 printf("Cria0");   
    /*if(argc < 3){
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        //printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR>", argv[0]);
        //printf("Exemplo: %s 192.168.10.1 8000", argv[0]);
        //return 1;
    }*/
 printf("Cria");   
    
    char ip_servidor[255];
    strcpy(ip_servidor, "102");//argv[1];
    int porta_servidor = 8000;//atoi(argv[2]);
    int porta_aplicacao = 8001; //atoi(argv[3]);
 printf("Cria2");   
    Cliente* cliente = new Cliente(ip_servidor, porta_servidor, porta_aplicacao);
    
    //cliente->conectarAoServidor();

    cliente->iniciarGrep();
    
    //while(true){
 printf("\nAceitar...");       
        
        cliente->aceitarGrep();
 printf("Ok\n");       

  printf("\nReceber...");       
        Mensagem* m = cliente->receberDoGrep();
 printf("Ok\n");       
        
       char msg[255]; 
        m->toChar(msg);///
        printf("Mensagem: %s", msg);
 
        // TODO: Tratar mensagem
        //cliente->enviarAoServidor(new Mensagem());        
        //m = cliente->receberDoServidor();
        
 printf("\nEnviar...");       
        // TODO: Tratar mensagem
        cliente->enviarAoGrep(m);
 printf("Ok\n");       
        cliente->encerrarGrep();
        
    //}
    
    //cliente->encerrarServidor();
    
    //delete cliente;    
        
    return 0;
}

