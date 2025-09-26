#include <iostream>
#include "PGM.hpp"
#include "PPM.hpp"
#include "trans2d.hpp"
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace Eigen;

#include <queue>

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

void flood_fillPPM_iter(int start_x, int start_y, RGB target_color, RGB new_color, PPM *ppm) {
    if (!ppm->pixels) return;

    // Se a cor inicial já é a cor nova, nada a fazer
    RGB initial = getPixel(ppm, start_x, start_y);
    if (initial == new_color) return;

    std::queue<Point> q;
    q.push(Point(start_x, start_y));

    while (!q.empty()) {
        Point p = q.front(); q.pop();

        int x = p.x;
        int y = p.y;

        // Verifica limites
        if (x < 0 || y < 0 || x >= ppm->larg || y >= ppm->alt) continue;

        RGB current = getPixel(ppm, x, y);

        // Só preenche se for a cor alvo
        if (current != target_color) continue;

        setPixel(ppm, x, y, new_color);

        // Adiciona vizinhos na fila
        q.push(Point(x + 1, y));
        q.push(Point(x - 1, y));
        q.push(Point(x, y + 1));
        q.push(Point(x, y - 1));
    }
}

void flood_fillPGM_iter(int start_x, int start_y, char target_color, char new_color, PGM *pgm) {
    if (!pgm->pixels) return;

    // Se a cor inicial já é a cor nova, nada a fazer
    char initial = getPixel(pgm, start_x, start_y);
    if (initial == new_color) return;

    std::queue<Point> q;
    q.push(Point(start_x, start_y));

    while (!q.empty()) {
        Point p = q.front(); q.pop();

        int x = p.x;
        int y = p.y;

        // Verifica limites
        if (x < 0 || y < 0 || x >= pgm->larg || y >= pgm->alt) continue;

        char current = getPixel(pgm, x, y);

        // Só preenche se for a cor alvo
        if (current != target_color) continue;

        setPixel(pgm, x, y, new_color);

        // Adiciona vizinhos na fila
        q.push(Point(x + 1, y));
        q.push(Point(x - 1, y));
        q.push(Point(x, y + 1));
        q.push(Point(x, y - 1));
    }
}

void flood_fillPPM(int pos_x, int pos_y, RGB target_color, RGB color, PPM *ppm) {
    if (pos_x < 0 || pos_y < 0 || pos_x >= ppm->larg || pos_y >= ppm->alt) {
        return; 
    }

    RGB imageColor = getPixel(ppm, pos_x, pos_y);

    if (imageColor == color) {
        return;
    }

    if (imageColor != target_color) {
        return;
    }

    setPixel(ppm, pos_x, pos_y, color);

    flood_fillPPM(pos_x + 1, pos_y, target_color, color, ppm);
    flood_fillPPM(pos_x - 1, pos_y, target_color, color, ppm);
    flood_fillPPM(pos_x, pos_y + 1, target_color, color, ppm);
    flood_fillPPM(pos_x, pos_y - 1, target_color, color, ppm);
}
void preenche(PPM *ppm, RGB cor, int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float Xinc = (float) dx / steps;
    float Yinc = (float) dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        if (x >= 0 && x < ppm->larg && y >= 0 && y < ppm->alt) {
            setPixel(ppm, (int)x, (int)y, cor);
        }
        x += Xinc;
        y += Yinc;
    }
}

int main(void)
{
    setlocale(LC_ALL, "portuguese");

    PPM imagem; 
    criar(&imagem, 400, 400);
    RGB new_color = {255, 0, 0}; 

    // pontos do triângulo (do teu print do viewport)
    int x1 = 156, y1 = 224;
    int x2 = 243, y2 = 224;
    int x3 = 200, y3 = 150;

    // desenhar as arestas
    preenche(&imagem, new_color, x1, y1, x2, y2);
    preenche(&imagem, new_color, x2, y2, x3, y3);
    preenche(&imagem, new_color, x3, y3, x1, y1);

    gravar(&imagem, "exercicio4.ppm");

    cout << "Pressione uma tecla para encerrar o programa.\n";
    getchar();
    return EXIT_SUCCESS; 
}
