#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

//__________________________________________
//Turning of the scene with the right mouse button
//_________________________________________

//width und height of the windows
int _width = 500;
int _height = 500;

//start position of the arm-part rotations and the multiBucket-Rotation
int _ArmRotation1 = -40;
int _ArmRotation2 = 40;
int _ArmRotation3 = 90;
int _multiBucketRotation = -90;

//rotation values of the whole scene
double _m_angleY;
double _m_angleX;

//last position of the mouse when dragged or klicked
int _mouseLastPosionX;
int _mouseLastPosionY;

//how much the x rotation should be for the mouse drag
double _rotationValueX = 1.0;

//x movement of the excavator 
double _excavatorMovement = 0;

//rotation of the cabin
double _cabinRotation = 0;

//drag is false on initial (if the left mouse button is pressed it will be drawn)
bool drag = false;

//variables used to calculate the frames per second
static double elapsedTime = 0;
static double frameCount = 0;
static double fps = 0;

//rotation value of the rotation animation of the excavator
static double _rotationValueExc = 0;

//rotationRadios of the excavator-animation
static double _rotationRadius = 10;

//pause is used to stop or resume the animation
static bool pause = false;

//velocity of the animation
int _velocity = 5;				//default value is 5 seconds per rotation


//materialSpecular and materialShininess can be used to change the initial settings of the light source
static const GLfloat materialSpecular[] = { 1.0, 0.95, 0.95, 1.0 };
static const GLfloat materialShininess = 13.0;

//the following colors can be used to set either the ambient or diffuse material color
static const GLfloat yellow[] = { 1.0, 0.8, 0.0, 1.0 };
static const GLfloat yellow2[] = { 1.0, 0.79, 0.0, 1.0 };
static const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static const GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat darkGray[] = { 0.2, 0.2, 0.2, 1 };
static const GLfloat ligtGray[] = { 0.8, 0.8, 0.8, 1 };
static const GLfloat gray[] = { 0.05, 0.05, 0.05, 1.0 };
static const GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };


//boolean variable to switch between the observer and the scene coordinate system for the light source.
static bool observer = true;

//helper function to set materials (diffuse and ambient color) in a little bit "nicer" way
void setMaterial(const GLfloat *materialDiffuse, const GLfloat *materialAmbient)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
}

//mouse event to recognize the drag
void MouseEvents(int button, int state, int x, int y) {
	switch(state)
	{
		//if button is down
	case GLUT_DOWN:
		//if the right button is down
		if (button == GLUT_RIGHT_BUTTON) {

			//sets the current position of the mouse to the last position (used to calculate the drag)
			_mouseLastPosionX = x;
			_mouseLastPosionY = y;
			
			//sets drag to true
			drag = true;
		}
		break;

	//if the right button is released again
	case GLUT_UP:
		if (button == GLUT_RIGHT_BUTTON) {
			//drag will be set to false
			drag = false;
		}
		break;
	}
}

//sets the rotation when the mouse is dragged
void rotate(int x, int y) {
	if (drag) {

		//calculates the angle to rotate the scene (0,6 is the damping factor)
		_m_angleY += (x - _mouseLastPosionX) * 0.6;
		_m_angleX += (y - _mouseLastPosionY) * 0.6;

		//sets the last position to get a delta again
		_mouseLastPosionX = x;
		_mouseLastPosionY = y;

		//repaints the scene
		glutPostRedisplay();
	}
}

//Prints the helping Text
void printHelpingText() {
	system("cls");
	std::cout << "  _____                          _             " << std::endl;
	std::cout << " | ____|_  _____ __ ___   ____ _| |_ ___  _ __ " << std::endl;
	std::cout << " |  _| \\ \\/ / __/ _` \\ \\ / / _` | __/ _ \\| '__|" << std::endl;
	std::cout << " | |___ >  < (_| (_| |\\ V / (_| | || (_) | |   " << std::endl;
	std::cout << " |_____/_/\\_\\___\\__,_| \\_/ \\__,_|\\__\\___/|_|   " << std::endl;
	std::cout << "+++++++++++++++Helping Manual++++++++++++++++++" << std::endl << std::endl;

	std::cout << "Press the right Mouse button and move it to rotate the scene." << std::endl << std::endl;
	std::cout << "KEYBOARD INPUTS:" << std::endl;
	std::cout << "[<-] and [->]			 Rotates the cabin" << std::endl;
	std::cout << "[UP] and [DOWN]			 Moves the excavator in one direction" << std::endl;
	std::cout << "[y] and [x]			 Rotation of the first excavator-arm-part" << std::endl;
	std::cout << "[c] and [v]			 Rotation of the second excavator-arm-part" << std::endl;
	std::cout << "[b] and [n]			 Rotation of the third excavator-arm-part" << std::endl;
	std::cout << "[m] and [,]			 Rotation of the multi-part-bucket" << std::endl;
	std::cout << "[SPACE]				 Start/Stop Animation" << std::endl;
	std::cout << "[1][2][3][4][5]			 Change the velocity of the animation" << std::endl;
	std::cout << "[h]			         Prints the helping menu" << std::endl;
	std::cout << "[RIGHT MOUSEBUTTON]	Rotates the entire Scene" << std::endl;


}

