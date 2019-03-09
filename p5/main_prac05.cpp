/*---------------------------------------------------------*/
/* ----------------   Pr當tica 5 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computaci gr畴ica e interacci humano computadora ---------------*/
/*------------- Miramontes Sarabia Luis Enrique  ---------------------------*/
/*------------- Visual Studio 2017  ---------------------------*/
/*------------- Práctica 5 trabajo en clase  ---------------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <vector>
#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotY = 0.0f;

std::vector<float> arti = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };



void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	//glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
	//glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	std::vector<glm::mat4> temp = {glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));

	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);

	projectionShader.setVec3("aColor",glm::vec3(1,1,0));
	//hombro
	model = glm::rotate(model, glm::radians(arti.at(0)), glm::vec3(0.0f, 0.0f, 1.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.9, 0.8, 0));
	glDrawArrays(GL_QUADS,0,24);


	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(2, 1, 1));
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //bicep
	model = temp.at(0);


	model = glm::translate(model, glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(arti.at(1)), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //antebrazo
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(arti.at(2)), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.5, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(1, 1, 0.5));
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //mano
	temp.at(1) = model;

	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(arti.at(3)), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.23f, 0.342f, 0.87f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //pulgar
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(arti.at(3)*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.443f, 0.605f, 0.43f));
	glDrawArrays(GL_QUADS, 0, 24); //pulgar2
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(arti.at(3)*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.54f, 0.67f, 0.35f));
	glDrawArrays(GL_QUADS, 0, 24); //pulgar3
	model = temp.at(1);


	model = glm::translate(model, glm::vec3(0.5, 0.375, 0));
	model = glm::rotate(model, glm::radians(-arti.at(4)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.85f, 0.43f, 0.234f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //indice1
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(4) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.32f, 0.234f, 0.75f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //indice2
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(4) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.342f, 0.45f, 0.63f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //indice3
	model = temp.at(1);


	model = glm::translate(model, glm::vec3(0.5, 0.125, 0));
	model = glm::rotate(model, glm::radians(-arti.at(5)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.403f, 0.28f, 0.230f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Medio1
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(5) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.4, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.32f, 0.68f, 0.30f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Medio2
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(5) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.4, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.34f, 0.54f, 0.23f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Medio3
	model = temp.at(1);


	model = glm::translate(model, glm::vec3(0.5, -0.125, 0));
	model = glm::rotate(model, glm::radians(-arti.at(6)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.8f, 0.3f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Anular1
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0.0, 0.0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(6) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.3, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.453f, 0.34f, 0.76f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Anular2
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(6) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.3, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.3f, 0.32f, 0.63f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Anular3
	model = temp.at(1);

	model = glm::translate(model, glm::vec3(0.5, -0.375, 0));
	model = glm::rotate(model, glm::radians(-arti.at(7)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.49f, 0.57f, 0.84f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Meñique1
	model = temp.at(0);


	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(7) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.76f, 0.83f, 0.54f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Meñique2
	model = temp.at(0);

	model = glm::translate(model, glm::vec3(0, -0, 0));
	model = glm::rotate(model, glm::radians(-arti.at(7) * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	temp.at(0) = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(0.89f, 0.77f, 0.64f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Meñique3
	model = temp.at(0);


	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi funci de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 0.2f;
	

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(0) < 90)
				arti.at(0) += 0.08f;
		}
		else {
			if (arti.at(0) > -90)
				arti.at(0) -= 0.08f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(1) < 0)
				arti.at(1) += 0.08f;
		}
		else {
			if (arti.at(1) > -140)
				arti.at(1) -= 0.08f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(2) < 20)
				arti.at(2) += 0.09f;
		}
		else {
			if (arti.at(2) > -40)
				arti.at(2) -= 0.09f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(3) < 80)
				arti.at(3) += 0.09f;
		}
		else {
			if (arti.at(3) > 0)
				arti.at(3) -= 0.09f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(4) < 80)
				arti.at(4) += 0.09f;
		}
		else {
			if (arti.at(4) > 0)
				arti.at(4) -= 0.09f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(5) < 80)
				arti.at(5) += 0.09f;
		}
		else {
			if (arti.at(5) > 0)
				arti.at(5) -= 0.09f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(6) < 80)
				arti.at(6) += 0.09f;
		}
		else {
			if (arti.at(6) > 0)
				arti.at(6) -= 0.09f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (arti.at(7) < 80)
				arti.at(7) += 0.09f;
		}
		else {
			if (arti.at(7) > 0)
				arti.at(7) -= 0.09f;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}