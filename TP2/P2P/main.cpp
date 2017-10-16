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

        pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);
    }        
    // Nó inicial da rede
    else {
        node->getSucessor()->iniciar();
    }

    iniciaThreadServidor = true;
    pthread_create(&(thread_ac), NULL, thread_aceita_con, NULL);

    // Execução de comandos
    while (true) {
        // Aguarda opções find/store/status
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
                    // Interpreta mensagem
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    key = partes.at(1);

                    // Verifica se a chave foi encontrada ou se a busca continua
                    posicao = node->findPar(key);

                    // Chave encontrada, envia resultado ao nó que fez a requisição
                    if (posicao >= 0) {
                        Pair* pair = node->getPares().at(posicao);
                        node->getSucessor()->enviar(Mensagem::criarMensagemRespostaFind(solicitante, node->getIndice(), pair));
                    }                        
                    // Chave não encontrada, possivelmente localizada nos nós antecessores
                    else if (posicao == -1) {
                        node->getAntecessor()->enviar(m);
                    }                        
                    // Chave não encontrada, possivelmente localizada nos nós sucessores
                    else if (posicao == -2) {
                        node->getSucessor()->enviar(m);
                    }
                    // Chave não encontrada, não foi inserida na rede
                    else if (posicao == -3) {
                        // Este foi o nó que fez a requisição, portanto exibe o resultado da busca
                        if (solicitante == node->getIndice()) {
                            printf("Chave %s não encontrada na rede.", key.c_str());
                            fflush(stdout);
                        }                            
                        // Informa o nó que fez a requisição que a chave não foi encontrada
                        else {
                            node->getSucessor()->enviar(Mensagem::criarMensagemRespostaFindNaoEncontrado(solicitante, key));
                        }
                    }

                    break;
                case Mensagem::RESPOSTA_FIND:
                    // Interpreta mensagem
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    detentor = atoi(partes.at(1).c_str());
                    key = partes.at(2);
                    value = partes.at(3);

                    // Se for o nó que fez a requisição, exibe o resultado
                    if (solicitante == node->getIndice()) {
                        printf("\n[%s] = {%s} no node %d\n", key.c_str(), value.c_str(), detentor);
                        fflush(stdout);
                    }
                    // Se for outro nó que fez a requisição, envia o resultado para o próximo
                    else {
                        node->getSucessor()->enviar(m);
                    }
                    break;
                case Mensagem::RESPOSTA_FIND_NAO_ENCONTRADO:
                    // Interpreta mensagem
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    key = partes.at(1);

                    // Se for o nó que fez a requisição, exibe o resultado
                    if (solicitante == node->getIndice()) {
                        printf("Chave %s não encontrada na rede.", key.c_str());
                        fflush(stdout);
                    }                        // Se for outro nó que fez a requisição, envia o resultado para o sucessor
                    else {
                        node->getSucessor()->enviar(m);
                    }
                    break;
                case Mensagem::STORE:
                    // Interpreta mensagem
                    partes = m->getPartes();
                    solicitante = atoi(partes.at(0).c_str());
                    key = partes.at(1);
                    value = partes.at(2);

                    // Verifica se a chave deve ser armazenada neste nó
                    posicao = node->findPar(key);

                    // Chave encontrada, atualiza valor
                    if (posicao >= 0) {
                        node->storePar(new Pair(key, value));
                    }
                    // Chave não encontrada, armazenamento nos antecessores
                    if (posicao == -1) {
                        node->getAntecessor()->enviar(m);
                    }
                    // Chave não encontrada, armazenamento nos sucessores
                    else if (posicao == -2) {
                        node->getSucessor()->enviar(m);
                    }                        
                    // Chave não encontrada, armazena chave e valor
                    else if (posicao == -3) {
                        node->storePar(new Pair(key, value));
                    }

                    break;
                case Mensagem::NOVO_NODE:
                    // Interpreta mensagem
                    partes = m->getPartes();
                    ip = partes.at(0);
                    porta = atoi(partes.at(1).c_str());

                    // Desfaz conexão com o antecessor e adiciona nova conexão de antecessor
                    node->getAntecessor()->desconectar();
                    node->setAntecessor(ip, porta);
                    node->getAntecessor()->conectar();

                    break;
                case Mensagem::ATUALIZACAO_INDICE:
                    // Interpreta mensagem
                    indice = atoi(m->getTexto().c_str());

                    // Atualiza índice
                    node->setIndice(indice);
                    break;
                case Mensagem::ATUALIZACAO_NODE_SUC:
                    // Interpreta mensagem
                    partes = m->getPartes();
                    indice = atoi(partes.at(0).c_str());
                    num_nodes = atoi(partes.at(1).c_str());

                    pares = *new vector<Pair*>();
                    for (i = 2; i < partes.size(); i += 2) {
                        pares.push_back(new Pair(partes.at(i), partes.at(i + 1)));
                    }

                    // Atualiza quantidade de nós da rede 
                    node->setNumNodes(num_nodes);

                    // Verifica quais entre os pares recebidos devem ser armazenados neste nó
                    for (i = 0; i < pares.size(); i++) {
                        posicao = node->findPar(pares.at(i)->getKey());
                        if (posicao == 0 || posicao == -3) {
                            node->storePar(pares.at(i));
                            pares.erase(pares.begin() + i);
                            i--;
                        }
                    }

                    // Verifica se a mensagem ainda não percorreu toda a rede 
                    if (indice != node->getIndice()) {

                        // Verifica quais entre os pares devem ser redirecionados após a introdução de novo nó
                        for (int i = 0; i < node->getPares().size(); i++) {
                            Pair* p = node->getPares().at(i);
                            if (p->getKey().at(0) > node->getEnderecoFinal()) {
                                pares.push_back(p);
                                node->removePar(p);
                                i--;
                            }
                        }

                        // Envia pares para serem armazenados nos nós sucessores
                        node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(indice, num_nodes, pares));
                    }

                    break;
                case Mensagem::SOLICITACAO_PORTA:
                    // Interpreta mensagem
                    porta = node->getSucessor()->getPorta();

                    // Envia porta ao nó antecessor para que o anel seja fechado 
                    node->getAntecessor()->enviar(Mensagem::criarRespostaMensagemSolicitacaoPorta(porta));
                    break;
                default:
                    // Mensagem fora do padrão
                    printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto().c_str());
                    fflush(stdout);
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
                // Interpreta mensagem
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                key = partes.at(1);

                // Verifica se a chave foi encontrada ou se a busca continua
                posicao = node->findPar(key);

                // Chave encontrada, envia resultado ao nó que fez a requisição
                if (posicao >= 0) {
                    Pair* pair = node->getPares().at(posicao);
                    node->getAntecessor()->enviar(Mensagem::criarMensagemRespostaFind(solicitante, node->getIndice(), pair));
                }                    // Chave não encontrada, possivelmente localizada nos nós antecessores
                else if (posicao == -1) {
                    node->getAntecessor()->enviar(m);
                }                    // Chave não encontrada, possivelmente localizada nos nós sucessores
                else if (posicao == -2) {
                    node->getSucessor()->enviar(m);
                }
                    // Chave não encontrada, não foi inserida na rede
                else if (posicao == -3) {
                    // Este foi o nó que fez a requisição, portanto exibe o resultado da busca
                    if (solicitante == node->getIndice()) {
                        printf("Chave %s não encontrada na rede.", key.c_str());
                        fflush(stdout);
                    }                        // Informa o nó que fez a requisição que a chave não foi encontrada
                    else {
                        node->getAntecessor()->enviar(Mensagem::criarMensagemRespostaFindNaoEncontrado(solicitante, key));
                    }
                }

                break;
            case Mensagem::RESPOSTA_FIND:
                // Interpreta mensagem
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                detentor = atoi(partes.at(1).c_str());
                key = partes.at(2);
                value = partes.at(3);

                // Se for o nó que fez a requisição, exibe o resultado
                if (solicitante == node->getIndice()) {
                    printf("\n[%s] = {%s} no node %d\n", key.c_str(), value.c_str(), detentor);
                    fflush(stdout);
                }
                    // Se for outro nó que fez a requisição, envia o resultado para o próximo
                else {
                    node->getAntecessor()->enviar(m);
                }
                break;
            case Mensagem::RESPOSTA_FIND_NAO_ENCONTRADO:
                // Interpreta mensagem
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                key = partes.at(1);

                // Se for o nó que fez a requisição, exibe o resultado
                if (solicitante == node->getIndice()) {
                    printf("Chave %s não encontrada na rede.", key.c_str());
                    fflush(stdout);
                }                    // Se for outro nó que fez a requisição, envia o resultado para o antecessor
                else {
                    node->getAntecessor()->enviar(m);
                }
                break;
            case Mensagem::STORE:
                // Interpreta mensagem
                partes = m->getPartes();
                solicitante = atoi(partes.at(0).c_str());
                key = partes.at(1);
                value = partes.at(2);

                // Verifica se a chave deve ser armazenada neste nó
                posicao = node->findPar(key);

                // Chave encontrada, atualiza valor
                if (posicao >= 0) {
                    node->storePar(new Pair(key, value));
                }
                // Chave não encontrada, armazenamento nos antecessores
                if (posicao == -1) {
                    node->getAntecessor()->enviar(m);
                }                    // Chave não encontrada, armazenamento nos sucessores
                else if (posicao == -2) {
                    node->getSucessor()->enviar(m);
                }                    // Chave não encontrada, armazena chave e valor
                else if (posicao == -3) {
                    node->storePar(new Pair(key, value));
                }

                break;
            case Mensagem::ATUALIZACAO_NODE_ANT:
                // Interpreta mensagem
                partes = m->getPartes();
                indice = atoi(partes.at(0).c_str());
                num_nodes = atoi(partes.at(1).c_str());

                pares = *new vector<Pair*>();
                for (i = 2; i < partes.size(); i += 2) {
                    pares.push_back(new Pair(partes.at(i), partes.at(i + 1)));
                }

                // Atualiza quantidade de nós da rede 
                node->setNumNodes(num_nodes);

                // Verifica quais entre os pares recebidos devem ser armazenados neste nó
                for (i = 0; i < pares.size(); i++) {
                    posicao = node->findPar(pares.at(i)->getKey());
                    if (posicao == 0 || posicao == -3) {
                        node->storePar(pares.at(i));
                        pares.erase(pares.begin() + i);
                        i--;
                    }
                }

                // Verifica se a mensagem ainda não percorreu toda a rede 
                if (indice != node->getIndice()) {

                    // Verifica quais entre os pares devem ser redirecionados após a introdução de novo nó
                    for (int i = 0; i < node->getPares().size(); i++) {
                        Pair* p = node->getPares().at(i);
                        if (p->getKey().at(0) < node->getEnderecoFinal()) {
                            pares.push_back(p);
                            node->removePar(p);
                            i--;
                        }
                    }

                    // Envia pares para serem armazenados nos nós antecessores
                    node->getAntecessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(indice, num_nodes, pares));
                }

                break;
            case Mensagem::RESPOSTA_SOLICITACAO_PORTA:
                // Interpreta mensagem
                ip = node->getSucessor()->getIpCliente();
                porta = atoi(m->getTexto().c_str());

                // Conecta ao novo nó da rede para fechar o anel 
                node->setAntecessor(ip, porta);
                node->getAntecessor()->conectar();

                // Inicia thread de recebimento de mensagens do antecessor
                pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);

                break;
            default:
                // Mensagem fora do padrão
                printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto().c_str());
                fflush(stdout);
                break;
        }
    }

}

