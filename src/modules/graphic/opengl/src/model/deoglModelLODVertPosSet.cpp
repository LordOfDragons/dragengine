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

#include "deoglModelLODVertPosSet.h"

#include <dragengine/common/exceptions.h>


// Class deoglModelLODVertPosSet
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelLODVertPosSet::deoglModelLODVertPosSet() :
pVBOOffset(0),
pPositions(nullptr),
pPositionCount(0){
}

deoglModelLODVertPosSet::~deoglModelLODVertPosSet(){
	pCleanUp();
}



// Management
///////////////

void deoglModelLODVertPosSet::SetVBOOffset(int offset){
	pVBOOffset = offset;
}

void deoglModelLODVertPosSet::SetPositionCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	sPosition *positions = nullptr;
	if(count > 0){
		positions = new sPosition[count];
	}
	
	if(pPositions){
		delete [] pPositions;
	}
	
	pPositions = positions;
	pPositionCount = count;
}



// Private functions
//////////////////////

void deoglModelLODVertPosSet::pCleanUp(){
	if(pPositions){
		delete [] pPositions;
	}
}
