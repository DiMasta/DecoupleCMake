#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

enum GatherArgs {
	GA_INPUT_PTX_FILE = 1,
	GA_OUTPUT_GENERATED_FILE,
};

//************************************************************************************************************************
//************************************************************************************************************************

/// Tool to convert input .ptx file to C++ in array
/// It needs the following arguments:
///		argv[1] - the ptx file(path) to be converted
///		argv[2] - the path for the output generated file
int main(int argc, char *argv[]) {
	ifstream ptxFileStream(argv[GA_INPUT_PTX_FILE]);
	stringstream stringstreamBuffer;
	stringstreamBuffer << ptxFileStream.rdbuf();

	string ptxFileStr = stringstreamBuffer.str();
	const size_t ptxFileStrSize = ptxFileStr.size();

	vector<char> binary(ptxFileStr.begin(), ptxFileStr.end());
	const size_t ptxFileStrBytes = ptxFileStrSize * sizeof(char);
	const size_t charsToAdd = ptxFileStrBytes % sizeof(int);
	binary.insert(binary.end(), charsToAdd, ' ');

	int* ptxFileIntArray = (int*)binary.data();

	ofstream generatedFileStream(argv[GA_OUTPUT_GENERATED_FILE]);
	generatedFileStream << "static const int ptxFileAsIntArray[] = {";

	for (size_t i = 0; i < binary.size() / sizeof(int); ++i) {
		if (0 == (i % 64)) {
			generatedFileStream << endl;
		}

		generatedFileStream << ptxFileIntArray[i] << ",";
	}

	generatedFileStream << '0' << endl; // Trailing NULL
	generatedFileStream << "};" << endl;

	return 0;
}