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

    Servidor* servidor = new Servidor(8000);
    
    servidor->iniciar();
    
    int filho = 0;
    
    int cliente = servidor->aceitarCliente();
    
    while(true){
        if((filho = fork()) < 0){
            perror("Ocorreu um erro criando nova instância no servidor - fork");        
        }
        else{
            // Processo filho
            if(filho == 0){
                servidor->encerrar();

                // Lógica do programa

            }
        }

        close(cliente);
    }
    return 0;
}

