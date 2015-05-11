#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

#include "Singleton.h"
#include "Utils.h"

enum class EColorDepth : unsigned short int
{
	_8 = 8,
	_16 = 16,
	_32 = 32,
	_64 = 64
}; // enum ColorDepth

class AbstractLayer;
class CApplication : public CSingleton < CApplication >
{
	friend class CSingleton < CApplication >;
public:

	void initialize(int& argc, char** argv);
	void exec();
	void close();

	void setWindowTitle(const std::string &title);
	const std::string& getWindowTitle() const;

	void setFullScreen(bool is_full_screen);
	bool isFullScreen() const;

	void setWindowPosition(const CVector2& position);
	const CVector2& getWindowPosition() const;

	void setWindowSize(const CSize& size);
	const CSize& getWindowSize() const;

	void setColorDepth(EColorDepth depth);
	EColorDepth getColorDepth() const;

	const CSize& getDisplaySize() const;

	float getSecondOfLastFrame() const;
	static CVector2 getRelativeCoordinateFromAbsolute(const CVector2&);
	float getAspectRatio() const;
	std::string getApplicationFolder() const;

	void setGUI(AbstractLayer* gui);
	static void drawFPS(float time_delta);

	float getTimeLastFrame() const;

private:
	CApplication();


	static void key(unsigned char key, int x, int y);
	static void mouse(int button, int state, int x, int y);
	static void display();
	static void idle();


	std::string m_title;
	std::string m_pathToApp;
	bool m_isFullScreen;
	CVector2 m_position;
	CSize m_size;
	CSize m_displaySize;
	EColorDepth m_depth;
	AbstractLayer *m_guiLayer;
	int m_windowId;
	float m_secOfLastFrame;
}; // class Application

#endif /*APPLICATION_H*/

