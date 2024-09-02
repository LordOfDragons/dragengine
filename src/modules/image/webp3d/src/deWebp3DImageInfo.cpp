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

#include "deWebp3DImageInfo.h"


// Class deWebp3DImageInfo
////////////////////////////

// Constructor, destructor
////////////////////////////

deWebp3DImageInfo::deWebp3DImageInfo( const decString &filename ) :
pFilename( filename ),
width( 0 ),
height( 0 ),
hasAlpha( false ),
isGrayscale( false ){
}

deWebp3DImageInfo::~deWebp3DImageInfo(){
}



// Management
///////////////

int deWebp3DImageInfo::GetWidth(){
	return width;
}

int deWebp3DImageInfo::GetHeight(){
	return height;
}

int deWebp3DImageInfo::GetDepth(){
	return depth;
}

int deWebp3DImageInfo::GetComponentCount(){
	if( hasAlpha ){
		return isGrayscale ? 2 : 4;
		
	}else{
		return isGrayscale ? 1 : 3;
	}
}

int deWebp3DImageInfo::GetBitCount(){
	return 8;
}
