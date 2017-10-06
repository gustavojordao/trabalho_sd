
#include "Servidor.h"
#include "Cliente.h"

#include <vector>
using namespace std;

class Node
{
private:
	int indice;
	vector<Pair*> pares;

	Servidor* antecessor;
	Cliente* sucessor;
	
public:
	Node();
	Node(int indice, string ip_antecessor, int porta);
	~Node();

	void setIndice(int indice);
	int getIndice();
	
	// Retorna �ndice do par no vetor
	// Retorna -1 caso deve estar no antecessor
	// Retorna -2 caso deve estar no sucessor
	// Retorna -3 caso deveria estar neste n� mas n�o est�
	int findPar(string value);
	void storePar(Pair* pair);
	vector<Pair*> getPares();

	Cliente* getAntecessor();
	Servidor* getSucessor();
	
};

