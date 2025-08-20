#include <iostream>
#include "PGM.hpp"
#include <cmath>
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
/*
Exercício 9:
9. Crie um método para inverter a imagem verticalmente (flip).

*/
void inverteVerticalmente(PGM *pgm){


	for(int x = 0; x < pgm->larg; x++){
		for(int y = 0; y < pgm->alt/2; y++){
			unsigned char temp = getPixel(pgm, x, y);
			setPixel(pgm, x, y, getPixel(pgm, x, pgm->alt - y - 1));
			setPixel(pgm, x,  pgm->alt - y - 1, temp);
		}
	}

}
/*
Exercício 10:
10. Considere uma imagem PGM de entrada e uma região definida pelas coordenadas P1(x1,
y1) e P2(x2,y2). Desenvolva uma função para criar uma imagem de saída, a qual possui a
mesma dimensão da imagem de entrada e a cor de todos os pixels é preta. Em seguida,
copie a região definida na imagem de entrada para a imagem de saída.
*/
PGM coiaTrecho(PGM *pgm, int x1, int y1, int x2, int y2){
	PGM result;
	criarComCorDeFundo(&result, pgm->larg, pgm->alt, 0);
	for(int i = x1; i <= x2; i++){
		for(int j = y1; j <= y2; j++){
			unsigned char temp  = getPixel(pgm, i, j);
			setPixel(&result, i, j,temp );
		}
	}
	return result;
}
/*
Exercício  11:
11. Desenvolva uma função em C++ para desenhar uma borda em uma imagem PGM. A cor
e a espessura da borda devem ser passadas por parâmetro da função. Observação: A
espessura da borda não pode ultrapassar 10 pixels.
*/
void preencheBora(PGM *pgm, int espessura, unsigned char cor){
	if(espessura> 10 ){
		cout << "espessura maior que 10.";
		return;
	}

	for(int x = 0; x < pgm->larg; x++){
		for(int y = 0; y < pgm->alt; y++){
			if(x == 0){
				setPixel(pgm, x, y, cor);
			}
			if(x == pgm->larg - 1){
				setPixel(pgm, x, y, cor);
			}
			if(y == 0){
				setPixel(pgm, x, y, cor);
			}
			if(y == pgm->alt - 1){
				setPixel(pgm, x, y, cor);
			}
		}
	}
}

