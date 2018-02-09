#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <strsafe.h>
#include <fstream>

using namespace std;

const string QUOTE = "\"";
const string SPACE = " ";
LPCWSTR WCHAR_FORMAT_FOR_PRINT = L"%S";

enum NVCCArgs {
	NA_NVCC_EXE_PATH = 1,
	NA_CUDA_FILE_PATH,
	NA_OUTPUT_PTX_FILE,
};

//************************************************************************************************************************
//************************************************************************************************************************

void addQuotesIfNeeded(string& str) {
	if (str.find(" ") != string::npos) {
		string newString = QUOTE;
		newString.append(str);
		newString.append(QUOTE);
		str = newString;
	}
}

//************************************************************************************************************************
//************************************************************************************************************************

/// Tool to invoke nvcc to compile a given .cu file to .ptx file
/// It needs the following arguments:
///		argv[1] - the path to the nvcc
///		argv[2] - the path to the .cu file to be compiled
///		argv[3] - the path to the output .ptx file
int main(int argc, char *argv[]) {
	STARTUPINFOW si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));

	string nvccExePath(argv[NA_NVCC_EXE_PATH]);
	size_t nvccExePathLenght = nvccExePath.length() + 1;
	WCHAR* wNVCCExePath = new WCHAR[nvccExePathLenght];
	StringCchPrintfW(wNVCCExePath, nvccExePathLenght, WCHAR_FORMAT_FOR_PRINT, nvccExePath.c_str());

	addQuotesIfNeeded(nvccExePath);

	string cudaFilePath(argv[NA_CUDA_FILE_PATH]);
	addQuotesIfNeeded(cudaFilePath);

	const string nvccArgs = "--generate-code arch=compute_20,code=sm_20 -IK:\\msvs2010\\include --machine 64 --compiler-bindir K:\\cuda\\VC\\bin\\amd64\\cl.exe --device-c -ptx --output-file";

	string outputPtxFile(argv[NA_OUTPUT_PTX_FILE]);
	addQuotesIfNeeded(outputPtxFile);

	string commandLine = nvccExePath;
	commandLine.append(SPACE);
	commandLine.append(cudaFilePath);
	commandLine.append(SPACE);
	commandLine.append(nvccArgs);
	commandLine.append(SPACE);
	commandLine.append(outputPtxFile);

	const size_t commandLineLenght = commandLine.length() + 1;
	WCHAR* nvccParams = new WCHAR[commandLineLenght];
	StringCchPrintfW(nvccParams, commandLineLenght, WCHAR_FORMAT_FOR_PRINT, commandLine.c_str());

	ofstream debugFile;
	debugFile.open("C:\\cmakeDiDebug.txt");
	debugFile << "NVCC path: " << nvccExePath << endl;
	debugFile << "Command line:" << commandLine << endl;


	BOOL nvccCallRes = CreateProcessW(
		wNVCCExePath,	// No module name (use command line)
		nvccParams,		// Command line
		NULL,			// Process handle not inheritable
		NULL,			// Thread handle not inheritable
		FALSE,			// Set handle inheritance to FALSE
		0,				// No creation flags
		NULL,			// Use parent's environment block
		NULL,			// Use parent's starting directory 
		&si,			// Pointer to STARTUPINFO structure
		&pi				// Pointer to PROCESS_INFORMATION structure
	);


	if (!nvccCallRes) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 0;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	delete[] nvccParams;
	nvccParams = nullptr;

	delete[] wNVCCExePath;
	nvccParams = nullptr;

	system("pause");

	return 0;
}