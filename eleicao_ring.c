#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SEM_ELEICAO 0
#define EM_ELEICAO 1
#define SEM_COORDENADOR -1
#define COORDENADOR 1

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int proximo;
	if(world_rank == world_size -1) proximo = 0;
	else proximo = world_rank + 1;

	int anterior;
	if(world_rank == 0) anterior = world_size - 1;
	else anterior = world_rank - 1;

	srand(time(NULL) + world_rank);

	int probabilidade = (rand() % 100) + 1;
	int eleicao = SEM_ELEICAO;
	int eleito = SEM_COORDENADOR;
	int valor = rand() % 1000;
	int maior_valor = valor;

	MPI_Status status;
	int status_valor;

	printf("Processo: %d Meu valor é %d\n", world_rank, valor);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("\n");

	if(probabilidade >= 50) {
		eleicao = EM_ELEICAO;
		printf("O processo %d está começando uma eleição com probabilidade %d!\n\n", world_rank, probabilidade);
		MPI_Send(&maior_valor, 1, MPI_INT, proximo, 0, MPI_COMM_WORLD);
	}

	while(eleito == SEM_COORDENADOR) {
		MPI_Recv(&maior_valor, 1, MPI_INT, anterior, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		status_valor = status.MPI_TAG;

		if(status_valor == COORDENADOR) {
			eleito = world_rank;
		}

		else {
			if(maior_valor < valor) {
				maior_valor = valor;
			}
			else if(maior_valor == valor) {
				printf("Processo %d: Ganhei!\n", world_rank);
				status_valor = COORDENADOR;
			}
		}

		MPI_Send(&maior_valor, 1, MPI_INT, proximo, status_valor, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}