/*
Exercício 12:
12. Desenvolva uma função em C++ para desenhar um “X” imagem PGM. A cor da linha deve
ser passada por parâmetro.

diagonal secundaria: i + j  = n + 1
*/
void preencheComX(PGM *pgm, unsigned char cor){
	if(pgm->larg ==pgm->alt ){
		for(int x = 0; x < pgm->larg; x++){
			for(int y = 0; y < pgm->alt; y++){
				if(x == y){
					setPixel(pgm, x, y, cor);
				}
				if(x + y == pgm->larg + 1){
					setPixel(pgm, x, y, cor);
				}
			}
		}
	}else{
		int dx = pgm->larg;
		int dy = pgm->alt;

		 int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

		 float Xinc = (float) dx / steps;

		float Yinc = (float) dy/steps;

		float x = 0;
		float y = 0;
		for(int i = 0; i < steps && (x < pgm->larg || y < pgm->alt); i++){
			setPixel(pgm,  x, y, cor);
			setPixel(pgm, pgm->larg - x, y, cor);
			x += Xinc;
			y += Xinc;
	}
}
		// }else if(pgm->larg  > pgm->alt ){
	// 		float razao  = ceil(pgm->larg /  (float) pgm->alt);
	// 		cout << " a razao: " << razao;
	// 		int xAtual = 0;
	// 		for(int y = 0; y < pgm->alt; y++){
	// 			int x = xAtual;
	// 			while(x < xAtual + razao && xAtual < pgm->larg){
	// 				//cout << "no while o x: " << x << " ";
	// 				setPixel(pgm, x, y, cor);
	// 				setPixel(pgm, pgm->larg - x, y, cor);
	// 				x++;
	// 			}
	// 			xAtual = x;
	// 		}
			
	// }else{
	// 	int razao = pgm->alt /   pgm->larg;
	// 	int yAtual = 0;
	// 	for(int x = 0; x < pgm->larg; x++){
	// 		int y = yAtual;
	// 		while( y < yAtual + razao && yAtual < pgm->alt && y <   pgm->alt){
	// 		//	cout << "no while o y: " << y << " ";

	// 			setPixel(pgm, x, y, cor);
	// 			setPixel(pgm, pgm->larg - x, y, cor);
	// 			y++;
	// 		}
	// 		yAtual = y;
	// 	}
	// }

}
/*
13. Crie um programa em C++ para combinar duas imagens PGM de entrada I1 e I2 em uma
única imagem PGM de saída Is. Para os pixels P1(x, y)=C1 da imagem I1 e P2(x, y)=C2 da
imagem I2, ambos com as mesmas coordenas x e y, as cores devem ser combinadas
como segue:
• Média: (C1+C2) / 2
• Maior: max(C1,C2)
• Menor: min(C1,C2)
Observações: Considere que todas as imagens possuem a mesma dimensão; Crie um
menu que permita o usuário escolher a combinação desejada.

*/
PGM combinaImagens(PGM *pgm1, PGM *pgm2){
	PGM saida;
	criar(&saida, pgm1->larg, pgm1->alt);

	int opt = 0;
	cout << "Escolha uma opcao: "<<endl;
	cout << "1. Media (C1+C2)/2 " << endl;
	cout << "2. Maior: max(c1,c2) "<<endl;
	cout << "3. Menor: min(c1,c2) "<<endl;
	cin >> opt;
	cout << "Opcao escolhida: " << opt << endl;
	for(int x = 0; x < pgm1->larg; x++){
		for(int y = 0; y < pgm1->alt; y++){
			if(opt == 1){
				cout << "aqui"<<endl;
				unsigned corSaida =  (unsigned char) ((getPixel(pgm1, x, y) + getPixel(pgm2, x, y))/2);
				setPixel(&saida, x, y, corSaida);
			}
			if(opt == 2){
				unsigned corSaida = (unsigned char) max(getPixel(pgm1,x, y), getPixel(pgm2, x, y));
				setPixel(&saida, x, y, corSaida);
			}
			if(opt == 3){

				unsigned corSaida = (unsigned char)  min(getPixel(pgm1,x, y), getPixel(pgm2, x, y));
				setPixel(&saida, x, y, corSaida);
			}
		}
	}
	return saida;
}
void preencheBordaDentroDeImagem(PGM *pgm, int x1, int y1, int x2, int y2, unsigned char cor){
	for(int x = x1; x < x2; x++){
		for(int y = y1; y < y2; y++){
			if(x == x1){
				setPixel(pgm, x, y, cor);
			}
			if(x == x2 - 1){
				setPixel(pgm, x, y, cor);
			}
			if(y == y1){
				setPixel(pgm, x, y, cor);
			}
			if(y == y2 - 1){
				cout << "caiu nesse?";
				setPixel(pgm, x, y, cor);
			}
		}
	}
	/*
	
	if(espessura> 10 ){
		cout << "espessura maior que 10.";
		return;
	}

	for(int x = 0; x < pgm->larg; x++){
		for(int y = 0; y < pgm->alt; y++){
			if(x == 0){
				setPixel(pgm, x, y, cor);
			}
			if(x == pgm->larg - 1){
				setPixel(pgm, x, y, cor);
			}
			if(y == 0){
				setPixel(pgm, x, y, cor);
			}
			if(y == pgm->alt - 1){
				setPixel(pgm, x, y, cor);
			}
		}
	}
	*/
}
bool checkCoord(PGM* pgm, int x, int y)
{
	if(x<0 || y<0 || x>=pgm->larg || y>=pgm->alt)
		return false;
	return true;
}
void DDA(PGM* pgm, int X0, int Y0, int X1, int Y1, unsigned char color)
{
    // calculate dx & dy
    int dx = X1 - X0;
    int dy = Y1 - Y0;

    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // calculate increment in x & y for each steps
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    // Put pixel for each step
    float X = X0;
    float Y = Y0;
    for (int i = 0; i <= steps; i++)
	{
		int coorx = round(X);
		int coory = round(Y);

		if(checkCoord(pgm, coorx, coory))
			setPixel(pgm, coorx, coory, color);
        
        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
    }
}
void exerc12()
{
	PGM img;
	criar(&img, 700, 300);
	DDA(&img, 0, 0, img.larg-1, img.alt-1, 255); //"diagonal principal"
	DDA(&img, 0, img.alt-1, img.larg-1, 0, 255); //"diagonal secundária"
	
	//testando os pontos opostos
	//DDA(&img, img.larg-1, img.alt-1, 0, 0, 255); //"diagonal principal"
	//DDA(&img, img.larg-1, 0, 0, img.alt-1, 255); //"diagonal secundária"
	gravar(&img, "exer12_saida.pgm");
	destruir(&img);

}


