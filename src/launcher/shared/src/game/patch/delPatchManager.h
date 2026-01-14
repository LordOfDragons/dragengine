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

#ifndef _DELPATCHMANAGER_H_
#define _DELPATCHMANAGER_H_

#include "delPatch.h"

#include <dragengine/common/string/decString.h>

class delLauncher;
class deVirtualFileSystem;
class decPath;
class delEngineInstance;


/**
 * \brief Game patch manager.
 */
class DE_DLL_EXPORT delPatchManager{
private:
	delLauncher &pLauncher;
	delPatch::List pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch manager. */
	delPatchManager(delLauncher &launcher);
	
	/** \brief Clean up game patch manager. */
	~delPatchManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Game list. */
	inline delPatch::List &GetPatches(){ return pPatches; }
	inline const delPatch::List &GetPatches() const{ return pPatches; }
	
	/** \brief Load list of game patches from game patch directory. */
	void LoadPatches(delEngineInstance &instance);
	
	/** \brief Load a game patch from a disk file. */
	void LoadPatchFromDisk(delEngineInstance &instance, const decString &path, delPatch::List &list);
	
	/**
	 * \brief Search and load patches inside and below directory.
	 * \version 1.13
	 */
	void LoadPatchesFromDisk(delEngineInstance &instance, const decString &baseDir, delPatch::List &list);
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
	
	
	
protected:
	void pScanPatchDefFiles(delEngineInstance &instance, deVirtualFileSystem &vfs,
		const decPath &baseDir, const decPath &directory, delPatch::List &list);
	
	void pProcessFoundFiles(delEngineInstance &instance, const decPath &path, delPatch::List &list);
};

#endif
