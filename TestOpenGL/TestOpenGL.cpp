#include "core/Application.h"

#include "gui/GUIFactory.h"
#include "gui/GUILayer.h"
#include "entities/Rectangle.h"
#include "entities/RectangleAnimated.h"
#include "entities/RectangleAnimatedUV.h"
#include "entities/Line.h"
#include "entities/Lines.h"

#include "gl/Texture.h"
#include "gl/TextureFactory.h"

class MyButtonSelectLine : public CRectangleAnimated
{
public:
	MyButtonSelectLine() : CRectangleAnimated(), m_lines(nullptr)
	{
		m_texture1 = CTextureFactory::getInstance()->loadTexture("rectangle_blue.png");
		m_texture2 = CTextureFactory::getInstance()->loadTexture("rectangle_red.png");
	}

	virtual void onClicked(const CEventMouseClick &event) override
	{
		m_lines->setNextState();
	}

	CLines *m_lines;
};

class MyButtonStart : public CRectangleAnimated
{
public:
	MyButtonStart() : CRectangleAnimated(), m_drums()
	{
		m_texture1 = CTextureFactory::getInstance()->loadTexture("rectangle_blue.png");
		m_texture2 = CTextureFactory::getInstance()->loadTexture("rectangle_red.png");
	}

	virtual void onClicked(const CEventMouseClick &event) override
	{
		for (CRectangleAnimatedUV* drum : m_drums)
		{
			if (!drum->isRunning())
				drum->run();
		}
	}

	std::vector<CRectangleAnimatedUV*> m_drums;
};

class MyGUI : public CGUILayer
{
public:
	virtual void prepareGL() override
	{
		CGUIFactory* gui_factory = CGUIFactory::getInstance();

		CRectangle* root_rect = gui_factory->createElement<CRectangle>({ -1, -1, 2, 2 }, nullptr);
		root_rect->setColor({ 0.2f, 0.2f, 0.2f });

		CRectangle* header = gui_factory->createElement<CRectangle>({ -0.99, 0.71, 1.98, 0.28 }, root_rect);
		header->setColor({ 0.25f, 0.25f, 0.25f });
		header->setLabel("CAZINO V0.1");

		CRectangle* footer = gui_factory->createElement<CRectangle>({ -0.99, -0.99, 1.98, 0.28 }, root_rect);
		footer->setColor({ 0.25f, 0.25f, 0.25f });

		MyButtonSelectLine* btn_lines = gui_factory->createElement<MyButtonSelectLine>({ -0.99, -0.99, 0.98, 1.98 }, footer);
		btn_lines->setLabel("Select lines");

		MyButtonStart* btn_game = gui_factory->createElement<MyButtonStart>({ 0.01, -0.99, 0.98, 1.98 }, footer);
		btn_game->setLabel("Start");

		CRectangle* body = gui_factory->createElement<CRectangle>({ -0.99, -0.69, 1.98, 1.38 }, root_rect);
		body->setColor({ 0.25f, 0.25f, 0.25f }); //setTexture(CTextureFactory::getInstance()->loadTexture("background.jpg"));

		CLines* lines = gui_factory->createElement<CLines>({ -1, -1, 2, 2 }, body);

		CRectangleAnimatedUV* drum1 = gui_factory->createElement<CRectangleAnimatedUV>({ -0.95f, -0.15f, 0.3f, 0.3f }, body);
		drum1->setTexture(CTextureFactory::getInstance()->loadTexture("drum1.png"));
		drum1->setRPS(5, 4.0f);

		CRectangleAnimatedUV* drum2 = gui_factory->createElement<CRectangleAnimatedUV>({ -0.55f, -0.15f, 0.3f, 0.3f }, body);
		drum2->setTexture(CTextureFactory::getInstance()->loadTexture("drum2.png"));
		drum2->setRPS(10, 4.2f);

		CRectangleAnimatedUV* drum3 = gui_factory->createElement<CRectangleAnimatedUV>({ -0.15f, -0.15f, 0.3f, 0.3f }, body);
		drum3->setTexture(CTextureFactory::getInstance()->loadTexture("drum3.png"));
		drum3->setRPS(15, 4.4f);

		CRectangleAnimatedUV* drum4 = gui_factory->createElement<CRectangleAnimatedUV>({ 0.25f, -0.15f, 0.3f, 0.3f }, body);
		drum4->setTexture(CTextureFactory::getInstance()->loadTexture("drum4.png"));
		drum4->setRPS(20, 4.6f);

		CRectangleAnimatedUV* drum5 = gui_factory->createElement<CRectangleAnimatedUV>({ 0.65f, -0.15f, 0.3f, 0.3f }, body);
		drum5->setTexture(CTextureFactory::getInstance()->loadTexture("drum5.png"));
		drum5->setRPS(25, 4.8f);

		btn_lines->m_lines = lines;
		btn_game->m_drums.insert(btn_game->m_drums.end(), { drum1, drum2, drum3, drum4, drum5 });
		m_root = root_rect;

		CGUILayer::prepareGL();
	}
};

int main(int argc, char** argv)
{
	CApplication *app = CApplication::getInstance();

	app->initialize(argc, argv);
	app->setWindowTitle("Test OpenGL");
	app->setFullScreen(false);
	app->setWindowSize({800, 600});
	app->setColorDepth(EColorDepth::_32);

	app->setGUI(new MyGUI);

	app->exec();
}

