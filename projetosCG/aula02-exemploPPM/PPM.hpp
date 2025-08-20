#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct PPM
{
    string tipo;
    int larg;
    int alt;
    int vmax;
    unsigned char *pixels; // unsigned char -> 0 a 255
    PPM()                  // construtor -> inicializar variaveis
    {
        pixels = nullptr;
        larg = 0;
        alt = 0;
        vmax = 255;
        tipo = "";
    }
};

struct RGB{
    unsigned char r, g, b;
    RGB()
    {
        r=0;
        g=0;
        b=0;
    }
    RGB(unsigned char _r, unsigned char _g,unsigned char _b)
    {
        r=_r;
        g=_g;
        b=_b;
    }
};
ostream& operator<<(ostream& os, const RGB rgb)
{
    return os << (int) rgb.r << ", " << (int) rgb.g << ", " << (int) rgb.b ;
}

void destruir(PPM *ppm)
{
    if (ppm->pixels)
        delete ppm->pixels;
    ppm->pixels = nullptr;
    ppm->larg = 0;
    ppm->alt = 0;
    ppm->tipo = "";
    ppm->vmax = 255;
}

void imprimir(PPM *ppm)
{
    cout << "\nPPM\n";
    cout << "Tipo: " << ppm->tipo << endl;
    cout << "Dimensao: " << ppm->larg << "x" << ppm->alt << endl;
    cout << "vMax: " << ppm->vmax << endl;
    if (ppm->pixels) // verifica se o vetor de pixels está alocado
        cout << "Vetor de pixels: " << &(ppm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}

void criar(PPM *ppm, int largura, int altura)
{
    if (ppm->pixels)
        delete ppm->pixels;
    
    int tamanho = largura * altura * 3; //vezes 3, pois cada pixel possui RGB

    ppm->tipo = "P3";
    ppm->larg = largura;
    ppm->alt = altura;
    ppm->vmax = 255;
    ppm->pixels = new unsigned char[tamanho];

    // definir a cor preta para todos os pixels
    for (int i = 0; i < tamanho; i++)
        ppm->pixels[i] = 0;
}

bool gravar(PPM *ppm, string caminho)
{
    if (!ppm->pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    arq << ppm->tipo << endl;

    arq << ppm->larg << " " << ppm->alt << endl;

    arq << 255 << endl; // valor máximo de cor fixo em 255

    int tam = ppm->larg * ppm->alt * 3;  //vezes 3, pois cada pixel possui RGB
    for (int i = 0; i < tam; i++)
    {
        arq << (int)ppm->pixels[i] << endl;
        arq.flush();
    }

    arq.close();
    return true;
}


string lerLinhaArquivoPPM(std::ifstream &arq)
{
    string linha = "", dadoLido = "";
    while (!arq.eof())
    {
        linha = "";
        std::getline(arq, linha);
		
		// Remove \r do final da linha (caso exista)
        if (!linha.empty() && linha.back() == '\r')
            linha.pop_back();
		
        if (linha.size() > 0 && linha[0] != '#')
            return linha;
    }
    return linha;
}

bool ler(PPM *ppm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    // ler o tipo
    dado = lerLinhaArquivoPPM(arq);
    if (dado == "P3")
        ppm->tipo = dado;
    else
    {
        cout << "PPM: erro ao ler o tipo da imagem\n";
        return false;
    }

    // ler largura e altura
    dado = lerLinhaArquivoPPM(arq);
    str = stringstream(dado);
    if (!(str >> ppm->larg))
    {
        cout << "PPM: erro ao ler a largura\n";
        return false;
    }
    if (!(str >> ppm->alt))
    {
        cout << "PPM: erro ao ler a largura\n";
        return false;
    }

    // ler vmax
    dado = lerLinhaArquivoPPM(arq);
    str = stringstream(dado);
    if (!(str >> ppm->vmax))
    {
        cout << "PPM: erro ao ler vmax\n";
        return false;
    }

    if (ppm->pixels)
        delete ppm->pixels;

    int tam = ppm->larg * ppm->alt * 3; //vezes 3, pois cada pixel possui RGB

    ppm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {

        if (arq >> p)
        {
            if (i < tam)
                ppm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PPM: erro ao ler os pixels\n";
        return false;
    }
    arq.close();
    return true;
}


RGB getPixel(PPM *ppm, int x, int y)
{
    RGB rgb;
    if (!ppm->pixels)
        return rgb;

    rgb.r = ppm->pixels[y * ppm->larg*3 + x*3];
    rgb.g = ppm->pixels[y * ppm->larg*3 + x*3 +1];
    rgb.b = ppm->pixels[y * ppm->larg*3 + x*3 +2];

    return rgb;
}
void setPixel(PPM *ppm, int x, int y, RGB rgb)
{
    if (!ppm->pixels)
        return;

    ppm->pixels[y * ppm->larg*3 + x*3] = rgb.r;
    ppm->pixels[y * ppm->larg*3 + x*3 +1] = rgb.g;
    ppm->pixels[y * ppm->larg*3 + x*3 +2] = rgb.b;
}

#endif