PGM converteParPretoBranco(PGM *pgm){
	PGM img;
	criar(&img, pgm->larg, pgm->alt);

	for(int i = 0; i < pgm->larg; i++){
		for(int j = 0; j < pgm->alt; j++){
			unsigned char pixel = getPixel(pgm, i, j);
			if(pixel > 128){
				setPixel(&img, i, j, 255);

			}else if(pixel <= 128){
				setPixel(&img, i, j, 0);
			}
		}
	}
	return img;
}

void exercicio15(){
	PGM retorno;
	cout << "Exercicio 15\n";
	
	PGM img;
	criarComCorDeFundo(&img, 70, 50, 127);
	retorno = converteParPretoBranco(&img);
	gravar(&retorno, "exercicio15.pgm");

}
void exercicio14(){
		PGM img;
	cout << "#Exercicio 14#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 70, 50, 255);
	//150, 50, 550, 450
	preencheBordaDentroDeImagem(&img, 15, 5, 55, 45, 0);
		imprimir(&img);
		gravar(&img, "exercicio14.pgm");
	

}
void exercicio13(){
	PGM img;
	cout << "#Exercicio 13#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 700, 500, 255);

	PGM img2;

	imprimir(&img2);
	criarComCorDeFundo(&img2, 700, 500, 100);
	PGM result;
	 
	 result = combinaImagens(&img, &img2);
	imprimir(&result);
		gravar(&img, "exercicio13_input1.pgm");
		gravar(&img2, "exercicio13_input2.pgm");

	gravar(&result, "exercicio13_output.pgm");
		imprimir(&result);

	destruir(&result);
		destruir(&img);
				destruir(&img2);


}

void exercicio12(){
	PGM img3;
	unsigned char corP;
	cout << "\n\n#Exercicio 12#\n";
	criarComCorDeFundo(&img3, 700, 500, 255);

	preencheComX(&img3,  0);

	gravar(&img3, "exercicio12.pgm"); //gravar o resultado no disco
	destruir(&img3);
}

void exercicio11(){
	PGM img3;
	unsigned char corP;
	cout << "\n\n#Exercicio 11#\n";
	criarComCorDeFundo(&img3, 700, 500, 255);

	preencheBora(&img3, 10,  0);

	gravar(&img3, "exercicio11.pgm"); //gravar o resultado no disco
	destruir(&img3);
}
void exercicio10(){
	PGM img;
	cout << "#Exercicio 10#\n";
	imprimir(&img);
	criarComCorDeFundo(&img, 700, 500, 255);

	PGM result = coiaTrecho(&img,150, 50, 550, 450 );
	imprimir(&result);
	gravar(&img, "exercicio10_input.pgm");
	gravar(&result, "exercicio10_output.pgm");
	destruir(&result);
	imprimir(&result);
}
void exercicio9(){
	PGM img3;
	unsigned char corP;
	cout << "\n\n#Exercicio 9#\n";
	ler(&img3, "feep.pgm");

	inverteVerticalmente(&img3);

	gravar(&img3, "exercicio9.pgm"); //gravar o resultado no disco
	destruir(&img3);

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
	//exercicio8();
	//exercicio9();
	//exercicio10();
	//exercicio11();
	exerc12();
	//exercicio13();
	//exercicio14();
//	exercicio15();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}