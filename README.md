# PlinkoEvolutivo

## Projeto
Projeto feito por:

Felipe Guilermmo Santuche Moleiro - 10724010

Este é um proejeto feito para a disciplina SSC0713 - Sistemas Evolutivos e Aplicados à Robótica.

Neste projeto nós temos um jogo muito parecido com o Pinko, em que se tem bolas sendo jogadas no topo de uma tabua, com varios obstaculos no caminho, e essas bolas colidem com estes obstaculos até atingir algum dos diversos cestos no fundo da tabua. Para ficar mais clara a ideia que deu origem a este projeto basta pesquisar sobre o jogo Plinko.

O que fazemos nesse projeto é modificar estes objetos que as bolas irão colidir para retangulos, e variando o angulo deles, então temos um problema de otimização, queremos otimizar o angulo de inclinação dessas plataformas para que o score obtido no jogo seja o maximo. Para simplificar a ideia aqui temos uma imagem do problema programado:

![PlinkoGif](https://github.com/FelipeMoleiro/PlinkoEvolutivo/blob/main/plinko.gif?raw=true)

Podemos ver que as bolas caem do topo e interagem com as plataformas até cair em um dos cestos, e cada cesto tem um valor de score. O programa é um algoritmo evolutivo que gera diversos individos(valores de angulos para plataforma), e tenta evoluir eles para achar os melhor valores.

O loop principal é basicamente o seguinte:

	-inicia os individuos com um valor aleatorio(angulos aleatorios para as plataformas)

	Enquanto não termina a evolução:

		-avalia o score de cada individuo
  
		-seleciona o melhor e mistura o gene dele com todos(elitismo)
  
		-mutaciona todos os individuos menos o melhor
  
		-se estamos a muito tempo sem melhoras, aumenta o valor da mutação. Se aumentarmos muito este valor sem melhoras, nós mantemos o melhor e reiniciamos todos os outros individuos com valores aleatorios(genocidio e mutação variada)
  
  
Com isso o programa vai encontrando angulos melhores a cada geração para fazer mais pontos. Em geral ele é bem rapido para encontrar otimos locais e a mutação variada e genocidio ajudam ele a destravar destes otimos locais, porém para chegar no melhor de todos precisamos de um tempo um pouco maior e um pouco de sorte.

## Grafico Da Evolição Do Sistema

![EvoluçãoGrafico](https://github.com/FelipeMoleiro/PlinkoEvolutivo/blob/main/Scores%20Individuos.png?raw=true)

Este Grafico diz respeito a simulação exemplo em simulations/ex1.in, que é a mesma do gif anterior. Neste caso o score de 400 atingido é o score maximo global, ou seja, todas as bolinhas estão caindo no cesto de maior score.

## Dependencias

Instalar GLFW https://www.glfw.org/

Instalar Glew http://glew.sourceforge.net/

Instalar Box2D https://box2d.org/

Instalar Glm https://github.com/g-truc/glm

Em debian/ubuntu:

	sudo apt install libglfw3-dev libbox2d-dev libglew-dev libglm-dev

Instalando pela linha de comando voce não deve ter problemas, entretanto se tiver algum problema com a livraria box2d, verifique se no seu computador na pasta /usr/include como está capitalizado a livraria box2d, por padrão ao instalar a livraria pelo apt install ela vem como Box2D, entretanto, se compilar do codigo fonte do site ela vem como box2d. Caso voce esteja com a livraria em minusculo basta editar o arquivo gamesimulation.cpp no #include de <Box2D/Box2D.h> para <box2d/box2d.h> e no makefile de -lBox2D para -lbox2d.

## Compilar

	make all

## Rodar

	make run

Basta seguir as instruções do programa, mas basicamente podemos ter input por texto ou com mouse e teclado.

## Apresentação

https://youtu.be/7_-23916xaE
