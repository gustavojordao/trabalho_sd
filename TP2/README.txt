README

COMO EXECUTAR

Cada execução do programa realiza uma conexão da máquina que está executando com o anel. Cada execução exige como parâmetros um ip e uma porta para conexão com o nó antecessor e uma porta para conexão com o nó sucessor, exceto quando a primeira máquina executa. Neste caso, apenas a porta de conexão com o nó sucessor deve ser passada.

A conexão com o nó sucessor corresponde a um servidor escutando uma determinada porta, enquanto a conexão com o antecessor é um cliente que se conecta ao servidor.

Em cada execução, deve-se conhecer o último nó (ip e porta) a entrar na rede, de modo que este é o ponto de acesso ao anel.

Portanto, as execuções ocorrem com os seguintes comandos.

./p2p <PORTA_SUCESSOR>
./p2p <IP_ANTECESSOR> <PORTA_ANTECESSOR> <PORTA_SUCESSOR>

O primeiro nó da rede deve usar o primeiro modelo de comando e os demais nós devem usar o segundo.

Exemplo de uma rede com 4 máquinas (192.168.0.X, sendo X = {1, 2, 3, 4})

-> 192.168.0.1
	./p2p 8000

-> 192.168.0.2
	./p2p 192.168.0.1 8000 8001

-> 192.168.0.3
	./p2p 192.168.0.2 8001 8002

-> 192.168.0.4
	./p2p 192.168.0.3 8002 8003

Note que a primeira máquina (192.168.0.1) não precisa apontar para outra, apenas especificar a porta usada para se comunicar com o sucessor. As demais máquinas necessitam apontar para a última máquina a entrar na rede.

COMO UTILIZAR

A aplicação simula um terminal. Nele existem 3 opções de comando:

find <KEY>
store <KEY> <VALUE>
status

find - Busca na rede a chave especificada pelo parâmetro <KEY> e caso seja encontrada, o valor é exibido.

store - Armazena o valor especificado pelo parâmetro <VALUE> na chave <KEY>. Se a chave já existir na rede, o valor é atualizado. Caso contrário, a chave é inserida de acordo com um índice.

status - Exibe informações sobre o nó, dentre elas os pares chave-valor armazenados localmente, os limites dos índices.

DEFINIÇÕES DE ARMAZENAMENTOS

Foi assumido que as chaves iniciam com letras (A-Z ou a-z) e não possuem espaços. As chaves são insensitivas, isto é, letras maiúsculas ou minúsculas são consideras iguais. As letras pertencentes a cada nó são definidas pelos seguintes cálculos:

Sendo, 

	N - Número de nós da rede
	I - Índice do nó de acordo com a ordem de entrada (0 a N-1)

	P - Primeira letra do nó
	U - Última letra do nó

P e U podem ser obtidos por:

	P = 65+(I*ROUNDUP(26/N))
	U = MIN(65+((I+1)*ROUNDUP(26/N))-1, 90)

Os valores de P e U são números de 65 a 90 que correspondem ao código ASCII dos caracteres (A-Z).

Exemplos:

N = 1
I = 0

P = 65+(0*ROUNDUP(26/1)) = 65 -> A
U = MIN(65+((0+1)*ROUNDUP(26/1))-1, 90) = 90 -> Z

--------------------------------------------------

N = 2
I = 0

P = 65+(0*ROUNDUP(26/2)) = 65 -> A
U = MIN(65+((0+1)*ROUNDUP(26/2))-1, 90) = 77 -> M


N = 2
I = 1

P = 65+(1*ROUNDUP(26/2)) = 78 -> N
U = MIN(65+((1+1)*ROUNDUP(26/2))-1, 90) = 90 -> Z

-------------------------------------------------

N = 3
I = 0

P = 65+(0*ROUNDUP(26/3)) = 65 -> A
U = MIN(65+((0+1)*ROUNDUP(26/3))-1, 90) = 73 -> I


N = 3
I = 1

P = 65+(1*ROUNDUP(26/3)) = 74 -> J
U = MIN(65+((1+1)*ROUNDUP(26/3))-1, 90) = 82 -> R


N = 3
I = 2

P = 65+(2*ROUNDUP(26/3)) = 83 -> S
U = MIN(65+((2+1)*ROUNDUP(26/3))-1, 90) = 90 -> Z


