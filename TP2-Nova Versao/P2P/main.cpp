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
bool recebeNovoAntecessor;

int main(int argc, char**argv)
{

	string ip_antecessor;
	int porta_antecessor;
	int porta_sucessor;

	if (argc > 2) {
		ip_antecessor = argv[1];
		porta_antecessor = atoi(argv[2]);
		porta_sucessor = atoi(argv[3]);
	}
	else {
		ip_antecessor = "";
		porta_sucessor = atoi(argv[1]);
	}

	node = new Node(0, ip_antecessor, porta_antecessor, porta_sucessor);

	if (ip_antecessor.compare("") != 0) {
		// Não é primeiro
		node->getAntecessor()->conectar();
		node->getAntecessor()->enviar(Mensagem::criarMensagemSolicitacaoIndice());
		Mensagem* m = node->getAntecessor()->receber();

		int indice = atoi(m->getPartes().at(0).c_str());

		node->setIndice(indice);
	}

	recebeNovoAntecessor = true;
	node->getSucessor()->iniciar();
	
	pthread_create(&(thread_ac), NULL, thread_aceita_con, NULL);
        pthread_create(&(thread_ra), NULL, thread_recebe_ant, NULL);
        pthread_create(&(thread_rs), NULL, thread_recebe_suc, NULL);

    return 0;
}

