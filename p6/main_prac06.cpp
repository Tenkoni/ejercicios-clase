/*---------------------------------------------------------*/
/* ----------------   Pr當tica 6 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computaci gr畴ica e interacci humano computadora ---------------*/
/*------------- Miramontes Sarabia Luis Enrique  ---------------------------*/
/*------------- Visual Studio 2017  ---------------------------*/
/*------------- Práctica 6 trabajo en clase  ---------------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>
#include <vector>

#define M_PI 3.14159265358979323846264338327950288
//#include "Sphere.h"

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
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

float	sol = 0.0f;
std::vector<float> planets(8);
std::vector < glm::mat4 > savestate(31.0f);

//To Buid the Sphere
//---------------------------------------------------------------------------
//const int na = 36;        // vertex grid size
const int meridianos = 200;			// vertex grid size   na -> meridianos
const int paralelos = 200;			// nb -> paralelos
const int meridianos3 = meridianos * 3;     // line in grid size
const int nn = paralelos * meridianos3;    // whole grid size
GLfloat sphere_pos[nn]; // vertex
GLfloat sphere_nor[nn]; // normal
//GLfloat sphere_col[nn];   // color
GLuint  sphere_ix[meridianos*(paralelos - 1) * 6];    // indices
GLuint sphere_vbo[4] = { -1,-1,-1,-1 };
GLuint sphere_vao[4] = { -1,-1,-1,-1 };

void sphere_init()
{
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(meridianos);
	db = (GLfloat)M_PI / GLfloat(paralelos - 1);
	// [Generate sphere point data]
	// spherical angles a,b covering whole sphere surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < paralelos; ib++, b += db)
		for (a = 0.0, ia = 0; ia < meridianos; ia++, a += da, ix += 3)
		{
			// unit sphere
			x = cos(b)*cos(a);
			y = cos(b)*sin(a);
			z = sin(b);
			sphere_pos[ix + 0] = x * r;
			sphere_pos[ix + 1] = y * r;
			sphere_pos[ix + 2] = z * r;
			sphere_nor[ix + 0] = x;
			sphere_nor[ix + 1] = y;
			sphere_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < paralelos; ib++)
	{
		for (ia = 1; ia < meridianos; ia++, iy++)
		{
			// first half of QUAD
			sphere_ix[ix] = iy;      ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos;   ix++;
			// second half of QUAD
			sphere_ix[ix] = iy + meridianos;   ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos + 1; ix++;
		}
		// first half of QUAD
		sphere_ix[ix] = iy;       ix++;
		sphere_ix[ix] = iy + 1 - meridianos;  ix++;
		sphere_ix[ix] = iy + meridianos;    ix++;
		// second half of QUAD
		sphere_ix[ix] = iy + meridianos;    ix++;
		sphere_ix[ix] = iy - meridianos + 1;  ix++;
		sphere_ix[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, sphere_vao);
	glGenBuffers(4, sphere_vbo);
	glBindVertexArray(sphere_vao[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_pos), sphere_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_ix), sphere_ix, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	i = 2; // normal
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_nor), sphere_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/*
		i=3; // color
		glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_col),sphere_col,GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
	*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void sphere_exit()
{
	glDeleteVertexArrays(4, sphere_vao);
	glDeleteBuffers(4, sphere_vbo);
}
void sphere_draw()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glBindVertexArray(sphere_vao[0]);
	//  glDrawArrays(GL_POINTS,0,sizeof(sphere_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_LINE_LOOP, sizeof(sphere_ix) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}



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

void animate(void)
{
	sol += 5.2f;
	planets.at(0) += 1.12f; //merc
	planets.at(1) += 0.500f; //venus
	planets.at(2) += 0.300f; //tierra
	planets.at(3) += 0.150f; //marte
	planets.at(4) += 0.025f; //jupiter
	planets.at(5) += 0.0125f; //saturno
	planets.at(6) += 0.003125f; //urano
	planets.at(7) += 0.0015625f; //neptuno
}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 temp01 = glm::mat4(1.0f); //Temp

	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	sphere_draw();	//Sun


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(0)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(3, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(0)), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.15, 0.15, 0.15));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)139 / 255, (float)126 / 255, (float)110 / 255));
	sphere_draw();	//Mercury


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(1)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(5, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(1)), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.35, 0.35, 0.35));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)232 / 255, (float)226 / 255, (float)208 / 255));
	sphere_draw();	//Venus


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(7, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.35, 0.35, 0.35));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)57 / 255, (float)108 / 255, (float)145 / 255));
	sphere_draw();	//Tierra
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.45, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(2)), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Luna


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(3)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(9, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(3)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)174 / 255, (float)142 / 255, (float)98 / 255));
	sphere_draw();	//Marte
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(3)*6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.35, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(3)*5), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Fobos
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(3)*4), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.35, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(3)*3), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Deimos


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(4)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(11, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(4)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.8, 0.8, 0.8));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)161 / 255, (float)124 / 255, (float)86 / 255));
	sphere_draw();	//Jupiter
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.3, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Europa
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Ganymede
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Io
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.3, 0));
	model = glm::rotate(model, glm::radians(planets.at(4) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Callisto


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(5)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(14, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(5)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.65, 0.65, 0.65));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)228 / 255, (float)207 / 255, (float)154 / 255));
	sphere_draw();	//Saturno
	//sats
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Enceladus
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Titan
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Mimas
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Dione
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(5) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1.0, 0.0, 1.000));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)138 / 255, (float)128 / 255, (float)118 / 255));
	sphere_draw();	//ring1


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(6)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(18, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(6)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)165 / 255, (float)193 / 255, (float)207 / 255));
	sphere_draw();	//uranus
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Umbriel
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Titania
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Oberon
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(6) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Ariel
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(6) * 90), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.0, 1.0, 1.000));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)183 / 255, (float)209 / 255, (float)222 / 255));
	sphere_draw();	//ring


	model = glm::rotate(model = glm::mat4(1.0f), glm::radians(planets.at(7)), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(23, 0, 0));
	model = glm::rotate(model, glm::radians(planets.at(7)), glm::vec3(0, 1, 0));
	savestate.at(0) = model;
	model = glm::scale(model, glm::vec3(0.52, 0.52, 0.52));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)135 / 255, (float)163 / 255, (float)203 / 255));
	sphere_draw();	//neptune
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 60), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 50), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Triton
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, -0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Nereid
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 40), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.6, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Proteus
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0.1, 0));
	model = glm::rotate(model, glm::radians(planets.at(7) * 30), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)243 / 255, (float)228 / 255, (float)225 / 255));
	sphere_draw();	//Galatea
	model = savestate.at(0);
	model = glm::rotate(model, glm::radians(planets.at(7) * 90), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1.0, 0.0, 1.0));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3((float)200 / 255, (float)222 / 255, (float)246 / 255));
	sphere_draw();	//ring

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
	sphere_init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);
		animate();

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
	sphere_exit();
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
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}