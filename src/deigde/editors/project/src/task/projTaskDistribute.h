/* 
 * Drag[en]gine IGDE Project Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _PROJTASKDISTRIBUTE_H_
#define _PROJTASKDISTRIBUTE_H_

#include "zip.h"

#include <deigde/gui/igdeStepableTask.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>

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
	
	deVirtualFileSystemReference pVFS;
	decObjectList pStackDirectories;
	eStates pState;
	decStringSet pUsedFileExtensions;
	decStringSet pExcludePatterns;
	dePathList pExcludeBaseGameDefPath;
	
	zipFile pZipFile;
	decString pDelgaPath;
	decBaseFileWriterReference pDelgaWriter;
	
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
	void pCopyFile( const decPath &path );
	void pZipBeginFile( const decPath &path );
	void pZipWriteFile( const void *buffer, long size );
	void pZipCloseFile();
	void pZipWriteMemoryFile( const decMemoryFile &memoryFile );
	void pCloseDirectory();
	void pAddUsedFileExtension( const decPath &path );
	void pWriteGameXml();
	void pWriteGameXml( decXmlWriter &writer );
	void pWriteGameXmlRequiredFormats( decXmlWriter &writer );
	cProcessDirectory *GetProcessDirectory();
};

#endif
