#include <pthread.h>
#include "matriz-operacoes-threads.h"

typedef struct
{
    int tid;
    int ntasks;
    mymatriz *mat_a;
    mymatriz *mat_b;
    mymatriz *mat_c;
    matriz_bloco_t *mat_bloco_a;
    matriz_bloco_t *mat_bloco_b;
    matriz_bloco_t *mat_bloco_c;
} param_t;

void *exec_multi_thread(void *arg)
{
    param_t *p = (param_t *)arg;
    multiplicarTh(p->mat_a, p->mat_b, p->mat_c, p->tid, p->ntasks);
    return NULL;
}

void *exec_multi_thread_blocos(void *arg)
{
    param_t *p = (param_t *)arg;
    multiplicarThblocos(p->mat_bloco_a, p->mat_bloco_b, p->mat_bloco_c);
    return NULL;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[])
{
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // DECLARAÇÃO DE VARIÁVEIS
    mymatriz mat_a, mat_b;
    mymatriz **mmult_MATRIZ_SeqC;
    mymatriz **mmult_MATRIZ_SeqBlC;
    mymatriz **mmult_MATRIZ_ThreadC;
    mymatriz **mmult_MATRIZ_ThreadBlC;

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

    //For para executar calculo da média
    int ntasks = 2;
    int count_for = 10;

    param_t *args;
    pthread_t *threads;

    double tempo_MATRIZ_SeqC = 0;
    double tempo_MATRIZ_SeqBlC = 0;
    double MATRIZ_ThreadC = 0;
    double MATRIZ_ThreadBlC = 0;
    double speedup_seqC;
    double speedup_BlC;

    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    if (argc != 3)
    {
        printf("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
        exit(1);
    }

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //Leitura da Matriz A (arquivo)
    fmat = fopen(argv[1], "r");
    if (fmat == NULL)
    {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
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
    printf("\rMultiplicação Sequencial...             ");
    fflush(stdout);
    mmult_MATRIZ_SeqC = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {
        start_time = wtime();
        mmult_MATRIZ_SeqC[0] = mmultiplicar(&mat_a, &mat_b, 1);
        end_time = wtime();
        tempo_MATRIZ_SeqC += end_time - start_time;
        //printf("sequencial %d. tempo: %.20f\n",count, end_time - start_time);
    }
    sprintf(filename, "MATRIZ_SeqC.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmult_MATRIZ_SeqC[0], fmat);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    printf("\rMultiplicação Sequencial em Bloco...");
    fflush(stdout);
    mmult_MATRIZ_SeqBlC = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {

        Vsubmat_a = particionar_matriz(mat_a.matriz, N, La, 1, nro_submatrizes);
        Vsubmat_b = particionar_matriz(mat_b.matriz, Lb, M, 0, nro_submatrizes);
        Vsubmat_c = csubmatrizv2(N, M, nro_submatrizes);
        start_time = wtime();
        //multiplicacao de blocos
        for (int i = 0; i < nro_submatrizes; i++)
        {
            multiplicar_submatriz(Vsubmat_a[i], Vsubmat_b[i], Vsubmat_c[i]);
        }

        //soma os blocos separados
        mmult_MATRIZ_SeqBlC[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

        for (int i = 2; i < nro_submatrizes; i++)
        {
            mmult_MATRIZ_SeqBlC[0] = msomar(mmult_MATRIZ_SeqBlC[0], Vsubmat_c[i]->matriz, 1);
        }

        end_time = wtime();
        tempo_MATRIZ_SeqBlC += end_time - start_time;
        //printf("bloco %d. tempo: %.20f \t avg= %.20f\n",count, end_time - start_time, tempo_MATRIZ_SeqBlC / (count+1));
    }
    sprintf(filename, "MATRIZ_SeqBlC.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmult_MATRIZ_SeqBlC[0], fmat);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //
    printf("\rMultiplicação MultiThread...             ");
    fflush(stdout);
    mmult_MATRIZ_ThreadC = (mymatriz **)malloc(sizeof(mymatriz *));
    mmult_MATRIZ_ThreadC[0] = malloc(sizeof(mymatriz));
    mmult_MATRIZ_ThreadC[0]->matriz = NULL;
    mmult_MATRIZ_ThreadC[0]->lin = mat_a.lin;
    mmult_MATRIZ_ThreadC[0]->col = mat_b.col;

    //realiza a alocação de memória para matriz resultado
    if (malocar(mmult_MATRIZ_ThreadC[0]))
    {
        printf("ERROR: Out of memory\n");
        exit(1);
    }
    else
    {
        mzerar(mmult_MATRIZ_ThreadC[0]);
    }

    for (int count = 0; count < count_for; count++)
    {
        mzerar(mmult_MATRIZ_ThreadC[0]);
        threads = (pthread_t *)malloc(ntasks * sizeof(pthread_t));
        args = (param_t *)malloc(ntasks * sizeof(param_t));
        start_time = wtime();
        for (int i = 0; i < ntasks; i++)
        {
            args[i].tid = i;
            args[i].ntasks = ntasks;
            args[i].mat_a = &mat_a;
            args[i].mat_b = &mat_b;
            args[i].mat_c = mmult_MATRIZ_ThreadC[0];
            pthread_create(&threads[i], NULL, exec_multi_thread, (void *)(args + i));
        }

        for (int i = 0; i < ntasks; i++)
        {
            pthread_join(threads[i], NULL);
        }
        end_time = wtime();
        MATRIZ_ThreadC += end_time - start_time;
        //printf("thread %d. tempo: %.20f\n",count, end_time - start_time);
    }
    sprintf(filename, "MATRIZ_ThreadC.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmult_MATRIZ_ThreadC[0], fmat);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    printf("\rMultiplicação MultiThreads em Bloco...");
    fflush(stdout);
    mmult_MATRIZ_ThreadBlC = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {
        Vsubmat_a = particionar_matriz(mat_a.matriz, N, La, 1, nro_submatrizes);
        Vsubmat_b = particionar_matriz(mat_b.matriz, Lb, M, 0, nro_submatrizes);
        Vsubmat_c = csubmatrizv2(N, M, nro_submatrizes);
        threads = (pthread_t *)malloc(ntasks * sizeof(pthread_t));
        args = (param_t *)malloc(ntasks * sizeof(param_t));
        start_time = wtime();
        for (int i = 0; i < ntasks; i++)
        {
            args[i].tid = i;
            args[i].ntasks = ntasks;
            args[i].mat_bloco_a = Vsubmat_a[i];
            args[i].mat_bloco_b = Vsubmat_b[i];
            args[i].mat_bloco_c = Vsubmat_c[i];
            pthread_create(&threads[i], NULL, exec_multi_thread_blocos, (void *)(args + i));
        }

        for (int i = 0; i < ntasks; i++)
        {
            pthread_join(threads[i], NULL);
        }

        mmult_MATRIZ_ThreadBlC[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

        end_time = wtime();
        MATRIZ_ThreadBlC += end_time - start_time;
        //printf("thread bloco %d. tempo: %.20f\n",count, end_time - start_time);
    }
    sprintf(filename, "MATRIZ_ThreadBlC.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmult_MATRIZ_ThreadBlC[0], fmat);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    printf("\r                                              ");
    fflush(stdout);
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Impressao dos resultados de tempo
    printf("\n\tCOMPARAR MATRIZ_SeqC c/ MATRIZ_SeqBlC\n\t");
    mcomparar(mmult_MATRIZ_SeqC[0], mmult_MATRIZ_SeqBlC[0]);

    printf("\n\tCOMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadC\n\t");
    mcomparar(mmult_MATRIZ_SeqC[0], mmult_MATRIZ_ThreadC[0]);

    printf("\n\tCOMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadBlC\n\t");
    mcomparar(mmult_MATRIZ_SeqC[0], mmult_MATRIZ_ThreadBlC[0]);

    printf("\n\t***** Tempo médio *****");
    printf("\n\tSequencial (MATRIZ_SeqC):\t\t%.20f\n", tempo_MATRIZ_SeqC / count_for);
    printf("\tEm bloco (MATRIZ_SeqBlC):\t\t%.20f\n", tempo_MATRIZ_SeqBlC / count_for);
    printf("\tThread (MATRIZ_ThreadC):\t\t%.20f\n", MATRIZ_ThreadC / count_for);
    printf("\tThread em bloco (MATRIZ_ThreadBlC):\t%.20f\n", MATRIZ_ThreadBlC / count_for);

    speedup_seqC = (tempo_MATRIZ_SeqC / count_for) / (MATRIZ_ThreadC / count_for);
    speedup_BlC = (tempo_MATRIZ_SeqBlC / count_for) / (MATRIZ_ThreadBlC / count_for);
    printf("\n\tSPEEDUP (MATRIZ_C): \t%.5f (%.2f %c)", speedup_seqC, speedup_seqC * 100, 37);
    printf("\n\tSPEEDUP (MATRIZ_BLC): \t%.5f (%.2f %c)\n\n", speedup_BlC, speedup_BlC * 100, 37);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //Liberação de memória
    mliberar(mmult_MATRIZ_SeqC[0]);
    mliberar(mmult_MATRIZ_SeqBlC[0]);
    mliberar(mmult_MATRIZ_ThreadC[0]);
    mliberar(mmult_MATRIZ_ThreadBlC[0]);

    free(mmult_MATRIZ_SeqC[0]);
    free(mmult_MATRIZ_SeqBlC[0]);
    free(mmult_MATRIZ_ThreadC[0]);
    free(mmult_MATRIZ_ThreadBlC[0]);

    mliberar(&mat_a);
    mliberar(&mat_b);

    free(mmult_MATRIZ_SeqC);
    free(mmult_MATRIZ_SeqBlC);
    free(mmult_MATRIZ_ThreadC);
    free(mmult_MATRIZ_ThreadBlC);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    return 0;
}