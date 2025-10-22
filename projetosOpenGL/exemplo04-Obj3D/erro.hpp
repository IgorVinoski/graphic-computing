#ifndef ERRO_H_INCLUDED
#define ERRO_H_INCLUDED

#include <stdexcept>
#include <sstream>

inline void erro(int linha, const char* arquivo) {
    std::ostringstream oss;
    oss << "Erro na linha " << linha << " do arquivo " << arquivo;
    throw std::runtime_error(oss.str());
}

#define ERRO() erro(__LINE__, __FILE__)


#endif