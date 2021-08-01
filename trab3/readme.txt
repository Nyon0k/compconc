Link planilha -> https://docs.google.com/spreadsheets/d/1atuYkV8vybGUmnNwvo0YsrFzGLhohI2XPyQhCdc9ziE/edit?usp=sharing

Enfatizando os resultados obtidos no estudo da planilha dos resultados, temos:

- Caso 10^5:
	- 1 thread:
		O resultado do tempo médio sequencial foi 0,000154 e o tempo médio concorrente foi 0,000685, o que não resultou em uma aceleração mas sim um atraso, conforme esperado. Seu valor Tmedioseq/Tmedioconc foi 0,2248175182.

	- 2 threads:
		O Tmedio sequencial foi de 0,0001584 e Tmedio concorrente foi 0,0006532, o qual também não teve um ganho e sim uma perda no tempo. Tmedioseq/medioConc = 0,2424984691

	- 4 threads:
		Tmedio sequencial = 0,0001552 e Tmedio concorrente = 0,000643. Não teve uma melhoria em relação aos anteriores. Tmedioseq/Tmedioconc = 0,2413685848

- Caso 10^7:
	- 1 thread:
		Tmedio sequencial = 0,0158088 e Tmedio concorrente = 0,0161784. Tmedioseq/Tmedioconc = 0,9771547248. Neste caso teve uma melhoria significativa em relação aos casos anterior, mesmo que ainda não tenha havido um ganho de tempo.

	- 2 threads:
		Tmedioseq = 0,0706646 e Tmedioconc = 0,0088462. Tmedioseq/Tmedioconc = 7,988130497. Agora ele finalmente teve um ganho significativo. De todos os teste que foram feitos esse caso foi o que rendeu mais tempo de execução.

	- 4 threads:
		Tmedioseq = 0,0156634 e Tmedioconc = 0,0046646. Tmedioseq/Tmedioconc = 3,35792994. Como esperado, também teve um ganho de tempo.

- Caso 10^9:
	- 1 thread:
		Tmedioseq = 1,6389094 e Tmedioconc = 1,55996096. Tmedioseq/Tmedioconc = 1,050609241. Nesse caso, mesmo sendo uma thread, ele conseguiu ser mais rápido na execução.

	- 2 threads:
		Tmedioseq = 1,6685032 e Tmedioconc = 0,922327. Tmedioseq/Tmedioconc = 1,809014807. Aqui a aceleração foi significativa em relação ao caso anterior.

	- 4 threads:
		Tmedioseq = 1,689623 e Tmedioconc = 0,5240878. Tmedioseq/Tmedioconc = 3,223931181. Por fim esse caso teve uma aceleração muito boa levando em consideração a dimensão da entrada.

