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

pthread_t thread_ra;
pthread_t thread_rs;
pthread_t thread_ac;

Node* node;
bool iniciaThreadServidor = false;

int main(int argc, char**argv) {

    char linha[500];

    string ip_antecessor;
    int porta_antecessor;
    int porta_sucessor;

    node = NULL;
    
    if (argc > 2) {
        ip_antecessor = argv[1];
        porta_antecessor = atoi(argv[2]);
        porta_sucessor = atoi(argv[3]);
        
        node = new Node(ip_antecessor, porta_antecessor, porta_sucessor);

    } else {
        ip_antecessor = "";
        porta_antecessor = 8001;
        porta_sucessor = atoi(argv[1]);

        node = new Node(porta_sucessor);
    }
    
    if (node->getAntecessor() != NULL) {
        // Não é primeiro
        node->getSucessor()->iniciar();
        node->getAntecessor()->conectar();
        
        string this_ip = "127.0.0.1";
        
        node->getAntecessor()->enviar(Mensagem::criarMensagemNovoNode(this_ip, porta_sucessor));
        pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);
    }
    else{
        node->getSucessor()->iniciar();
    }

    iniciaThreadServidor = true;
    pthread_create(&(thread_ac), NULL, thread_aceita_con, NULL);
    
    // TODO: Realocar
    /*{
        node->getAntecessor()->enviar(Mensagem::criarMensagemSolicitacaoIndice());
        Mensagem* m = node->getAntecessor()->receber();

        int indice = atoi(m->getPartes().at(0).c_str());

        node->setIndice(indice);
    }*/

/*    if (node->getAntecessor() == 0) {
        node->getSucessor()->aceitar();

        string ip = node->getSucessor()->getIpCliente();
        int porta = porta_antecessor;

        node->getAntecessor()->setEndereco(ip, porta);
        
        sleep(2);
        node->getAntecessor()->conectar();
        
        node->getSucessor()->setConexao(node->getSucessor()->getConexaoCliente());

        node->incNumNodes();
    }
*/
    

    while (true) {
        // Aguarda opções find/store
        printf("\n\nEntre com o comando:\n");
        printf("find <KEY>\n");
        printf("store <KEY> <VALUE>\n>");
        //scanf("%[^\r]");
        fgets(linha, 499, stdin);
        printf("%s\n", linha);

        stringstream ss(linha);
        string str_linha = linha;
        string item;
        vector<string> tokens;
        while (getline(ss, item, ' ')) {
            tokens.push_back(item);
        }

        string key;
        string value;

        if (tokens.size() > 2) {
            if (tokens.at(0).compare("store") == 0) {
                key = tokens.at(1);
                int qtd_letras = tokens.at(0).length() + 1 + tokens.at(1).length();
                value = str_linha.substr(qtd_letras+1, str_linha.length()-qtd_letras-2);
                printf("[%s] = {%s}", key.c_str(), value.c_str());
                fflush(stdout);
                
                int ind = node->findPar(key);
                
                if(ind >= 0){
                    node->storePar(new Pair(key, value));
                    printf("\n[%s] = {%s} armazenado.", key.c_str(), value.c_str());
                    fflush(stdout);
                }
                else if(ind == -1){
                    node->getAntecessor()->enviar(Mensagem::criarMensagemStore(node->getIndice(), new Pair(key, value)));                   
                }
                else if(ind == -2){
                    node->getSucessor()->enviar(Mensagem::criarMensagemStore(node->getIndice(), new Pair(key, value)));                   
                }
                else if(ind == -3){
                    node->storePar(new Pair(key, value));
                    printf("\n[%s] = {%s} armazenado.", key.c_str(), value.c_str());
                    fflush(stdout);
                }
            } else {
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        } else if (tokens.size() == 2) {
            if (tokens.at(0).compare("find") == 0) {
                key = tokens.at(1).substr(0, tokens.at(1).length()-1);
                printf("[%s]", key.c_str());
                fflush(stdout);
                int ind = node->findPar(key);
                
                if(ind >= 0){
                    printf("\n[%s] = {%s}", key.c_str(), node->getPares().at(ind)->getValue().c_str());
                    fflush(stdout);
                }
                else if(ind == -1){
                    node->getAntecessor()->enviar(Mensagem::criarMensagemFind(node->getIndice(), key));                   
                }
                else if(ind == -2){
                    node->getSucessor()->enviar(Mensagem::criarMensagemFind(node->getIndice(), key));                   
                }
                else if(ind == -3){
                    printf("[%s] não foi encontrado.", key.c_str());
                    fflush(stdout);
                }
            } else {
                perror("Erro de sintaxe. Tente novamente.\n");
            }
        } else if(tokens.size() == 1){
            if (tokens.at(0).substr(0, tokens.at(0).length()-1).compare("status") == 0) {
                printf("%s", node->toString().c_str());
                fflush(stdout);
            } else {
                perror("Erro de sintaxe. Tente novamente.\n");
            }  
        } else {
            perror("Erro de sintaxe. Tente novamente.\n");
        }
        
        /*getchar();
        printf("Antecessor: %d", node->getAntecessor()->getPorta());
        printf("Sucessor: %d", node->getSucessor()->getPorta());*/
    }
    return 0;
}

