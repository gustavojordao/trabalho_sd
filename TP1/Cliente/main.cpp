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

    Cliente* cliente = new Cliente("192.168.10.1", 8000);
    
    cliente->conectar();
    
    while(true){
        
    }
    
    cliente->encerrar();
    
    return 0;
}

