#include "nvrtc_program.h"

#include <sstream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <iostream>

using namespace std;

NVRTCProgram::NVRTCProgram() :
	mainProgramFileName(),
	cudaProgramStr(),
	headersCount(0),
	headerFilesSource(),
	headerFilesNames(),
	program(),
	ptx(nullptr)
{

}

//************************************************************************************************************************
//************************************************************************************************************************

NVRTCProgram::~NVRTCProgram() {
	if (ptx) {
		delete[] ptx;
		ptx = nullptr;
	}
}

//************************************************************************************************************************
//************************************************************************************************************************

std::string NVRTCProgram::getMainProgramFileName() const {
	return std::string();
}

//************************************************************************************************************************
//************************************************************************************************************************

string NVRTCProgram::getCudaProgramStr() const {
	return cudaProgramStr;
}

//************************************************************************************************************************
//************************************************************************************************************************

int NVRTCProgram::getHeadersCount() const {
	return headersCount;
}

//************************************************************************************************************************
//************************************************************************************************************************

StringList NVRTCProgram::getHeaderFilesSource() const {
	return headerFilesSource;
}

//************************************************************************************************************************
//************************************************************************************************************************

StringList NVRTCProgram::getHeaderFilesNames() const {
	return headerFilesNames;
}

//************************************************************************************************************************
//************************************************************************************************************************

StringList NVRTCProgram::getCompileOptions() const {
	return compileOptions;
}

//************************************************************************************************************************
//************************************************************************************************************************

nvrtcProgram NVRTCProgram::getProgram() const {
	return program;
}

//************************************************************************************************************************
//************************************************************************************************************************

char * NVRTCProgram::getPtx() const {
	return ptx;
}

//************************************************************************************************************************
//************************************************************************************************************************

size_t NVRTCProgram::getPtxSize() const {
	return ptxSize;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setMainProgramFileName(const std::string& mainProgramFileName) {
	this->mainProgramFileName = mainProgramFileName;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setCudaProgarmStr(const string& cudaProgramStr) {
	this->cudaProgramStr = cudaProgramStr;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setHeaderFilesSource(const StringList& headerFilesSource) {
	this->headerFilesSource = headerFilesSource;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setHeaderFilesNames(const StringList& headerFilesNames) {
	this->headerFilesNames = headerFilesNames;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setCompileOptions(const StringList& compileOptions) {
	this->compileOptions = compileOptions;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setProgram(const nvrtcProgram & program) {
	this->program = program;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setPtx(char* ptx) {
	this->ptx = ptx;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::setPtxSize(size_t ptxSize) {
	this->ptxSize = ptxSize;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::createProgram() {
	const char** headersSourcePtr = new const char*[headersCount];
	const char** headersNamesPtr = new const char*[headersCount];

	for (int headerIdx = 0; headerIdx < headersCount; ++headerIdx) {
		headersSourcePtr[headerIdx] = headerFilesSource[headerIdx].c_str();
		headersNamesPtr[headerIdx] = headerFilesNames[headerIdx].c_str();
	}

	nvrtcResult createResult = nvrtcCreateProgram(&program, cudaProgramStr.c_str(), "megaKernel", headersCount, headersSourcePtr, headersNamesPtr);

	delete[] headersSourcePtr;
	delete[] headersNamesPtr;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::compileProgram() {
	const int optionsCount = int(compileOptions.size());
	const char** compileOptionPtr = new const char*[optionsCount];

	for (int optionIdx = 0; optionIdx < optionsCount; ++optionIdx) {
		compileOptionPtr[optionIdx] = compileOptions[optionIdx].c_str();
	}

	nvrtcResult compileResult = nvrtcCompileProgram(program, optionsCount, compileOptionPtr);

	delete[] compileOptionPtr;
}

//************************************************************************************************************************
//************************************************************************************************************************

string NVRTCProgram::readSourceCodeFile(const string& fileName) const {
	ifstream fileStream(fileName);
	stringstream buffer;
	buffer << fileStream.rdbuf();

	return buffer.str();
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::readHeaderFiles(const string& fileName) {
	ifstream mainProgramFileStream(fileName);

	regex reInclude("^#[[:space:]]*include[[:space:]]+[\"<]([^\"]*?)[\">].*", regex::icase); // #include "{match1}" ... ...
	string line;
	while (getline(mainProgramFileStream, line)) {
		if (line.empty()) {
			continue;
		}

		string includeFile;

		cmatch what;
		if (regex_match(line.c_str(), what, reInclude) && what[0].matched) {
			includeFile.assign(what[1].first, what[1].second);
		}

		if (includeFile.empty()) {
			continue;
		}

		if (find(headerFilesNames.begin(), headerFilesNames.end(), includeFile) == headerFilesNames.end()) {
			headerFilesSource.push_back(readSourceCodeFile(includeFile));
			headerFilesNames.push_back(includeFile);
			readHeaderFiles(includeFile);
		}
	}

	headersCount = int(headerFilesSource.size());
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::addCompileOption(const std::string& compileOption) {
	compileOptions.push_back(compileOption);
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::obtainCompileLog() const {
	size_t logSize;
	nvrtcGetProgramLogSize(program, &logSize);

	char *log = new char[logSize];

	nvrtcGetProgramLog(program, log);
	cout << log << endl;

	delete[] log;
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::obtainPtx() {
	nvrtcGetPTXSize(program, &ptxSize);

	ptx = new char[ptxSize];
	nvrtcGetPTX(program, ptx);
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::destroyProgram() {
	nvrtcDestroyProgram(&program);
}

//************************************************************************************************************************
//************************************************************************************************************************

void NVRTCProgram::createAndCompile() {
	cudaProgramStr = readSourceCodeFile(mainProgramFileName);
	readHeaderFiles(mainProgramFileName);
	createProgram();
	compileProgram();
	//obtainCompileLog();
	obtainPtx();
	destroyProgram();
}
