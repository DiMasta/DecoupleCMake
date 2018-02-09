#include "texture_manager.h"
#include "checker.h"
#include "dirt.h"
#include "ramp.h"

using namespace std;

TextureManager::TextureManager() {
	
}

//*****************************************************************************************************************
//*****************************************************************************************************************

TextureManager::~TextureManager() {
	for (size_t textureIdx = 0; textureIdx < textures.size(); ++textureIdx) {
		delete textures[textureIdx];
	}

	textures.clear();
}

//*****************************************************************************************************************
//*****************************************************************************************************************

void TextureManager::createNeededTextures(const TextureInstrustionsSet& textureInstructions) {
	for (TextureInstrustionsSet::iterator it = textureInstructions.begin(); it != textureInstructions.end(); ++it) {
		TextureInstruction texId = TextureInstruction(*it);

		Texture* texture = nullptr;

		switch (texId) {
			case TI_DIRT: {
				texture = new Dirt();
				break;
			}

			case TI_CHECKER: {
				texture = new Checker();
				break;
			}

			case TI_RAMP: {
				texture = new Ramp();
				break;
			}

			default: {
				break;
			}
		}

		textures.push_back(texture);
	}
}

//*****************************************************************************************************************
//*****************************************************************************************************************

void TextureManager::linkTexturesPTXFiles(CUlinkState& linkState) const {
	for (size_t textureIdx = 0; textureIdx < textures.size(); ++textureIdx) {
		const char* texturePTXFile = textures[textureIdx]->getPTX();

		CUresult addDataRes = cuLinkAddData(linkState, CU_JIT_INPUT_PTX, (void *)texturePTXFile, strlen(texturePTXFile), "texture.ptx", 0, 0, 0);

		if (CUDA_SUCCESS != addDataRes) {
			break;
		}
	}
}
