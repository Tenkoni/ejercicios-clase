/*---------------------------------------------------------*/
/* ----------------   Práctica 7 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Alumno: Miramontes Sarabia Luis Enrique ---------------*/
/*------------- Visual Studio 2017  ---------------------------*/
/*------------- Práctica 7 trabajo en clase  ---------------------------*/
#include "esfera.h"
#include "camera.h"

Esfera my_sphere(1.0);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, lightVAO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void myData(void);
void display(void);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

float	sol = 0.0f,
year = 0.0f,
day = 0.0f,
moon = 0.0f,
mars_year = 0.0f,
jupiter_year = 0.0f;

std::vector<float> planets(8);
std::vector < glm::mat4 > savestate(1.0f);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//To configure Second Objecto to represet Light
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void animate(void)
{
	sol += 0.3f;
	planets.at(0) += 0.112f; //merc
	planets.at(1) += 0.0500f; //venus
	planets.at(2) += 0.0300f; //tierra
	planets.at(3) += 0.0150f; //marte
	planets.at(4) += 0.0025f; //jupiter
	planets.at(5) += 0.00125f; //saturno
	planets.at(6) += 0.0003125f; //urano
	planets.at(7) += 0.00015625f; //neptuno
}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	//Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_light_Gouraud.vs", "shaders/shader_light_Gouraud.fs");
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	//To Use Lighting
	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to inlcude Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders}
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquE

	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//model = glm::translate(model, lightPos);
	lampShader.setMat4("model", model);
	my_sphere.render();	//Sun


	projectionShader.use();
	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(0)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(3, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(0)), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.15, 0.15, 0.15));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)139 / 255, (float)126 / 255, (float)110 / 255);
	projectionShader.setVec3("diffuseColor", (float)139 / 255, (float)126 / 255, (float)110 / 255);
	projectionShader.setVec3("specularColor", (float)139 / 255, (float)126 / 255, (float)110 / 255);
	my_sphere.render();	//Mercury


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(1)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(5, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(1)), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.35, 0.35, 0.35));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)232 / 255, (float)226 / 255, (float)208 / 255);
	projectionShader.setVec3("diffuseColor", (float)232 / 255, (float)226 / 255, (float)208 / 255);
	projectionShader.setVec3("specularColor", (float)232 / 255, (float)226 / 255, (float)208 / 255);
	my_sphere.render();	//Venus


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(7, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.35, 0.35, 0.35));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)57 / 255, (float)108 / 255, (float)145 / 255);
	projectionShader.setVec3("diffuseColor", (float)57 / 255, (float)108 / 255, (float)145 / 255);
	projectionShader.setVec3("specularColor", (float)57 / 255, (float)108 / 255, (float)145 / 255);
	my_sphere.render(); //Tierra
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.45, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render(); //Luna

	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(3)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(9, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(3)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)174 / 255, (float)142 / 255, (float)98 / 255);
	projectionShader.setVec3("diffuseColor", (float)174 / 255, (float)142 / 255, (float)98 / 255);
	projectionShader.setVec3("specularColor", (float)174 / 255, (float)142 / 255, (float)98 / 255);
	my_sphere.render();	//Marte
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(3) * 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.35, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(3) * 5), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Fobos
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(3) * 4), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.35, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(3) * 3), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Deimos


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(4)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(11, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(4)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.8, 0.8, 0.8));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)161 / 255, (float)124 / 255, (float)86 / 255);
	projectionShader.setVec3("diffuseColor", (float)161 / 255, (float)124 / 255, (float)86 / 255);
	projectionShader.setVec3("specularColor", (float)161 / 255, (float)124 / 255, (float)86 / 255);
	my_sphere.render();	//Jupiter
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.3, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Europa
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Ganymede
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Io
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.3, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Callisto


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(5)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(14, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(5)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.65, 0.65, 0.65));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)228 / 255, (float)207 / 255, (float)154 / 255);
	projectionShader.setVec3("diffuseColor", (float)228 / 255, (float)207 / 255, (float)154 / 255);
	projectionShader.setVec3("specularColor", (float)228 / 255, (float)207 / 255, (float)154 / 255);
	my_sphere.render();	//Saturno
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Enceladus
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Titan
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Mimas
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Dione
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1.0, 0.0, 1.000));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	projectionShader.setVec3("diffuseColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	projectionShader.setVec3("specularColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	my_sphere.render();	//ring1


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(6)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(18, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(6)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)165 / 255, (float)193 / 255, (float)207 / 255);
	projectionShader.setVec3("diffuseColor", (float)165 / 255, (float)193 / 255, (float)207 / 255);
	projectionShader.setVec3("specularColor", (float)165 / 255, (float)193 / 255, (float)207 / 255);
	my_sphere.render();	//Uranus
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Umbriel
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Titania
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Oberon
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Ariel
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 90), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.0, 1.0, 1.000));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	projectionShader.setVec3("diffuseColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	projectionShader.setVec3("specularColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	my_sphere.render();	//Ring1


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(7)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(23, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(7)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.52, 0.52, 0.52));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)135 / 255, (float)163 / 255, (float)203 / 255);
	projectionShader.setVec3("diffuseColor", (float)135 / 255, (float)163 / 255, (float)203 / 255);
	projectionShader.setVec3("specularColor", (float)135 / 255, (float)163 / 255, (float)203 / 255);
	my_sphere.render();	//Neptune
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Triton
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Nereid
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Proteus
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("diffuseColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	projectionShader.setVec3("specularColor", (float)243 / 255, (float)228 / 255, (float)225 / 255);
	my_sphere.render();	//Galatea
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 90), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1.0, 0.0, 1.0));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	projectionShader.setVec3("diffuseColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	projectionShader.setVec3("specularColor", (float)200 / 255, (float)222 / 255, (float)246 / 255);
	my_sphere.render();	//ring1




	//glDrawArrays(GL_TRIANGLES, 0, 36);	//Light



	

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);	//Light


	//glBindVertexArray(0);

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 7", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}