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

// include only once
#ifndef _DESCRIPTSOURCE_H_
#define _DESCRIPTSOURCE_H_

// includes
#include "dragengine/common/file/decPath.h"
#include <libdscript/dsScriptSource.h>

// predefinitions
class deVirtualFileSystem;
class decBaseFileReader;
class decPath;



// script source class
class deScriptSource : public dsScriptSource{
private:
	const deVirtualFileSystem &pVFS;
	decBaseFileReader *pFileReader;
	const decPath pPath;
	decString pPathString;
	int pPosition;
	int pLength;
	
public:
	// Constructor, destructor
	deScriptSource( const deVirtualFileSystem &vfs, const decPath &path );
	virtual ~deScriptSource();
	
	// Management
	virtual const char *GetName();
	virtual void Open();
	virtual int ReadData(char *Buffer, int Size);
	virtual void Close();
};

// end of include only once
#endif
