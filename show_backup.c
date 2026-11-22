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
#include "quiz.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define TOTAL_OPCOES 3
#define PAINEL_WIDTH 300

typedef enum{TELA_MENU, TELA_JOGO, TELA_OPCOES} Tela;

//Recursos que são carregdos no programa
typedef struct {
    Music musicaMenu;
    Sound Somselecao;
    Font FonteMenu;
    Font FonteJogo;
    Texture2D logo;
    Texture2D FundoMenu;
    Texture2D FundoJogo;
}Recursos;

//Layout do menu
typedef struct {
    int PainelX;
    int PainelY;
    int Painelwidth;
    int Painelheigth;
    float LogoWidth;
    float LogoHeitgh;
    int InicioOP;
    Rectangle origem;
    Rectangle destino;
}Layout;

//Estado do menu -> Animações e seleção
typedef struct {
    int selecionado;
    float escala[TOTAL_OPCOES];
    float deslocamento[TOTAL_OPCOES];
    float pulso;
}Menu;

const char *MenuOP[TOTAL_OPCOES] = {"Jogar", "Opcoes", "Sair"};

//Função do Load de recursos
Recursos LoadRecursos(void){
    Recursos r;
    int codepoints[224];

    //Música do menu:
    r.musicaMenu = LoadMusicStream("recursos/Musicatema.wav");
    r.musicaMenu.looping = true;
    SetMusicVolume(r.musicaMenu, 0.8f);
    PlayMusicStream(r.musicaMenu);

    //Som de selecionar:
    r.Somselecao = LoadSound("recursos/Menuselect.mp3");
    SetSoundVolume(r.Somselecao, 0.7f);

    //Fonte e logo
    r.FonteMenu = LoadFont("recursos/ArialCEBoldItalic.ttf");
    for(int c = 32; c <= 255; c++){
        codepoints[c - 32] = c;
    }
    r.FonteJogo = LoadFontEx("recursos/ArialCEBoldItalic.ttf", 32, codepoints, 224);
    SetTextureFilter(r.FonteJogo.texture, TEXTURE_FILTER_BILINEAR);
    r.logo = LoadTexture("recursos/logo.png");
    SetTextureFilter(r.logo, TEXTURE_FILTER_BILINEAR);

    //Fundos: Menu e Jogo
    r.FundoMenu = LoadTexture("recursos/FundoJogo.png");
    SetTextureFilter(r.FundoMenu,TEXTURE_FILTER_BILINEAR);
    r.FundoJogo = LoadTexture("recursos/FundoJogo.png");
    SetTextureFilter(r.FundoJogo, TEXTURE_FILTER_BILINEAR);

    return r;
}

//Função do Unload de recursos
void UnloadRecursos(Recursos *r){
    UnloadTexture(r->logo);
    UnloadTexture(r->FundoMenu);
    UnloadTexture(r->FundoJogo);
    UnloadFont(r->FonteMenu);
    UnloadFont(r->FonteJogo);
    UnloadMusicStream(r->musicaMenu);
    UnloadSound(r->Somselecao);
}

//Inicialização do layout e do menu
Layout CalcularLayout(const Recursos *r){
    Layout l;

    //Dados do painel vermelho do menu:
    l.PainelX = 0;
    l.PainelY = 0;
    l.Painelwidth = PAINEL_WIDTH;
    l.Painelheigth = SCREEN_HEIGHT;

    //Dados do Logo
    l.LogoWidth = l.Painelwidth - 40;
    l.LogoHeitgh = l.LogoWidth * r->logo.height/r->logo.width;
    l.InicioOP = 40 + (int)l.LogoHeitgh + 40;
    l.origem = (Rectangle){0, 0, (float)l.LogoWidth, (float)l.LogoHeitgh};
    l.destino = (Rectangle){l.PainelX + (l.Painelwidth - l.LogoWidth)/2, 40, l.LogoWidth, l.LogoHeitgh};
    
    return l;
}

Menu criarMenu(void){
    Menu m;
    m.selecionado = 0;
    m.pulso = 1.0f;
    for (int i = 0; i < TOTAL_OPCOES; i++){
        m.escala[i] = 1.0f;
        m.deslocamento[i] = 0.0f;
    }
    
    return m;
}

//Atualização do menu
void atualMenu(Menu *m, Tela *telaAtual, bool *sair, const Recursos *r){
    //Navegação entre as opções
    if (IsKeyPressed(KEY_DOWN)){
        m->selecionado++;
        if (m->selecionado >= TOTAL_OPCOES){
            m->selecionado = 0;
        }
    }
    if (IsKeyPressed(KEY_UP)){
        m->selecionado--;
        if (m->selecionado < 0){
            m->selecionado = TOTAL_OPCOES - 1;
        }
    }
    
    //Animação das opções
    float dt = GetFrameTime();
    for (int i = 0; i < TOTAL_OPCOES; i++){
        float alvoEscala = (i == m->selecionado) ? 1.3f : 1.0f;
        float alvoDesloc = (i == m->selecionado) ? 20.0f : 0.0f;

        m->escala[i] += (alvoEscala - m->escala[i]) * 10.0f * dt; 
        m->deslocamento[i] += (alvoDesloc - m->deslocamento[i]) * 10.0f * dt;
    }
    m->pulso = 1.0f + sinf((float)GetTime() * 5.0f) * 0.05f;

    //Confirmar a opção
    if (IsKeyPressed(KEY_ENTER)){
        if (m->selecionado == 0){
            *telaAtual = TELA_JOGO;
            PlaySound(r->Somselecao);
            PauseMusicStream(r->musicaMenu);
        }
        if (m->selecionado == 1){
            *telaAtual = TELA_OPCOES;
            PlaySound(r->Somselecao);
        }
        if (m->selecionado == 2){
            *sair = true;
        }
        
    }
    
}

