#ifndef __DIRT_H__
#define __DIRT_H__

#include "texture.h"

class Dirt : public Texture {
public:
	Dirt();
	~Dirt();
	
	const char* getPTX() override;
private:
};

#endif //__DIRT_H__