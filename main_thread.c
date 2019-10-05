#include "matriz-operacoes-threads.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[])
{
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // DECLARAÇÃO de VARIÁVEIS
    mymatriz mat_a, mat_b;
    mymatriz **mmultbloco, **mmult;
    char filename[100];
    FILE *fmat;
    int nr_line;
    int *vet_line = NULL;
    int N, M, La, Lb;
    double start_time, end_time;

    matriz_bloco_t **Vsubmat_a = NULL;
    matriz_bloco_t **Vsubmat_b = NULL;
    matriz_bloco_t **Vsubmat_c = NULL;
    int nro_submatrizes = 2;

    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    if (argc != 3)
    {
        printf("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
        exit(1);
    }

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //LER MATRIZ_A
    //LER MATRIZ_B
    //CRIAR MATRIZ_C

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //                Leitura da Matriz A (arquivo)
    fmat = fopen(argv[1], "r");
    if (fmat == NULL)
    {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
    //return 1;
    mat_a.matriz = NULL;
    mat_a.lin = N;
    mat_a.col = La;
    if (malocar(&mat_a))
    {
        printf("ERROR: Out of memory\n");
    }
    filein_matriz(mat_a.matriz, N, La, fmat, vet_line, nr_line);
    free(vet_line);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Leitura da Matriz B (arquivo)
    fmat = fopen(argv[2], "r");
    if (fmat == NULL)
    {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &Lb, &M, &vet_line, &nr_line);
    mat_b.matriz = NULL;
    mat_b.lin = Lb;
    mat_b.col = M;
    if (malocar(&mat_b))
    {
        printf("ERROR: Out of memory\n");
    }
    filein_matriz(mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
    free(vet_line);
    fclose(fmat);

    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação Sequencial
    /*
 LOOP (10x)
	MARCAR_TEMPO INICIO
	MATRIZ_SeqC = MULTIPLICAR MATRIZ_A x MATRIZ_B
	MARCAR_TEMPO FIM
	GRAVAR_DISCO MATRIZ_SeqC*/
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação Sequencial em Bloco
    /*
	DEFINIR N (numero de pedacos entre 2 e 20)
LOOP (10x)

	MARCAR_TEMPO INICIO
	PARTICIONAR_MATRIZ em N pedacos
	MATRIZ_SeqBlC = MULTIPLICARBLOCOS MATRIZ_A x MATRIZ_B
	MARCAR_TEMPO FIM
	GRAVAR_DISCO MATRIZ_SeqBlC*/
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%

    // Multiplicação MultiThread
    /*
 LOOP (10x)
        MARCAR_TEMPO INICIO
	MATRIZ_SeqC = MULTIPLICARMULTITHREADS MATRIZ_A x MATRIZ_B
	MARCAR_TEMPO FIM
	GRAVAR_DISCO MATRIZ_ThreadC
    */
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação MultiThreads em Bloco
    /*
	N = mesmo nro de blocos da Sequencial
 LOOP (10x)
	MARCAR_TEMPO INICIO
	PARTICIONAR_MATRIZ em N pedacos
	MATRIZ_SeqBlC = MULTIPLICARBLOCOSMultithreads MATRIZ_A x MATRIZ_B
	MARCAR_TEMPO FIM
	GRAVAR_DISCO MATRIZ_ThreadBlC
    */
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // Comparar resultados e tempos
    /*
    (print) COMPARAR MATRIZ_SeqC c/ MATRIZ_SeqBlC
    (print) COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadC
	(print) COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadBlC
	(print) TEMPO_MEDIO MATRIZ_SeqC 
	(print) TEMPO_MEDIO MATRIZ_SeqBlC
	(print) TEMPO_MEDIO MATRIZ_ThreadC
	(print) TEMPO_MEDIO MATRIZ_ThreadBlC
    (print) SPEEDUP (MATRIZ_C)
    (print) SPEEDUP (MATRIZ_BLC)
    */
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%

    // LIBERAR MEMÓRIA
    /*
	LIBERAR MATRIZES
	FECHAR ARQUIVOS
    */

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //                   Liberação de memória
    mliberar(mmult[0]);
    free(mmult[0]);
    mliberar(mmultbloco[0]);
    free(mmultbloco[0]);

    mliberar(&mat_a);
    mliberar(&mat_b);
    free(mmult);
    free(mmultbloco);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    return 0;
}