void atualJogo(Tela *telaAtual, const Recursos *r){
    if (IsKeyPressed(KEY_ESCAPE)){
        *telaAtual = TELA_MENU;
        ResumeMusicStream(r->musicaMenu);
    }
}

void atualOpcoes(Tela *telaAtual){
    if (IsKeyPressed(KEY_ESCAPE)){
        *telaAtual = TELA_MENU;
    }
}

//Desenhos da tela: Menu, Opções e Jogo
void DesenhoMenu(const Menu *m, const Recursos *r, const Layout *l){
    ClearBackground(BLACK);
    
    //Fundo do show do milhão
    DrawTexturePro(r->FundoMenu, 
    (Rectangle){0, 0, (float)r->FundoMenu.width, (float)r->FundoMenu.height},
    (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
    (Vector2){0, 0}, 0.0f, WHITE);
    
    //Painel vermelho do menu
    DrawRectangle(l->PainelX, l->PainelY, l->Painelwidth, l->Painelheigth, RED);

    //Logo do show do milhão
    DrawTexturePro(r->logo, l->origem, l->destino, (Vector2){0, 0}, 0.0f, WHITE);

    //Opções que tem no menu
    for (int i = 0; i < TOTAL_OPCOES; i++){
        float fator = m->escala[i] * ((i == m->selecionado) ? m->pulso : 1.0f);
        int tamFonte = (int)(30 * fator);
        Vector2 tam = MeasureTextEx(r->FonteMenu, MenuOP[i], tamFonte, 2);

        float X = l->PainelX + (l->Painelwidth - tam.x) / 2 + (int)m->deslocamento[i];
        float Y = l->InicioOP + i * 50 - (tamFonte - 30)/2;

        Color cor = (i == m->selecionado) ? YELLOW : WHITE;
        DrawTextEx(r->FonteMenu, MenuOP[i], (Vector2){X, Y}, 30, 2, cor);
    }
}

void DesenhoJogo(const Recursos *r, int totalPerguntas){
    ClearBackground(DARKBLUE);

    DrawTexturePro(r->FundoJogo,
    (Rectangle){0, 0, (float)r->FundoJogo.width, (float)r->FundoJogo.height},
    (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
    (Vector2){0, 0}, 0.0f, WHITE);

    DrawText("TELA JOGO", 250, 200, 40, WHITE);
    DrawText(TextFormat("Perguntas Carregadas: %d", totalPerguntas), 250, 250, 20, YELLOW);
    DrawText("Pressione ESC para voltar ao menu", 280, 290, 20, LIGHTGRAY);
}

void Desenhoopcoes(void){   
    ClearBackground(DARKGRAY);
    DrawText("OPÇÕES", 300, 200, 40, WHITE);
    DrawText("Pressione ESC para voltar ao menu", 280, 260, 20, WHITE);

}

//main
int main(){
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Teste show"); // Inicializa a janela
    SetTargetFPS(60); // Define a taxa de quadros
    SetExitKey(KEY_NULL);//o jogo não fecha com a tecla ESC
    InitAudioDevice();

    //Recursos, Layout e estado inicial:
    Recursos recursos = LoadRecursos();
    Layout layout = CalcularLayout(&recursos);
    Menu menu = criarMenu();

    FILE *arqPerguntas = NULL;
    int totalPerguntas = 0;
    Quiz *perguntas = F_Inicializa_Sistema(&arqPerguntas, &totalPerguntas);

    Tela telaAtual = TELA_MENU;
    bool sair = false;

    // Loop principal
    while (!WindowShouldClose() && !sair){
        // Acoes e desenhos acontecem aqui dentro

        //Música que toca no menu principal
        UpdateMusicStream(recursos.musicaMenu);

        switch (telaAtual){
        case TELA_MENU:
            atualMenu(&menu, &telaAtual, &sair, &recursos);
            break;
        case TELA_JOGO:
            atualJogo(&telaAtual, &recursos);
            break;
        case TELA_OPCOES:
            atualOpcoes(&telaAtual);
            break;
        }
        
        BeginDrawing();
            switch (telaAtual){
            case TELA_MENU:
                DesenhoMenu(&menu, &recursos, &layout);
                break;
            case TELA_JOGO:
                DesenhoJogo(&recursos, totalPerguntas);
                break;
            case TELA_OPCOES:
                Desenhoopcoes();
                break;
            }
            
        EndDrawing();
    }

    //Limpeza de dados
    free(perguntas);
    UnloadRecursos(&recursos);
    // Finalizacao
    CloseAudioDevice();
    CloseWindow(); // Fecha a janela
    return 0;
}