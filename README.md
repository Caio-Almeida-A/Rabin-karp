# Informações sobre o projeto

Implementações do algoritmo Rabin-karp e comparações da perfomance desse algoritmo de busca de substrings (Rolling Hash) em duas linguagens de programação: C e Python.

# Geração de Gráficos

A visualização dos resultados (comparação das curvas de crescimento) é feita utilizando a biblioteca Matplotlib em Python.


# 🎯 Objetivo

O principal objetivo deste projeto é medir e comparar empiricamente a eficiência e a escalabilidade do Rabin-Karp, testando complexidade de tempo e avaliando a diferença de performance entre uma linguagem de baixo nível (C) e uma de alto nível (Python).

# 🧪 Metodologia

Padrão (M): Fixo em 5 caracteres ("teste").

Texto (N): Testado em 6 diferentes tamanhos, de 1.000 a 600.000 caracteres.

Amostragem: Cada cenário (tamanho de N) foi executado 30 vezes para garantir a estabilidade das medições.

Coleta de Dados: O tempo de execução, a média e o desvio-padrão das 30 repetições são salvos em formato CSV para análise posterior.