void* thread_recebe_ant(void* arg) {

    while (true) {
        if (node->getAntecessor() == NULL) {
            continue;
        } else {
            Mensagem* m = node->getAntecessor()->receber();

            if(m == NULL)
                continue;
            
            int codigo = m->getCodigo();

            vector<string> partes;
            int solicitante;
            string key;
            string value;
            int posicao;
            int indice;
            string ip;
            int porta;
            vector<Pair*> pares;
            int i;
            int num_nodes;
            int detentor;
            vector<Pair*> paresAnt = *new vector<Pair*>();
            vector<Pair*> paresSuc = *new vector<Pair*>();
                
            switch (codigo) {

                case Mensagem::FIND:
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    key = partes.at(1);

                    posicao = node->findPar(key);

                    if(posicao >= 0){
                        Pair* pair = node->getPares().at(posicao);
                        node->getSucessor()->enviar(Mensagem::criarMensagemRespostaFind(solicitante, node->getIndice(), pair));
                    }
                    else if (posicao == -1) {
                        node->getAntecessor()->enviar(m);
                    } else if (posicao == -2) {
                        node->getSucessor()->enviar(m);
                    } else if (posicao == -3) {
                        if(solicitante == node->getIndice()){
                            // TODO: Informa que chave não foi localizada.
                            printf("Chave %s não encontrada na rede.", key.c_str());
                            fflush(stdout);
                        }
                        else{
                            node->getSucessor()->enviar(Mensagem::criarMensagemRespostaFindNaoEncontrado(solicitante, key));
                        }
                    } 

                    break;
                case Mensagem::RESPOSTA_FIND:
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    detentor = atoi(partes.at(1).c_str());
                    key = partes.at(2);
                    value = partes.at(3);

                    if (solicitante == node->getIndice()) {
                        // TODO: Faz alguma coisa com o par, exibe, salva, sei lá...
                        printf("\n[%s] = {%s} no node %d\n", key.c_str(), value.c_str(), detentor);
                        //node->storePar(new Pair(key, value));
                    } else {
                        node->getSucessor()->enviar(m);
                    }
                    break;
                case Mensagem::RESPOSTA_FIND_NAO_ENCONTRADO:
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    key = partes.at(1);

                    if(solicitante == node->getIndice()){
                        printf("Chave %s não encontrada na rede.", key.c_str());
                        fflush(stdout);
                    }
                    else {
                        node->getSucessor()->enviar(m);
                    }
                    break;
                case Mensagem::STORE:
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    key = partes.at(1);
                    value = partes.at(2);

                    // TODO: Linhas abaixo devem ser alocadas em storePar 
                    posicao = node->findPar(key);

                    if (posicao >= 0){
                        node->storePar(new Pair(key, value));
                    }
                    if (posicao == -1) {
                        node->getAntecessor()->enviar(m);
                    } else if (posicao == -2) {
                        node->getSucessor()->enviar(m);
                    } else if (posicao == -3) {
                        node->storePar(new Pair(key, value));
                    }

                    break;
                    /*		case Mensagem::NOVO_NODE:
                                            recebeNovoAntecessor = true;

                                            partes = m->getPartes();
                                            ip = partes.at(0);
                                            porta = atoi(partes.at(1)); // TODO: Rever formato de mensagem adicionando porta

                                            node->getAntecessor()->desconectar();
                                            node->getAntecessor()->setEndereco(ip, porta);
                                            node->getAntecessor()->conectar();

                                            break;
                                            //case Mensagem::RESPOSTA_NOVO_NODE:
                                            //	
                                            //	break;
                                    case Mensagem::SOLICITACAO_INDICE:
                                            node->getSucessor()->enviar(Mensagem::criarMensagemRespostaSolicitacaoIndice(node->getIndice() + 1));
                                            break;
                     */
                case Mensagem::NOVO_NODE:
                    partes = m->getPartes();
                    printf("Ai amor me duele tanto. Me duele tanto.%s:%s %s", partes.at(0).c_str(), partes.at(1).c_str(), m->getTexto().c_str());
                    fflush(stdout);
                    
                    ip = partes.at(0);
                    porta = atoi(partes.at(1).c_str()); // TODO: Rever formato de mensagem adicionando porta
                    printf("Vai conectar%s:%d", ip.c_str(), porta);
                    fflush(stdout);
                    node->getAntecessor()->desconectar();
                    printf("It:");
                    fflush(stdout);
                    node->setAntecessor(ip, porta);
                    printf("Nii:");
                    fflush(stdout);
                    node->getAntecessor()->conectar();
                    printf("San:");
                    fflush(stdout);
                    
                    break;
                case Mensagem::RESPOSTA_SOLICITACAO_INDICE:
                    partes = m->getPartes();

                    indice = atoi(partes.at(0).c_str());
                    node->setIndice(indice);
                    break;
                case Mensagem::ATUALIZACAO_INDICE:
                    indice = atoi(m->getTexto().c_str());
                    printf("#######INDICE: %d", indice);
                    fflush(stdout);
                    node->setIndice(indice);
                    break;
                case Mensagem::ATUALIZACAO_NODE_SUC:
                    partes = m->getPartes();
                    indice = atoi(partes.at(0).c_str());
                    num_nodes = atoi(partes.at(1).c_str());
                    pares = *new vector<Pair*>();
                    for (i = 2; i < partes.size(); i+=2) {
                        pares.push_back(new Pair(partes.at(i), partes.at(i + 1)));
                    }

                    printf("TAMTAMTAMTAAAAAAM: %d", num_nodes);
                    fflush(stdout);
                    node->setNumNodes(num_nodes);
                    for (i = 0; i < pares.size(); i++) {
                        posicao = node->findPar(pares.at(i)->getKey());
                        if(posicao == 0 || posicao == -3){
                            node->storePar(pares.at(i));
                            pares.erase(pares.begin()+i);
                            i--;
                        }
                    }
                    
                    if (indice != node->getIndice()) {
                        
                        for(int i=0; i<node->getPares().size(); i++){
                            Pair* p = node->getPares().at(i);
                            if(p->getKey().at(0) > node->getEnderecoFinal()){
                                pares.push_back(p);
                                node->removePar(p);
                            }
                        }
                        
                        node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(indice, num_nodes, pares));
                    }

                    break;
                case Mensagem::SOLICITACAO_PORTA:
                    porta = node->getSucessor()->getPorta();
                    node->getAntecessor()->enviar(Mensagem::criarRespostaMensagemSolicitacaoPorta(porta));
                    break;
                default:
                    printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto().c_str());
                    fflush(stdout);
                    break;
            }
        }
    }

}