void* thread_aceita_con(void* arg) {

    while (true) {

        // Aceita conexão de novo nó sucessor
        node->getSucessor()->aceitar();

        // Se for a primeira execução, habilita a thread de recebimento de mensagem de sucessor
        if (iniciaThreadServidor) {
            pthread_create(&(thread_rs), NULL, thread_recebe_suc, NULL);
            iniciaThreadServidor = false;
        }

        // Verifica se é o primeiro nó da rede
        if (node->getAntecessor() == NULL) {

            // Associa conexão efetiva com conexão recém-aceita 
            node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());

            // Envia índice do novo nó
            node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoIndice(node->getIndice() + 1));

            // Atualiza a quantidade de nós da rede
            node->incNumNodes();

            // Verifica quais entre os pares devem ser redirecionados após a introdução de novo nó
            vector<Pair*> paresSuc = *new vector<Pair*>();

            for (int i = 0; i < node->getPares().size(); i++) {
                Pair* p = node->getPares().at(i);
                if (p->getKey().at(0) > node->getEnderecoFinal()) {
                    paresSuc.push_back(p);
                    node->removePar(p);
                    i--;
                }
            }

            // Envia pares para serem armazenados nos nós sucessores
            node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(node->getIndice(), node->getNumNodes(), paresSuc));

            // Solicita ao novo nó que informe a porta de seu servidor para que o anel seja fechado
            node->getSucessor()->enviar(Mensagem::criarMensagemSolicitacaoPorta());
        }            // Se já houver algum nó na rede
        else {
            // Verifica se o novo nó já é o nó sucessor
            if (node->getSucessor()->getConexaoCliente() == node->getSucessor()->getNovaConexaoCliente()) {
                // Garante que o novo nó vai ser o nó sucessor
                node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());
            }                // Novo nó ainda não é o sucessor efetivo
            else {
                // Recebe mensagem do novo cliente enquanto ainda não é o sucessor
                Mensagem* m = node->getSucessor()->receberDoNovoCliente();

                // Interpreta mensagem
                string ip = node->getSucessor()->getIpNovoCliente();
                int porta = atoi(m->getPartes().at(1).c_str());

                // Informa nó sucessor que o novo nó será seu antecessor
                node->getSucessor()->enviar(Mensagem::criarMensagemNovoNode(ip, porta));

                // Associa novo nó como sucessor 
                node->getSucessor()->setConexao(node->getSucessor()->getNovaConexaoCliente());

                // Envia índice do novo nó
                node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoIndice(node->getIndice() + 1));

                // Atualiza a quantidade de nós da rede
                node->incNumNodes();

                // Verifica quais entre os pares devem ser redirecionados após a introdução de novo nó
                vector<Pair*> paresAnt = *new vector<Pair*>();
                vector<Pair*> paresSuc = *new vector<Pair*>();

                for (int i = 0; i < node->getPares().size(); i++) {
                    Pair* p = node->getPares().at(i);
                    if (p->getKey().at(0) < node->getEnderecoInicial()) {
                        paresAnt.push_back(p);
                        node->removePar(p);
                        i--;
                    }
                    if (p->getKey().at(0) > node->getEnderecoFinal()) {
                        paresSuc.push_back(p);
                        node->removePar(p);
                        i--;
                    }
                }

                // Envia pares para serem armazenados nos nós antecessores apenas se existirem                    
                if (paresAnt.size() > 0) {
                    node->getAntecessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(node->getIndice(), node->getNumNodes(), paresAnt));
                }
                // Envia pares para serem armazenados nos nós sucessores apenas se existirem                    
                if (paresSuc.size() > 0) {
                    node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(node->getIndice(), node->getNumNodes(), paresSuc));
                }

            }
        }

    }

}
