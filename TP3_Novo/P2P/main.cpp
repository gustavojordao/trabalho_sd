/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on October 5, 2017, 9:04 PM
 */

#include "Node.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <pthread.h>

using namespace std;

void* thread_recebe_ant(void* arg);
void* thread_recebe_suc(void* arg);
void* thread_aceita_con(void* arg);
void* thread_aceita_falha(void* arg);
void* thread_ping_suc(void* arg);

pthread_t thread_ra;
pthread_t thread_rs;
pthread_t thread_ac;
pthread_t thread_af;
pthread_t thread_ps;

Node* node;
bool iniciaThreadServidor = false;
bool houveFalha = false;
bool pronto = true;
bool prontoAtualizacaoSuc = true;

bool solicita_porta = true;
bool atualiza_lista_nodes = true;
bool atualiza_indices = true;
bool notifica_node = true;
bool identifica = true;

bool primeira_conexao = true;

int tipo_node;        

int main(int argc, char**argv) {

    char linha[500];

    string ip_antecessor;
    int porta_antecessor;
    int porta_sucessor;

    node = NULL;

    // Nó inicial da rede
    if (argc > 2) {
        ip_antecessor = argv[1];
        porta_antecessor = atoi(argv[2]);
        porta_sucessor = atoi(argv[3]);

        node = new Node(ip_antecessor, porta_antecessor, porta_sucessor);

    }
        // Já existe algum nó na rede
    else {
        ip_antecessor = "";
        porta_antecessor = 8001;
        porta_sucessor = atoi(argv[1]);

        node = new Node(porta_sucessor);
    }

    // Já existe algum nó na rede
    if (node->getAntecessor() != NULL) {
        node->getSucessor()->iniciar();
        node->getAntecessor()->conectar();
        node->getAntecessor()->enviar(Mensagem::criarMensagemIdentifica(1));

        pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);
        //pthread_create(&(thread_af), NULL, thread_aceita_falha, NULL);
    }        
    // Nó inicial da rede
    else {
        node->getSucessor()->iniciar();
    }

    primeira_conexao = node->getAntecessor() == NULL;
    iniciaThreadServidor = true;
    pthread_create(&(thread_rs), NULL, thread_recebe_suc, NULL);
    pthread_create(&(thread_ac), NULL, thread_aceita_con, NULL);
    
    // Execução de comandos
    while (true) {
        // Aguarda opções find/store/status
        printf("\n---------------------------\n[Porta: %d]\n", node->getPortaSucessor());
        printf("\n---------------------------\nOpções de comando:\n");
        printf("find <KEY>\n");
        printf("store <KEY> <VALUE>\n");
        printf("status\n>");
        fgets(linha, 499, stdin);
        
        stringstream ss(linha);
        string str_linha = linha;
        string item;
        vector<string> tokens;
        while (getline(ss, item, ' ')) {
            tokens.push_back(item);
        }

        string key;
        string value;

        // Comandos com mais de 1 parâmetro
        if (tokens.size() > 2) {
            // Comando store
            if (Pair::compareStrings(tokens.at(0), "store", false)) {
                // Interpreta comando
                key = tokens.at(1);
                int qtd_letras = tokens.at(0).length() + 1 + tokens.at(1).length();
                value = str_linha.substr(qtd_letras + 1, str_linha.length() - qtd_letras - 2);
                
                // Verifica se a chave foi encontrada ou se a busca continua
                int ind = node->findPar(key);

                // Chave encontrada, envia resultado ao nó que fez a requisição
                if (ind >= 0) {
                    node->storePar(new Pair(key, value));
                    printf("\n[%s] = {%s} armazenado.", key.c_str(), value.c_str());
                    fflush(stdout);
                } 
                // Chave não encontrada, armazenamento nos antecessores
                else if (ind == -1) {
                    node->getAntecessor()->enviar(Mensagem::criarMensagemStore(node->getIndice(), new Pair(key, value)));
                }
                // Chave não encontrada, armazenamento nos sucessores
                else if (ind == -2) {
                    node->getSucessor()->enviar(Mensagem::criarMensagemStore(node->getIndice(), new Pair(key, value)));
                }
                // Chave não encontrada, armazena chave e valor
                else if (ind == -3) {
                    node->storePar(new Pair(key, value));
                    printf("\n[%s] = {%s} armazenado.", key.c_str(), value.c_str());
                    fflush(stdout);
                }
            } else {
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        } 
        // Comandos com 1 parâmetro
        else if (tokens.size() == 2) {
            // Comando find
            if (Pair::compareStrings(tokens.at(0), "find", false)) {
                // Interpreta comando
                key = tokens.at(1).substr(0, tokens.at(1).length() - 1);
                
                // Verifica se a chave foi encontrada ou se a busca continua
                int ind = node->findPar(key);
                // Chave encontrada, exibe resultado
                if (ind >= 0) {
                    printf("\n[%s] = {%s}", key.c_str(), node->getPares().at(ind)->getValue().c_str());
                    fflush(stdout);
                }
                // Chave não encontrada, possivelmente localizada nos nós antecessores
                else if (ind == -1) {
                    node->getAntecessor()->enviar(Mensagem::criarMensagemFind(node->getIndice(), key));
                }
                // Chave não encontrada, possivelmente localizada nos nós sucessores
                else if (ind == -2) {
                    node->getSucessor()->enviar(Mensagem::criarMensagemFind(node->getIndice(), key));
                }
                // Chave não encontrada, não foi inserida na rede
                else if (ind == -3) {
                    printf("[%s] não foi encontrado.", key.c_str());
                    fflush(stdout);
                }
            } else {
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        } 
        // Comandos sem parâmetros
        else if (tokens.size() == 1) {
            // Comando status
            if (Pair::compareStrings(tokens.at(0).substr(0, tokens.at(0).length() - 1), "status", false)) {
                // Exibe status do nó
                printf("%s", node->toString().c_str());
                fflush(stdout);
            } else {
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        } else {
            perror("Erro de sintaxe. Tente novamente.\n");
        }

    }
    return 0;
}

void* thread_recebe_ant(void* arg) {

    while (true) {
        // Primeiro nó não possui antecessor
        if (node->getAntecessor() == NULL) {
            continue;
        }
            // Nó que não é o primeiro
        else {
            // Recebe mensagem do nó antecessor
            Mensagem* m = node->getAntecessor()->receber();

            if (m == NULL)
                continue;

            if (m->getCodigo() == 12)
                continue;
            
            //printf("\nANT: %d|%s\n", m->getCodigo(), m->getTexto().c_str());
            
            int codigo = m->getCodigo();

            vector<string> partes;
            vector<string> lista_nodes;
            string endereco;
            int porta;
            int inicial;
            int indice;
            
            switch (codigo) {
                case Mensagem::SOLICITA_PORTA:
                    primeira_conexao = false;
                    
                    porta = node->getPortaSucessor();
                    node->getAntecessor()->enviar(Mensagem::criarMensagemAckSolicitaPorta(porta, node->getEnderecoAntecessor()));
                    break;
                case Mensagem::ATUALIZA_LISTA_NODES:
                    partes = m->getPartes();
                    inicial = atoi(partes.at(0).c_str());
                    
                    lista_nodes = *new vector<string>();
                    for(int i=1; i<partes.size(); i++){
                        lista_nodes.push_back(partes.at(i));
                    }
                    
                    node->setNodes(lista_nodes);
                    
                    if(!atualiza_lista_nodes){
                        atualiza_lista_nodes = true;
                    }
                    else{
                        node->getSucessor()->enviar(m);
                    }
                    break;
                case Mensagem::ATUALIZA_INDICES:
                    partes = m->getPartes();
                    inicial = atoi(partes.at(0).c_str());
                    indice = atoi(partes.at(1).c_str());
                    
                    if(!atualiza_indices){
                        atualiza_indices = true;
                    }
                    else{
                        node->setIndice(indice);
                        node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaIndices(inicial, (node->getIndice()+1)%node->getNumNodes()));
                    }
                    break;
            case Mensagem::NOTIFICA_NODE:
                    partes = m->getPartes();
                    endereco = partes.at(0);
                    porta = atoi(partes.at(1).c_str());
                    
                    node->getAntecessor()->desconectar();
                    node->setAntecessor(endereco, porta);
                    node->getAntecessor()->conectar();
                    node->getAntecessor()->enviar(Mensagem::criarMensagemIdentifica(2));
                    
                    break;
                default:
                
                    break;
            }
        }
    }

}

void* thread_recebe_suc(void* arg) {

    while (true) {
        // Recebe mensagem do nó sucessor
        Mensagem* m = node->getSucessor()->receber();

        if (m == NULL)
            continue;

        if (m->getCodigo() == 12)
            continue;


        //printf("\nSUC: %d|%s\n", m->getCodigo(), m->getTexto().c_str());
        
        int codigo = m->getCodigo();

        vector<string> partes;
        string endereco;        
        int porta;
        
        stringstream ss;
        
        switch (codigo) {
            case Mensagem::ACK_SOLICITA_PORTA:
                partes = m->getPartes();
                porta = atoi(partes.at(0).c_str());
                endereco = partes.at(1);
                
                ss.str("");
                ss.clear();
                ss << node->getSucessor()->getIpNovoCliente() << ":" << porta;
                
                //if(tipo_node == 0){
                if(node->getAntecessor() == NULL){
                    node->setAntecessor(node->getSucessor()->getIpNovoCliente(), porta);
                    node->getAntecessor()->conectar();
                    node->getAntecessor()->enviar(Mensagem::criarMensagemIdentifica(3));
                    pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);
                    //pthread_create(&(thread_af), NULL, thread_aceita_falha, NULL);
                    tipo_node = 1;
                }
                else if(tipo_node == 1){
                //else{
                    // Notifica próximo node que receberá um novo antecessor
                    notifica_node = false;
                    node->getSucessor()->enviar(Mensagem::criarMensagemNotificaNode(node->getSucessor()->getIpNovoCliente(), porta));
                    while(!notifica_node);
                }
                node->getNodes().at(node->getIndice()) = endereco;
                node->addNode(node->getIndice()+1, ss.str());
                
                solicita_porta = true;
                break;
            case Mensagem::ACK_NOTIFICA_NODE:
                notifica_node = true;
                break;
            case Mensagem::IDENTIFICA:
                partes = m->getPartes();
                tipo_node = atoi(partes.at(0).c_str());
                
                /*if(node->getAntecessor() == NULL && tipo_node != 3){
                    tipo_node = 0;
                }
                else if(node->getAntecessor() != NULL && tipo_node != 3){
                    tipo_node = 1;
                }*/
                identifica = true;
                break;
            case Mensagem::PING:

                break;
            default:
                break;
        }
    }

}

