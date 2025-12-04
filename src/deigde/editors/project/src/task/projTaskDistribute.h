/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _PROJTASKDISTRIBUTE_H_
#define _PROJTASKDISTRIBUTE_H_

#include "zip.h"

#include <deigde/gui/igdeStepableTask.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>

class projWindowMain;
class projProject;
class projProfile;

class decMemoryFile;
class decXmlWriter;



/**
 * \brief Distribute game task.
 */
class projTaskDistribute : public igdeStepableTask{
private:
	class cProcessDirectory : public deObject{
	public:
		decString path;
		dePathList directories;
		dePathList files;
		int nextDirectory;
		int nextFile;
		bool hasCountedDir;
	};
	
	enum eStates{
		esInitial,
		esProcessFiles,
		esFinished
	};
	
	
	
	projWindowMain &pWindowMain;
	const projProject &pProject;
	const projProfile &pProfile;
	
	deVirtualFileSystem::Ref pVFS;
	decObjectList pStackDirectories;
	eStates pState;
	decStringSet pUsedFileExtensions;
	decStringSet pExcludePatterns;
	dePathList pExcludeBaseGameDefPath;
	
	zipFile pZipFile;
	decString pDelgaPath;
	decBaseFileWriter::Ref pDelgaWriter;
	
	long pDelgaSize;
	long pDelgaPosition;
	int pDelgaDirectoryCount;
	int pDelgaFileCount;
	
	char *pReadBuffer;
	const int pReadBufferSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	projTaskDistribute( projWindowMain &windowMain,
		const projProject &project, const projProfile &profile );
	
	/** \brief Clean up task. */
	virtual ~projTaskDistribute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Project. */
	inline const projProject &GetProject() const{ return pProject; }
	
	/** \brief Profile. */
	inline const projProfile &GetProfile() const{ return pProfile; }
	
	
	
	/** \brief Delga file size. */
	inline long GetDelgaSize() const{ return pDelgaSize; }
	
	/** \brief Number of directories in delga file. */
	inline int GetDelgaDirectoryCount() const{ return pDelgaDirectoryCount; }
	
	/** \brief Number of files in delga file. */
	inline int GetDelgaFileCount() const{ return pDelgaFileCount; }
	
	
	
	/**
	 * \brief Run next step of task.
	 * 
	 * The step should consume a small amount of time to allow the GUI to stay responsive.
	 * In case of an error an exception has to be thrown which aborts the task.
	 * 
	 * \retval true Task requires more steps.
	 * \retval false Task finished after this step.
	 */
	virtual bool Step();
	
	
	
	/** \brief Write data to delga file for internal use only. */
	void WriteToDelga( const void *buffer, long length );
	
	/** \brief Get delga file position for internal use only. */
	inline long GetDelgaPosition() const{ return pDelgaPosition; }
	
	/** \brief Seek delga file for internal use only. */
	void SeekDelgaFile( long offset, int zlibOrigin );
	/*@}*/
	
	
	
private:
	bool pExcludedByBaseGameDefPath( const decPath &path );
	bool pExcludedByPattern( const decPath &path );
	void pBuildExcludeBaseGameDefPath();
	void pCreateDelgaWriter();
	void pCloseDelgaWriter();
	void pScanDirectory( const decPath &path );
	void pProcessFiles();
	void pProcessFile( const decPath &path );
	decString pGetFileExtension( const decPath &path ) const;
	deLoadableModule *pGetMatchingModule( const decString &extension ) const;
	const char *pGetModuleTypeName( deModuleSystem::eModuleTypes type ) const;
	void pCopyFile( const decPath &path );
	void pZipBeginFile( const decPath &path, bool compress );
	void pZipWriteFile( const void *buffer, long size );
	void pZipCloseFile();
	void pZipWriteMemoryFile( const decMemoryFile &memoryFile );
	void pCloseDirectory();
	void pWriteGameXml();
	void pWriteGameXml( decXmlWriter &writer );
	void pWriteGameXmlRequiredFormats( decXmlWriter &writer );
	cProcessDirectory *GetProcessDirectory();
};

#endif
