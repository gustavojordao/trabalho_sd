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

    /*if(argc < 3){
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        //printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR>", argv[0]);
        //printf("Exemplo: %s 192.168.10.1 8000", argv[0]);
        //return 1;
    }*/
    
    char ip_servidor[255];
    strcpy(ip_servidor, "102");//argv[1];
    int porta_servidor = 8000;//atoi(argv[2]);
    int porta_aplicacao = 8001; //atoi(argv[3]);
    Cliente* cliente = new Cliente(ip_servidor, porta_servidor, porta_aplicacao);
    
    //cliente->conectarAoServidor();

    cliente->iniciarGrep();
    
    while(true){
 printf("\nAceitar...");              
        int conexaoGrep = cliente->aceitarGrep();
 printf("Ok - Cliente %d\n", conexaoGrep);       

        //--------------------------------------------
        //Receber mensagem de solicitação de grep distribuído - 0
 
 printf("\nReceber...");       
        Mensagem* m = cliente->receberDoGrep();
 printf("Ok\n");       

        char msg[255]; 
        m->toChar(msg);///
        printf("Mensagem: %s", msg);
 
        //--------------------------------------------
        //Enviar mensagem ao servidor solicitando grep distribuído - 1
        //cliente->enviarAoServidor(m);        
 
        //--------------------------------------------
        //Receber mensagem do servidor solicitando grep local - 2
        //m = cliente->receberDoServidor();
        
        // TODO: criar comando
        //--------------------------------------------
        //Executar grep local
        //execv()
        
        //--------------------------------------------
        //Enviar mensagem de resposta da solicitação de grep local - 3 [Resposta de 2]
        //m = new Mensagem();
        //cliente->enviarAoServidor(m);        

        //--------------------------------------------
        //Receber mensagem do servidor com resposta de grep distribuído - 4 [Resposta de 1]
        //m = cliente->receberDoServidor();

        //--------------------------------------------
        //Enviar mensagem à aplicação com resposta de grep distribuído - 5 [Resposta de 0]
 printf("\nEnviar...");       
        m = new Mensagem();
        cliente->enviarAoGrep(m);
 printf("Ok\n");
 
        cliente->encerrarGrep();
        
    }
    
    //cliente->encerrarServidor();
    
    //delete cliente;    
        
    return 0;
}

