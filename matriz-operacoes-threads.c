#include "matrizv3.h"

int multiplicarTh(mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int tid, int ntasks)
{
    printf("\nchamei multiplicharTh\n");

    //inicializa variáveis de controle dos for`s
    int i_max = mat_a->lin;
    int j_max = mat_b->col;
    int k_max = mat_a->col;

    for (int i = tid; i < i_max; i += ntasks)
    {
        //printf("\nfor tid\n");
        for (int k = 0; k < k_max; k++)
        {
            for (int j = 0; j < j_max; j++)
            {
                //printf("\nfor k\n");
                mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                //printf("mat_c->matriz[%d][%d] = %d, calculado por thread %d\n", i, j, mat_c->matriz[i][j], tid);
            }
        }
    }

    return 0;
}

int multiplicarThblocos(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc)
{
    return 0;
}