SET(dirtCUFilePath "${CMAKE_CURRENT_SOURCE_DIR}/dirt.cu")

# First the .cu file must be compiled to .ptx using DiNVVCCExec
ADD_CUSTOM_COMMAND (
	OUTPUT dirt.ptx
	COMMAND DiNVVCCExec ${PATH_TO_NVCC_EXE} ${dirtCUFilePath} dirt.ptx
	DEPENDS dirt.cu
)

# The .ptx file must be converted to C++ int array using DiCLGatherPTX and stored in .generated file
ADD_CUSTOM_COMMAND (
	OUTPUT dirt.generated
	COMMAND DiCLGatherPTX dirt.ptx dirt.generated
	DEPENDS dirt.ptx
)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Texture")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

SET(getDirtPTXCPP "${CMAKE_CURRENT_BINARY_DIR}/get_dirt_ptx.cpp")
SET(dirtGeneratedFilePath "${CMAKE_CURRENT_BINARY_DIR}/dirt.generated")

# Configure the .cpp file to include the .generated file
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/get_dirt_ptx.cpp.in"
	${getDirtPTXCPP}
)

# Add dependency to the .generated file
SET_SOURCE_FILES_PROPERTIES(${getDirtPTXCPP} PROPERTIES OBJECT_DEPENDS dirt.generated)

SET (
	EXTRA_LIBS
	Texture
)

SET (
	SORCE_FILES
	${getDirtPTXCPP}
	dirt.cpp
	dirt.cu
)

SET (
	HEADER_FILES
	dirt.h
)

ADD_LIBRARY(Dirt ${SORCE_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(Dirt ${EXTRA_LIBS})