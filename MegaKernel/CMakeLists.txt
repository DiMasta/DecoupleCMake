SET(megaKernelCUFilePath "${CMAKE_CURRENT_SOURCE_DIR}/mega_kernel.cu")

# First the .cu file must be compiled to .ptx using DiNVVCCExec
ADD_CUSTOM_COMMAND (
	OUTPUT mega_kernel.ptx
	COMMAND DiNVVCCExec ${PATH_TO_NVCC_EXE} ${megaKernelCUFilePath} mega_kernel.ptx
	DEPENDS mega_kernel.cu
)

# The .ptx file must be converted to C++ int array using DiCLGatherPTX and stored in .generated file
ADD_CUSTOM_COMMAND (
	OUTPUT mega_kernel.generated
	COMMAND DiCLGatherPTX mega_kernel.ptx mega_kernel.generated
	DEPENDS mega_kernel.ptx
)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/MegaKernel")

SET(getMegaKernelPTXCPPFilePath "${CMAKE_CURRENT_BINARY_DIR}/get_mega_kernel_ptx.cpp")
SET(generatedFilepath "${CMAKE_CURRENT_BINARY_DIR}/mega_kernel.generated")

# Configure the .cpp file to include the .generated file
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/get_mega_kernel_ptx.cpp.in"
	${getMegaKernelPTXCPPFilePath}
)

# Add dependency to the .generated file
SET_SOURCE_FILES_PROPERTIES(get_mega_kernel_ptx.h PROPERTIES OBJECT_DEPENDS mega_kernel.generated)

SET (
	SOURCE_FILES
	${getMegaKernelPTXCPPFilePath}
	mega_kernel.cu
)

SET (
	HEADER_FILES
	get_mega_kernel_ptx.h
)

ADD_LIBRARY(MegaKernel ${SOURCE_FILES} ${HEADER_FILES})