#ifndef _DETZFILE_H_
#define _DETZFILE_H_

#include "../detCase.h"

class decZFileReader;
class decMemoryFileReader;
class decMemoryFileWriter;
class decMemoryFile;
class decZFileWriter;

// class detZFile
class detZFile : public detCase{
private:
	decMemoryFile::Ref pMemoryFileCompressed;
	char *pTestBuffer;
	
	decMemoryFileWriter::Ref pMemoryFileWriter;
	decZFileWriter::Ref pZWriter;
	
	decMemoryFileReader::Ref pMemoryFileReader;
	decZFileReader::Ref pZReader;
	
public:
	detZFile();
	~detZFile() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
private:
	void pTestZFile();
	
	void pOutputCompressedToFile();
	void pOutputTestBufferToFile(int size);
	
	void pCreateZWriter();
	void pDestroyZWriter();
	void pCreateZReader();
	void pDestroyZReader();
};

#endif

