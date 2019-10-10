# Matriz Multithread

Este programa em sua execução realiza operações básicas de multiplição de estruturas matriciais de quatro diferentes maneiras: sequencial, em blocos, multi-thread e multi-thread em blocos. 

Estão presentes nos códigos funções para alocação de memória, geração e preenchimento de todas as posições das matrizes, liberação da memória, etc.

## Compilação

Estando dentro do diretório correto basta executar o *Makefile*.

> $ make

Após execução deste comando os códigos serão compilados e gerados os executáveis e binarios.

## Excecução

Deve-se gerar os arquivos com as matrizes para realizar os testes:

> ./gmat 500 500

Em seguida chamar o programa principal que realizará os devidos testes e apresentará os resultados obtidos.

> ./main_thread 500x500-mat.map 500x500-mat.map

Será apresentado no terminal um retorno semelhante a este:

```
        COMPARAR MATRIZ_SeqC c/ MATRIZ_SeqBlC
        Matrizes são idênticas!! :) 

        COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadC
        Matrizes são idênticas!! :) 

        COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadBlC
        Matrizes são idênticas!! :) 

        ***** Tempo médio *****
        Sequencial (MATRIZ_SeqC):               0.09926869869232177179
        Em bloco (MATRIZ_SeqBlC):               0.33803539276123045765
        Thread (MATRIZ_ThreadC):                0.03931689262390136719
        Thread em bloco (MATRIZ_ThreadBlC):     0.12550466060638426624

        SPEEDUP (MATRIZ_C):     2.52484 (252.48 %)
        SPEEDUP (MATRIZ_BLC):   2.69341 (269.34 %)
```