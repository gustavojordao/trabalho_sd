
#include "Mensagem.h"

class Cliente
{
private:
	string ip;
	int porta;

public:
	Cliente(string ip, int porta);
	~Cliente();
	
	void conectar();
	void desconectar();

	void enviar(Mensagem*);
	Mensagem* receber();

	void setEndereco(string ip, int porta);
};

