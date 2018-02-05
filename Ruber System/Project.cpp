#define __Windows__ // define your target operating system
#include "../includes465/include465.hpp"
#include "Shape3D.hpp"
#include "Camera.hpp"
#include "MovementControl.hpp"
#include "SmartMissile.hpp"

// naming stuff
#define RUBER				0
#define UNUM				1
#define DUO					2
#define PRIMUS				3
#define SECUNDUS			4
#define SHIP				5
#define UNUM_SITE			6
#define SECUNDUS_SITE		7
#define THRUSTER_LEFT		8
#define THRUSTER_RIGHT		9
#define MISSILE				10
#define SHIP_MISSILE		11
#define UNUM_MISSILE		12
#define SECUNDUS_MISSILE	13
#define Star				14


#define FRONT_VIEW		0
#define TOP_VIEW		1
#define SHIP_VIEW		2
#define UNUM_VIEW		3
#define DUO_VIEW		4

#include "Shape3D.hpp"

int width = 640, height = 480;
char * fileName = "skynew.raw";


const GLuint starVertices = 6;
int warp = 0;
int warpPositions = 2;

// shapes
const int nModels = 14;
const int nMissiles = 3;

MovementControl* player;
SmartMissile* missile[nMissiles];
Shape3D* shape[nModels];
char* modelFile[nModels] = { "Ruber.tri", "Unum.tri", "Duo.tri", "Primus.tri", "Secundus.tri",
							"Ship.tri", "Missile_Site.tri", "Missile_Site.tri",
							"Thruster.tri", "Thruster.tri",
							"Missile.tri", "Missile.tri", "Missile.tri", "Missile.tri" };
const int nVertices[nModels] = { 264 * 3, 264 * 3, 264 * 3,	264 * 3, 264 * 3,
								1212 * 3, 864 * 3, 864 * 3,
								24 * 3, 24 * 3,
								316 * 3, 316 * 3, 316 * 3, 316 * 3 };
float modelBR[nModels];
GLuint VAO[nModels + 1];
GLuint buffer[nModels + 1];
float modelSize[nModels] = { 2000, 200, 400, 100, 150,
							100, 30, 30,
							15, 15,
							25, 25, 25, 25};
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translations[nModels] = {	glm::vec3(0), glm::vec3(4000,0,0), glm::vec3(9000,0,0),	glm::vec3(-900, 0, 0), glm::vec3(-1750, 0, 0),
									glm::vec3(5000, 1000, 5000), glm::vec3(0,200,0), glm::vec3(0,150,0),
									glm::vec3(-75, -7, 100), glm::vec3(75, -7, 100),
									glm::vec3(0, -2, -70), glm::vec3(0), glm::vec3(0), glm::vec3(0) };
glm::mat4 modelMatrix;          // set in display()
float radians[nModels] = { 0, 0.004f, 0.002f, 0.004f, 0.002f,
							0, 0, 0,
							0, 0,
							0, 0, 0, 0};

// camera
const int nCams = 5;
const glm::vec3 camView[nCams * 3] = {	// front view 
	glm::vec3(0, 10000, 20000),
	glm::vec3(0),
	glm::vec3(0, 1, 0),
	// top view
	glm::vec3(0, 20000, 0),
	glm::vec3(0),
	glm::vec3(0, 0, -1),
	// ship view
	glm::vec3(0, 300, 1000),
	glm::vec3(0, 300, 0),
	glm::vec3(0, 1, 0),
	// unum view
	glm::vec3(0, 0, -8000),
	glm::vec3(0),
	glm::vec3(0, 1, 0),
	// duo view
	glm::vec3(0, 0, -8000),
	glm::vec3(0),
	glm::vec3(0, 1, 0),
};
Camera* cam[nCams];


glm::mat4 viewMatrix;           // set in init()
int camera;


// other
GLfloat aspectRatio;
const int nTQ = 4;
int timeQuantums[nTQ] = { 5, 500, 100, 40 }; // 40 millisecond delay approximates 35 fps
int TQ = 0;
char* vertexShaderFile = "simpleVertex.glsl";
char* fragmentShaderFile = "simpleFragment.glsl";
GLuint shaderProgram;
GLuint ibo;
GLuint vTexCoord;
GLuint texture, Texture, showTexture;
int numRockets = 9;
int unumAmmo = 5;
int secundusAmmo = 5;
int updateCount = 0;
GLuint MVP;
GLuint MV;
GLuint NM;
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();
glm::mat4 modelView;
glm::mat3 NormalMatrix;
glm::mat4 starRotation;
glm::mat4 starTranslate;

