#ifndef ABSTRACTENTITY_H
#define ABSTRACTENTITY_H

#include <string>

class AbstractEntity
{
public:
	
	virtual void configure() = 0;
	virtual void paint() = 0;
	virtual const std::string& getId() const = 0;

protected:
	AbstractEntity() {}
	virtual ~AbstractEntity() {}
}; // class AbstractEntity

#endif // ABSTRACTENTITY_H