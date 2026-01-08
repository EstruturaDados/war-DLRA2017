// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para representar os territórios do jogo
typedef struct {
    char nome[30];
    char cor[20];
    int tropas;
} Territorio;

// --- PROTÓTIPOS DAS FUNÇÕES ---
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao); 
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missao);

int main() {
    // 1. Configurações iniciais
    srand(time(NULL));
    int numTerritorios = 4;
    char* corDoJogador = "Verde";
    
    // Lista de missões disponíveis
    char* bancoDeMissoes[] = {
        "Conquistar 3 territorios",
        "Eliminar as tropas da cor Vermelha",
        "Conquistar o territorio de Madagascar",
        "Ter mais de 10 tropas em um unico territorio",
        "Dominar territorios de duas cores diferentes"
    };

    // 2. Alocação Dinâmica
    Territorio* mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    char* missaoJogador = NULL;

    // Inicializando o mapa
    strcpy(mapa[0].nome, "Brasil");     strcpy(mapa[0].cor, "Verde");    mapa[0].tropas = 10;
    strcpy(mapa[1].nome, "Argentina");  strcpy(mapa[1].cor, "Azul");     mapa[1].tropas = 5;
    strcpy(mapa[2].nome, "Madagascar"); strcpy(mapa[2].cor, "Vermelha"); mapa[2].tropas = 3;
    strcpy(mapa[3].nome, "Egito");      strcpy(mapa[3].cor, "Amarela");  mapa[3].tropas = 4;

    // 3. Atribuição de Missão
    atribuirMissao(&missaoJogador, bancoDeMissoes, 5);

    printf("========================================\n");
    printf("           WAR: MISSOES C               \n");
    printf("========================================\n");
    
    exibirMissao(missaoJogador);

    // 4. Loop de Jogo Dinâmico
    int jogoAtivo = 1;
    while (jogoAtivo) {
        exibirMapa(mapa, numTerritorios);

        int idxAtacante, idxDefensor;
        
        printf("\n--- SEU TURNO (Cor: %s) ---\n", corDoJogador);
        printf("Escolha o ID do SEU territorio para atacar (-1 para sair): ");
        scanf("%d", &idxAtacante);

        if (idxAtacante == -1) break;

        // Validação do Atacante
        if (idxAtacante < 0 || idxAtacante >= numTerritorios || strcmp(mapa[idxAtacante].cor, corDoJogador) != 0) {
            printf("Erro: Escolha um territorio que seja seu!\n");
            continue;
        }
        if (mapa[idxAtacante].tropas <= 1) {
            printf("Erro: Voce precisa de pelo menos 2 tropas para atacar!\n");
            continue;
        }

        printf("Escolha o ID do territorio INIMIGO para o alvo: ");
        scanf("%d", &idxDefensor);

        // Validação do Defensor
        if (idxDefensor < 0 || idxDefensor >= numTerritorios) {
            printf("Erro: ID de territorio invalido!\n");
            continue;
        }

        // Executa o ataque
        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

        // 5. Verificação Silenciosa de Vitória
        if (verificarMissao(missaoJogador, mapa, numTerritorios, corDoJogador)) {
            printf("\n========================================\n");
            printf("!!! VITORIA !!!\n");
            printf("Voce cumpriu sua missao: [%s]\n", missaoJogador);
            printf("========================================\n");
            exibirMapa(mapa, numTerritorios);
            jogoAtivo = 0;
        }
    }

    // 6. Limpeza
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    *destino = (char*)malloc((strlen(missoes[sorteio]) + 1) * sizeof(char));
    if (*destino != NULL) {
        strcpy(*destino, missoes[sorteio]);
    }
}

void exibirMissao(char* missao) {
    printf("SUA MISSAO SECRETA: %s\n", missao);
    printf("Dica: Conquiste territorios inimigos para vencer!\n");
}

void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: Voce nao pode atacar seu proprio territorio!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);
    printf("Ataque jogou: %d | Defesa jogou: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("RESULTADO: Vitoria! %s foi conquistado!\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        // Transferência de tropas: Atacante move metade para o novo território
        int transferidas = atacante->tropas / 2;
        defensor->tropas = transferidas;
        atacante->tropas -= transferidas;
    } else {
        printf("RESULTADO: Derrota! Voce perdeu 1 tropa em %s.\n", atacante->nome);
        atacante->tropas--;
    }
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    int contagemTerritorios = 0;
    
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            contagemTerritorios++;
        }
    }

    // Logica baseada na string da missão
    if (strstr(missao, "3 territorios") && contagemTerritorios >= 3) return 1;
    
    if (strstr(missao, "Madagascar")) {
        for(int i=0; i<tamanho; i++) {
            if(strcmp(mapa[i].nome, "Madagascar") == 0 && strcmp(mapa[i].cor, corJogador) == 0) return 1;
        }
    }

    if (strstr(missao, "Vermelha")) {
        int vermelhasVivas = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0) vermelhasVivas++;
        }
        if (vermelhasVivas == 0) return 1;
    }

    return 0;
}

void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n========== MAPA MUNDI ==========\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d | [%-10s] Cor: %-10s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("================================\n");
}

void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
    printf("\n[Sistema] Memoria liberada. Ate a proxima, General!\n");
}
