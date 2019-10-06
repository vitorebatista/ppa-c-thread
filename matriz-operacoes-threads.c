#include "matrizv3.h"

int multiplicarTh(mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int tid, int ntasks)
{
    int i_max = mat_a->lin;
    int j_max = mat_b->col;
    int k_max = mat_a->col; //ou mat_b->lin
    printf("vai entrarfor");
    for (int i = tid; i < i_max; i += ntasks)
    {
        printf("for");
        for (int j = 0; j < j_max; j++)
        {
            for (int k = 0; k < k_max; k++)
            {
                mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
            }
        }
    }
}

int multiplicarThblocos(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc)
{

}