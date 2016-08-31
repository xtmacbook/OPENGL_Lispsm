//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2005.

#include <Engine/OS/OSFunc.h>
#include <Base/BaseException.h>
#include <Base/StringTools.h>
#include <Base/LogTools.h>
#include <Mathematic/Vector4.h>
#include <Mathematic/Matrix4Xform.h>
#include <Mathematic/MathTools.h>
#include "FrameState.h"
#include <Engine/WindowManager/GlutKeyState.h>
#include "MathStuff.h"
#include "SceneData.h"


//handles keyboard state
Engine::GlutKeyState keyState;

//holds all global data (constants, status, ...)
FrameState* frameState(0);

//mouse navigation state
bool rotatingEye = false;
bool movingLight = false;
int xEyeBegin, yEyeBegin;
int xLightBegin, yLightBegin;

void handleInput(void) {
	using Engine::KeyState;
	using Engine::KeyConst;
	const V3& up = frameState->up;
	ActionStateHandler& state = *frameState->mActionStateHandler;
	 float fact = 0.1;
	if(!frameState->getFrames().empty())
	{
		fact = state.fastMoving()?
			(frameState->getFrames().last()/100.0f):
		(frameState->getFrames().last()/300.0f);
	}

	V3 eyePos = frameState->getEyePos();
	if(keyState.isKeyDown(KeyConst::KEY_PAGE_UP)) {
		state.setFly(true);
		eyePos[1] += fact;
	}
	if(keyState.isKeyDown(KeyConst::KEY_PAGE_DOWN)) {
		state.setFly(true);
		eyePos[1] -= fact;
	}
	if(keyState.isKeyDown(KeyConst::KEY_UP_ARROW)) {
		eyePos += frameState->getEyeDir()*fact;
	}
	if(keyState.isKeyDown(KeyConst::KEY_DOWN_ARROW)) {
		eyePos -= frameState->getEyeDir()*fact;
	}
	if(keyState.isKeyDown(KeyConst::KEY_LEFT_ARROW)) {
		eyePos += V3().unitCross(up,frameState->getEyeDir())*fact;
	}
	if(keyState.isKeyDown(KeyConst::KEY_RIGHT_ARROW)) {
		eyePos -= V3().unitCross(up,frameState->getEyeDir())*fact;
	}
	if(keyState.isKeyDown('8')) {
		const V3& viewDir = frameState->getEyeDir();
		M4::V3 axe;
		axe.unitCross(viewDir,up);
		M4 r;
		rotationDeg(r,Real(10.0)*fact,axe);
		frameState->setEyeDir(r.mulHomogenPoint(viewDir));
	}
	if(keyState.isKeyDown('5')) {
		const V3& viewDir = frameState->getEyeDir();
		M4::V3 axe;
		axe.unitCross(viewDir,up);
		M4 r;
		rotationDeg(r,Real(-10.0)*fact,axe);
		frameState->setEyeDir(r.mulHomogenPoint(viewDir));
	}
	if(keyState.isKeyDown('4')) {
		M4 r;
		rotationDeg(r,Real(10.0)*fact,M4::V3::UNIT_Y());
		frameState->setEyeDir(r.mulHomogenPoint(frameState->getEyeDir()));
	}
	if(keyState.isKeyDown('6')) {
		M4 r;
		rotationDeg(r,Real(-10.0)*fact,M4::V3::UNIT_Y());
		frameState->setEyeDir(r.mulHomogenPoint(frameState->getEyeDir()));
	}
	frameState->setEyePos(eyePos);
}

void display(void) {
	handleInput();
	frameState->render();
}

void idle() {
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	using Engine::KeyState;
	using Engine::KeyConst;
	keyState.glutMouseButton(button,state);
	const bool eyeButton = GLUT_LEFT_BUTTON == button;
	const bool lightButton = GLUT_RIGHT_BUTTON == button;
	const bool down = GLUT_DOWN == state;

	if(lightButton && down && keyState.isKeyDown(KeyConst::KEY_CTRL)) {
		//todo: check setting of point light
		frameState->setSpotLightPos(frameState->getEyePos());
		frameState->setLightDir(frameState->getEyeDir());
		return;
	}
	if(eyeButton && down) {
		rotatingEye = true;
		xEyeBegin = x;
		yEyeBegin = y;
	}
	if(eyeButton && !down) {
		rotatingEye = false;
	}
	if(lightButton && down) {
		movingLight = true;
		xLightBegin = x;
		yLightBegin = y;
	}
	if(lightButton && !down) {
		movingLight = false;
		glutPostRedisplay();
	}
}

