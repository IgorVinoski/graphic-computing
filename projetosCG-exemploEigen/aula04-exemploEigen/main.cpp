#include <iostream>
#include "PGM.hpp"
#include "trans2d.hpp"
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace Eigen;

void lista4_exerc2()
{
	string entrada = "T 4 2 4 1 7 3 5 2";

	stringstream strBufffer(entrada);
	string op;
	
	strBufffer >> op; 
	if(op == "T")
	{
		float tx, ty;
		strBufffer >> tx;
		strBufffer >> ty; 
	}
}


void lista3_exerc2()
{
	Matrix3f E = getEscala(1.5f, 0.5f);
	Matrix3f R = getRotacao(90.0f);
	Matrix3f T = getTranslacao(-6, 0);
	Matrix3f T_ = getTranslacao(4, 4);
	Matrix3f T_Inv = getTranslacao(-4, -4);

	Matrix3f M = T_ * T * R * E * T_Inv;
    //Matrix3f M2 =  getTranslacao(4, 4) * getTranslacao(-6, 0);

	vector<Vector3f> pontos;
	pontos.push_back(Vector3f(2,2,1));
	pontos.push_back(Vector3f(6,2,1));
	pontos.push_back(Vector3f(6,6,1));
	pontos.push_back(Vector3f(2,6,1));

	for (size_t i = 0; i < pontos.size(); i++)
	{
		Vector3f resposta = M * pontos[i];
		cout << i+1 << " - " << resposta.hnormalized().transpose() <<endl;
	}
	

}
void transf2D(PGM* pgmE, PGM* pgmS, Matrix3f m){
	if(pgmE->alt != pgmS->alt || pgmE->larg != pgmS->larg){
		cout << "Erro: as dimensoes de entrada e saida devem ser iguais.\n";
		return;
	}
	for (size_t y = 0; y < pgmE->alt; y++)
	{
		/* code */
		for (size_t x = 0; x < pgmE->larg; x++)
		{
			/* code */
			Vector3f coordE(x, y, 1);
			Vector3f coordS = m * coordE;

			float xS = round(coordS.x());
			float yS = round(coordS.y());

			if(coordValida(pgmS, xS, yS)){
				//copia as cores
				setPixel(pgmS, xS, yS, getPixel(pgmE, coordE.x(), coordE.y()));
			}
		}
		
	}
	
}

void transf2DInversa(PGM* pgmE, PGM* pgmS, Matrix3f mInv){
	if(pgmE->alt != pgmS->alt || pgmE->larg != pgmS->larg){
		cout << "Erro: as dimensoes de entrada e saida devem ser iguais.\n";
		return;
	}
	for (size_t y = 0; y < pgmS->alt; y++)
	{
		/* code */
		for (size_t x = 0; x < pgmS->larg; x++)
		{
			/* code */
			Vector3f coordS(x, y, 1);
			Vector3f coordE = mInv * coordS;

			float xE = round(coordE.x());
			float yE = round(coordE.y());

			if(coordValida(pgmE, xE, yE)){
				//copia as cores
				setPixel(pgmS, coordS.x(), coordS.y(), getPixel(pgmE, xE, yE));
			}
		}
		
	}
	
}

void exercicio1(){
	PGM entrada, saida;	
	ler(&entrada, "numeros.pgm");
	criar(&saida, entrada.larg, entrada.alt);
	Vector2f centro((entrada.larg-1)/2.0, (entrada.alt-1)/2.0);

	Matrix3f m =   getTranslacao(centro.x(), centro.y()) *  getReflexaoY()  *  getCisalhamentoHorizontal(0.9) * getTranslacao(-centro.x(), -centro.y());
	transf2DInversa(&entrada, &saida, m.inverse());
	gravar(&saida, "exercicio1.pgm");

}

void exercicio2(){
	PGM entrada, saida;	
	ler(&entrada, "numeros.pgm");
	criar(&saida, entrada.larg, entrada.alt);
	Vector2f centro((entrada.larg-1)/2.0, (entrada.alt-1)/2.0);

	Matrix3f m =   getTranslacao(centro.x(), centro.y()) *   getCisalhamentoHorizontal(0.7f) * getRotacao(20.0f) * getEscala(0.5f, 0.5f) * getRotacao(15.0f) * getTranslacao(-centro.x(), -centro.y());
	transf2DInversa(&entrada, &saida, m.inverse());
	gravar(&saida, "exercicio2.pgm");
}
void exercicio3(){
		PGM entrada, saida;	
	ler(&entrada, "numeros.pgm");
	criar(&saida, entrada.larg, entrada.alt);
	Vector2f centro((entrada.larg-1)/2.0, (entrada.alt-1)/2.0);

	Matrix3f m =   getTranslacao(centro.x(), centro.y()) *  getRotacao(45.0f) * getTranslacao(-centro.x(), -centro.y());
	transf2DInversa(&entrada, &saida, m.inverse());
	gravar(&saida, "exercicio3.pgm");
}

