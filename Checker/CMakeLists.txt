SET(checkerCUFilePath "${CMAKE_CURRENT_SOURCE_DIR}/checker.cu")

# First the .cu file must be compiled to .ptx using DiNVVCCExec
ADD_CUSTOM_COMMAND (
	OUTPUT checker.ptx
	COMMAND DiNVVCCExec ${PATH_TO_NVCC_EXE} ${checkerCUFilePath} checker.ptx
	DEPENDS checker.cu
)

# The .ptx file must be converted to C++ int array using DiCLGatherPTX and stored in .generated file
ADD_CUSTOM_COMMAND (
	OUTPUT checker.generated
	COMMAND DiCLGatherPTX checker.ptx checker.generated
	DEPENDS checker.ptx
)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Texture")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

SET(getCheckerPTXCPP "${CMAKE_CURRENT_BINARY_DIR}/get_checker_ptx.cpp")
SET(checkerGeneratedFilePath "${CMAKE_CURRENT_BINARY_DIR}/checker.generated")

# Configure the .cpp file to include the .generated file
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/get_checker_ptx.cpp.in"
	${getCheckerPTXCPP}
)

# Add dependency to the .generated file
SET_SOURCE_FILES_PROPERTIES(${getCheckerPTXCPP} PROPERTIES OBJECT_DEPENDS checker.generated)

SET (
	EXTRA_LIBS
	Texture
)

SET (
	SORCE_FILES
	${getCheckerPTXCPP}
	checker.cpp
	checker.cu
)

SET (
	HEADER_FILES
	checker.h
)

ADD_LIBRARY(Checker ${SORCE_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(Checker ${EXTRA_LIBS})