//events for the pop up of the helping text and for the movements of the excavator arm
void KeyboardEvents(unsigned char key, int x, int y) {
	switch (key)
	{
		//third excavator arm
	case 'b':
		//rotation restriction
		if (_ArmRotation3 > 45) {
			//rotates the arm
			_ArmRotation3 -= 5;
			//redisplays the whole scene
			glutPostRedisplay();
		}
		break;

	case 'n':
		if (_ArmRotation3 < 90) {
			_ArmRotation3 += 5;
			glutPostRedisplay();
		}
		break;

		//second excavator arm
	case 'c':
		if (_ArmRotation2 > 0) {
			_ArmRotation2 -= 5;
			glutPostRedisplay();
		}
		break;

	case 'v':
		if (_ArmRotation2 < 85) {
			_ArmRotation2 += 5;
			glutPostRedisplay();
		}
		break;

		//first excavator arm
	case 'y':
		if (_ArmRotation1 > -100) {
			_ArmRotation1 -= 5;
			glutPostRedisplay();
		}
		break;

	case 'x':
		if (_ArmRotation1 < -40) {
			_ArmRotation1 += 5;
			glutPostRedisplay();
		}
		
		break;

		//multi part bucket
	case 'm':
		if (_multiBucketRotation > -225) {
			_multiBucketRotation -= 5;
			glutPostRedisplay();
		}
		break;

	case ',':
		if (_multiBucketRotation < -60) {
			_multiBucketRotation += 5;
			glutPostRedisplay();
		}
		break;

		//prints the helping text on press of the button h
	case 'h':
		printHelpingText();
		break;

	case 'l':
		observer = std::abs(observer - 1);
		glutPostRedisplay();
		break;

		//plays and pauses the animation
	case ' ':
		pause = abs(pause - 1);
		break;

		//changes the velocity of the animation speed (1 = 1 second for one duration, 2 = 2 seconds...)
	case '1':
		_velocity = 1;
		break;

	case '2':
		_velocity = 2;
		break;

	case '3':
		_velocity = 3;
		break;

	case '4':
		_velocity = 4;
		break;

	case '5':
		_velocity = 5;
		break;

	default:
		break;
	}
}

//movement of the excavator and rotation of the cabin on arrow key
void ArrowMovement(int key, int x, int y) {
	switch (key)
	{
		//if the key "down" is pressed then the excavator moves back on the x axis (+ 0.1)
	case GLUT_KEY_DOWN:
		_excavatorMovement += 0.1;
		glutPostRedisplay();
		break;

		//if the key "up" is pressed then the excavator moves forward on the x axis (- 0.1)
	case GLUT_KEY_UP:
		_excavatorMovement -= 0.1;
		glutPostRedisplay();
		break;

	//on left and right arrow key you can rotate the cabin (restriction is 180 degrees)
	case GLUT_KEY_LEFT:
		if (_cabinRotation > -180){
			_cabinRotation -= 5;
			glutPostRedisplay();
		}
		break;

	case GLUT_KEY_RIGHT:
		if (_cabinRotation < 180) {
			_cabinRotation += 5;
			glutPostRedisplay();
		}
		break;

	default:
		break;
	}
}


//____Draws the Elements
void drawWheel() {
	//push and pop matrix to capsulate the created tranformation matrix
	glPushMatrix();
	//sets the color for the object
	setMaterial(black, darkGray);
	glScaled(0.5, 0.5, 0.6);
	if (!pause) {
		glRotated(_rotationValueExc * -_rotationRadius, 0, 0, 1);   //wheel rotation when the excavator is animated
	}
	glutSolidTorus(0.5, 1, 6, 10);
	//resets the color again
	setMaterial(white, white);		//resets the material for secure reasons
	glPopMatrix();
}

