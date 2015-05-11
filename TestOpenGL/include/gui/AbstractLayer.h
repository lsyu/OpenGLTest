#ifndef ABSTRACTLAYER_H
#define ABSTRACTLAYER_H

class CEventMouseClick;

/**
* @brief ��������� ����.
* ��� ����� ���� ���� 3D �����, ���� GUI
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