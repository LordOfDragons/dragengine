/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_CALCULATEDIRECTORYSIZE_H_
#define _DEGLB_CALCULATEDIRECTORYSIZE_H_

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deThread.h>


/**
 * \brief Thread calculating the size of a disc directory.
 */
class deglbCalculateDirectorySize : public deThread{
private:
	const decString pDirectory;
	const deVirtualFileSystem::Ref pVFS;
	uint64_t pSize;
	bool pAbort;
	bool pFailed;
	deMutex pMutex;
	decStringList pException;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create. */
	deglbCalculateDirectorySize(const char *directory);
	
	/** \brief Clean up. */
	~deglbCalculateDirectorySize() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Directory to scan. */
	decString GetDirectory();
	
	/** \brief Directory size. */
	uint64_t GetSize();
	
	/** \brief Calculating size failed. */
	bool GetFailed();
	
	/** \brief Exception if failed. */
	decStringList GetException();
	
	/** \brief Abort thread. */
	void Abort();
	
	/** \brief Run function of the thread. */
	void Run() override;
	
	/** \brief For internal use only. */
	void IncrementSize(int size);
	/*@}*/
	
	
	
private:
	void pScanDirectory(const decPath &directory);
};

#endif
