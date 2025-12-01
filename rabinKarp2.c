#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 


#define D 256 
#define Q 1000000007LL 
#define TAMANHO_PADRAO 5 


char* gerar_texto_aleatorio(int N) {
    
    char* texto = (char*)malloc(N + 1);
    if (!texto) return NULL;
    
    
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    
    for (int i = 0; i < N; i++) {
        
        texto[i] = 'a' + (rand() % 26);
    }
    texto[N] = '\0';
    return texto;
}




int search(const char pattern[], const char text[]) {
    long long p = 0; // valor hash do padrão
    long long t = 0; // valor hash da janela de texto
    long long h = 1; // D^(M-1) % Q (fator de remoção)

    int M = (int)strlen(pattern); 
    int N = (int)strlen(text);
    int i, j;

    if (M > N) return -1; 

  
    for (i = 0; i < M - 1; i++) {
        h = (h * D) % Q;
    }

    for (i = 0; i < M; i++) {
         
        p = (D * p + pattern[i]) % Q;
        t = (D * t + text[i]) % Q;
    }

    
    for (i = 0; i <= N - M; i++) {
        if (p == t) {
            for (j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }
            if (j == M) {
                return i; 
            }
        }
        
        if (i < N - M) {
            long long char_out_hash = ((long long)text[i] * h) % Q;
            t = (t - char_out_hash);
            
            t = (t * D + text[i + M]) % Q;
            
            if (t < 0) {
                t = (t + Q);
            }
        }
    }
    return -1;
}


// --- Função Principal ---

int main() {
    FILE *fp;
    int tamanhos_testes[] = {1000, 10000, 100000, 150000, 300000, 600000};
    int num_testes = sizeof(tamanhos_testes) / sizeof(tamanhos_testes[0]);
    // Número de repetições conforme solicitado
    const int repeticoes = 30;
    
    // O padrão fixo
    const char padrao[TAMANHO_PADRAO + 1] = "teste"; 
    
    // Array para armazenar os tempos individuais de cada repetição
    double tempos_individuais[repeticoes];

    // Abre o arquivo CSV para escrita.
    fp = fopen("dados_tempo_c_completo.csv", "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return 1;
    }

    fprintf(fp, "Tamanho do Texto (N),Media de Execucao (ms),Desvio-Padrao (ms),Tempos Individuais (ms)\n");
    
    printf("--- Iniciando Benchmark Rabin-Karp em C ---\n");
    printf("Lista de N: {1000, 10000, 100000, 150000, 300000, 600000}\n");
    printf("Repeticoes por N: %d\n\n", repeticoes);

    for (int i = 0; i < num_testes; i++) {
        int N = tamanhos_testes[i];
        printf("Rodando testes para texto de %d caracteres...\n", N);

        double tempo_total = 0.0;
        
        char* texto = gerar_texto_aleatorio(N);
        
        if (texto == NULL) {
             printf("Erro: Falha ao alocar memoria para N=%d. Encerrando.\n", N);
             break;
        }

        strncpy(texto + (N / 2), padrao, TAMANHO_PADRAO); 
        
        
        for (int r = 0; r < repeticoes; r++) {
            clock_t start = clock();
            
            
            search(padrao, texto);
            
            clock_t end = clock();
            
            
            double tempo_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
            
            
            tempos_individuais[r] = tempo_ms;
            tempo_total += tempo_ms;
        }

        double media_tempo = tempo_total / repeticoes;
        
        double soma_quadrados_diferencas = 0.0;
        for (int r = 0; r < repeticoes; r++) {
            soma_quadrados_diferencas += pow(tempos_individuais[r] - media_tempo, 2);
        }
       
        double variancia = soma_quadrados_diferencas / (repeticoes - 1);
        
        double desvio_padrao = sqrt(variancia);

       
        fprintf(fp, "%d,%.6f,%.6f,", N, media_tempo, desvio_padrao);
        
       
        for (int r = 0; r < repeticoes; r++) {
            fprintf(fp, "%.6f%s", tempos_individuais[r], (r == repeticoes - 1) ? "" : ";");
        }
        fprintf(fp, "\n"); 
        
        free(texto); 
        
        printf("    Media para N=%d: %.6f ms | Desvio-padrao: %.6f ms\n", N, media_tempo, desvio_padrao);
    }

    fclose(fp);
    printf("\n--- Benchmark Concluido ---");
    printf("\nDados de tempo salvos em 'dados_tempo_c_completo.csv'.\n");
    
    return 0;
}