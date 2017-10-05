/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.cpp
 * Author: gustavo
 * 
 * Created on October 1, 2017, 2:54 PM
 */

#include "Node.h"

Node::Node(string ip, int porta) {
    sucessor = new Cliente(ip.c_str(), porta);
    antecessor = new Servidor(porta);
}

Node::Node(const Node& orig) {
}

Node::~Node() {
}

