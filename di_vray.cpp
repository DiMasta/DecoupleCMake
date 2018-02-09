#include <iostream>
#include <iterator>
#include <algorithm>
#include <nvrtc.h>
#include <cuda.h>

#include "di_vray.h"
#include "process_instruction.h"
#include "get_mega_kernel_ptx.h"

using namespace std;

const int INPUT_TERMINATION = 9;

//************************************************************************************************************************
//************************************************************************************************************************

DiVRay::DiVRay() :
	texInstrs(),
	textureManager(),
	processInstrCompileOptions(),
	processInstructionProgram(),
	cuDevice(),
	context(),
	linkState(),
	module(),
	megaKernel(),
	cubin(nullptr)
{
	// Create all CUDA elements needed to create and link the GPU programs
	cuInit(0);
	cuDeviceGet(&cuDevice, 0);
	cuCtxCreate(&context, 0, cuDevice);
	cuLinkCreate(0, nullptr, nullptr, &linkState);
}

//************************************************************************************************************************
//************************************************************************************************************************

DiVRay::~DiVRay() {
	cuModuleUnload(module);
	cuLinkDestroy(linkState);
	CUresult destroyResult;
	destroyResult = cuCtxDestroy(context);
}

//************************************************************************************************************************
//************************************************************************************************************************

void DiVRay::collectDataFromTheUser() {
	cout << "Tell us which textures are used in your scene" << endl;
	cout << "{(0, 1, 2) = (dirt, checker, ramp)}" << endl;
	cout << "Type \"9\" to stop the input:" << endl;

	while (true) {
		int id;
		cin >> id;

		if (INPUT_TERMINATION == id) {
			break;
		}

		// Store which texture programs should be created and compiled in a set, to avoid repetitions
		texInstrs.insert(id);
	}
}

//************************************************************************************************************************
//************************************************************************************************************************

void DiVRay::addProcessInstrunctionCompileFlags() {
	for (TextureInstrustionsSet::iterator it = texInstrs.begin(); it != texInstrs.end(); ++it) {
		// The cases for all user textures should be presented
		processInstrCompileOptions.push_back(TEXTURE_COMPILE_OPTIONS[*it]);
	}
}

//************************************************************************************************************************
//************************************************************************************************************************

void DiVRay::linkGPUCode() {
	const char* megaKernelPTX = getMegaKernelPTX();

	CUresult addDataRes;
	addDataRes = cuLinkAddData(linkState, CU_JIT_INPUT_PTX, (void *)megaKernelPTX, strlen(megaKernelPTX), "megaKernel.ptx", 0, 0, 0);
	addDataRes = cuLinkAddData(linkState, CU_JIT_INPUT_PTX, (void *)processInstructionProgram.getPtx(), processInstructionProgram.getPtxSize(), "processInstruction.ptx", 0, 0, 0);

	// Link all user textures to the megaKernel and processInstruction
	textureManager.linkTexturesPTXFiles(linkState);

	size_t cubinSize;
	CUresult linkResult;
	linkResult = cuLinkComplete(linkState, &cubin, &cubinSize);
}

//************************************************************************************************************************
//************************************************************************************************************************

void DiVRay::prepareProcessInstrunctionGPUProgram() {
	const string DEVICE_C_OPTION = "--device-c"; // Needed so the program could be linked afterwards
	processInstrCompileOptions.push_back(DEVICE_C_OPTION);
	addProcessInstrunctionCompileFlags();

	const string processIsntructionCUFilePath = getProcessInstructionCUFilePath();
	processInstructionProgram.setMainProgramFileName(processIsntructionCUFilePath);
	processInstructionProgram.setCompileOptions(processInstrCompileOptions);
	processInstructionProgram.createAndCompile();
}

//************************************************************************************************************************
//************************************************************************************************************************

void DiVRay::launchGPUMegaKernel() {
	CUresult moduleLoadResult = cuModuleLoadData(&module, cubin);
	CUresult getFuncResult = cuModuleGetFunction(&megaKernel, module, "megaKernel");

	int* hostMegaKernelRes = new int[1];
	CUdeviceptr deviceMegaKernelRes; // Result from megaKernel
	CUdeviceptr deviceTextureInstructions; // All user textures that should be evaluated

	size_t texInstrsCount = texInstrs.size();
	const size_t texInstrsBufferSize = sizeof(int) * texInstrsCount;

	CUresult mallocResult;
	mallocResult = cuMemAlloc(&deviceTextureInstructions, texInstrsBufferSize);
	mallocResult = cuMemAlloc(&deviceMegaKernelRes, sizeof(int));

	CUresult memCpyResult;
	vector<int> texInstrsVec;
	copy(texInstrs.begin(), texInstrs.end(), back_inserter(texInstrsVec));
	memCpyResult = cuMemcpyHtoD(deviceTextureInstructions, texInstrsVec.data(), texInstrsBufferSize);

	void* megaKernelArgs[] = { &deviceTextureInstructions, &texInstrsCount, &deviceMegaKernelRes };

	CUresult launchResult =
		cuLaunchKernel(
			megaKernel,
			1, 1, 1,		// grid dim
			1, 1, 1,		// block dim
			0, nullptr,		// shared mem and stream
			megaKernelArgs, 0
		);

	CUresult synchResult = cuCtxSynchronize();

	memCpyResult = cuMemcpyDtoH(hostMegaKernelRes, deviceMegaKernelRes, sizeof(int));

	cout << "The shading result is: " << *hostMegaKernelRes << endl;

	cuMemFree(deviceMegaKernelRes);
	cuMemFree(deviceTextureInstructions);

	delete[] hostMegaKernelRes;
}

//************************************************************************************************************************
//************************************************************************************************************************

void DiVRay::run() {
	collectDataFromTheUser();
	textureManager.createNeededTextures(texInstrs);
	prepareProcessInstrunctionGPUProgram();
	linkGPUCode();
	launchGPUMegaKernel();
}