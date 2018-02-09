#ifndef __NVRTC_PROGRAM_H__
#define __NVRTC_PROGRAM_H__

#include <nvrtc.h>
#include <string>
#include <vector>

typedef std::vector<std::string> StringList;

/// Manages the creation and (runtime)compilation of a GPU program using nvrtc
class NVRTCProgram {
public:
	NVRTCProgram();
	~NVRTCProgram();

	/// Getters
	std::string getMainProgramFileName() const;
	std::string getCudaProgramStr() const;
	int getHeadersCount() const;
	StringList getHeaderFilesSource() const;
	StringList getHeaderFilesNames() const;
	StringList getCompileOptions() const;
	nvrtcProgram getProgram() const;
	char* getPtx() const;
	size_t getPtxSize() const;

	/// Setters
	void setMainProgramFileName(const std::string& mainProgramFileName);
	void setCudaProgarmStr(const std::string& cudaProgramStr);
	void setHeaderFilesSource(const StringList& headerFilesSource);
	void setHeaderFilesNames(const StringList& headerFilesNames);
	void setCompileOptions(const StringList& compileOptions);
	void setProgram(const nvrtcProgram& program);
	void setPtx(char* ptx);
	void setPtxSize(size_t ptxSize);

	/// Create nvrtc program using the read headers and sources
	void createProgram();

	/// Compile created nvrtc program with specified compile options
	void compileProgram();

	/// Read the a source code file
	/// @param[in] fileName file to be read
	std::string readSourceCodeFile(const std::string& fileName) const;

	/// Read all used header files using recursion
	/// @param[in] fileName the file from which the reading will start
	void readHeaderFiles(const std::string& fileName);

	/// Set compile options for the nvrtc program
	/// @param[in] compileOption the input compile options
	void addCompileOption(const std::string& compileOption);

	/// Output the compilation log
	void obtainCompileLog() const;

	/// Get resulting ptx from the compilation
	void obtainPtx();

	/// Destroy created program
	void destroyProgram();

	/// Create and compile GPU program using nvrtc
	void createAndCompile();

private:
	std::string mainProgramFileName; ///< The main file for the GPU program
	std::string cudaProgramStr; ///< GPU program source
	int headersCount; ///< Count of the used headers
	StringList headerFilesSource; ///< All header files
	StringList headerFilesNames; ///< All header file names
	StringList compileOptions; ///< Compilation options for the GPU program
	nvrtcProgram program; ///< NVRTC program to be compiled
	char* ptx; ///< Resulting PTX
	size_t ptxSize; /// Resulting PTX size
};

#endif //__NVRTC_PROGRAM_H__
