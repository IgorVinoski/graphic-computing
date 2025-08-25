#include <iostream>
#include "PPM.hpp"
#include "PGM.hpp"

#include <cmath>
#include <ctime>
using namespace std;

/*
Exercicio	3
Crie uma função para criar uma imagem PPM. A função deve conter os seguintes 
parâmetros:


*/
void criarComCorDeFundo(PPM *ppm, int largura, int altura, RGB corFundo){
	if(ppm->pixels)
		delete ppm->pixels;
	
	int tamanho = largura * altura * 3;
	ppm->tipo = "P3";
	ppm->larg = largura;
	ppm->alt = altura;
	ppm->vmax = 255;
	ppm->pixels  = new unsigned char[tamanho];

	for(int i = 0; i < tamanho; i+=3){
		ppm->pixels[i] = corFundo.r;
		ppm->pixels[i+1] = corFundo.g;
		ppm->pixels[i+2] = corFundo.b;
	}
}
/*
Exercicio 4
Crie uma função para preencher uma determinada linha de uma imagem PPM com uma 
cor específica passada por parâmetro.

*/

void setLinha(PPM *ppm, int linha, RGB cor){
	for(int i = 0; i < ppm->larg; i++){
		setPixel(ppm, i, linha, cor);
	}
}


/*
Exericico 5
Crie uma função para verificar se as coordenadas de um pixel passadas por parâmetro 
são válidas.

*/
bool coordValida(PPM *ppm, int x, int y){
	if(ppm->larg > x && ppm->alt > y){
		return true;
	}
	return false;
}

/*
Exercicio 6
Implemente uma aplicação para marcar uma área em uma imagem PPM: Exemplo:
- O programa deve receber como entrada o caminho da imagem PPM, as 
coordenadas dos pontos P1(x1, y1) e P2(x2,y2) e a cor RGB da linha.
- O programa deve prevenir erros quando as coordenadas ultrapassarem as 
dimensões da imagem, traçando a linha somente nas coordenadas válidas da 
imagem (use a função criada no exercício 5)
*/
void marcaArea(PPM *ppm, int x1, int y1, int x2, int y2){
	RGB vermelho(255,0,0);
	for(int i = x1; i < x2; i++){
		for(int j = y1; j < y2; j++){
			if(i == x1){
				setPixel(ppm, i, j, vermelho);

			}
			if(j == y1){
				setPixel(ppm, i, j, vermelho);
			}
			if(i == x2 -1 ){
				setPixel(ppm, i, j, vermelho);
			}
			if(j == y2 - 1){
				setPixel(ppm, i, j, vermelho);
			}
		}
	}
}
RGB gerarCorAleatoria() {
 
    unsigned char r = (unsigned char)round(rand() % 256);
    unsigned char g = (unsigned char)round(rand() % 256);
    unsigned char b = (unsigned char)round(rand() % 256);
    return RGB(r, g, b);
}
/*
Exercicio 7
Crie uma imagem PPM de 500x500 pixels com fundo preto (colocar o valor zero para 
todos os pixels). Em seguida, atribuir uma cor RGB gerada aleatoriamente a cada 50 
linhas imagem.

*/
void marcaDe50Em50(PPM *ppm){	
	   srand(time(0)); 
	for(int i = 0; i < ppm->alt; i+=50){
		RGB cor = gerarCorAleatoria();
		setLinha(ppm, i, cor);

	}
}


/*
Exercicio 8:
Considere uma imagem PPM de entrada e uma região definida pelas coordenadas P1(x1, 
y1) e P2(x2,y2). Desenvolva um método para criar uma imagem de saída, a qual possui a 
mesma dimensão da imagem de entrada e a cor de todos os pixels é preta. Em seguida, 
copie a região definida na imagem de entrada para a imagem de saída.

*/


/*
Exercicio 9:
Crie um programa pra converter imagens PPM para PGM:
- Ler a imagem PPM;
- Criar uma imagem PGM com a mesma dimensão da imagem PPM e definir a 
cor preta para todos pixels;
- Construir um laço para percorrer os pixels da imagem PPM. Para cada pixel
PPM(x, y), converta a cor RGB para cinza:
- Cinza = 0.299*R + 0.587*G + 0.114*B
- Atribuir a cor cinza calculada para o pixel na imagem PGM(x,y), na
mesma posição .
- Salvar a imagem PGM criada

*/
PGM converteParaPGM(PPM *ppm){
	PGM retorno;
	criar(&retorno, ppm->larg, ppm->alt);
	for(int x = 0; x < retorno.larg; x++){
		for(int y = 0; y < retorno.alt; y++){
			RGB cor = getPixel(ppm, x, y);
			unsigned char corConvertida =   0.299*cor.r + 0.587*cor.g + 0.114*cor.b;
			cout << " a cor convertida: "  << (int) corConvertida << " \n";
			setPixel(&retorno, x, y, corConvertida);
		}
	}

	return retorno;
}

