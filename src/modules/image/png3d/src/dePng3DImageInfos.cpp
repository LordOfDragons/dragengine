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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dePng3DImageInfos.h"

#include <dragengine/common/exceptions.h>



// Class dePng3DImageInfo
///////////////////////////

// Constructor, destructor
////////////////////////////

dePng3DImageInfo::dePng3DImageInfo( const char *nfilename ){
	if( ! nfilename ) DETHROW( deeInvalidParam );
	
	filename = nfilename;
}
dePng3DImageInfo::~dePng3DImageInfo(){
}



// Management
///////////////

int dePng3DImageInfo::GetWidth(){
	return width;
}

int dePng3DImageInfo::GetHeight(){
	return height;
}

int dePng3DImageInfo::GetDepth(){
	return depth;
}

int dePng3DImageInfo::GetComponentCount(){
	return componentCount;
}

int dePng3DImageInfo::GetBitCount(){
	return bitCount;
}