void* thread_recebe_suc(void* arg) {

    while (true) {
        Mensagem* m = node->getSucessor()->receber();

        if(m == NULL)
            continue;
            
        int codigo = m->getCodigo();

        vector<string> partes;
        int solicitante;
        int detentor;
        string key;
        string value;
        int posicao;
        int indice;
        string ip;
        int porta;
        int num_nodes;
        vector<Pair*> pares;
        int i;
        vector<Pair*> repasseParesAnt;
        vector<Pair*> repasseParesSuc;
        vector<Pair*> paresAnt = *new vector<Pair*>();
        vector<Pair*> paresSuc = *new vector<Pair*>();
        
        switch (codigo) {

            case Mensagem::FIND:
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                key = partes.at(1);

                posicao = node->findPar(key);

                if(posicao >= 0){
                    Pair* pair = node->getPares().at(posicao);
                    node->getAntecessor()->enviar(Mensagem::criarMensagemRespostaFind(solicitante, node->getIndice(), pair));
                }
                else if (posicao == -1) {
                    node->getAntecessor()->enviar(m);
                } else if (posicao == -2) {
                    node->getSucessor()->enviar(m);
                } else if (posicao == -3) {
                    if(solicitante == node->getIndice()){
                        // TODO: Informa que chave não foi localizada.
                        printf("Chave %s não encontrada na rede.", key.c_str());
                        fflush(stdout);
                    }
                    else{
                        node->getAntecessor()->enviar(Mensagem::criarMensagemRespostaFindNaoEncontrado(solicitante, key));
                    }
                } 

                break;
            case Mensagem::RESPOSTA_FIND:
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                detentor = atoi(partes.at(1).c_str());
                key = partes.at(2);
                value = partes.at(3);
                
                if (solicitante == node->getIndice()) {
                    // TODO: Faz alguma coisa com o par, exibe, salva, sei lá...
                    printf("\n[%s] = {%s} no node %d\n", key.c_str(), value.c_str(), detentor);
                    //node->storePar(new Pair(key, value));
                } else {
                    node->getAntecessor()->enviar(m);
                }
                break;
            case Mensagem::RESPOSTA_FIND_NAO_ENCONTRADO:
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                key = partes.at(1);

                if(solicitante == node->getIndice()){
                    printf("Chave %s não encontrada na rede.", key.c_str());
                    fflush(stdout);
                }
                else {
                    node->getAntecessor()->enviar(m);
                }
                break;
            case Mensagem::STORE:
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                key = partes.at(1);
                value = partes.at(2);

                // TODO: Linhas abaixo devem ser alocadas em storePar 
                posicao = node->findPar(key);

                if (posicao >= 0){
                    node->storePar(new Pair(key, value));
                }
                if (posicao == -1) {
                    node->getAntecessor()->enviar(m);
                } else if (posicao == -2) {
                    node->getSucessor()->enviar(m);
                } else if (posicao == -3) {
                    node->storePar(new Pair(key, value));
                }

                break;
           /* case Mensagem::NOVO_NODE:
                partes = m->getPartes();
                ip = node->getSucessor()->getIpNovoCliente();
                porta = atoi(partes.at(1).c_str()); // TODO: Rever formato de mensagem adicionando porta

                node->getAntecessor()->desconectar();
                node->setAntecessor(ip, porta);
                node->getAntecessor()->conectar();

                break;*/
                //case Mensagem::RESPOSTA_NOVO_NODE:
                //	
                //	break;
            case Mensagem::SOLICITACAO_INDICE:
                node->getSucessor()->enviar(Mensagem::criarMensagemRespostaSolicitacaoIndice(node->getIndice() + 1));
                sleep(1); //TODO: Verificar unidade de tempo

                // TODO: Verificar necessidade de exigir que nó que entra na rede solicita atualização para então iniciar o ciclo.
                // Não é complicado de fazer.
                repasseParesAnt = *new vector<Pair*>();
                repasseParesSuc = *new vector<Pair*>();
                for (i = 0; i < node->getPares().size(); i++) {
                    int key_0 = node->getPares().at(i)->getKey()[0];
                    if (node->getEnderecoInicial() < key_0) {
                        repasseParesAnt.push_back(node->getPares().at(i));
                    } else if (node->getEnderecoFinal() > key_0) {
                        repasseParesSuc.push_back(node->getPares().at(i));
                    }
                }

                //TODO: Enviar em ambos os sentidos
                
                sleep(1);
                node->getAntecessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(node->getIndice(), node->getNumNodes(), repasseParesAnt));
                sleep(1);
                node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(node->getIndice(), node->getNumNodes(), repasseParesSuc));

                break;
                case Mensagem::ATUALIZACAO_NODE_ANT:
                    partes = m->getPartes();
                    indice = atoi(partes.at(0).c_str());
                    num_nodes = atoi(partes.at(1).c_str());
                    pares = *new vector<Pair*>();
                    for (i = 2; i < partes.size(); i+=2) {
                        pares.push_back(new Pair(partes.at(i), partes.at(i + 1)));
                    }

                    printf("TAMTAMTAMTAAAAAAM: %d", num_nodes);
                    fflush(stdout);
                    node->setNumNodes(num_nodes);
                    for (i = 0; i < pares.size(); i++) {
                        posicao = node->findPar(pares.at(i)->getKey());
                        if(posicao == 0 || posicao == -3){
                            node->storePar(pares.at(i));
                            pares.erase(pares.begin()+i);
                            i--;
                        }
                    }
                    
                    if (indice != node->getIndice()) {
                        
                        for(int i=0; i<node->getPares().size(); i++){
                            Pair* p = node->getPares().at(i);
                            if(p->getKey().at(0) < node->getEnderecoFinal()){
                                pares.push_back(p);
                                node->removePar(p);
                            }
                        }
                        
                        node->getAntecessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(indice, num_nodes, pares));
                    }

                    break;
                /*			case Mensagem::RESPOSTA_SOLICITACAO_INDICE:
                                                partes = m->getPartes();

                                                indice = atoi(partes.at(0));
                                                node->setIndice(indice);
                                                break;
                 */ 
            case Mensagem::RESPOSTA_SOLICITACAO_PORTA:
                ip = node->getSucessor()->getIpCliente();
                porta = atoi(m->getTexto().c_str());
                node->setAntecessor(ip, porta);
                node->getAntecessor()->conectar();
                
                pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);
                                
                break;
            default:
                printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto().c_str());
                fflush(stdout);
                break;
        }
    }

}

