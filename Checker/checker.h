#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "texture.h"

class Checker : public Texture {
public:
	Checker();
	~Checker();
	
	const char* getPTX() override;
private:
};

#endif //__CHECKER_H__