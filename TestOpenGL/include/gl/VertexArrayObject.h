#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

class CVertexArrayObject
{
public:

	CVertexArrayObject();
	virtual ~CVertexArrayObject();

	void genBuffer();
	void bind() const;
	void disable() const;

private:
	unsigned int m_vao;
}; // class VertexArrayObject

#endif // VERTEXARRAYOBJECT_H