void* thread_aceita_con(void* arg) {

    while (true) {

        // TODO: Aqui trata conexão de nó com último do anel. Tratar segunda parte, conexão de nó com primeiro do anel
        // Variável recebeNovoAntecessor é responsável por tratar a segunda parte.
        node->getSucessor()->aceitar();

        if(iniciaThreadServidor){
            pthread_create(&(thread_rs), NULL, thread_recebe_suc, NULL);
            iniciaThreadServidor = false;
        }
        if(node->getAntecessor() == NULL){
            printf("--Antecessor NULL--%d %d", node->getSucessor()->getConexaoCliente(), node->getSucessor()->getNovaConexaoCliente());
            fflush(stdout);
            node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());
            
            node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoIndice(node->getIndice()+1));

            node->incNumNodes();
                
            vector<Pair*> paresAnt = *new vector<Pair*>();
            vector<Pair*> paresSuc = *new vector<Pair*>();

            for(int i=0; i<node->getPares().size(); i++){
                Pair* p = node->getPares().at(i);
                /*if(p->getKey().at(0) < node->getEnderecoInicial()){
                    paresAnt.push_back(p);
                }*/
                if(p->getKey().at(0) > node->getEnderecoFinal()){
                    paresSuc.push_back(p);
                }
            }

            printf("SO DARLING, DARLING: %d", node->getNumNodes());

            sleep(1);
            //node->getAntecessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(node->getIndice(), node->getNumNodes(), paresAnt));
            node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(node->getIndice(), node->getNumNodes(), paresSuc));

            /*for(int i=0; i<paresAnt.size(); i++){
                Pair* p = paresAnt.at(i);
                node->removePar(p);
            }*/

            for(int i=0; i<paresSuc.size(); i++){
                Pair* p = paresSuc.at(i);
                node->removePar(p);
            }
            
            sleep(1);
            node->getSucessor()->enviar(Mensagem::criarMensagemSolicitacaoPorta());
        }
        else{        
            printf("--Antecessor NOT NULL--");
            fflush(stdout);
            if(node->getSucessor()->getConexaoCliente() == node->getSucessor()->getNovaConexaoCliente()){
            printf("--Igual--");
            fflush(stdout);
            node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());
                
            }
            else{
                printf("--Diferente--");
                fflush(stdout);
                        
                Mensagem* m = node->getSucessor()->receberDoNovoCliente();
                
                string ip = node->getSucessor()->getIpNovoCliente();
                int porta = atoi(m->getPartes().at(1).c_str());

                node->getSucessor()->enviar(Mensagem::criarMensagemNovoNode(ip, porta));

                node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());

                node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoIndice(node->getIndice()+1));

                node->incNumNodes();
                
                vector<Pair*> paresAnt = *new vector<Pair*>();
                vector<Pair*> paresSuc = *new vector<Pair*>();
                
                for(int i=0; i<node->getPares().size(); i++){
                    Pair* p = node->getPares().at(i);
                    if(p->getKey().at(0) < node->getEnderecoInicial()){
                        paresAnt.push_back(p);
                    }
                    if(p->getKey().at(0) > node->getEnderecoFinal()){
                        paresSuc.push_back(p);
                    }
                }
                
                printf("SO DARLING, DARLING: %d", node->getNumNodes());
                
                if(paresAnt.size() > 0){
                    sleep(1);
                    node->getAntecessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(node->getIndice(), node->getNumNodes(), paresAnt));
                    for(int i=0; i<paresAnt.size(); i++){
                        Pair* p = paresAnt.at(i);
                        node->removePar(p);
                    }

                }
                if(paresSuc.size() > 0){
                    sleep(1);
                    node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(node->getIndice(), node->getNumNodes(), paresSuc));
                    for(int i=0; i<paresSuc.size(); i++){
                        Pair* p = paresSuc.at(i);
                        node->removePar(p);
                    }
                }
                
                
            }
        }
        printf("--THIS IS THE END--");

    }

}