GLuint debugSetOn;
int debugLightOn = 0;

GLuint isTheSun;

//Point Light vars
GLuint pointLightSetOn; // handle for bool in shader
GLuint pointLightLoc; //handle
GLuint pointLightIntensity; //handle
int pointLightOn = 1; //0 = off 1 = on
glm::vec3 PointLightIntensity = glm::vec3(1.0, 1.0, 1.0);//RBG values of the light

														 //Ambient Light vars
GLuint ambientSetOn;
GLuint ambientLightIntensity;
int ambientOn = 1;
glm::vec3 AmbientLightIntensity = glm::vec3(0.5, 0.5, 0.5);//RBG values of the light

														   //Head Light vars
GLuint headLightSetOn; // handle for bool in shader
GLuint headLightLoc; //handle
GLuint headLightIntensity; //handle
int headLightOn = 0; //0 = off 1 = on
glm::vec3 HeadLightIntensity = glm::vec3(1.0, 1.0, 1.0);//RBG values of the light



														//title
int frameCount = 0;
double currentTime;
double lastTime;
double timeInterval;
char fpsStr[150], timerStr[20];
char titleStr[100];
char baseStr[50] = "Ruber System: ";
char viewStr[15] = "Front";


static const GLfloat point[] = {
	-1,  1, -1, 1,
	-1, -1, -1, 1,
	1, -1, -1, 1,
	1,  1, -1, 1,
	1, -1, -1, 1,
	-1, 1, -1, 1,
};
static const unsigned int indices[] = { // 12 triangles, 3 vertices per triangle
	3, 0, 1,
	3, 1, 2
};

static const GLfloat texCoords[] = {
	0.0f, 0.0f,     // 0
	1.0f, 0.0f,     // 1
	1.0f, 1.0f,     // 2
	0.0f, 1.0f };




void updateTitle();

