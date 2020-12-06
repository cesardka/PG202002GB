#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
static double xMousePos, yMousePos;
static double espacoShaders;
static string currentSticker;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Filters", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Adicionando os shaders
	addShader("../shaders/FiltroVignete.vs", "../shaders/FiltroVignete.frag");         // 0. BOOM
	addShader("../shaders/FiltroVignete.vs", "../shaders/FiltroVignete.frag");         // 1. CODE
	addShader("../shaders/FiltroVignete.vs", "../shaders/FiltroVignete.frag");         // 2. DOG MAULEY
	addShader("../shaders/FiltroVignete.vs", "../shaders/FiltroVignete.frag");         // 3. ELE EH
	addShader("../shaders/FiltroVignete.vs", "../shaders/FiltroVignete.frag");         // 4. PENSATIVO
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");     // 5. code
	addShader("../shaders/FiltroColorizacao.vs", "../shaders/FiltroColorizacao.frag"); // 6. corColorizadora
	addShader("../shaders/FiltroGrayscale.vs", "../shaders/FiltroGrayscale.frag");     // 7.
	addShader("../shaders/FiltroBinarizacao.vs", "../shaders/FiltroBinarizacao.frag"); // 8. limiar
	addShader("../shaders/FiltroInversao.vs", "../shaders/FiltroInversao.frag");       // 9.
	addShader("../shaders/FiltroLivre.vs", "../shaders/FiltroLivre.frag");             // 10.
	addShader("../shaders/FiltroVignete.vs", "../shaders/FiltroVignete.frag");         // 11.

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	Shader *shader = new Shader (vFilename.c_str(), fFilename.c_str());
	shaders.push_back(shader);
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xMousePos, &yMousePos);
		cout << "Mouse clicado na posição X [" << xMousePos << "] : Y [" << yMousePos << "]" << endl;
	}
}

void SceneManager::updatePictureShader(int posObject, int posShader)
{
	objects[posObject]->setShader(shaders[posShader]);
}

void SceneManager::placeSticker(string textureName)
{

	int textureSticker = loadTexture("../textures/" + textureName + ".png");
	GameObject* obj = new GameObject;
	obj->setPosition(glm::vec3(xMousePos, yMousePos, 1.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setTexture(textureSticker);
	obj->setShader(shaders[6]);
	objects.push_back(obj);
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (xMousePos >= 700 && xMousePos <= 800) {
		int selectedShader = int(yMousePos) / 50;
		if (selectedShader == 0)
			currentSticker = "boom";
		else if (selectedShader == 1)
			currentSticker = "coder";
		else if (selectedShader == 2)
			currentSticker = "dog-mauley";
		else if (selectedShader == 3)
			currentSticker = "ele-eh";
		else if (selectedShader == 4)
			currentSticker = "pensativo";
		else
			currentSticker = "";

		// Seleciona qual o sticker que foi selecionado de acordo com a posição que foi clicada
		// cout << "Shader  [" << selectedShader << "] de [" << shaders.size() << "]" << endl;
		if (selectedShader < shaders.size()) {
			updatePictureShader(0, selectedShader);
		}
	}
	else {
		// cout << "Sticker [" << currentSticker << "] " << endl;
		if (currentSticker != "") {
			placeSticker(currentSticker);
		}
	}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os gameObjects

	for (int i = 0; i < objects.size(); i++)
	{ 
		objects[i]->update();

		glActiveTexture(GL_TEXTURE1);
		// Se utilizar um dos shaders que precisam clippar em cima da textura... (Polaroid ou vignette)
		if (objects[i]->getShader() == shaders[10]) {
			glUniform1i(glGetUniformLocation(shaders[10]->Program, "ourTexture2"), 1);
			glBindTexture(GL_TEXTURE_2D, texturePolaroid);
		}
		else if (objects[i]->getShader() == shaders[11]) {
			glUniform1i(glGetUniformLocation(shaders[11]->Program, "ourTexture2"), 1);
			glBindTexture(GL_TEXTURE_2D, textureVignette);
		}

		objects[i]->draw();
	}
	

}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	//Carregamento das texturas (pode ser feito intercalado na criação)
	//Futuramente, utilizar classe de materiais e armazenar dimensoes, etc
	unsigned int texID = loadTexture("../textures/lena.png");
	unsigned int textureMenu = loadTexture("../textures/menu.png");

	// Carrega textura dos stickers
	unsigned int textSticker = loadTexture("../textures/boom.png");
	stickers.push_back(textSticker);
	textSticker = loadTexture("../textures/coder.png");
	stickers.push_back(textSticker);
	textSticker = loadTexture("../textures/ele-eh.png");
	stickers.push_back(textSticker);
	textSticker = loadTexture("../textures/dog-mauley.png");
	stickers.push_back(textSticker);
	textSticker = loadTexture("../textures/pensativo.png");
	stickers.push_back(textSticker);

	// Carrega objeto da Leninha
	GameObject* obj = new GameObject;
	obj->setPosition(glm::vec3(350.0f, 300.0f, 0.0));
	obj->setDimension(glm::vec3(500.0f, 500.0f, 1.0f));
	obj->setTexture(texID);
	obj->setShader(shaders[2]);
	objects.push_back(obj);
	
	// Menu de filtros e stickers
	obj = new GameObject;
	obj->setPosition(glm::vec3(750.0f, 300.0f, 1.0));
	obj->setDimension(glm::vec3(100.0f, 600.0f, 1.0f));
	obj->setTexture(textureMenu);
	obj->setShader(shaders[0]);
	objects.push_back(obj);

	shaders[5]->setInt("code", 2);
	shaders[6]->setVec3("corColorizadora", 0.0, 1.0, 0.5);
	shaders[8]->setInt("limiar", 120);

	textureVignette = loadTexture("../textures/Vignette.png");
	texturePolaroid = loadTexture("../textures/polaroid.png");

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	//corrigindo o aspecto
	float ratio = 1.0;
	float xMin = 0.0, xMax = 800.0, yMin = 0.0, yMax = 600.0, zNear = -1.0, zFar = 1.0;

	projection = glm::ortho(xMin * ratio, xMax * ratio, yMin, yMax, zNear, zFar);

	// Get their uniform location
	for (int i = 0; i < shaders.size(); i++)
	{
		shaders[i]->Use();
		GLint projLoc = glGetUniformLocation(shaders[i]->Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}
