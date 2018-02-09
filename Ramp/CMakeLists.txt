SET(rampCUFilePath "${CMAKE_CURRENT_SOURCE_DIR}/ramp.cu")

# First the .cu file must be compiled to .ptx using DiNVVCCExec
ADD_CUSTOM_COMMAND (
	OUTPUT ramp.ptx
	COMMAND DiNVVCCExec ${PATH_TO_NVCC_EXE} ${rampCUFilePath} ramp.ptx
	DEPENDS ramp.cu
)

# The .ptx file must be converted to C++ int array using DiCLGatherPTX and stored in .generated file
ADD_CUSTOM_COMMAND (
	OUTPUT ramp.generated
	COMMAND DiCLGatherPTX ramp.ptx ramp.generated
	DEPENDS ramp.ptx
)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Texture")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

SET(getRampPTXCPP "${CMAKE_CURRENT_BINARY_DIR}/get_ramp_ptx.cpp")
SET(rampGeneratedFilePath "${CMAKE_CURRENT_BINARY_DIR}/ramp.generated")

# Configure the .cpp file to include the .generated file
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/get_ramp_ptx.cpp.in"
	${getRampPTXCPP}
)

# Add dependency to the .generated file
SET_SOURCE_FILES_PROPERTIES(${getRampPTXCPP} PROPERTIES OBJECT_DEPENDS ramp.generated)

SET (
	EXTRA_LIBS
	Texture
)

SET (
	SORCE_FILES
	${getRampPTXCPP}
	ramp.cpp
	ramp.cu
)

SET (
	HEADER_FILES
	ramp.h
)

ADD_LIBRARY(Ramp ${SORCE_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(Ramp ${EXTRA_LIBS})