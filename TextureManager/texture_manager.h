#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <cuda.h>
#include <vector>
#include <set>

#include "texture.h"
#include "texture_utils.h"

typedef std::set<int> TextureInstrustionsSet;

/// Holds all C++ objects for the user textures
class TextureManager {
public:
	TextureManager();
	~TextureManager();

	/// Create C++ texture objects base on the user input data
	/// @param[in] textureInstructions user input texture ids
	void createNeededTextures(const TextureInstrustionsSet& textureInstructions);

	/// Link all ptx files for the user textures
	/// @param[in] linkState pending JIT linker invocation
	void linkTexturesPTXFiles(CUlinkState& linkState) const;

private:
	std::vector<Texture*> textures; ///< User textures' C++ objects
};

#endif //__TEXTURE_MANAGER_H__