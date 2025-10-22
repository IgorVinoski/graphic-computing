#include <iostream>
using namespace std;

#include <glad/glad.h> //GLAD
#include "opengl_func.hpp"
#include "erro.hpp"



GLFWwindow *criarJanela(int larg, int alt, string titulo, bool tela_cheia)
{
    GLFWwindow* janela = nullptr;

	//inicializar  a biblioteca GLFW
	if (!glfwInit())
		ERRO(); //termina a execução do programa
		
	

	//define que a versão minímina necessária do OPENGL é 3.3 ou superior!
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLSL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLSL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//não permite redimensionar a janela
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	if(tela_cheia)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		//pega informacoes do monitor
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		//cout << mode->redBits << endl;
		//cout << mode->greenBits << endl;
		//cout << mode->blueBits << endl;
		//cout << mode->refreshRate << endl << endl;
		
		//cria a janela
		janela = glfwCreateWindow(mode->width, mode->height, titulo.c_str(), glfwGetPrimaryMonitor(), NULL); //tela cheia
	}
	else
	{
		janela = glfwCreateWindow(larg, alt, titulo.c_str(), NULL, NULL); //janela
	}
	
	

	//veririca se a janela foi criada com sucesso
	if (!janela)
    {
        glfwTerminate();
        ERRO(); //termina a execução do programa
    }

	//cria o contexto de renderização
    glfwMakeContextCurrent(janela);


	//iniciliza a biblioteca GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ERRO(); //termina a execução do programa
	} 
		

	//VSYNC
	//Monitor de 240HZ -> 0= DESATIVADO, 1= 240FPS, 2=120FPS, 3=80FPS
	glfwSwapInterval(1); 


	return janela;
}

void destruirJanela(GLFWwindow *jan)
{
    //remove a janela 
	glfwDestroyWindow(jan);
	glfwTerminate();	
}

void imprimirInfOpenGL()
{
    GLFWwindow* janela = glfwGetCurrentContext();
    if (!janela) {
        cerr << "Nenhum contexto OpenGL está ativo no momento.\n";
        return;
    }


    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor   = glGetString(GL_VENDOR);
    const GLubyte* version  = glGetString(GL_VERSION);

    if (renderer && vendor && version) {
        cout << "Renderer: " << renderer << "\n";
        cout << "Vendor:   " << vendor   << "\n";
        cout << "Version:  " << version  << "\n";
    } else
        ERRO();


}