void init(void)
{
	// load the shader programs
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nModels + 1, VAO);
	glGenBuffers(nModels + 1, buffer);

	camera = 0;
	// do shape3d stuff
	for (int i = 0; i <= SECUNDUS; i++)
		shape[i] = new Shape3D(translations[i], radians[i], true, true);
	for (int i = SHIP; i <= SECUNDUS_SITE; i++)
		shape[i] = new Shape3D(translations[i], radians[i], false, true);
	for (int i = THRUSTER_LEFT; i <= THRUSTER_RIGHT; i++)
		shape[i] = new Shape3D(translations[i], radians[i], false, false);
	shape[MISSILE] = new Shape3D(translations[MISSILE], radians[MISSILE], false, true);
	for (int i = SHIP_MISSILE; i < nModels; i++)
		shape[i] = new Shape3D(translations[i], radians[i], false, false);

	shape[PRIMUS]->setAnchor(shape[DUO], radians[PRIMUS] - radians[DUO]);
	shape[SECUNDUS]->setAnchor(shape[DUO], radians[SECUNDUS] - radians[DUO]);
	shape[UNUM_SITE]->setAnchor(shape[UNUM], 0);
	shape[SECUNDUS_SITE]->setAnchor(shape[SECUNDUS], 0);
	shape[THRUSTER_LEFT]->setAnchor(shape[SHIP], 0);
	shape[THRUSTER_RIGHT]->setAnchor(shape[SHIP], 0);
	shape[MISSILE]->setAnchor(shape[SHIP], 0);



	// load the buffers from the model files
	for (int i = 0; i < nModels; i++)
	{
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram, vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius
		shape[i]->setScale(modelBR[i], modelSize[i]);
	}




	// do movementcontrol stuff
	player = new MovementControl(shape[SHIP]);

	// do missile stuff
	missile[0] = new SmartMissile(shape[SHIP_MISSILE], new Shape3D*[2]{ shape[UNUM_SITE], shape[SECUNDUS_SITE] }, 2, 20, modelSize[SHIP_MISSILE] + modelSize[UNUM_SITE]);
	missile[1] = new SmartMissile(shape[UNUM_MISSILE], &shape[SHIP], 1, 5, modelSize[UNUM_MISSILE] + modelSize[SHIP]);
	missile[2] = new SmartMissile(shape[SECUNDUS_MISSILE], &shape[SHIP], 1, 5, modelSize[SECUNDUS_MISSILE] + modelSize[SHIP]);

	// do camera stuff
	for (int i = 0; i < nCams; i++)
		cam[i] = new Camera(camView[i * 3], camView[i * 3 + 1], camView[i * 3 + 2]);

	cam[SHIP_VIEW]->setAnchor(shape[SHIP]);
	cam[UNUM_VIEW]->setAnchor(shape[UNUM]);
	cam[DUO_VIEW]->setAnchor(shape[DUO]);

	// set up the indices buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set up the indexed pyramid vertex attributes
	glBindVertexArray(buffer[Star]);

	//  initialize a buffer object
	glEnableVertexAttribArray(buffer[Star]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Star]);

	//glBufferData( GL_ARRAY_BUFFER, sizeof(point) + sizeof(texCoords) + sizeof(normal), NULL, GL_STATIC_DRAW );
	glBufferData(GL_ARRAY_BUFFER, sizeof(point) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point), point);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point), sizeof(texCoords), texCoords);

	// set up vertex arrays (after shaders are loaded)
	vPosition[Star] = glGetAttribLocation(shaderProgram, "vPosition");
	glVertexAttribPointer(vPosition[Star], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition[Star]);

	vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point)));
	glEnableVertexAttribArray(vTexCoord);


	//Uniforms

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	MV = glGetUniformLocation(shaderProgram, "ModelView");
	NM = glGetUniformLocation(shaderProgram, "NormalMatrix");

	pointLightSetOn = glGetUniformLocation(shaderProgram, "PointLightOn");
	pointLightLoc = glGetUniformLocation(shaderProgram, "PointLightPosition");
	pointLightIntensity = glGetUniformLocation(shaderProgram, "PointLightIntensity");

	headLightSetOn = glGetUniformLocation(shaderProgram, "HeadLightOn");
	headLightLoc = glGetUniformLocation(shaderProgram, "HeadLightPosition");
	headLightIntensity = glGetUniformLocation(shaderProgram, "HeadLightIntensity");

	ambientLightIntensity = glGetUniformLocation(shaderProgram, "AmbientLightIntensity");
	ambientSetOn = glGetUniformLocation(shaderProgram, "AmbientOn");

	debugSetOn = glGetUniformLocation(shaderProgram, "DebugOn");
	isTheSun = glGetUniformLocation(shaderProgram, "IsTheSun");

	showTexture = glGetUniformLocation(shaderProgram, "IsTexture");

	// load texture
	texture = loadRawTexture(texture, fileName, width, height);
	if (texture != 0) {
		printf("texture file, read, texture generated and bound  \n");
		Texture = glGetUniformLocation(shaderProgram, "Texture");
	}
	else  // texture file loaded
		printf("Texture in file  NOT LOADED !!! \n");



	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

	viewMatrix = glm::lookAt(camView[0], camView[1], camView[2]);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'v' || key == 'V')
		camera = (camera + nCams + 1) % nCams;
	else if (key == 'x' || key == 'X')
		camera = (camera + nCams - 1) % nCams;
	else if (key == 't' || key == 'T')
		TQ = (TQ + nTQ + 1) % nTQ;
	else if (key == 's' || key == 'S')
		player->changeSpeed();
	else if (key == 'f' || key == 'F')

	{
		if (!missile[0]->isActive())
		{
			if (numRockets > 0)
			{
				shape[SHIP_MISSILE]->setTranslationMatrix(translate(glm::mat4(), getPosition(shape[MISSILE]->getTransformMatrix())));
				shape[SHIP_MISSILE]->setRotationMatrix(shape[SHIP]->getRotationMatrix());
				missile[0]->activate();
				numRockets--;
			}
		}
	}
	else if (key == 'w' || key == 'W')
	{
		warp = (warp + warpPositions + 1) % warpPositions;
		switch (warp)
		{
		case 0:
			shape[SHIP]->controlledWarp(shape[UNUM], camView[UNUM_VIEW * 3]);
			break;
		case 1:
			shape[SHIP]->controlledWarp(shape[DUO], camView[DUO_VIEW * 3]);
			break;
		}
	}
	else if (key == 'q' || key == 'Q')
		exit(EXIT_SUCCESS);
	else if (key == 'd' || key == 'D') {
		debugLightOn = (debugLightOn + 1) % 2;
	}
	else if (key == 'p' || key == 'P') {
		pointLightOn = (pointLightOn + 1) % 2;
	}
	else if (key == 'a' || key == 'A') {
		ambientOn = (ambientOn + 1) % 2;
	}
	else if (key == 'h' || key == 'H') {
		headLightOn = (headLightOn + 1) % 2;
	}
	switch (camera)
	{
	case 0: // front camera
		strcpy(viewStr, "Front");
		break;
	case 1: // top camera
		strcpy(viewStr, "Top");
		break;
	case 2: // ship camera
		strcpy(viewStr, "Warbird");
		break;
	case 3: // unum camera
		strcpy(viewStr, "Unum");
		break;
	case 4: // duo camera
		strcpy(viewStr, "Duo");
		break;
	}
}

