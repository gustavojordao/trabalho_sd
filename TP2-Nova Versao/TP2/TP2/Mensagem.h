
#include "Pair.h"

#include <string>
using namespace std;

class Mensagem
{
private:
	int codigo;
	string texto;

public:
	Mensagem();
	~Mensagem();

	static Mensagem* criarMensagemFind(/*int codigo, */int solicitante, string key);
	static Mensagem* criarMensagemRespostaFind(/*int codigo, */int solicitante, int detentor, Pair* pair);
	static Mensagem* criarMensagemStore(/*int codigo, */int solicitante, Pair* pair);
	static Mensagem* criarMensagemNovoNode(/*int codigo, */string ipNovoNode, int portaNovoNode);
	static Mensagem* criarMensagemRespostaNovoNode(/*int codigo, */int novoNode);
	static Mensagem* criarMensagemSolicitacaoIndice();
	static Mensagem* criarMensagemRespostaSolicitacaoIndice(int indice);
	//static Mensagem* criarMensagemMorra();

	static const int FIND = 0;
	static const int RESPOSTA_FIND = 1;
	static const int STORE = 2;
	static const int NOVO_NODE = 3;
	//static const int RESPOSTA_NOVO_NODE = 4;
	static const int SOLICITACAO_INDICE = 5;
	static const int RESPOSTA_SOLICITACAO_INDICE = 6;

	int getCodigo();
	string getTexto();

	vector<string> getPartes();
};
