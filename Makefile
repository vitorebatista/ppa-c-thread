# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS = -pthread
TARGET = main_thread gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

main_thread: main_thread.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-threads.o
			$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o matriz-operacoes-threads.o main_thread.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./gmat 10 5"
	@echo "./gmat 5 10"
	@echo "./main_thread 10x5-mat.map 5x10-mat.map"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