void* thread_aceita_con(void* arg) {

    while (true) {

        // Aceita conexão de novo nó sucessor
        node->getSucessor()->aceitar();
        identifica = false;
        while(!identifica);
        
        if(tipo_node == 1 && node->getAntecessor() == NULL){
          
        //if(node->getAntecessor() == NULL){
            solicita_porta = false;
            node->getSucessor()->enviarParaNovoCliente(Mensagem::criarMensagemSolicitaPorta());            
            while(!solicita_porta);
            
            // Lista de nodes atualizada neste node, enviar para o próximo
            
            atualiza_lista_nodes = false;
            node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaListaNodes(node->getIndice(), node->getNodes()));
            while(!atualiza_lista_nodes);
            
            // Lista de nodes atualizada em todos os nodes, atualizar índices em todos os nodes
            
            atualiza_indices = false;
            node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaIndices(node->getIndice(), (node->getIndice()+1)%node->getNumNodes()));
            while(!atualiza_indices);
            
            // Indices atualizados em todos os nodes
        }
        else if(tipo_node == 1 && node->getAntecessor() != NULL){
        //else{
        //    if(primeira_conexao){
                node->getSucessor()->enviarParaNovoCliente(Mensagem::criarMensagemSolicitaPorta());            

                // Aguardando Mensagem de AckSolicitacaoPorta
                Mensagem* m = node->getSucessor()->receberDoNovoCliente();
                
                vector<string> partes = m->getPartes();
                int porta = atoi(partes.at(0).c_str());
                string endereco = partes.at(1);
                
                stringstream ss;
                ss << node->getSucessor()->getIpNovoCliente() << ":" << porta;
                
                // Notifica próximo node que receberá um novo antecessor
                //notifica_node = false;
                node->getSucessor()->enviar(Mensagem::criarMensagemNotificaNode(node->getSucessor()->getIpNovoCliente(), porta));
                node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());
                
                // Aguardando Mensagem de AckNotificacaoNode
                m = node->getSucessor()->receberDoNovoCliente();
                
                //while(!notifica_node);

                node->getNodes().at(node->getIndice()) = endereco;
                node->addNode(node->getIndice()+1, ss.str());
                
                //solicita_porta = true;
                
                // Lista de nodes atualizada neste node, enviar para o próximo
            
                atualiza_lista_nodes = false;
                node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaListaNodes(node->getIndice(), node->getNodes()));
                while(!atualiza_lista_nodes);

                // Lista de nodes atualizada em todos os nodes, atualizar índices em todos os nodes

                atualiza_indices = false;
                node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaIndices(node->getIndice(), (node->getIndice()+1)%node->getNumNodes()));
                while(!atualiza_indices);

                // Indices atualizados em todos os nodes
        }
        else if(tipo_node == 2){
            //else{
                node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());
                node->getAntecessor()->enviar(Mensagem::criarMensagemAckNotificaNode());
                primeira_conexao = true;
            //}
        }
        else if(tipo_node == 3){
            node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());
            tipo_node = 1;
        }
        
    }

}