void exercicio4(){
	string imagemEntrada;
	Matrix3f m;
	m = m.Identity();
	PGM entrada, saida;
	ler(&entrada, "numeros.pgm");
	criar(&saida, entrada.larg, entrada.alt);
	string transformacao;
	int opt;
	int count = 0;
	do{
		count++;
		cout << "0. Sair.\n";
		cout << "1. Aplicar transformacao\n";
		cin >> opt;		
		if(opt == 1){
			transformacao = "";
			cout << "Transformacao geometrica (R, T, E, C ou RE): T \n";
			cin >> transformacao;
			if(transformacao[0] == 'R'){
				float angulo;
				cout << "Informe o angulo da rotacado: \n";
				cin>> angulo;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
				m *= getRotacao(angulo);
				string filename = "exercicio4-rotacao-"+to_string(count)+".pgm";
				transf2DInversa(&entrada, &saida, m.inverse());
				gravar(&saida, filename);

			}
			if(transformacao[0]=='T'){
				float tx;
				float ty;
				cout << "Informe o tx da translacao: \n";
				cin >> tx;
				cout << "Informe o ty da translacao: \n";
				cin >> ty;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

				m *= getTranslacao(tx, ty);
				string filename = "exercicio4-translacao-"+to_string(count)+".pgm";
				transf2DInversa(&entrada, &saida, m.inverse());
				gravar(&saida, filename);
			}
			if(transformacao[0]=='E'){
				float sx, sy;
				cout << "Informe o sx da escala: \n";
				cin >> sx;
				cout << "Informe o sy da escala: \n";
				cin >> sy;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
				m *= getEscala(sx, sy);
				string filename = "exercicio4-escala-"+to_string(count)+".pgm";
				transf2DInversa(&entrada, &saida, m.inverse());
				gravar(&saida, filename);
			}
			if(transformacao[0]=='C'){
				char h_or_v;
				cout << "Cisalhamento horizonal digite h\nCisalhamento vertical digite v\n";
				cin >> h_or_v;
				
				if(h_or_v == 'h'){
					float a;
					cout << "Informe o fator (a) do cisalhamento\n";
					cin >> a;
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

					m*= getCisalhamentoHorizontal(a);
					string filename = "exercicio4-cisalhamento-horicontal-"+to_string(count)+".pgm";
					transf2DInversa(&entrada, &saida, m.inverse());
					gravar(&saida, filename);
				}else{
					float b;
					cout << "Informe o fator (b) do cisalhamento.\n";
					cin >> b;
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

					m*= getCisalhamentoVertical(b);
					string filename = "exercicio4-cisalhamento-vertical-"+to_string(count)+".pgm";
					transf2DInversa(&entrada, &saida, m.inverse());
					gravar(&saida, filename);
				}
			}
			if(transformacao =="RE" ){
				float h_or_v;
				cout << "Reflexao horizonal digite h\nCisalhamento vertical digite v\n";
				cin >> h_or_v;
				if(h_or_v == 'h'){
					m*= getReflexaoH();
					string filename = "exercicio4-reflexao-horitonzal-"+to_string(count)+".pgm";
					transf2DInversa(&entrada, &saida, m.inverse());
					gravar(&saida, filename);
				}else{
					m*= getReflexaoY();
					string filename = "exercicio4-reflexao-vertical-"+to_string(count)+".pgm";
					transf2DInversa(&entrada, &saida, m.inverse());
					gravar(&saida, filename);
				}
			}
		}

	}while(opt !=0);
}
void exercicio5() {
    ifstream fin("entrada.txt");
    if(!fin.is_open()){
        cout << "Erro ao abrir arquivo entrada.txt\n";
        return;
    }

    int nTrans;
    string imgEntrada, imgSaida, mapTipo;

    string token;
    fin >> token >> nTrans;   // NTRANS
    fin >> token >> imgEntrada; // IMGE
    fin >> token >> imgSaida;   // IMGS
    fin >> token >> mapTipo;    // MAP

    PGM entrada, saida;
    ler(&entrada, imgEntrada.c_str());

    // largura = N * largura original
    criar(&saida, entrada.larg * nTrans, entrada.alt);

    Matrix3f m = Matrix3f::Identity();

    for(int i=0; i<nTrans; i++){
        string op;
        fin >> op;
Vector2f centro((entrada.larg-1)/2.0, (entrada.alt-1)/2.0);

        if(op == "R"){ // Rotação
            float angulo;
            fin >> angulo;
			cout << "entru aqui\n";
    m *= getTranslacao(centro.x(), centro.y()) *
         getRotacao(angulo) *
         getTranslacao(-centro.x(), -centro.y());        }
        else if(op == "T"){ // Translação
            float tx, ty;
            fin >> tx >> ty;
            m *= getTranslacao(tx, ty);
        }
        else if(op == "E" || op == "S"){ // Escala
            float sx, sy;
            fin >> sx >> sy;
    m *= getTranslacao(centro.x(), centro.y()) *
         getEscala(sx, sy) *
         getTranslacao(-centro.x(), -centro.y());
			cout << "No S\n";
        }
        else if(op == "C" || op == "CI"){ // Cisalhamento
            char hv;
            float fator;
            fin >> hv >> fator;
            if(hv == 'H' || hv == 'h'){

				cout << "CI H\n";
        m *= getTranslacao(centro.x(), centro.y()) *
             getCisalhamentoHorizontal(fator) *
             getTranslacao(-centro.x(), -centro.y());
			}
            else
        m *= getTranslacao(centro.x(), centro.y()) *
             getCisalhamentoVertical(fator) *
             getTranslacao(-centro.x(), -centro.y());
			        }
        else if(op == "RE"){ // Reflexão
            char hv;
            fin >> hv;
            if(hv == 'H' || hv == 'h' || hv == 'X')
                        m *= getTranslacao(centro.x(), centro.y()) *
             getReflexaoH() *
             getTranslacao(-centro.x(), -centro.y());
            else
                       m *= getTranslacao(centro.x(), centro.y()) *
             getReflexaoY() *
             getTranslacao(-centro.x(), -centro.y());
				cout << "RE Y\n";
        }

        // cada segmento ocupa um "bloco" da imagem de saída
        PGM segmento;
        criar(&segmento, entrada.larg, entrada.alt);

        if(mapTipo == "DIR")
            transf2D(&entrada, &segmento, m);
        else
            transf2DInversa(&entrada, &segmento, m.inverse());

        // copiar segmento para a saída na posição correta
        for(int y=0; y<segmento.alt; y++){
            for(int x=0; x<segmento.larg; x++){
                setPixel(&saida, x + i*entrada.larg, y, getPixel(&segmento, x, y));
            }
        }
    }

    gravar(&saida, imgSaida.c_str());
    cout << "Transformacoes aplicadas. Arquivo gerado: " << imgSaida << endl;
}

