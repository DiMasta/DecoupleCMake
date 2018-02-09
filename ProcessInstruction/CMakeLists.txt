SET(processInstructionCUFilePath "${PROJECT_SOURCE_DIR}/ProcessInstruction/process_instruction.cu")

SET(generatedCPPFile "${CMAKE_CURRENT_BINARY_DIR}/process_instruction.cpp")

# Configure the .cpp file to contain the .cu file path
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/process_instruction.cpp.in"
	${generatedCPPFile}
)

SET (
	SOURCE_FILES
	process_instruction.cu
	${generatedCPPFile}
)

SET (
	HEADER_FILES
	process_instruction.h
)

ADD_LIBRARY(ProcessInstruction ${SOURCE_FILES} ${HEADER_FILES})