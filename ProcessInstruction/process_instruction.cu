enum TextureInstruction {
	TI_INVALID = -1,
	TI_DIRT = 0,
	TI_CHECKER,
	TI_RAMP,
};

//*****************************************************************************************************************
//*****************************************************************************************************************

// COMPILE_?? flags are passed to nvrtc based on which textures must be used
__device__ int processInstruction(int instruction) {
	int textureResult = -1;

	switch (instruction) {
#ifdef COMPILE_DIRT
		extern __device__ int evalDirt();
		case TI_DIRT: {
			textureResult = evalDirt();
			break;
		}
#endif // COMPILE_DIRT

#ifdef COMPILE_CHECKER
		extern __device__ int evalChecker();
		case TI_CHECKER: {
			textureResult = evalChecker();
			break;
		}
#endif // COMPILE_CHECKER

#ifdef COMPILE_RAMP
		extern __device__ int evalRamp();
		case TI_RAMP: {
			textureResult = evalRamp();
			break;
		}
#endif // COMPILE_RAMP

		default: {
			printf("NO TEXTURE USED\n");
		}
	}

	return textureResult;
}