PPM copiaRegiao(PPM *ppm, int x1, int y1, int x2, int y2){
	PPM retorno;
	criar(&retorno, ppm->larg, ppm->alt);

	for(int x = x1; x < x2; x++){
		for(int y = y1; y < y2; y++){
			setPixel(&retorno, x, y, getPixel(ppm, x, y));
		}
	}
	return retorno;
}


void inverteHorizontalmente(PPM  *ppm){
	for(int i = 0; i < ppm->larg/2; i++){
		for(int j  = 0; j < ppm->alt; j++){
			RGB pixel = getPixel(ppm, i, j);
			setPixel(ppm, i, j, getPixel(ppm, ppm->larg - i, j));
			setPixel(ppm, ppm->larg - i , j, pixel);
		}
	}
}
void marcaAreaComLargura(PPM *ppm, int x1, int y1, int x2, int y2, int largura){
	RGB vermelho(255,0,0);
	for(int i = x1; i < x2; i++){
		for(int j = y1; j < y2; j++){
			for(int k = 0; k < largura; k++){
				if(i == x1){
					setPixel(ppm, i + k, j + k, vermelho);

				}
				if(j == y1){
					setPixel(ppm, i + k, j + k, vermelho);
				}
				if(i == x2 -1 ){
					setPixel(ppm, i + k, j + k, vermelho);
				}
				if(j == y2 - 1){
					setPixel(ppm, i + k, j +  k, vermelho);
				}
			}

		}
	}
}

RGB quantizar(RGB rgb, int Q) {
	RGB retorno;

		int passo = 255 / (Q - 1);  // intervalo

		int nivelR = (rgb.r + passo/2) / passo; // arredonda para o mais próximo
		retorno.r = nivelR*passo;
		int nivelG = (rgb.g + passo/2) / passo; // arredonda para o mais próximo
		retorno.g = nivelG*passo;
		int nivelB = (rgb.b + passo/2) / passo; // arredonda para o mais próximo
		retorno.b = nivelB*passo;

    return retorno;
}
void quantizaImagem(PPM *ppm, int q){
	for(int i = 0 ; i < ppm->larg; i++){
		for(int j = 0; j < ppm->alt; j++){
			RGB pixel = getPixel(ppm, i, j);
			setPixel(ppm, i, j, quantizar(pixel, q));
		}
	}
}

void exercicio(){
	PPM img1;
	cout << "Exercicio 14 #\n";
	string entrada;
	cout << "Informe o Q: ";
	getline(cin, entrada);
	int q = stoi(entrada);

	ler(&img1, "numeros.ppm");
	quantizaImagem(&img1, q);
	imprimir(&img1);
	gravar(&img1, "exercicio14.ppm");
	destruir(&img1);

}
void exercicio12(){
	
}

