// TP2.cpp : Define o ponto de entrada para a aplicação de console.
//

#include "stdafx.h"

#include "Node.h"

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

void thread_recebe_ant(void* arg);
void thread_recebe_suc(void* arg);
void thread_aceita_con(void* arg);

Node* node;
bool recebeNovoAntecessor;

int main(int argc, char**argv)
{

	string ip;
	int porta;

	if (argc > 2) {
		ip = argv[1];
		porta = stoi(argv[2]);
	}
	else {
		ip = "";
		porta = stoi(argv[1]);
	}

	node = new Node();

	if (ip.compare("") != 0) {
		// Não é primeiro
		node->getAntecessor()->conectar();
		node->getAntecessor()->enviar(Mensagem::criarMensagemSolicitacaoIndice());
		Mensagem* m = node->getAntecessor()->receber();

		int indice = stoi(m->getPartes().at(0));

		node->setIndice(indice);
	}

	recebeNovoAntecessor = true;
	node->getSucessor()->iniciar();
	
	//pthread
	//pthread
	//pthread

    return 0;
}

void thread_recebe_ant(void* arg) {

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

		switch (codigo) {

		case Mensagem::FIND:
			partes = m->getPartes();
			solicitante = stoi(partes.at(0));
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
			solicitante = partes.at(0);

			if (solicitante == node->getIndice()) {
				// TODO: Faz alguma coisa com o par, exibe, salva, sei lá...
				printf("\n%s: %s\n", key, value);
				node->storePar(new Pair(key, value));
			}
			else {
				node->getSucessor()->enviar(m);
			}
			break;
		case Mensagem::STORE:
			partes = m->getPartes();
			solicitante = stoi(partes.at(0));
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
			porta = stoi(partes.at(1)); // TODO: Rever formato de mensagem adicionando porta

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

			indice = stoi(partes.at(0));
			node->setIndice(indice);
			break;
		default:
			printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto());
			break;
		}
	}

}

void thread_recebe_suc(void* arg) {

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

		switch (codigo) {
			
			case Mensagem::FIND:
				partes = m->getPartes();
				solicitante = stoi(partes.at(0));
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
				solicitante = partes.at(0);
				
				if (solicitante == node->getIndice()) {
					// TODO: Faz alguma coisa com o par, exibe, salva, sei lá...
					printf("\n%s: %s\n", key, value);
					node->storePar(new Pair(key, value));
				}
				else {
					node->getAntecessor()->enviar(m);
				}
				break;
			case Mensagem::STORE:
				partes = m->getPartes();
				solicitante = stoi(partes.at(0));
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
				porta = stoi(partes.at(1)); // TODO: Rever formato de mensagem adicionando porta

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
/*			case Mensagem::RESPOSTA_SOLICITACAO_INDICE:
				partes = m->getPartes();

				indice = stoi(partes.at(0));
				node->setIndice(indice);
				break;
*/			default:
				printf("Mensagem não identificada\nCódigo:%d\nTexto:%s", m->getCodigo(), m->getTexto());
				break;
		}
	}

}

void thread_aceita_con(void* arg) {

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
		node->getSucessor()->setConexao(conexaoNovoNode);
	}

}