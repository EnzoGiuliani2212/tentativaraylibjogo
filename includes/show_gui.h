#ifndef SHOW_GUI_H
#define SHOW_GUI_H

#include "quiz.h"

typedef enum{JOGO_PERGUNTA, JOGO_ACERTOU, JOGO_ERROU, JOGO_PAROU, JOGO_VENCEU, JOGO_SEM_PERGUNTAS} EstadoJogo;

typedef struct {
    EstadoJogo estado;
    int questaoAtual;
    int sorteada;
    int marco;
    int *usadas;
    char respostaDada;
}Jogo;

void F_Jogo_Iniciar(Jogo *j, Quiz *q, int total);
void F_Jogo_Responder(Jogo *j, Quiz *q, char resp);
void F_Jogo_Proxima(Jogo *j, Quiz *q, int total);
void F_Jogo_Liberar(Jogo *j);

#endif /*SHOW_GUI_H*/