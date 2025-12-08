#ifndef _DETZFILE_H_
#define _DETZFILE_H_

#include "../detCase.h"

#include <dragengine/common/file/decZFileReader.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decZFileWriter.h>

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

