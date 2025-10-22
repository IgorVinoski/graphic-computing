#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <map>
#include <chrono>
#include <Eigen/Dense>

#include <filesystem> // C++17
namespace fs = std::filesystem;

#include <glad/glad.h> //GLAD
#include <GLFW/glfw3.h> //GLFW

//ImGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


using namespace std;
using namespace Eigen;
using namespace chrono;
using namespace ImGui;


#include "opengl_func.hpp"
#include "shader.hpp"
#include "CGMath.hpp"
#include "objeto3d.hpp"
#include "erro.hpp"
#include "camera.hpp"


struct Cena {
    static Cena& instancia() {
        static Cena unica; // A variável só é criada uma única vez, matendo seu valor entre as chamadas subsequentes e não é destruída quando a função termina.
        return unica;
    }

	//atributos da cena
    bool wireframe = false;
	bool cameraAtiva = false;
	bool usarLuzAmbiente = true;
	bool usarLuzDifusa = true;
	bool usarLuzEspecular = true;
	bool usarAtenuacao = true;
	bool animObjeto =  true;
	float animAng = 0.0f;
	float animVel = 0.02f;
	Camera camera;
	LuzPontual luz;
	Vector3f posInicialLuz = Vector3f(0, 4, 0);
	Vector3f posInicialCamera = Vector3f(0, 20, 15);

	vector<Objeto3D> objetos;
	string nomeObjetoLuz = "cena-luz-lampada";

private:
	Cena() = default; 

	//= delete informa ao compilador que a função foi intencionalmente desabilitada
	Cena(const Cena&) = delete; //proibe cópia -> Cena a = Cena::instancia(); 
	Cena& operator=(const Cena&) = delete; //proibe atribuição - > Cena a; a = Cena::instancia(); 
};


//eventos de mouse e teclado da janela
void keyEvent(GLFWwindow *win, int key, int scancode, int action, int mods);
void mouseEvent(GLFWwindow *win, double x, double y);
void renderizarGUI();