//draws the four wheels on the base
void drawWheels() {
	glPushMatrix();
	glPushMatrix();
	glTranslated(0, -2.8, 0.3);		//sets the first wheel pair (front wheels on z axis)
		drawWheel();
		glTranslated(4, 0, 0);		//second wheel is 4 behind of the first wheel on the x axis
		drawWheel();
	glPopMatrix();
	glTranslated(0, 0, -3.3);		//sets -3.3 behind on the z axis
	glPushMatrix();
		glTranslated(0, -2.8, 0); //same as above
		drawWheel();
		glTranslated(4, 0, 0);
		drawWheel();
	glPopMatrix();
	glPopMatrix();
}

//base consist of the Solid cube and rotation module (torus between base and cabin) of the excavator
void drawBase() {
	glPushMatrix();
		//sets the base to the right position
		glTranslated(2, -2.5, -1.5);
		glPushMatrix();
			setMaterial(darkGray, gray);
			glScaled(5, 0.7, 3);
			glutSolidCube(1);			//solid cube
		glPopMatrix();
		glTranslated(0, 0.75, 0);		//gets up
		glScaled(1, 0.7, 1);			//scale
		glTranslated(0, -0.15, 0);
		glRotated(_cabinRotation, 0, 1, 0);					//should rotate the torus on right or left pressed key so we need the global variable _cabinRotation
		glRotated(90, 1, 0, 0);								//flips the torus
		glutSolidTorus(0.5, 1, 4, 8);						//draws the torus
		setMaterial(white, white);
	glPopMatrix();
}

//cabin
void drawCabin() {
	glPushMatrix();
	setMaterial(yellow, gray);
	//sets the whole cabin on the right place
		glTranslated(2, -0.7, -2.1);
		glPushMatrix();
			glScaled(5, 1.5, 1.5);			//first cube
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glTranslated(1, 0, 1.5);		//1.5 on z axis because the first cube is scaled on z 1.5
			glScaled(3, 1.5, 1.5);			//second cube
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glTranslated(-1.5, 1.25, 1.5);	//third cube (cabin)
			glScaled(2, 4, 1.5);
			glutSolidCube(1);
		glPopMatrix();
		setMaterial(white, white);
	glPopMatrix();
}

//draw the arm part for the excavator arm
void drawArmPart() {
		glPushMatrix();
			glPushMatrix();
			setMaterial(yellow2, gray);
				glScaled(3, 0.75, 1);
				glutSolidCube(1);		//cube part
			glPopMatrix();
			glTranslated(1.5, 0, 0);
			glutSolidSphere(0.55, 10, 10);	//joint
		setMaterial(white, white);
		glPopMatrix();
}

//multipart bucket
void drawMultipartBucket() {
	glPushMatrix();
	setMaterial(ligtGray, white);
		glPushMatrix();					//first cube
			glScaled(1.5, 1.2, 1.5);
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0.9, 0.15, 0);	//second cube
			glScaled(0.3, 1.5, 1.5);
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
		setMaterial(yellow,yellow2);					//joint
			glTranslated(-0.9, 0.6, 0);
			glutSolidSphere(0.6, 10, 10);
			glPopMatrix();
		setMaterial(white, white);
	glPopMatrix();
}

void drawExcavatorArm() {
	glPushMatrix();
		glTranslated(-2.1, -0.1, -2);			//to get the arm-part to the right position

		glTranslated(1.5,0,0);					//rotation point of the first arm
		glRotated(_ArmRotation1, 0, 0, 1);
		glTranslated(-1.5, 0, 0);

		drawArmPart();							//draws the first arm-part
			glPushMatrix();
				glTranslated(-3, 0, 0);			//second arm
				glTranslated(1.5, 0, 0);
				glRotated(_ArmRotation2, 0, 0, 1);
				glTranslated(-1.5, 0, 0);
				drawArmPart();
				glPushMatrix();
					glTranslated(-3, 0, 0);			//third arm
					glTranslated(1.5, 0, 0);
					glRotated(_ArmRotation3, 0, 0, 1);
					glTranslated(-1.5, 0, 0);
					drawArmPart();
						glPushMatrix();
						glTranslated(-1.5, 0, 0);	//multibucket
						glRotated(_multiBucketRotation, 0, 0, 1);
						glTranslated(0.9, -0.6, 0);
						drawMultipartBucket();
						glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			glPopMatrix();
}