int main(void)
{
	setlocale(LC_ALL, "portuguese");
	
	
	// //lista3_exerc2();

	// Vector3f pA = Vector3f(4.0f, 2.0f, 1.0f);
	// 	Vector3f pB = Vector3f(8.0f, 2.0f, 1.0f);
	// Vector3f pC = Vector3f(6.0f, 7.0f, 1.0f);


	// //definindo uma matriz de translação

	// Matrix3f mT = Matrix3f::Identity();

	// mT(0,2) = 3; //Tx
	// mT(1,2) = 1; //Ty

	// //oU USANDO UMA FUNÇÃO
	// //Matrix3f mT = getTranslacao(3,1);

	// //Matrix x ponto

	// Vector3f pA_ = mT * pA;
	// Vector3f pB_ = mT * pB;
	// Vector3f pC_ = mT * pC;


	// cout << "Ponto a: " << pA_.transpose() << endl;
	// cout << "Ponto b: " << pB_.transpose() << endl;
	// cout << "Ponto c: " << pC_.transpose() << endl;

	// cout << "Matrizn";
	// cout << endl << mT << endl;	


	// vector<Vector3f> pontos;
	// pontos.push_back(Vector3f(2.0f, 2.0f, 1.0f));
	// pontos.push_back(Vector3f(6.0f, 2.0f, 1.0f));
	// pontos.push_back(Vector3f(6.0f, 6.0f, 1.0f));
	// pontos.push_back(Vector3f(2.0f, 6.0f, 1.0f));

	// Matrix3f M = getTranslacao(4, 4) * getTranslacao(-6,0) * getRotacao(90.0f) * getEscala(1.5, 0.5) * getTranslacao(-4, -4);

	// cout << endl << M << endl;


	// for (size_t i = 0; i < pontos.size(); i++)
	// {
	// 	/* code */

	// 	Vector3f p = M * pontos[i];
	// 	Vector2f pC = p.hnormalized();
	// 	cout << pontos[i].hnormalized().transpose() << " -> ";
	// 	cout << pC.transpose() << endl;
	// }
	
	PGM entrada, saida;
	ler(&entrada, "numeros.pgm");
	
	criar(&saida, entrada.larg, entrada.alt);

	// Vector2f centro((entrada.larg-1)/2.0, (entrada.alt-1)/2.0);
	// Matrix3f m =  getTranslacao(centro.x(), centro.y()) * getRotacao(45.0f) * getTranslacao(-centro.x(), -centro.y());
	// transf2DInversa(&entrada, &saida, m.inverse());

	//  gravar(&saida, "rot45.pgm");

	// exercicio1();
	// exercicio2();
	// exercicio3();
	// exercicio4();
	exercicio5();
	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}