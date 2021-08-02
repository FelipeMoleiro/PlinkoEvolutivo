# PlinkoEvolutivo

## Projeto
Projeto feito por:
Felipe Guilermmo Santuche Moleiro - 10724010

Este é um proejeto feito para a disciplina SSC0713 - Sistemas Evolutivos e Aplicados à Robótica.

Neste projeto nós temos um jogo muito parecido com o Pinko, em que se tem bolas sendo jogadas no topo de uma tabua, com varios obstaculos no caminho, e essas bolas colidem com estes obstaculos até atingir algum dos diversos cesto no fundo da tabua. Para ficar mais clara a ideia que deu origem a este projeto basta pesquisar sobre Plinko.

O que fazemos nesse projeto é colocar estes objetos que as bolas irão colidir como retangulos, e temos um problema de otimização, queremos otimizar o angulo de inclinação dessas plataformas para que o score obtido no jogo seja o maximo. Para simplificar a ideia aqui temos uma imagem do problema programado:

COLOCAR IMAGEM

Podemos ver que as bolas caem do topo e interagem com as plataformas até cair em um dos cestos, e cada cesto tem um valor de score. O programa é um algoritmo evolutivo que gera diversos individos(valores de angulos para plataforma), e tenta evoluir eles para achar os melhor valores.

O loop principal é basicamente o seguinte:

inicia os individuos com um valor aleatorio(angulos aleatorios para as plataformas)

enquanto não termina a evolução:

  avalia o score de cada individuo
  
  seleciona o melhor e mistura o gene dele com todos(elitismo)
  
  mutaciona todos os individuos menos o melhor
  
  se estamos a muito tempo sem melhoras aumenta o valor da mutação e se aumentarmos muito sem melhoras mantemos o melhor e reiniciamos todos os outros individuos com valores aleatorios(genocidio e mutação variada)
  
  
Com isso o programa vai encontrando angulos melhores em cada geração para fazer mais pontos. Em geral ele é bem rapido para encontrar otimos locais e a mutação variada e genocidio ajudam ele a destravar de otimos locais, porém para chegar no melhor de todos precisamos de um tempo um pouco maior e um pouco de sorte.

## Dependencias

Instalar GLFW https://www.glfw.org/
Instalar Box2D https://box2d.org/

## Compilar

make all

## Rodar

make run

Basta seguir as instruções do programa, mas basicamente podemos ter input por texto ou com mouse e teclado.

## Apresentação

VIDEO NO YOUTUBE