void inverteCores(PPM *ppm){
	for(int i = 0; i < ppm->larg; i++){
		for(int j  = 0 ; j < ppm->alt; j++){
			RGB pixel = getPixel(ppm, i, j);
			RGB newCor;
			newCor.r = 255 - pixel.r;
			newCor.g = 255 - pixel.g;
			newCor.b = 255 - pixel.b;
			setPixel(ppm, i, j, newCor);
		}
	}
}
void exercicio13(){
	PPM img1;
	cout << "Exercicio 13 #\n";
	ler(&img1, "numeros.ppm");
	inverteCores(&img1);
	imprimir(&img1);
	gravar(&img1, "exercicio13.ppm");
	destruir(&img1);

}
void exercicio11(){
	PPM img1;
	cout << "Exercicio 11 #\n";
	ler(&img1, "numeros.ppm");

	marcaAreaComLargura(&img1, 391, 327, 574, 575, 10);
	imprimir(&img1);
	gravar(&img1, "exercicio11.ppm");
	destruir(&img1);
}
void exercicio10(){
	PPM img1;
	cout << "Exercicio 10 #\n";
	ler(&img1, "numeros.ppm");

	inverteHorizontalmente(&img1);
	imprimir(&img1);
	gravar(&img1, "exercicio10.ppm");
	destruir(&img1);
}
void exercicio9(){
	PPM img1;
	cout << "Exercicio 9 #\n";
	ler(&img1, "spider.ppm");

	PGM copia = converteParaPGM(&img1);
	imprimir(&copia);
	cout << "imprimiu copia\n";
	gravar(&copia, "exercicio9.pgm");
	destruir(&copia);
}
void exercicio8(){
	PPM img1;
	cout << "Exercicio 8 #\n";
	RGB corP(100, 20, 30);

	criarComCorDeFundo(&img1, 700, 500, corP);

	PPM copia = copiaRegiao(&img1, 300, 200, 400, 300);
	imprimir(&copia);
	cout << "imprimiu copia\n";
	gravar(&copia, "exercicio8.ppm");
	destruir(&copia);
}
void exercicio7(){
	PPM img;
	imprimir(&img);
	criar(&img, 500, 500);

	marcaDe50Em50(&img);
	gravar(&img, "exercicio7.ppm");

}
void exercicio6(){
	PPM img2;
	cout << "Exercicio 6 #\n";
	imprimir(&img2);
	criar(&img2, 700, 500);
	marcaArea(&img2, 300, 200, 400, 300);
	imprimir(&img2);
	gravar(&img2, "exercicio6.ppm");
	destruir(&img2);
	imprimir(&img2);
}
void exercicio5(){
	PPM img1;
	RGB corP(10, 20, 30);
	RGB cor(20, 0, 255);

	criarComCorDeFundo(&img1, 5, 7, corP);
	bool valida = coordValida(&img1, 6, 0);
	if(valida){
		cout << "é valida! \n";
	}else{
		cout << "Not valida yet!\n";
	}
	setLinha(&img1, 2,  cor);

	gravar(&img1, "exercicio5.ppm");
	destruir(&img1);
}

void exercicio4(){
	PPM img1;
	RGB corP(10, 20, 30);
	RGB cor(20, 0, 255);

	criarComCorDeFundo(&img1, 5, 7, corP);
	setLinha(&img1, 2,  cor);

	gravar(&img1, "exercicio4.ppm");
	destruir(&img1);
}
void exercicio3(){
	PPM img1;
	RGB corP(10, 20, 30);

	criarComCorDeFundo(&img1, 5, 7, corP);
	gravar(&img1, "exercicio3.ppm");
	destruir(&img1);
}


int main(void)
{

	//#Exemplo1#: criando uma imagem e gravando no disco
	// PPM img1;
	// criar(&img1, 5, 7);
	// gravar(&img1, "exemplo1.ppm");
	// destruir(&img1);

	// //#Exemplo2#: imprimindo os atributos da imagem 
	// PPM img2;
	// cout << "#Exemplo 2#\n";
	// imprimir(&img2);
	// criar(&img2, 700, 500);
	// imprimir(&img2);
	// gravar(&img2, "exemplo2.ppm");
	// destruir(&img2);
	// imprimir(&img2);

	// //#Exemplo3#: ler imagem, getPixel, setPixel 
	// PPM img3;
	// RGB corP;
	// cout << "\n\n#Exemplo 3#\n";
	// ler(&img3, "exemploPPM.ppm");

	// //ler o valor do pixel de coordenadas (x=1, y=1)
	// corP = getPixel(&img3, 0,0);
	// cout << "A cor do pixel(0,0): [" << corP << "] " << endl;

	// setPixel(&img3, 1, 0, RGB(0, 128, 128)); //alterar a cor do pixel(1, 0) para RGB(0, 128, 128)
	// setPixel(&img3, 2, 0, RGB(255, 0, 255)); //alterar a cor do pixel(2, 0) para rosa
	// setPixel(&img3, 2, 4, RGB(128, 0, 128)); //alterar a cor do pixel(2, 4) para roxo
	// gravar(&img3, "exemplo3.ppm"); //gravar o resultado no disco
	// destruir(&img3);

	//exercicio3();
	// exercicio4();
	// exercicio5();
	// exercicio6();
	// exercicio7();
	// exercicio8();
		//exercicio9();
		// exercicio10();
		// exercicio11();

		// exercicio13();
		exercicio();
	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}