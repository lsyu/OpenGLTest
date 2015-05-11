#include "core/Application.h"

#include "event/AbstractEvent.h"
#include "event/EventMouseClick.h"

#include "gui/GUILayer.h"
#include "gui/GUIFactory.h"

//#include "abstractlayer.h"
#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"
#include "IL/il.h"

//#include "core/ogl/shader.h"
//#include "core/ogl/vertexbufferobject.h"

//#include "core/lua/resourcemanager.h"
//#include "core/objects/gui/entity2dfactory.h"

#include <iostream>
#include <chrono>
#include <ctime>


CApplication::CApplication() : CSingleton<CApplication>(),
	m_title("Unknown"), m_pathToApp(""), m_isFullScreen(false), m_position({ 0, 0 }), m_size({ 640, 480 }), m_displaySize({ 0, 0 }),
	m_depth(EColorDepth::_32), m_guiLayer(nullptr), m_windowId(0), m_secOfLastFrame(0)
{
}

void CApplication::initialize(int &argc, char** argv)
{
	std::string fileSeparator = "\\";
	m_pathToApp = std::string(argv[0]);
	size_t len = m_pathToApp.find_last_of(fileSeparator);
	m_pathToApp = m_pathToApp.substr(0, len) + fileSeparator;

	// Инициализация glut
	glutInit(&argc, argv);
	m_displaySize = {float(glutGet(GLUT_SCREEN_WIDTH)), float(glutGet(GLUT_SCREEN_HEIGHT))};
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	std::srand(unsigned(std::time(0)));
}

void CApplication::setWindowTitle(const std::string &title)
{
	this->m_title = title;
}

const std::string& CApplication::getWindowTitle() const
{
	return m_title;
}

void CApplication::setFullScreen(bool fullScreen)
{
	this->m_isFullScreen = fullScreen;
}

bool CApplication::isFullScreen() const
{
	return m_isFullScreen;
}

void CApplication::setWindowPosition(const CVector2& position)
{
	this->m_position = position;
}

const CVector2& CApplication::getWindowPosition() const
{
	return m_position;
}

void CApplication::setWindowSize(const CSize& size)
{
	this->m_size = size;
}

const CSize& CApplication::getWindowSize() const
{
	return m_size;
}

const CSize& CApplication::getDisplaySize() const
{
	return m_displaySize;
}

void CApplication::setColorDepth(EColorDepth depth)
{
	this->m_depth = depth;
}

EColorDepth CApplication::getColorDepth() const
{
	return m_depth;
}

float CApplication::getSecondOfLastFrame() const
{
	return m_secOfLastFrame;
}

CVector2 CApplication::getRelativeCoordinateFromAbsolute(const CVector2& absolute_coordinate)
{
	float x = (absolute_coordinate.x / m_instance->m_size.width - 0.5f) * 2.0f;
	float y = (absolute_coordinate.y / m_instance->m_size.height - 0.5f) * 2.0f;
	return{ x, y };
}

float CApplication::getAspectRatio() const
{
	return m_size.height != 0 ? m_size.width / m_size.height : 1.0f;
}


void CApplication::key(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q' || key == 'Q')
		glutLeaveMainLoop();
}

void CApplication::mouse(int button, int state, int x, int y)
{
	CGUIFactory::getInstance()->clearEntity4Event();

	EMouseButton btn = EMouseButton::left;
	if (button == GLUT_RIGHT_BUTTON)
		btn = EMouseButton::right;
	else if (button == GLUT_MIDDLE_BUTTON)
		btn = EMouseButton::middle;

	EMouseState s = (state == GLUT_UP ? EMouseState::up : EMouseState::down);

	CEventMouseClick e(x, y, btn, s);
	if (m_instance->m_guiLayer && !m_instance->m_guiLayer->updateGL(&e))
		glutLeaveMainLoop();
}

void CApplication::display()
{
	using namespace std::chrono;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	if (m_instance->m_guiLayer)
		m_instance->m_guiLayer->paintGL();

	drawFPS(m_instance->m_secOfLastFrame);

	glutSwapBuffers();

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<float> time_span = duration_cast<duration<float>>(t2 - t1);
	m_instance->m_secOfLastFrame = time_span.count();
}

void CApplication::drawFPS(float time_last_frame)
{
	static float time_delta = 0.0f;
	time_delta += time_last_frame;

	static int frame_cnt = 0;
	frame_cnt++;

	static std::string fps = "???";

	if (time_delta > 2.0f)
	{
		fps = std::to_string(float(frame_cnt) / time_delta);
		fps.resize(5, ' ');
		time_delta = 0.0f;
		frame_cnt = 0;
	}

	void* font = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor3f(1.0f, 0.0f, 0.0f);
	float x = 0.98f - float(glutBitmapLength(font, (const unsigned char*)fps.c_str())*2.0f) / m_instance->m_size.width;
	float y = -1.0f + float(glutBitmapHeight(font)) / m_instance->m_size.height;
	glRasterPos2d(x, y);
	for (char symbol : fps)
	{
		glutBitmapCharacter(font, symbol);
	}
}

void CApplication::idle()
{
	if (m_instance->m_guiLayer)
		m_instance->m_guiLayer->updateGL();

	glutPostRedisplay();
}

void CApplication::exec()
{
	if (m_guiLayer) {
		glutInitContextVersion(2, 1);
		glutInitWindowPosition(int(m_position.x), int(m_position.y));
		glutInitWindowSize(int(m_size.width), int(m_size.height));
		m_windowId = glutCreateWindow(m_title.c_str());
		glutKeyboardFunc(&CApplication::key);
		glutMouseFunc(&CApplication::mouse);
		glutDisplayFunc(&CApplication::display);
		glutIdleFunc(&CApplication::idle);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();

		ilInit();
		
		if (m_instance->m_guiLayer)
			m_instance->m_guiLayer->prepareGL();

		glutMainLoop();
	}
}

void CApplication::close()
{
	glutLeaveMainLoop();
}

void CApplication::setGUI(AbstractLayer *gui)
{
	if (m_guiLayer && m_guiLayer != gui)
		delete m_guiLayer;
	m_guiLayer = gui;
}

std::string CApplication::getApplicationFolder() const
{
	return m_pathToApp;
}

float CApplication::getTimeLastFrame() const
{
	return m_secOfLastFrame;
}