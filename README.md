# Relatório - Projeto de Implementação

Integrantes: André de Lima Rocha, Douglas de Almeida Nicida Soares, Enzo Tinelli Lopes e Victor Hugo Maquiné Martins

O algoritmo de Floyd-Warshall é uma técnica clássica em Ciência da Computação utilizada para encontrar os caminhos entre todos os pares de vértices em um grafo orientado e ponderado. Ele atualiza iterativamente a matriz de adjacência do grafo, onde a cada iteração, o algoritmo considera um vértice intermediário e verifica se um caminho que passa por esse vértice pode possuir uma distância menor entre outro par de vértices.
No algoritmo, vimos a necessidade de paralelizar o trecho de código que possui 3 laços aninhados que iteram sobre a matriz utilizando o grafo. Este trecho possui a matriz de adjacência e um intermediário chamado k para verificar se existe um caminho entre dois nós do grafo. No laço mais interno ocorre a atualização do valor dentro da matriz de adjacência. <br><br>
Realizamos a paralelização atribuindo as diretivas collapse(3) que possui como objetivo otimizar a distribuição entre as threads ao unir em um bloco maior aquilo que seriam blocos menores, juntamente com shared(graph, nNodes)  para que esses dois valores passados por parâmetro sejam compartilhados entre as threads.<br><br>
No laço mais interno ocorre a atualização dos valores da matriz de adjacências e eventualmente essa operação poderia gerar uma condição de corrida, caso 1 ou mais threads tentassem acessar e modificar o mesmo endereço de memória com o uso do mesmo i e j. Para evitar isso, passamos juntamente com o omp parallel for, a diretiva private(i,j,k), para que a seção crítica seja acessada e modificada por apenas 1 thread por vez.
<br>
<br>
A seguir estão os resultados da paralelização, seguido de sua respectiva média em comparação com a versão sequencial, também com sua respectiva média.

## Versão Paralelizada:
- 5.145480 segundos
- 4.966749 segundos
- 4.939093 segundos
- 5.172574 segundos
- 5.215553 segundos
- 4.993084 segundos
- 4.830074 segundos
- 5.423492 segundos
- 4.924347 segundos
- 5.040458 segundos
<br>
A média dos tempos é de aproximadamente <strong>5,0650904 segundos</strong>.

## Versão Sequencial:
- 181.184708 segundos
- 179.686234 segundos
- 180.720963 segundos
- 172.414093 segundos
- 171.646515 segundos
- 171.702301 segundos
- 177.869156 segundos
- 176.113388 segundos
- 174.386734 segundos
- 170.657761 segundos
<br>
A média dos tempos é de aproximadamente <strong>175,6381853 segundos</strong>.

### Comparação final:
Ao compararmos as médias dos resultados obtidos, pode-se perceber uma melhora de aproximadamente ```170,5730949``` segundos, ou seja, um ```speedup de 41``` e uma ```melhora de aproximadamente 97,116%```.