void motion(int x, int y) {
	const double PI_2 = Math::Const<double>::pi_2();
	if(rotatingEye) {
		static double eyeXAngle = 0.0;
		static double eyeYAngle = 0.0;
		eyeXAngle -= 0.015*(x - xEyeBegin);
		eyeYAngle -= 0.015*(y - yEyeBegin);
		Math::clamp(eyeYAngle,-PI_2+0.1,PI_2-0.1);
		xEyeBegin = x;
		yEyeBegin = y;
		frameState->setEyeDir(setDir(eyeXAngle,eyeYAngle));
	}
	if(movingLight) {
		static double lightXAngle = 0.0;
		static double lightYAngle = -0.99;
		lightXAngle -= 0.005*(x - xLightBegin);
		lightYAngle += 0.005*(y - yLightBegin);
		Math::clamp(lightYAngle,-PI_2+0.01,0.0);
		xLightBegin = x;
		yLightBegin = y;
		frameState->setLightDir(setDir(lightXAngle,lightYAngle));
	}
	glutPostRedisplay();
}

void closeProgram(void) {
	delete frameState;
	glutKeyboardFunc(0);
	glutSpecialFunc(0);
	glutReshapeFunc(0);
	glutMouseFunc(0);
	glutMotionFunc(0);
	glutKeyboardUpFunc(0);
	glutSpecialUpFunc(0);
	glutIdleFunc(0);
	exit(0);
}

void keyboardUp(const unsigned char c, const int, const int) {
	const Engine::KeyState::KEY key = Engine::GlutKeyState::convertGlutKey(c);
	keyState.keyUp(key);
	switch(c) {
	case 27:
		closeProgram();
		break;
	case '0':
		frameState->bufferChange(256);
		break;
	case '1':
		frameState->bufferChange(512);
		break;
	case '2':
		frameState->bufferChange(1024);
		break;
	case '3':
		frameState->bufferChange(2048);
		break;
	case '4':
		frameState->bufferChange(4096);
		break;
	default:
		try {
			frameState->mActionStateHandler->doAction(key);
		}
		catch(BaseException&) {
//			frameState->debug() << b.getMsg() << '\n'; //debug
		}
	}
	glutPostRedisplay();
}

void keyboard(const unsigned char c, const int, const int) {
	keyState.keyDown(Engine::GlutKeyState::convertGlutKey(c));
}

void specialUp(const int c, const int, const int) {
	keyState.keyUp(Engine::GlutKeyState::convertGlutSpecialKey(c));
}

void special(const int c, const int, const int) {
	const Engine::KeyState::KEY key = Engine::GlutKeyState::convertGlutSpecialKey(c);
	keyState.keyDown(key);
	try {
		frameState->mActionStateHandler->doAction(key);
	}
	catch(BaseException&) {
		//frameState->debug() << b.getMsg() << '\n';
	}
	glutPostRedisplay();
}
void reshape(const int w, const int h) {
	frameState->onResize(w,h);
	glViewport(0,0,w,h);
	glutPostRedisplay();
	std::string title = "LispSM ";
	title += Tools::toString(glutGet(GLUT_WINDOW_WIDTH));
	title += 'x';
	title += Tools::toString(glutGet(GLUT_WINDOW_HEIGHT));
	glutSetWindowTitle(title.c_str());
}

int main(int argc, char **argv) {
	try {
		OS::detectMemoryLeaks();
		Tools::logFileName = Tools::changeFileExt(argv[0],".log");
		
		glutInitWindowSize(512,512);
		glutInitWindowPosition(710,30);
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
		glutCreateWindow("LispSM");

		//holds all global data (constants, status, ...)
		frameState = new FrameState(0, //start frame
							0.1, // eye near plane distance
							300.0, // eye far plane distance
							V3(0.0, 0.0, 0.0),  // eye position
							V3(0.0,0.0,-1.0),  // eye view dir 
							V3(0.0,10.0,0.0),  // light pos 
							V3(0.0,-0.99,0.01),  // light dir 
							C4(0.6f,0.6f,0.8f,1.0f) //fog color
							); 

		frameState->debug() << "Depth:" << glutGet(GLUT_WINDOW_DEPTH_SIZE) << "bits\n";
		frameState->debug() << "Alpha:" << glutGet(GLUT_WINDOW_ALPHA_SIZE) << "bits\n";

		glutDisplayFunc(display);
		glutIgnoreKeyRepeat(1); //true
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(special);
		glutReshapeFunc(reshape);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialUpFunc(specialUp);
		glutIdleFunc(idle);
		movingLight = true;
		motion(0,0);
		movingLight = false;
		glutMainLoop();
		closeProgram();
		return 0;
	}
	catch(BaseException &e) {
		try {
			std::cerr << e.getMsg() << '\n';
			Tools::log(e);
			closeProgram();
		}
		catch(...) { }
		return 1;
	}
}
