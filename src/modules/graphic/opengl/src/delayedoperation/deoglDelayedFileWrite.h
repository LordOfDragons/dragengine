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

#ifndef _DEOGLDELAYEDFILEWRITE_H_
#define _DEOGLDELAYEDFILEWRITE_H_

#include <dragengine/common/file/decPath.h>

#include <dragengine/common/file/decMemoryFile.h>
class deVirtualFileSystem;
class decBaseFileWriter;



/**
 * Delayed file write.
 */
class deoglDelayedFileWrite{
private:
	decMemoryFile::Ref pMemoryFile;
	const decPath pPath;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create delayed file write object. */
	deoglDelayedFileWrite(const decPath &path);
	
	/** Clean up delayed file write object. */
	~deoglDelayedFileWrite();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create file writer for the memory file. */
	decBaseFileWriter *GetFileWriter() const;
	
	/**
	 * Write file to virtual file system.
	 * \details This has to be called during main thread.
	 */
	void SaveFile(deVirtualFileSystem &vfs);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