int main(void) 
{

	//O diretório de trabalho é a pasta onde estão os arquivos do projeto
	if (fs::current_path().filename() == "build") 
		fs::current_path(fs::current_path().parent_path()); 

	

	//--------JANELA--------
	GLFWwindow * janela = criarJanela(1024, 768, "Proj_OpenGL_Luz_Pontual", false); //inicializa as bibliotecas GLAD e GLFW. Cria-se o contexto do opengl e janela
	
	
	//--------Versao OpenGL--------
	imprimirInfOpenGL();
	
	//--------SHADERS--------
	//compila os shaders
	map<ShaderTipo, GLuint> shaders; //array de shaders compilados
	shaders[SHADER_COR_SOLIDA] = criarPrograma ("shaders/corSolidaVtx.glsl", "shaders/corSolidaFrag.glsl");
	shaders[SHADER_COR_VERT] = criarPrograma ("shaders/corVerticeVtx.glsl", "shaders/corVerticeFrag.glsl");
	shaders[SHADER_TEX] = criarPrograma ("shaders/texturaVtx.glsl", "shaders/texturaFrag.glsl");
	shaders[SHADER_MATERIAL_ILUM] = criarPrograma ("shaders/ilumMaterialVtx.glsl", "shaders/ilumMaterialFrag.glsl");
	shaders[SHADER_TEX_ILUM] = criarPrograma ("shaders/ilumTexturaVtx.glsl", "shaders/ilumTexturaFrag.glsl");

	for (const auto& [tipo, prog] : shaders)
	{
		if(!prog) //verifica se foi compilado com sucesso
		{
			glfwDestroyWindow(janela);
			glfwTerminate();
			ERRO(); //termina a execução
		}
	}

	
	//--------Projecao--------
	float vFOV=60.0f, zNear=0.1f, zFar=200.0f;
	int largJan, altJan;
	glfwGetWindowSize(janela, &largJan, &altJan);
	Matrix4f matProj = getMatProj(largJan, altJan, vFOV, zNear, zFar);
	

	//--------CENA--------
	Cena& cena = Cena::instancia(); //pega a instancia única da cena, acessível em qualquer lugar, qualquer função
	
	if(cena.cameraAtiva)
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //desativar o cursor do mouse, permitindo a rotação da câmera

	cena.camera = Camera(cena.posInicialCamera, Vector3f(0, 0, 0));


	//--------Modelos 3D--------
	string nomeArq = "../Objetos3D/lista_objetos_exemplo07.txt";
	ifstream arq(nomeArq);
	fs::path dir = fs::absolute(nomeArq).parent_path();
    string linha;
    while (arq && getline(arq, linha))
	{
		if (linha.empty() || linha[0] == '#')
			continue;
		
		if (!linha.empty() && linha.back() == '\r') linha.pop_back();
		fs::path ph = dir / linha;
        string ext = ph.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });
        if (ext == ".obj" && fs::exists(ph))
			carregarOBJ(cena.objetos, ph.string(), ph.stem().string());
		else if (ext == ".ply" && fs::exists(ph))
			carregarPLY(cena.objetos, ph.string(), ph.stem().string());
    }
	


	int somaTriangulos = 0;
	for (const auto& obj : cena.objetos)
	{
		cout << "Obj[" << obj.nome << "] posicao: " << obj.posicao.transpose() << endl;
		cout << "Obj[" << obj.nome << "] triangulos: " << obj.nVertices / 3 << endl << endl;
		somaTriangulos += obj.nVertices / 3;
	}
	cout << "Total de triangulos na cena: " << somaTriangulos << endl << endl;


	//--------ILUMINACAO--------
	cena.luz.posicao = cena.posInicialLuz;
	

	//--------EVENTOS--------
	glfwSetKeyCallback(janela, keyEvent); //registra o evento de teclado para função keyEvent
	glfwSetCursorPosCallback(janela, mouseEvent ); 


	//--------CONFIGURACOES DO OPENGL--------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo da tela - > RGBA

	//ativar o teste de profundidade para cada fragmento processado dentro do fragment shader
	//antes de cada fragmento ser desenhado é realizada uma comparação com o valor de profundidade
	glEnable(GL_DEPTH_TEST);

	//define qual algoritmo sera usado para comparar os valores de profundidade dentro do fragment shader
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
	glDepthFunc(GL_LESS);



	// Inicializa ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(janela, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	
	

	//--------LOOP DE RENDERIZACAO--------
	while (!glfwWindowShouldClose(janela) && glfwGetKey(janela, GLFW_KEY_ESCAPE) != GLFW_PRESS) //a cada iteração, um quadro é renderizado!
	{

		
		//Limpa os buffers que armazenam os valores de pronfundidade de cada fragmento para seu valor inicial (1.0)
		//Caso isso não seja feito, os fragmentos de novos triângulos serão comparados com os fragmentos renderizados do quadro anterior!
		glClear(GL_DEPTH_BUFFER_BIT);

		//Limpa os buffers que armazenam os valores de cor
		glClear(GL_COLOR_BUFFER_BIT);



		//wireframe		
		if(cena.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//pega a matriz lookat atualizada pela câmera
		Matrix4f matPV = matProj * getMatLookAt(cena.camera);
		
		
		//renderiza cada objeto da cena
		for (size_t i = 0; i < cena.objetos.size(); ++i)
		{
			Objeto3D& obj = cena.objetos[i];

		


			Matrix4f matModel = Matrix4f::Identity(); //matriz identidade
			

			ShaderTipo tipo = definirShader(obj); //escolhe o shader com base no objeto a ser renderizado
			GLuint programa = shaders[tipo];
    		glUseProgram(programa);


			//-------fazer as animações e transformações geométricas do objeto
			if(obj.nome == cena.nomeObjetoLuz)
				matModel =  getMatTranslacao(obj.posicao) *  getMatTranslacao(cena.luz.posicao - Vector3f(0, 0.1, 0)) * getMatTranslacao(-obj.posicao); 
			if(obj.nome == "cena-luz-chaleira")
			{
				if(cena.animObjeto)
				{
					cena.animAng += cena.animVel;
					if (cena.animAng > 0.0f)
						cena.animAng = fmod(cena.animAng, 2.0f * M_PI); //depois de 360º(2Pi), pega o resto (angulo-360)
					else if (cena.animAng < 0.0f)
						cena.animAng = fmod(cena.animAng, -2.0f * M_PI);
				}
					
				matModel =  getMatTranslacao(obj.posicao) * getMatRotacaoY(cena.animAng) * getMatTranslacao(-obj.posicao); 
			}
			
			
			

			//------Enviar as matrizes e dados para os shaders, de acordo com cada objeto...
			glUniformMatrix4fv(glGetUniformLocation(programa, "matPV"), 1, GL_FALSE, matPV.data());
			glUniformMatrix4fv(glGetUniformLocation(programa, "matModel"), 1, GL_FALSE, matModel.data());
			if(tipo == SHADER_MATERIAL_ILUM || tipo == SHADER_TEX_ILUM)
			{
				glUniform3fv(glGetUniformLocation(programa, "posCamera"), 1, cena.camera.pos.data());
				
				glUniform3fv(glGetUniformLocation(programa, "luz.posicao"), 1, cena.luz.posicao.data());
				glUniform1f(glGetUniformLocation(programa, "luz.kl"), cena.luz.kl);
				glUniform1f(glGetUniformLocation(programa, "luz.kq"), cena.luz.kq);
				glUniform3fv(glGetUniformLocation(programa, "luz.Ia"), 1, cena.luz.Ia.data());
				glUniform3fv(glGetUniformLocation(programa, "luz.Id"), 1, cena.luz.Id.data());
				glUniform3fv(glGetUniformLocation(programa, "luz.Is"), 1, cena.luz.Is.data());

				glUniform1i(glGetUniformLocation(programa, "usarLuzAmbiente"), cena.usarLuzAmbiente);
				glUniform1i(glGetUniformLocation(programa, "usarLuzEspecular"), cena.usarLuzEspecular);
				glUniform1i(glGetUniformLocation(programa, "usarLuzDifusa"), cena.usarLuzDifusa);
				glUniform1i(glGetUniformLocation(programa, "usarAtenuacao"), cena.usarAtenuacao);
				

				glUniform3fv(glGetUniformLocation(programa, "mat.ka"), 1, obj.mat.ka.data());
				glUniform3fv(glGetUniformLocation(programa, "mat.kd"), 1, obj.mat.kd.data());
				glUniform3fv(glGetUniformLocation(programa, "mat.ks"), 1, obj.mat.ks.data());
				glUniform1f(glGetUniformLocation(programa, "mat.brilho"), obj.mat.brilho);
			}

				
			if(tipo == SHADER_COR_SOLIDA)
				glUniform3fv(glGetUniformLocation(programa, "cor"), 1, obj.mat.kd.data());
			else if(tipo == SHADER_TEX)
				glUniform1i(glGetUniformLocation(programa, "textura0"), 0); // GL_TEXTURE0
			else if(tipo == SHADER_TEX_ILUM)
			{
				glUniform1i(glGetUniformLocation(programa, "textura0"), 0); // GL_TEXTURE0
				glUniform1i(glGetUniformLocation(programa, "textura1"), 1); // GL_TEXTURE1
				if(obj.texturaPtrKs)
					glUniform1i(glGetUniformLocation(programa, "usarTexturaDifusa"), 1);
				else
					glUniform1i(glGetUniformLocation(programa, "usarTexturaDifusa"), 0);

			}
				

			renderizarObjeto(obj);

		}//fim loop para renderizar objeto
		

		//evento teclado dentro dentro do loop de renderização
		if(cena.cameraAtiva)
		{
			if(glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS)
				moveFrente(cena.camera);
			if(glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS)
				moveTras(cena.camera);
			if(glfwGetKey(janela, GLFW_KEY_A) == GLFW_PRESS)
				moveEsquerda(cena.camera);
			if(glfwGetKey(janela, GLFW_KEY_D) == GLFW_PRESS)
				moveDireita(cena.camera);
			if(glfwGetKey(janela, GLFW_KEY_Q) == GLFW_PRESS)
				moveCima(cena.camera);
			if(glfwGetKey(janela, GLFW_KEY_E) == GLFW_PRESS)
				moveBaixo(cena.camera);
		}

		
	
		
		
		glfwPollEvents(); //processa os eventos de teclado e mouse da janela

		renderizarGUI();

		//atualiza a janela de visualização com o quadro renderizado
		glfwSwapBuffers(janela);

		
		
		
		
	} //fim while renderizacao
	
	//--------DESALOCA MEMORIA--------
	//remove os shaders
	for (auto& [tipo, prog] : shaders)
		glDeleteProgram(prog);

	//excluirObjeto(&obj);
	for (auto& ob: cena.objetos)
		excluirObjeto(ob);
	cena.objetos.clear();


	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	destruirJanela(janela);


	return EXIT_SUCCESS;

} //fim main