void specialKeyEvent(int key, int x, int y) {

	if (key == GLUT_KEY_UP  && glutGetModifiers() != GLUT_ACTIVE_CTRL)
	{
		player->setMove(1);
		player->setPitch(0);
		shape[THRUSTER_LEFT]->enable();
		shape[THRUSTER_RIGHT]->enable();
	}

	if (key == GLUT_KEY_DOWN && glutGetModifiers() != GLUT_ACTIVE_CTRL)
	{
		player->setMove(-1);
		player->setPitch(0);
	}

	if (key == GLUT_KEY_LEFT && glutGetModifiers() != GLUT_ACTIVE_CTRL)
	{
		player->setYaw(1);
		player->setRoll(0);
	}

	if (key == GLUT_KEY_RIGHT && glutGetModifiers() != GLUT_ACTIVE_CTRL)
	{
		player->setYaw(-1);
		player->setRoll(0);
	}

	if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		player->setPitch(-1);
		player->setMove(0);
	}

	if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		player->setPitch(1);
		player->setMove(0);
	}

	if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		player->setRoll(1);
		player->setPitch(0);
	}

	if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		player->setRoll(-1);
		player->setPitch(0);
	}
}

void specialKeyUpEvent(int key, int x, int y) {

	if ((key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) && glutGetModifiers() != GLUT_ACTIVE_CTRL)
	{
		player->setMove(0);
		shape[THRUSTER_LEFT]->disable();
		shape[THRUSTER_RIGHT]->disable();
	}

	if ((key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) && glutGetModifiers() != GLUT_ACTIVE_CTRL)
		player->setYaw(0);

	if ((key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) && glutGetModifiers() == GLUT_ACTIVE_CTRL)
		player->setPitch(0);

	if ((key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) && glutGetModifiers() == GLUT_ACTIVE_CTRL)
		player->setRoll(0);

	if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R)
	{
		player->setRoll(0);
		player->setPitch(0);
		player->setMove(0);
		player->setYaw(0);
	}
}

