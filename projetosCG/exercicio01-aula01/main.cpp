#include <iostream>
#include "PGM.hpp"

using namespace std;

/*
Exercício 4:
4. Crie uma função para criar uma imagem. A função deve conter os seguintes parâmetros:
void criar(PGM *pgm, int largura, int altura, unsigned char corFundo);
*/
void criarComCorDeFundo(PGM *pgm, int largura, int altura, unsigned char corFundo){
	if(pgm->pixels)
		delete pgm->pixels;

	int tamanho = largura * altura;
	pgm->tipo = "P2";
	pgm->larg = largura;
	pgm->alt = altura;
	pgm->vmax = 255;
	pgm->pixels = new unsigned char[tamanho];

	for(int i = 0; i < tamanho; i++){
		pgm->pixels[i] = corFundo;
	}
}
/*
Exercício 5
5. Crie uma função para preencher uma determinada linha de uma imagem PGM com uma 
cor específica (branco, cinza ou preto).
void setLinha(PGM *pgm, int linha, unsigned char cor);
(y*largura + x)

LarguraxAltura
x, y


*/
void setLinha(PGM *pgm, int linha, unsigned char cor){
	for(int i = 0; i < pgm->larg; i++){
		pgm->pixels[linha*pgm->larg + i] = cor;

	}

}
/*
Crie uma função para verificar se as coordenadas de um pixel passadas por parâmetro 
são válidas.
bool coordValida(PGM *pgm, int x, int y);
*/
bool coordValida(PGM *pgm, int x, int y){
	if(x < 0 || y < 0){
		return false;
	}
	if(x >= pgm->larg ){
		return false;
	}
	if( y >= pgm->alt){
		return false;
	}
	return true;
}

/*
7. Crie uma função para preencher uma região de uma imagem PGM com uma 
determinada cor (branco, cinza ou preto). A região é definida pelas coordenadas P1(x1, 
y1) e P2(x2,y2). 

(y*largura + x)

LarguraxAltura
x, y
*/
void preencheRegiao(PGM *pgm, int x1, int y1, int x2, int y2, unsigned char  cor){
		for(int i = x1; i <= x2; i++){
			for(int j = y1; j <= y2; j++){
				setPixel(pgm, i, j, cor);
			}
		}
}

/*
Exercício 8
8. Crie uma imagem PGM de 500x500 pixels com fundo preto (colocar o valor zero para 
todos os pixels). Em seguida, defina uma linha branca a cada 50 linhas da imagem.
*/
void linhaDe50Em50(PGM *pgm){
	for(int i = 0; i < pgm->alt; i+=50){
		setLinha(pgm, i, 255);

	}

}

void exercicio8(){
	PGM img;
	cout << "#Exercicio 8#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 500, 500, 0);
linhaDe50Em50(&img);
	imprimir(&img);

	gravar(&img, "exercicio8.pgm");
	destruir(&img);
	imprimir(&img);
}

void exercicio7(){
	PGM img;
	cout << "#Exercicio 7#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 700, 500, 0);
	preencheRegiao(&img, 150, 50, 550, 450, 255);

	imprimir(&img);

	gravar(&img, "exercicio7.pgm");
	destruir(&img);
	imprimir(&img);
}
void exercicio6(){
	PGM img;
	cout << "#Exercicio 6#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 700, 500, 0);
	imprimir(&img);
	cout << (coordValida(&img, 900, 100) ? "valido " : "invalido") << endl;
	setLinha(&img, 2, 255);
	gravar(&img, "exercicio6.pgm");
	destruir(&img);
	imprimir(&img);
}
void exercicio5(){
		PGM img;
	cout << "#Exercicio 5#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 700, 500, 0);
	imprimir(&img);
	setLinha(&img, 2, 255);
	gravar(&img, "exercicio5.pgm");
	destruir(&img);
	imprimir(&img);
}
void exercicio4(){
	PGM img;
	cout << "#Exercicio 4#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 700, 500, 255);
	imprimir(&img);
	gravar(&img, "exercicio4.pgm");
	destruir(&img);
	imprimir(&img);
}
int main(void)
{

	// //#Exemplo1#: criando uma imagem e gravando no disco
	// PGM img1;
	// criar(&img1, 5, 7);
	// gravar(&img1, "exemplo1.pgm");
	// destruir(&img1);

	// //#Exemplo2#: imprimindo os atributos da imagem 
	// PGM img2;
	// cout << "#Exemplo 2#\n";
	// imprimir(&img2);
	// criar(&img2, 700, 500);
	// imprimir(&img2);
	// gravar(&img2, "exemplo2.pgm");
	// destruir(&img2);
	// imprimir(&img2);

	// //#Exemplo3#: ler imagem, getPixel, setPixel 
	// PGM img3;
	// unsigned char corP;
	// cout << "\n\n#Exemplo 3#\n";
	// ler(&img3, "feep.pgm");

	// //ler o valor do pixel de coordenadas (x=1, y=1)
	// corP = getPixel(&img3, 1,1);
	// cout << "A cor do pixel(1,1): " << (int) corP << endl;

	// setPixel(&img3, 1, 1, 255); //alterar a cor do pixel(1,1) para branco(255)
	// gravar(&img3, "exemplo3.pgm"); //gravar o resultado no disco
	// destruir(&img3);

	

	// exercicio4();
	// exercicio5();
	// exercicio6();
	//exercicio7();
	exercicio8();
	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}