/*void* thread_aceita_falha(void* arg) {

    int tentativas = 0;
    
    while(true){
        
        int result = node->getSucessor()->enviar(Mensagem::criarMensagemPing());
        
        if(result <= 0){
            tentativas++;
            
            if(tentativas >= 3){
                node->getSucessor()->aceitarAposFalha();
            }
        }
        
    }
    
}*/

void* thread_ping_suc(void* arg){
    
    int tentativas = 0;
    
    while(true){
        
        int result = node->getAntecessor()->enviar(Mensagem::criarMensagemPing());
        
        printf("\nResultado: %d", result);
        fflush(stdout);
        
        if(result <= 0){
            tentativas++;
            
            if(tentativas >= 3){
                // Antecessor morreu
                // Notifica sucessor que antecessor morreu
                printf("\n----------Um node foi desconectado.----------\n>");
                fflush(stdout);
                houveFalha = true;
                if(node->getNumNodes() > 2){
                    //node->getAntecessor()->enviar(Mensagem::criarMensagemNotificacaoFalha(node->getIndice(), (node->getIndice()+1)%node->getNumNodes()));
                    
                    vector<string> copia_lista = node->getNodes();
                    for(int i=node->getIndice()-1; i!=node->getIndice(); i--){
                        if(i<0){
                            i += node->getNumNodes();
                        }
                        
                        string endereco = node->getNodes().at(i);
                        string ip = endereco.substr(0, endereco.find_first_of(':'));
                        int porta = atoi(endereco.substr(endereco.find_first_of(':')+1, endereco.length()-endereco.find_first_of(':')-1+1).c_str());
                        node->desconectarAntecessor();
                        node->setAntecessor(ip, porta);
                        //int status = node->getAntecessor()->conectarAposFalha();
                        int status = node->getAntecessor()->conectar();
                        
                        if(status >= 0){
                            node->getAntecessor()->enviar(Mensagem::criarMensagemIdentifica(3));
                            break;
                        }
                        else{
                            copia_lista.erase(copia_lista.begin()+i);
                        }
                    }
                    
                    node->setNodes(copia_lista);
                    
                    if(node->getNumNodes() > 1){
                    
                        atualiza_lista_nodes = false;
                        node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaListaNodes(node->getIndice(), node->getNodes()));
                        while(!atualiza_lista_nodes);

                        // Lista de nodes atualizada em todos os nodes, atualizar índices em todos os nodes

                        atualiza_indices = false;
                        node->getSucessor()->enviar(Mensagem::criarMensagemAtualizaIndices(node->getIndice(), (node->getIndice()+1)%node->getNumNodes()));
                        while(!atualiza_indices);

                        // Indices atualizados em todos os nodes
                    }
                    else{
                        node->setIndice(0);
                        node->desconectarAntecessor();
                    }
                                    
                }
                else{
                    if(node->getIndice() == 0){
                        node->removeNode(1);
                    }
                    else if(node->getIndice() == 1){
                        node->removeNode(0);
                    }
                    node->desconectarAntecessor();
                }
                printf("\n----------Notificado----------\n>");
                fflush(stdout);
                                
                return NULL;
            }
        }
        else{
            tentativas = 0;
        }
        
    }
    
}