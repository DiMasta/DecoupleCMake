#ifndef __TEXTURE_UTILS__
#define __TEXTURE_UTILS__

#include <string>

enum TextureInstruction {
	TI_INVALID = -1,
	TI_DIRT = 0,
	TI_CHECKER,
	TI_RAMP,
};

static const int TEXTURES_COUNT = 3;
static const std::string TEXTURE_COMPILE_OPTIONS[TEXTURES_COUNT] = {
	"-D COMPILE_DIRT",
	"-D COMPILE_CHECKER",
	"-D COMPILE_RAMP"
};

#endif //__TEXTURE_UTILS__