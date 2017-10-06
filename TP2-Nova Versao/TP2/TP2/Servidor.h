
#include "Mensagem.h"

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>

class Servidor
{
private:
	string ip_cliente;
	int porta;

	int conexao;
	int conexaoCliente;
public:
	Servidor(int porta);
	~Servidor();
	
	void iniciar();
	void aceitar();
	
	void setConexao(int conexao);

	void enviar(Mensagem*);
	Mensagem* receber();

	string getIpCliente();

	int getPorta();
	void setPorta(int porta);
};