void display(void)
{


	glm::vec3 RuberPos;
	glm::vec3 ShipPos;
	int isSun;
	int isTexture;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	isTexture = 1;
	glDepthMask(false);
	glBindVertexArray(VAO[Star]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Star]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUniform1i(showTexture, isTexture);

	//glm::mat4 rotCam = cameras[camerapost]->object->getRotationMatrix() * cameras[camerapost]->object->getScaleMatrix();
	glm::mat4 rotCam;
	if (cam[camera]->getAnchor() == NULL)
		rotCam = glm::mat4();
	else
		rotCam = cam[camera]->getAnchor()->getRotationMatrix() * cam[camera]->getAnchor()->getScaleMatrix();

	ModelViewProjectionMatrix = projectionMatrix * rotCam;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), 1.57f, glm::vec3(0.0, 1.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * rotCam * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), -1.57f, glm::vec3(0.0, 1.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * rotCam * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), 3.14159265f, glm::vec3(0.0, 1.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * rotCam * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), 1.57f, glm::vec3(1.0, 0.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * rotCam * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), -1.57f, glm::vec3(1.0, 0.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * rotCam * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	isTexture = 0;
	glUniform1i(showTexture, isTexture);
	glDepthMask(true);

	for (int i = 0; i < nModels && shape[i]->isEnabled(); i++)
	{
		if (i == 0) {
			isSun = 1;
		}
		else {
			isSun = 0;
		}


		//modelMatrix = glm::translate(glm::mat4(), translate[m]) * glm::scale(glm::mat4(), glm::vec3(scale[m]));
		modelMatrix = shape[i]->getModelMatrix();
		viewMatrix = cam[camera]->getViewMatrix();

		modelView = viewMatrix * modelMatrix;
		NormalMatrix = glm::mat3(modelView);

		ShipPos = getPosition((viewMatrix * shape[5]->getModelMatrix()));
		RuberPos = getPosition((viewMatrix * shape[0]->getModelMatrix()));

		// glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MV, 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glUniformMatrix3fv(NM, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

		glUniform1i(pointLightSetOn, pointLightOn);
		glUniform3fv(pointLightLoc, 1, glm::value_ptr(RuberPos)); // set location of PointLight
		glUniform3fv(pointLightIntensity, 1, glm::value_ptr(PointLightIntensity)); //sets RGB values in shader

		glUniform1i(headLightSetOn, headLightOn);
		glUniform3fv(headLightLoc, 1, glm::value_ptr(ShipPos)); // set location of HeadLight
		glUniform3fv(headLightIntensity, 1, glm::value_ptr(HeadLightIntensity)); //sets RGB values in shader

		glUniform1i(ambientSetOn, ambientOn);
		glUniform3fv(ambientLightIntensity, 1, glm::value_ptr(AmbientLightIntensity));

		glUniform1i(debugSetOn, debugLightOn);
		glUniform1i(isTheSun, isSun);


		glBindVertexArray(VAO[i]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[i]);
	}




	glutSwapBuffers();

	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;

	if (timeInterval >= 1000)
	{
		sprintf(fpsStr, " Warbird: %d Unum: %d Secundus: %d U/S: %d F/S: %4d View: ", (int)numRockets, (int)unumAmmo, (int)secundusAmmo, (int)updateCount, (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateCount = 0;
		updateTitle();
	}
}

void updateTitle()
{
	strcpy(titleStr, baseStr);
	strcat(titleStr, timerStr);
	strcat(titleStr, fpsStr);
	strcat(titleStr, viewStr);
	// printf("title string = %s \n", titleStr);
	glutSetWindowTitle(titleStr);
}

// set viewport and projectionMatrix on window resize events
void reshape(int width, int height)
{
	float aspectRatio = (float)width / (float)height;
	float FOVY = glm::radians(60.0f);
	glViewport(0, 0, width, height);
	printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n", FOVY, width, height, aspectRatio);
	projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f);
}

// Use with Idle and intervalTimer functions 
void update()
{
	updateCount++;
	player->update();

	for (int i = 0; i < nModels; i++) //sun, planets
		shape[i]->update();

	if (distance(shape[UNUM_SITE]->getPos(), shape[SHIP]->getPos()) < 5000)
	{
		if (unumAmmo > 0)
		{
			if (!missile[2]->isActive())
			{
				shape[UNUM_MISSILE]->setTranslationMatrix(translate(glm::mat4(), getPosition(shape[UNUM_SITE]->getTransformMatrix())));
				shape[UNUM_MISSILE]->setRotationMatrix(shape[UNUM_SITE]->getRotationMatrix());
				missile[2]->activate();
				unumAmmo--;
			}
		}
	}

	if (distance(shape[SECUNDUS_SITE]->getPos(), shape[SHIP]->getPos()) < 5000)
	{
		if (secundusAmmo > 0)
		{
			if (!missile[2]->isActive())
			{
				shape[SECUNDUS_MISSILE]->setTranslationMatrix(translate(glm::mat4(), getPosition(shape[SECUNDUS_SITE]->getTransformMatrix())));
				shape[SECUNDUS_MISSILE]->setRotationMatrix(shape[SECUNDUS_SITE]->getRotationMatrix());
				missile[2]->activate();
				secundusAmmo--;
			}
		}
	}

	for (int i = 0; i < nMissiles; i++)
		missile[i]->update();

	for (int i = 0; i < nCams; i++)
		cam[i]->update();
}

// interval timer for synching?
void intervalTimer(int i)
{
	glutTimerFunc(timeQuantums[TQ], intervalTimer, 1);
	update();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
# ifdef __Mac__
	// Can't change the version in the GLUT_3_2_CORE_PROFILE
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
# endif
# ifndef __Mac__
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
# endif
	glutInitWindowSize(800, 600);
	// set OpenGL and GLSL versions to 3.3 for Comp 465/L, comment to see highest versions
# ifndef __Mac__
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
# endif
	glutCreateWindow("RUBER SYSTEM");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else
	{
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	// initialize scene
	init();

	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyEvent);
	glutSpecialUpFunc(specialKeyUpEvent);
	glutTimerFunc(timeQuantums[TQ], intervalTimer, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}