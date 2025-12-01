import random
import string
import time
import statistics
import matplotlib.pyplot as plt



def rabin_karp(texto_completo, padrao_busca):
    tamanho_texto = len(texto_completo)
    tamanho_padrao = len(padrao_busca)

    if tamanho_padrao > tamanho_texto:
        return -1

    base_hash = 256
    modulo_hash = 10**9 + 7 

    hash_padrao = 0
    hash_janela = 0

    for i in range(tamanho_padrao):
        hash_padrao = (hash_padrao * base_hash + ord(padrao_busca[i])) % modulo_hash
        hash_janela = (hash_janela * base_hash + ord(texto_completo[i])) % modulo_hash

    fator_remocao = pow(base_hash, tamanho_padrao - 1, modulo_hash)

    for inicio in range(tamanho_texto - tamanho_padrao + 1):

        if hash_padrao == hash_janela:
            if texto_completo[inicio : inicio + tamanho_padrao] == padrao_busca:
                return inicio  

        if inicio < tamanho_texto - tamanho_padrao:
            hash_janela = (hash_janela - ord(texto_completo[inicio]) * fator_remocao) % modulo_hash

            hash_janela = (hash_janela * base_hash) % modulo_hash

            hash_janela = (hash_janela + ord(texto_completo[inicio + tamanho_padrao])) % modulo_hash

            hash_janela = (hash_janela + modulo_hash) % modulo_hash

    return -1


def gerar_texto_aleatorio(qtd_caracteres):
    return ''.join(random.choices(string.ascii_lowercase, k=qtd_caracteres))


def rodar_benchmark(tamanhos_texto, tamanho_padrao=5, repeticoes=30):
    resultados = {}

    for tamanho in tamanhos_texto:
        print(f"\nRodando testes para texto de {tamanho} caracteres")

        texto_gerado = gerar_texto_aleatorio(tamanho)
        padrao_gerado = gerar_texto_aleatorio(tamanho_padrao)

        tempos_execucao = []

        for _ in range(repeticoes):
            inicio = time.perf_counter()
            rabin_karp(texto_gerado, padrao_gerado)
            fim = time.perf_counter()

            tempos_execucao.append((fim - inicio) * 1000)  

        media_tempo = statistics.mean(tempos_execucao)
        desvio_tempo = statistics.stdev(tempos_execucao)

        resultados[tamanho] = {
            "media_ms": media_tempo,
            "desvio_ms": desvio_tempo,
            "tempos": tempos_execucao
        }

        print(f"Média: {media_tempo:.4f} ms | Desvio-padrão: {desvio_tempo:.4f} ms")

    return resultados


def gerar_grafico(resultados):
    tamanhos = list(resultados.keys())
    medias = [resultados[t]["media_ms"] for t in tamanhos]
    desvios = [resultados[t]["desvio_ms"] for t in tamanhos]

    plt.figure(figsize=(10, 6))
    plt.errorbar(tamanhos, medias, yerr=desvios, fmt='-o', capsize=5)
    plt.title("Tempo Médio de Execução do Algoritmo Rabin–Karp")
    plt.xlabel("Tamanho do Texto")
    plt.ylabel("Tempo (ms)")
    plt.grid(True)
    plt.savefig("grafico_rabin_karp.png")
    plt.show()

    print("\nGráfico salvo como: grafico_rabin_karp.png")


if __name__ == "__main__":
    tamanhos_testes = [1000, 10000, 100000, 150000, 300000, 600000]

    resultados = rodar_benchmark(
        tamanhos_texto=tamanhos_testes,
        tamanho_padrao=5,
        repeticoes=30
    )

    gerar_grafico(resultados)
