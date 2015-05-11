#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <algorithm>

struct CVector2
{
	float x;
	float y;
};

struct CSize
{
	float width;
	float height;
};

struct CBoundingBox
{
	CVector2 minimum;
	CVector2 maximum;

	float getWidth() const { return maximum.x - minimum.x; }
	float getHeight() const { return maximum.y - minimum.y; }
};

struct CColor
{
	float r;
	float g;
	float b;
};

struct CColorWithAlpha : public CColor
{
	float a;
};

class CRandomElement
{
public:
	size_t m_cnt;
	std::vector<size_t> m_values;

	CRandomElement(size_t cnt)
		: m_cnt(cnt),
		m_values(cnt, 0)
	{
		for (size_t i = 0; i < cnt; ++i)
		{
			m_values[i] = i;
		}
		std::random_shuffle(m_values.begin(), m_values.end(), &CRandomElement::random);
	}

	size_t CRandomElement::getElement(size_t i) const
	{
		i = i % m_cnt;
		return m_values[i];
	}

	static int CRandomElement::random(int i)
	{
		return std::rand() % i;
	}
};

#endif /*UTILS_H*/