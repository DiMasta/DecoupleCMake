#ifndef __DI_VRAY_H__
#define __DI_VRAY_H__

#include "texture_manager.h"
#include "nvrtc_program.h"

/// Represents the main application, which collects information from the user (which textures are needed)
/// links them to the main GPU code and executes the main GPU kernel(megaKernel)
class DiVRay {
public:
	DiVRay();
	~DiVRay();

	/// Ask the user which textures he wants to use
	void collectDataFromTheUser();

	/// Add compile option, for each texture, that will be linked to the main GPU code
	void addProcessInstrunctionCompileFlags();

	/// Runtime compile, using nvrtc, processInstruction kernel, based on user the user textures
	void prepareProcessInstrunctionGPUProgram();

	/// Link megaKernel(main GPU program), processInstruction and each user texture
	void linkGPUCode();

	/// Launch the main GPU program
	void launchGPUMegaKernel();

	/// Gather information, create and link needed GPU programs and execute megaKernel
	void run();

private:
	TextureInstrustionsSet texInstrs; ///< All user textures' ids
	TextureManager textureManager; ///< Holds user textures' c++ objects and ptx files
	StringList processInstrCompileOptions; ///< Compile options needed to compile processInstruction program
	NVRTCProgram processInstructionProgram; ///< processInstruction program managed by nvrtc
	CUdevice cuDevice; ///< Handle to a compute device
	CUcontext context; ///< CUDA context
	CUlinkState linkState; ///< Pending JIT linker invocation
	CUmodule module; ///< CUDA module
	CUfunction megaKernel; ///< Main GPU program kernel
	void *cubin; ///< Output image of linked GPU programs
};

#endif //__DI_VRAY_H__