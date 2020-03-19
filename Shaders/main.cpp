#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "ObjModel.h"
#include "Texture.h"
#include "Shader.h"
#include "Fbo.h"
#include "stb_image.h"

#pragma comment(lib, "glew32.lib")

Shader* roomShader;
ObjModel* room;

std::vector<Shader*> shaders;
std::vector<Shader*> postProcessShaders;
std::vector<ObjModel*> models;
std::vector<float> distances;

Fbo* fbo;
int activeModel = 0;

int currentShader;

glm::ivec2 screenSize;
float rotation;
int lastTime;


#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}
const char* result_string_pointer = "SOIL initialized";


unsigned char* SOIL_load_image(	const char* filename,	int* width, int* height, int* channels,	int force_channels)
{
	unsigned char* result = stbi_load(filename,
		width, height, channels, force_channels);
	if (result == NULL)
	{
		result_string_pointer = stbi_failure_reason();
	}
	else
	{
		result_string_pointer = "Image loaded";
	}
	return result;
}

void SOIL_free_image_data(	unsigned char* img_data)
{
	if (img_data)
		free((void*)img_data);
}

enum
{
	SOIL_LOAD_AUTO = 0,
	SOIL_LOAD_L = 1,
	SOIL_LOAD_LA = 2,
	SOIL_LOAD_RGB = 3,
	SOIL_LOAD_RGBA = 4
};




static GLuint LoadCubemap(std::vector<std::string > faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


GLfloat skyboxVertices[] = {
	// Positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};
unsigned int skyboxVAO, skyboxVBO;
Shader* skyboxShader;
unsigned int cubemapTexture;

void init()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(1, 0.7f, 0.3f, 1.0f);




	//room = new ObjModel("assets/models/room/room.obj");
	//roomShader = new Shader("assets/shaders/texture");
	skyboxShader = new Shader("assets/shaders/skybox");

		// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	// Cubemap (Skybox)
	std::vector<std::string> faces;
	faces.push_back("assets/textures/skybox/lposx.png");
	faces.push_back("assets/textures/skybox/lnegx.png");

	faces.push_back("assets/textures/skybox/lposy.png");
	faces.push_back("assets/textures/skybox/lnegy.png");

	faces.push_back("assets/textures/skybox/lposz.png");
	faces.push_back("assets/textures/skybox/lnegz.png");


	cubemapTexture = LoadCubemap(faces);
	skyboxShader->use();
	glUniform1i(skyboxShader->getUniform("skybox"), 0);

	   
	currentShader = 0;
	shaders.push_back(new Shader("assets/shaders/simple"));
	shaders.push_back(new Shader("assets/shaders/multitex"));
	shaders.push_back(new Shader("assets/shaders/textureanim"));
	shaders.push_back(new Shader("assets/shaders/texture"));
	shaders.push_back(new Shader("assets/shaders/vertexanim"));
	shaders.push_back(new Shader("assets/shaders/toon"));
	shaders.push_back(new Shader("assets/shaders/pixelation"));
	shaders.push_back(new Shader("assets/shaders/explosion"));
	shaders.push_back(new Shader("assets/shaders/brick"));
	auto* reflect = new Shader("assets/shaders/reflect");
	reflect->use();
	glUniform1i(reflect->getUniform("skybox"), 0);

	shaders.push_back(reflect);

	auto* refract = new Shader("assets/shaders/refract");
	refract->use();
	glUniform1i(refract->getUniform("skybox"), 0);

	shaders.push_back(refract);


	//postProcessShaders.push_back(new Shader("assets/shaders/post/bloom"));
	postProcessShaders.push_back(new Shader("assets/shaders/post/postprocess"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/screenwave"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/fisheye"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/filmgrain"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/rain"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/pixelation"));

	models.push_back(new ObjModel("assets/models/ship/shipA_OBJ.obj"));
	distances.push_back(50);
	models.push_back(new ObjModel("assets/models/car/honda_jazz.obj"));
	distances.push_back(150);
	//	model = new ObjModel("assets/models/bloemetje/PrimroseP.obj");
	models.push_back(new ObjModel("assets/models/normalstuff/normaltest.obj"));
	distances.push_back(2);
	models.push_back(new ObjModel("assets/models/normalstuff/normaltest2.obj"));
	distances.push_back(2);

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}

	rotation = 0;
	lastTime = glutGet(GLUT_ELAPSED_TIME);

	fbo = new Fbo(4096, 4096);

}




float time = 0;

void display()
{
	fbo->bind();
	glViewport(0, 0, fbo->width, fbo->height);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glm::mat4 projection = glm::perspective(glm::radians(70.0f), screenSize.x / (float)screenSize.y, 0.01f, 2000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, distances[activeModel]), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
	model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));
	
	
	//roomShader->use();
	//roomShader->setUniform("modelMatrix", glm::scale(glm::mat4(1), distances[activeModel] * glm::vec3(1, 1, 1)));
//	roomShader->setUniform("viewMatrix", view);
	//roomShader->setUniform("projectionMatrix", projection);
	//roomShader->setUniform("s_texture", 0);

	//room->draw();


	Shader* shader = shaders[currentShader];
	shader->use();
	shader->setUniform("modelMatrix", model);
	shader->setUniform("viewMatrix", view);
	shader->setUniform("projectionMatrix", projection);

	glUniformMatrix3fv(shader->getUniform("normalMatrix"), 1, 0, glm::value_ptr(normalMatrix));								//en zet de matrix in opengl
	glUniform1f(shader->getUniform("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniform1i(shader->getUniform("s_texture"), 0);

	models[activeModel]->draw();

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader->use();
	// Remove any translation component of the view matrix
	view = glm::lookAt(Position, Position + Front, Up);

	 skyboxShader->setUniform("modelMatrix", model);
	 skyboxShader->setUniform("viewMatrix", view);
	 skyboxShader->setUniform("projectionMatrix", projection);


	// skybox cube
	 glBindVertexArray(skyboxVAO);
	 glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default



	fbo->unbind();
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(-1, -1));
	verts.push_back(glm::vec2(1, -1));
	verts.push_back(glm::vec2(1, 1));
	verts.push_back(glm::vec2(-1, 1));

	postProcessShaders[0]->use();
	glUniform1f(postProcessShaders[0]->getUniform("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniform1i(postProcessShaders[0]->getUniform("s_texture"), 0);

	fbo->use();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
	glDrawArrays(GL_QUADS, 0, verts.size());


	glutSwapBuffers();
}

void reshape(int newWidth, int newHeight)
{
	screenSize.x = newWidth;
	screenSize.y = newHeight;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == VK_ESCAPE)
		glutLeaveMainLoop();
	if (key == '[')
	{
		currentShader = (currentShader + shaders.size() - 1) % shaders.size();
		std::cout << "Shader " << currentShader << std::endl;
	}
	if (key == ']')
	{
		currentShader = (currentShader + 1) % shaders.size();
		std::cout << "Shader " << currentShader << std::endl;
	}
	if (key == ',' || key == '.')
		activeModel = (activeModel + 1) % models.size();
}

void update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = time - lastTime;


	rotation += elapsed / 1000.0f;



	glutPostRedisplay();
	lastTime = time;
}




int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1900, 1000);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Visualisatietechnieken");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);

	init();


	glutMainLoop();

}