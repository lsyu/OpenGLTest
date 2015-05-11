#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

#include <vector>
#include "core/Utils.h"

enum class EArrayType : bool
{
	Data,
	Index
}; // enum EArrayType


class CVertexBufferObject
{
public:

	CVertexBufferObject(EArrayType type = EArrayType::Data);
	virtual ~CVertexBufferObject();


	void genBuffer();
	void bind() const;

	void disable() const;


	template <typename T>
	void setData(const std::vector<T> *buffer) const {
		setData(buffer->data(), buffer->capacity()*sizeof(T));
	}
	void setData(const void *data, size_t sizeOfData) const;

private:
	unsigned int m_vbo;
	EArrayType m_type;
}; // class VertexBufferObject

#endif // VERTEXBUFFEROBJECT_H