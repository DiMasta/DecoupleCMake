#ifndef __RAMP_H__
#define __RAMP_H__

#include "texture.h"

class Ramp : public Texture {
public:
	Ramp();
	~Ramp();
	
	const char* getPTX() override;
private:
};

#endif //__RAMP_H__