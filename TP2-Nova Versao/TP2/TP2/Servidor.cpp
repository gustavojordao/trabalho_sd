#include "stdafx.h"
#include "Servidor.h"


Servidor::Servidor()
{
}


Servidor::~Servidor()
{
}

void Servidor::aceitar() {
	struct sockaddr_in cliente_addr;
	socklen_t addrlen = sizeof(cliente_addr);

	conexaoCliente = accept(conexao, (struct sockaddr *) &cliente_addr, &addrlen);

	if (conexaoCliente <= 0) {
		perror("\nNão foi possível aceitar conexão. - accept");
	}

	ip_cliente = inet_ntoa(cliente_addr.sin_addr);
	porta = (int)ntohs(cliente_addr.sin_port);
}