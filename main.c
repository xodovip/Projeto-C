#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTOQUE 100
#define TAMANHO_NOME 50
#define ARQUIVO "estoque.txt"

// Estrutura para representar um perfume
typedef struct {
    char nome[TAMANHO_NOME];
    int quantidade;
    float tamanho; // Em mL
} Perfume;

// Estrutura para a fila
typedef struct {
    Perfume perfumes[MAX_ESTOQUE];
    int frente;
    int tras;
} Fila;

// Funções para manipulação da fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = 0;
}

int filaCheia(Fila *fila) {
    return (fila->tras + 1) % MAX_ESTOQUE == fila->frente;
}

int filaVazia(Fila *fila) {
    return fila->frente == fila->tras;
}

void adicionarPerfume(Fila *fila, Perfume perfume) {
    if (filaCheia(fila)) {
        printf("O estoque está cheio! Não é possível adicionar mais perfumes.\n");
        return;
    }
    fila->perfumes[fila->tras] = perfume;
    fila->tras = (fila->tras + 1) % MAX_ESTOQUE;
    printf("Perfume adicionado com sucesso!\n");
}

void removerPerfume(Fila *fila) {
    if (filaVazia(fila)) {
        printf("O estoque está vazio! Não há perfumes para remover.\n");
        return;
    }
    Perfume removido = fila->perfumes[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_ESTOQUE;
    printf("Perfume removido: %s (Quantidade: %d, Tamanho: %.2f mL)\n", 
           removido.nome, removido.quantidade, removido.tamanho);
}

void exibirEstoque(Fila *fila) {
    if (filaVazia(fila)) {
        printf("O estoque está vazio! Nenhum perfume para exibir.\n");
        return;
    }
    printf("\nEstoque de Perfumes:\n");
    int i = fila->frente;
    while (i != fila->tras) {
        Perfume p = fila->perfumes[i];
        printf("- Nome: %s | Quantidade: %d | Tamanho: %.2f mL\n", p.nome, p.quantidade, p.tamanho);
        i = (i + 1) % MAX_ESTOQUE;
    }
}

void salvarNoArquivo(Fila *fila) {
    FILE *arquivo = fopen(ARQUIVO, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    int i = fila->frente;
    while (i != fila->tras) {
        Perfume p = fila->perfumes[i];
        fprintf(arquivo, "%s;%d;%.2f\n", p.nome, p.quantidade, p.tamanho);
        i = (i + 1) % MAX_ESTOQUE;
    }
    fclose(arquivo);
    printf("Estoque salvo no arquivo com sucesso!\n");
}

void carregarDoArquivo(Fila *fila) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (!arquivo) {
        printf("Nenhum arquivo encontrado. Um novo será criado ao salvar.\n");
        return;
    }
    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Perfume p;
        sscanf(linha, "%[^;];%d;%f", p.nome, &p.quantidade, &p.tamanho);
        adicionarPerfume(fila, p);
    }
    fclose(arquivo);
    printf("Estoque carregado do arquivo com sucesso!\n");
}

// Função principal
int main() {
    Fila estoque;
    inicializarFila(&estoque);

    carregarDoArquivo(&estoque);

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Adicionar Perfume\n");
        printf("2. Remover Perfume\n");
        printf("3. Exibir Estoque\n");
        printf("4. Salvar Estoque\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        if (opcao == 1) {
            Perfume p;
            printf("Digite o nome do perfume: ");
            fgets(p.nome, TAMANHO_NOME, stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0'; // Remove o '\n'

            printf("Digite a quantidade: ");
            scanf("%d", &p.quantidade);

            printf("Digite o tamanho (em mL): ");
            scanf("%f", &p.tamanho);
            getchar(); // Limpa o buffer do teclado

            adicionarPerfume(&estoque, p);
        } else if (opcao == 2) {
            removerPerfume(&estoque);
        } else if (opcao == 3) {
            exibirEstoque(&estoque);
        } else if (opcao == 4) {
            salvarNoArquivo(&estoque);
        } else if (opcao == 5) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}

