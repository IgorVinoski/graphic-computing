#include "captura.hpp"

#include <filesystem>
#include <regex>
#include <vector>
#include <algorithm>

#include <stb/stb_image_write.h>
#include <glad/glad.h> //GLAD

namespace fs = std::filesystem;


void capturarTela(string pasta, int largura, int altura)
{
    fs::path dirPath(pasta);
    if (!fs::exists(dirPath)) {
        std::cerr << "capturarTela: Diretório não existe: " << pasta << std::endl;
        return;
    }

    // Regex para detectar arquivos do tipo screenshot_XXXX.png
    std::regex padrao("screenshot_(\\d{4})\\.png");
    int maiorIndice = 0;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            std::smatch match;
            std::string nome = entry.path().filename().string();
            if (std::regex_match(nome, match, padrao)) {
                int indice = std::stoi(match[1].str());
                if (indice > maiorIndice) {
                    maiorIndice = indice;
                }
            }
        }
    }

    // Próximo nome disponível
    int proximoIndice = maiorIndice + 1;
    char nomeArquivo[64];
    std::snprintf(nomeArquivo, sizeof(nomeArquivo), "screenshot_%04d.png", proximoIndice);
    fs::path caminhoCompleto = dirPath / nomeArquivo;

    // Alocar espaço para pixels (3 canais RGB)
    std::vector<unsigned char> pixels(largura * altura * 3);
    glReadPixels(0, 0, largura, altura, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Inverter imagem na vertical (OpenGL começa do canto inferior esquerdo)
    for (int y = 0; y < altura / 2; ++y) {
        for (int x = 0; x < largura * 3; ++x) {
            std::swap(pixels[y * largura * 3 + x], pixels[(altura - 1 - y) * largura * 3 + x]);
        }
    }

    // Salvar a imagem
    if (stbi_write_png(caminhoCompleto.string().c_str(), largura, altura, 3, pixels.data(), largura * 3)) {
        std::cout << "Screenshot salva em: " << caminhoCompleto << std::endl;
    } else {
        std::cerr << "Erro ao salvar screenshot." << std::endl;
    }
}