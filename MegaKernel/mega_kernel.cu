extern __device__ int processInstruction(int instruction);

extern "C" __global__ void megaKernel(int* texInstrs, size_t texInstrsCount, int* result) {
	int shadingResult = 0;
	
	for (size_t texInstrIdx = 0; texInstrIdx < texInstrsCount; ++texInstrIdx) {
		shadingResult += processInstruction(texInstrs[texInstrIdx]);
	}

	*result = shadingResult;
}