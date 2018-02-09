#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

/// Represents user texture
class Texture {
public:
		Texture();
		~Texture();
		
		/// Get PTX for the texture
		virtual const char* getPTX() = 0;
private:
};

#endif //__TEXTURE_H__