void* thread_recebe_ant(void* arg) {

	while (true) {
		Mensagem* m = node->getAntecessor()->receber();

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
		
		switch (codigo) {

		case Mensagem::FIND:
			partes = m->getPartes();
			solicitante = atoi(partes.at(0).c_str());
			key = partes.at(1);

			posicao = node->findPar(key);

			if (posicao == -1) {
				node->getAntecessor()->enviar(m);
			}
			else if (posicao == -2) {
				node->getSucessor()->enviar(m);
			}
			else if (posicao == -3) {
				// TODO: Informa que chave não foi localizada.
				printf("Não foi encontrada a chave %s", key.c_str());
			}
			else {
				Pair* pair = node->getPares().at(posicao);
				node->getSucessor()->enviar(Mensagem::criarMensagemRespostaFind(solicitante, node->getIndice(), pair));
			}

			break;
		case Mensagem::RESPOSTA_FIND:
			solicitante = atoi(partes.at(0).c_str());

			if (solicitante == node->getIndice()) {
				// TODO: Faz alguma coisa com o par, exibe, salva, sei lá...
				printf("\n%s: %s\n", key.c_str(), value.c_str());
				node->storePar(new Pair(key, value));
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

			if (posicao == -1) {
				node->getAntecessor()->enviar(m);
			}
			else if (posicao == -2) {
				node->getSucessor()->enviar(m);
			}
			else if (posicao == -3) {
				node->getPares().push_back(new Pair(key, value));
			}
			else {
				node->getPares().at(posicao) = new Pair(key, value);
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
*/		case Mensagem::RESPOSTA_SOLICITACAO_INDICE:
			partes = m->getPartes();

			indice = atoi(partes.at(0).c_str());
			node->setIndice(indice);
			break;
		case Mensagem::ATUALIZACAO_NODE_ANT:
			partes = m->getPartes();
			indice = atoi(partes.at(0));
			num_nodes = atoi(partes.at(1));
			pares = *new vector<Pair*>();
			for(i=2; i<(partes.size()-2)/2; i++){
				pares.push_back(new Pair(partes.at(i), partes.at(i+1)));
			}
			
			if(indice != node->getIndice()){
				node->setNumNodes(num_nodes);
				for(i=0; i<pares.size(); i++){
					node->storePar(pares.at(i));
				}
				node->getSucessor()->enviar(m);
			}
				
			break;
		default:
			printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto().c_str());
			break;
		}
	}

}

void* thread_recebe_suc(void* arg) {

	while (true) {
		Mensagem* m = node->getSucessor()->receber();

		int codigo = m->getCodigo();

		vector<string> partes;
		int solicitante;
		string key;
		string value;
		int posicao;
		int indice;
		string ip;
		int porta;
		int num_nodes;

		switch (codigo) {
			
			case Mensagem::FIND:
				partes = m->getPartes();
				solicitante = atoi(partes.at(0).c_str());
				key = partes.at(1);

				posicao = node->findPar(key);

				if (posicao == -1) {
					node->getAntecessor()->enviar(m);
				}
				else if (posicao == -2) {
					node->getSucessor()->enviar(m);
				}
				else if (posicao == -3) {
					// TODO: Informa que chave não foi localizada.
					printf("Não foi encontrada a chave %s", key.c_str());
				}
				else {
					Pair* pair = node->getPares().at(posicao);
					node->getAntecessor()->enviar(Mensagem::criarMensagemRespostaFind(solicitante, node->getIndice(), pair));
				}

				break;
			case Mensagem::RESPOSTA_FIND:
				solicitante = atoi(partes.at(0).c_str());
				
				if (solicitante == node->getIndice()) {
					// TODO: Faz alguma coisa com o par, exibe, salva, sei lá...
					printf("\n%s: %s\n", key.c_str(), value.c_str());
					node->storePar(new Pair(key, value));
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

				if (posicao == -1) {
					node->getAntecessor()->enviar(m);
				}
				else if (posicao == -2) {
					node->getSucessor()->enviar(m);
				}
				else if(posicao == -3){
					node->getPares().push_back(new Pair(key, value));
				}
				else {
					node->getPares().at(posicao) = new Pair(key, value);
				}

				break;
			case Mensagem::NOVO_NODE:
				recebeNovoAntecessor = true;

				partes = m->getPartes();
				ip = partes.at(0);
				porta = atoi(partes.at(1).c_str()); // TODO: Rever formato de mensagem adicionando porta

				node->getAntecessor()->desconectar();
				node->getAntecessor()->setEndereco(ip, porta);
				node->getAntecessor()->conectar();

				break;
			//case Mensagem::RESPOSTA_NOVO_NODE:
			//	
			//	break;
			case Mensagem::SOLICITACAO_INDICE:
				node->getSucessor()->enviar(Mensagem::criarMensagemRespostaSolicitacaoIndice(node->getIndice() + 1));
				sleep(1); //TODO: Verificar unidade de tempo

				// TODO: Verificar necessidade de exigir que nó que entra na rede solicita atualização para então iniciar o ciclo.
				// Não é complicado de fazer.
				vector<Pair*> repasseParesAnt = *new vector<Pair*>();
				vector<Pair*> repasseParesSuc = *new vector<Pair*>();
				for(int i=0; i<node->getPares().size(); i++){
					int key_0 = node->getPares()->getKey().at(i)[0];
					if(node->getEnderecoInicial() < key_0){
						repasseParesAnt.push_back(node->getPares().at(i));
					}
					else if(node->getEnderecoFinal() > key_0 ){
						repasseParesSuc.push_back(node->getPares().at(i));
					}			
				}
				
				//TODO: Enviar em ambos os sentidos
				
				node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeAnt(node->getIndice(), node->getNumNodes(), repasseParesAnt));
				node->getSucessor()->enviar(Mensagem::criarMensagemAtualizacaoNodeSuc(node->getIndice(), node->getNumNodes(), repasseParesSuc));

				break;
			case Mensagem::ATUALIZACAO_NODE_SUC:
				partes = m->getPartes();
				indice = atoi(partes.at(0));
				num_nodes = atoi(partes.at(1));
				pares = *new vector<Pair*>();
				for(i=2; i<(partes.size()-2)/2; i++){
					pares.push_back(new Pair(partes.at(i), partes.at(i+1)));
				}
				
				if(indice != node->getIndice()){
					node->setNumNodes(num_nodes);
					for(i=0; i<pares.size(); i++){
						node->storePar(pares.at(i));
					}
					node->getAntecessor()->enviar(m);
				}
					
				break;
/*			case Mensagem::RESPOSTA_SOLICITACAO_INDICE:
				partes = m->getPartes();

				indice = atoi(partes.at(0));
				node->setIndice(indice);
				break;
*/			default:
				printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto().c_str());
				break;
		}
	}

}

void* thread_aceita_con(void* arg) {

	while (true) {
		
		// TODO: Aqui trata conexão de nó com último do anel. Tratar segunda parte, conexão de nó com primeiro do anel
		// Variável recebeNovoAntecessor é responsável por tratar a segunda parte.
		node->getSucessor()->aceitar();
		
		string ip = node->getSucessor()->getIpCliente();
		int porta = node->getSucessor()->getPorta();
		
		if (!recebeNovoAntecessor) {
			node->getSucessor()->enviar(Mensagem::criarMensagemNovoNode(ip, porta));
		}
		else {
			recebeNovoAntecessor = false;
		}
		node->getSucessor()->setConexao(node->getSucessor()->getConexaoCliente());
		
		node->incNumNodes();
		
	}

}
