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



int main(void)
{
	setlocale(LC_ALL, "portuguese");
	
	
	lista3_exerc2();


	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}