//draws a flat rectangle for the ground
void drawQuad() {
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 0);
	glVertex3f(-0.5, 0.5, 0);
	glVertex3f(0.5, 0.5, 0);
	glVertex3f(0.5, -0.5, 0);
	glEnd();
}

//draws the ground and sets it to the right position
void drawGround(){
	glPushMatrix();
		setMaterial(white, gray);
		glTranslated(0, -3.563, 0);
		glScaled(100, 100, 100);
		glRotated(90, 1, 0, 0);
		drawQuad();
		setMaterial(white, white);
	glPopMatrix();
}

//draws the excavator
void drawExcavator() {
	drawWheels();
	drawBase();
		glPushMatrix();							//capsulate the matrix because of the rotation
		glTranslated(2, 0, -1.5);
		glRotated(_cabinRotation, 0, 1, 0);		//rotation of the cabin and excavator arm on the press of the right and left arrow key
		glTranslated(-2, 0, 1.5);
		drawCabin();
		drawExcavatorArm();
	glPopMatrix();
}

//sets the rotation of the scene
void setSceneRotation() {
	glRotated(_m_angleX, _rotationValueX, 0, 0);
	glRotated(_m_angleY, 0, 1, 0);
}

//set ups the scene with the correct lighting and make them swichable with the global variable observer
void setUpScene() {
	if (observer) {
		glLightfv(GL_LIGHT0, GL_POSITION, new float[4]{ 0,0,0,1});

		glTranslated(0, 0, -15); //scenePosition
		setSceneRotation();
	}
	if (!observer) {
		glTranslated(0, 0, -15); //scenePosition
		setSceneRotation();
		glLightfv(GL_LIGHT0, GL_POSITION, new float[4]{ 2 ,15, 3,3});
	}
}

//sets the FPS and calculates the speed of the animation
void setFPSAndCalcAnimationSpeed() {
	static long time = clock();
	long oldTime = time;
	float diffTime;
	time = clock();
	diffTime = ((float)(time - oldTime)) / ((float)CLOCKS_PER_SEC); //differention of the time when the previous display was called and when this diplay is called

	elapsedTime += diffTime;
	frameCount++;

	if (elapsedTime > 1.0)
	{
		fps = (float)frameCount / elapsedTime;
		char buffer[256];
		sprintf(buffer, "FPS: %f", fps);
		glutSetWindowTitle(buffer);
		elapsedTime = 0.0;
		frameCount = 0.0;
	}

	if (!pause) {
		if (_rotationValueExc < -360) {			//so the animation can run infinite (no running out of long number)
			_rotationValueExc = 0;
		}
		_rotationValueExc -= (diffTime * 360) / _velocity; //calculation of animation speed
		//std::cout << _rotationValueExc << std::endl;
	}
}

//render method (callback-function)
void display()
{	//sets the FPS and calculates the speed of the animation
	setFPSAndCalcAnimationSpeed();

	//clear all pixels
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	setUpScene();

	glPushMatrix();
		glTranslated(_excavatorMovement, 0, 0);
		glTranslated(0, 0, -_rotationRadius);
		glRotated(_rotationValueExc, 0, 1, 0);
		glTranslated(0, 0, _rotationRadius);
		drawExcavator();
	glPopMatrix();
	drawGround();

	//don't wait! process buffered OpenGL routines
	glFlush();	

	glutPostRedisplay();

}

//initialization of the application. only started once.
void init() 
{
	//select clearing color (color that is used as 'background')
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    //always same normals (light intesity is on every object the same indipendend of size)
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	
	glDepthFunc(GL_LEQUAL);

	//materials
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);

	//initialize view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
	gluPerspective(50, ((float)_width / (float) _height), 0.1, 1000);

	//set matrix-mode back to model-view for rendering
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	//initialize display mode (single buffer and RGBA)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	//initialize window size and position, open window
	glutInitWindowSize(_width, _height); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL - CGR");

	//call initialization routine
	init();
	
	//register callback function to display graphics
	glutDisplayFunc(display); 

	glutKeyboardFunc(KeyboardEvents); //register KeyboardEvent function for the Keyboard inputs
	glutSpecialFunc(ArrowMovement);		//register ArrowMovement function for the Arrow-Keyboard Inputs
	glutMouseFunc(MouseEvents);			//register the MouseEvents-function for the mouse clicks
	glutMotionFunc(rotate);				//register the mouse Motion Function for the rotation of the scene
	
	//enter main loop and process events
	glutMainLoop();

	return 0;
}