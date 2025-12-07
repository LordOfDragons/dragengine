#ifndef _DETZFILE_H_
#define _DETZFILE_H_

#include "../detCase.h"
#include "decZFileWriter.h"
#include "decMemoryFile.h"
#include "decZFileReader.h"
#include "decMemoryFileReader.h"
#include "decMemoryFileWriter.h"


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

