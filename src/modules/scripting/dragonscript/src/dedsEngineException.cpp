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

#include <string.h>

#include "dedsEngineException.h"

#include <dragengine/dragengine_configuration.h>


// Class dedsEngineException
//////////////////////////////

// Constructor
////////////////

dedsEngineException::dedsEngineException( const char *description, const char *file, int line ) :
duException( "InvalidAction", description, description, file, line ),
pStrDescription( description ),
pStrFile( file ){
}

dedsEngineException::~dedsEngineException(){
	delete [] pStrDescription;
	delete [] pStrFile;
}

dedsEngineException dedsEngineException::Wrap( const deException &exception ){
	const decString &file = exception.GetFile();
	const decString &description = exception.GetDescription();
	// const decString description( exception.GetDescription() + ":\n" + exception.GetBacktrace().Join("\n") );
	
	const int lenDescription = description.GetLength();
	char * const strDescription = new char[ lenDescription + 1 ];
	#ifdef OS_W32
	strcpy_s( strDescription, lenDescription + 1, description );
	#else
	strcpy( strDescription, description );
	#endif
	
	const int lenFile = file.GetLength();
	char * const strFile = new char[ lenFile + 1 ];
	#ifdef OS_W32
	strcpy_s( strFile, lenFile + 1, file );
	#else
	strcpy( strFile, file );
	#endif
	
	return dedsEngineException( strDescription, strFile, exception.GetLine() );
}