void keyEvent(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	GLFWwindow* janela = glfwGetCurrentContext();
  
	Cena& cena = Cena::instancia();

	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		cena.wireframe = !cena.wireframe;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		cena.cameraAtiva = !cena.cameraAtiva;
		if(cena.cameraAtiva)
		{
			if(janela)
			{
				glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				double x, y;
				glfwGetCursorPos(janela, &x, &y);
				rotacionar(cena.camera, x, y, true);
			}	
		}
		else
		{
			if(janela)
			{
				glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	
			}	
		}
	}

}

void mouseEvent(GLFWwindow *win, double x, double y)
{
	Cena& cena = Cena::instancia();

	if(cena.cameraAtiva)
		rotacionar(cena.camera, x, y);	
}


void renderizarGUI()
{
	Cena& cena = Cena::instancia();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Seu exemplo
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 765), ImGuiCond_Once);

	ImGui::SetNextWindowCollapsed(cena.cameraAtiva, ImGuiCond_Always);
	
	ImGui::Begin("Configurações", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	{
		
		
		TextColored(ImVec4(1,1,0,1), "Câmera");

		std::stringstream ss;
		ss << "Pos câmera ("
   		<< fixed << setprecision(2)
   		<< cena.camera.pos.x() << ", "
		<< cena.camera.pos.y() << ", "
		<< cena.camera.pos.z() << ")";

		ImGui::Text("%s", ss.str().c_str());
	
		static float transCamera = normalizarValores(cena.camera.velT, 0.002f, 0.1f, 1.0f, 100.f);
		ImGui::Text("Velocidade de translação:");
		if (ImGui::SliderFloat("##velTransCam", &transCamera, 1.0f, 100.f)) {
			cena.camera.velT = normalizarValores(transCamera, 1.0f, 100.f, 0.002f, 0.1f);
		}

		static float rotCamera = normalizarValores(cena.camera.velR, 0.00001f, 0.001f, 1.0f, 100.f);
		ImGui::Text("Velocidade de rotação:");
		if (ImGui::SliderFloat("##velRotCam", &rotCamera, 1.0f, 100.f)) {
			cena.camera.velR = normalizarValores(rotCamera, 1.0f, 100.f, 0.00001f, 0.001f);
		}

		if(ImGui::Button("Redefinir posição"))
		{
			cena.camera = Camera(cena.posInicialCamera, Vector3f(0, 0, 0));
			GLFWwindow* janela = glfwGetCurrentContext();
			double x, y;
			glfwGetCursorPos(janela, &x, &y);
			rotacionar(cena.camera, x, y, true);
		}
		
		ImGui::Spacing();  
		ImGui::Separator();
		ImGui::Spacing();  
		TextColored(ImVec4(1,1,0,1), "Iluminação");

		ImGui::Checkbox("Luz Ambiente", &cena.usarLuzAmbiente);
		ImGui::Checkbox("Reflexão Difusa", &cena.usarLuzDifusa);
		ImGui::Checkbox("Reflexão Especular", &cena.usarLuzEspecular);
	
		
		ImGui::Spacing();
		ImGui::Text("Definir cores");
		if (ImGui::BeginChild("CoresLuzGroup", ImVec2(0, 105), true)) // true = com borda	
		{
			ImGui::Text("Cor Ia: "); ImGui::SameLine();
			if(ImGui::ColorEdit3("##cor_luz_ambiente", cena.luz.Ia.data()))
			{
				cout << cena.luz.Ia.transpose() << endl;
			}
			ImGui::Text("Cor Id: "); ImGui::SameLine();
			if(ImGui::ColorEdit3("##cor_luz_difusa", cena.luz.Id.data()))
			{
				cout << cena.luz.Id.transpose() << endl;
			}

			ImGui::Text("Cor Is: "); ImGui::SameLine();
			if(ImGui::ColorEdit3("##cor_luz_especular", cena.luz.Is.data()))
			{
				cout << cena.luz.Is.transpose() << endl;
			}
			if(ImGui::Button("Redefinir cores"))
			{
				cena.luz.Ia = Vector3f::Ones();
				cena.luz.Id = Vector3f::Ones();
				cena.luz.Is = Vector3f::Ones();
			}
		}
		ImGui::EndChild();


		ImGui::Spacing();
	
	
		ImGui::Text("Definir posição");

		if (ImGui::BeginChild("PosicaoLuzGroup", ImVec2(0, 135), true)) // true = com borda	
		{
			std::stringstream ss;
			ss << "Pos Luz ("
			<< fixed << setprecision(2)
			<< cena.luz.posicao.x() << ", "
			<< cena.luz.posicao.y() << ", "
			<< cena.luz.posicao.z() << ")";

			ImGui::Spacing();
			ImGui::Text("%s", ss.str().c_str());

			
			ImGui::SliderFloat("Eixo X", &cena.luz.posicao.x(), -15.0f, 15.0f, "%.1f");
			ImGui::SliderFloat("Eixo Y", &cena.luz.posicao.y(), 1.2f, 15.0f, "%.1f");
			ImGui::SliderFloat("Eixo Z", &cena.luz.posicao.z(), -15.0f, 15.0f, "%.1f");
			
			ImGui::Spacing();
			if(ImGui::Button("Redefinir posição"))
			{
				cena.luz.posicao = cena.posInicialLuz;
			}
			
		}
		ImGui::EndChild();


		ImGui::Text("Definir atenuação");

		if (ImGui::BeginChild("AtenuacaoLuzGroup", ImVec2(0, 115), true)) // true = com borda	
		{

			ImGui::Checkbox("Atenuação", &cena.usarAtenuacao);
			ImGui::Spacing();
			
			ImGui::SliderFloat("kl", &cena.luz.kl, 0.001f, 0.5f, "%.3f");
			ImGui::SliderFloat("kq", &cena.luz.kq, 0.001f, 0.5f, "%.3f");
			
			ImGui::Spacing();
			if(ImGui::Button("Redefinir atenuação"))
			{
				LuzPontual l;
				cena.luz.kl = l.kl;
				cena.luz.kq = l.kq;
			}
			
		}
		ImGui::EndChild();


		ImGui::Spacing();  
		ImGui::Separator();
		ImGui::Spacing();  
		TextColored(ImVec4(1,1,0,1), "Animação");
		static float angRotAnim = normalizarValores(cena.animVel, 0.001f, 0.1f, 1.0f, 100.f);
		ImGui::Text("Velocidade de rotação:");
		if (ImGui::SliderFloat("##velAnimaObjeto", &angRotAnim, 1.0f, 100.f)) {
			cena.animVel = normalizarValores(angRotAnim, 1.0f, 100.f, 0.001f, 0.1f);
		}
		
	}
	ImGui::End();

	

	//FPS
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 tamanhoJanela = ImVec2(100, 30);

	// Calcula posição: canto superior direito menos a largura da janela
	ImVec2 posicao = ImVec2(io.DisplaySize.x - tamanhoJanela.x - 10, 10); // margem de 10px

	ImGui::SetNextWindowPos(posicao, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.3f); // transparência opcional

	ImGui::Begin("FPS", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_AlwaysAutoResize
	);
	{
		ImVec4 cor = ImVec4(0.2f, 1.0f, 0.3f, 1.0f); // verde claro vibrante
		ImGui::TextColored(cor, "FPS: %.1f",  io.Framerate);
	}
	ImGui::End();

	// Renderiza
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}







