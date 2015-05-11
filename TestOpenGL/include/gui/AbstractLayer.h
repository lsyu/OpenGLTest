#ifndef ABSTRACTLAYER_H
#define ABSTRACTLAYER_H

class CEventMouseClick;

/**
* @brief Интерфейс слоя.
* Это может быть слой 3D сцены, слой GUI
*/
class AbstractLayer
{
public:
	AbstractLayer() {};
	virtual ~AbstractLayer() {};

	virtual void prepareGL() = 0;
	virtual void paintGL() = 0;

	virtual bool updateGL() { return true; }
	virtual bool updateGL(CEventMouseClick *e) { return true; }
}; // class AbstractLayer


#endif // ABSTRACTLAYER_H