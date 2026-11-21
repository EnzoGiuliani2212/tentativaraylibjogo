/**
 * @file show.c
 * @author Enzo André Santos Tomiotto Giuliani (enzogiuliani.2006@alunos.utfpr.edu.br)
 * @brief Exibe um menu lateral em uma janela criada com raylib.
 * @version 0.1
 * @date 2026-11-21
 * 
 * @copyright Copyright (c) 2026
 * 
 */

/*
 * Enunciado:
 * NÃO TEM
 */

#include "raylib.h"
#include <math.h>
#include<stdio.h>  
#include<stdlib.h>

typedef enum{TELA_MENU, TELA_JOGO, TELA_OPCOES} Tela;

int main(){
    // Configuracao
    const int screenWidth = 800; // Largura da janela
    const int screenHeight = 450; // Altura da janela
    
    InitWindow(screenWidth, screenHeight, "Teste show"); // Inicializa a janela
    SetTargetFPS(60); // Define a taxa de quadros
    SetExitKey(KEY_NULL);//o jogo não fecha com a tecla ESC

    Font customFont = LoadFont("recursos/ArialCEBoldItalic.ttf");
    Texture2D logo = LoadTexture("recursos/logo.png");
    SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);

    //Opções do menu
    const char *Menuop[] = {"Jogar", "Opcoes", "Sair"};
    const int totalop = 3;
    int selecionado = 0;

    //Painel Vermelho do menu
    int PainelRedX = 0;
    int PainelRedY = 0;
    int PainelWidth = 300;
    int PainelHeight = screenHeight;

    //Cálculo do Logo
    float logoWidth = PainelWidth - 40;
    float logoHeight = logoWidth * logo.height / logoWidth;
    int InicioOp = 40 + (int)logoHeight + 40;

    Rectangle origem = {0, 0, (float)logoWidth, (float)logoHeight};
    Rectangle destino = {PainelRedX + (PainelWidth - logoWidth) / 2, 40, logoWidth, logoHeight};

    //Animação das opções
    float escala[3] = {1.0f, 1.0f, 1.0f};
    float deslocamento[3] = {0.0f, 0.0f, 0.0f};

    //Funcionamento do menu
    Tela telaAtual = TELA_MENU;
    bool sair = false;

    // Loop principal
    while (!WindowShouldClose() && !sair){
        // Acoes e desenhos acontecem aqui dentro
        float pulso;

        switch (telaAtual){
        case TELA_MENU:
            
            if (IsKeyPressed(KEY_DOWN)){
                selecionado++;
                if(selecionado >= totalop){
                    selecionado = 0;
                }
            }
            if (IsKeyPressed(KEY_UP)){
                selecionado--;
                if(selecionado < 0){
                    selecionado = totalop - 1;
                }
            }

            float dt = GetFrameTime();
            for (int i = 0; i < totalop; i++){
                float alvoEscala = (i == selecionado) ? 1.3f : 1.0f;
                float alvoDesloc = (i == selecionado) ? 20.0f : 0.0f;

                escala[i] += (alvoEscala - escala[i]) * 10.0f * dt;
                deslocamento[i] += (alvoDesloc - deslocamento[i]) * 10.0f * dt;
            }
            pulso = 1.0f + sinf((float)GetTime() * 5.0f) * 0.05f;

            if (IsKeyPressed(KEY_ENTER)){
                if (selecionado == 0){
                    telaAtual = TELA_JOGO;
                }
                if (selecionado == 1){
                    telaAtual = TELA_OPCOES;
                }
                if (selecionado == 2){
                    sair = true;
                }
                
            }
            break;
        case TELA_JOGO:
            if (IsKeyPressed(KEY_ESCAPE)){
                telaAtual = TELA_MENU;
            }
            break;
        case TELA_OPCOES:
            if (IsKeyPressed(KEY_ESCAPE)){
                telaAtual = TELA_MENU;
            }
            break;
        }
        
        BeginDrawing();
            switch (telaAtual){
            case TELA_MENU:
                ClearBackground(BLACK);//background

                //Painel vermelho do menu
                DrawRectangle(PainelRedX, PainelRedY, PainelWidth, PainelHeight, RED);

                //Logo do show do milhao
                DrawTexturePro(logo, origem, destino, (Vector2){0,0}, 0.0f, WHITE);

                //Opções que tem no menu
                for (int i = 0; i < totalop; i++){
                    float fator = escala[i] * ((i == selecionado) ? pulso : 1.0f);
                    int tamFonte = (int)(30 * fator);
                    Vector2 tam = MeasureTextEx(customFont, Menuop[i], tamFonte, 2);

                    float X = PainelRedX + (PainelWidth - tam.x)/2 + (int)deslocamento[i];
                    int Y = InicioOp + i * 50 - (tamFonte - 30) / 2;

                    Color cor = (i == selecionado) ? YELLOW : WHITE;
                    DrawTextEx(customFont, Menuop[i], (Vector2){X,Y}, 30, 2, cor);
                }
                break;
            case TELA_JOGO:
                ClearBackground(DARKBLUE);
                DrawText("TELA DO JOGO", 250, 200, 40, WHITE);
                DrawText("Pressione ESC para voltar", 280, 260, 20, LIGHTGRAY);
                break;
            case TELA_OPCOES:
                ClearBackground(DARKGREEN);
                DrawText("OPÇÕES", 300, 200, 40, WHITE);
                DrawText("Pressione ESC para voltar", 280, 260, 20, LIGHTGRAY);
                break;
            }
            
        EndDrawing();
    }

    //Limpeza de dados
    UnloadTexture(logo);
    UnloadFont(customFont);
    // Finalizacao
    CloseWindow(); // Fecha a janela
    return 0;
}