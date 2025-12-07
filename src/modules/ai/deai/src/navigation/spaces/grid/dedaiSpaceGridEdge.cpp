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
#include <string.h>

#include "dedaiSpaceGridEdge.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceGridEdge
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceGridEdge::dedaiSpaceGridEdge() :
pGrid(NULL),
pIndex(0),
pVertex1(0),
pVertex2(0),
pTypeNumber1(0),
pTypeNumber2(0),
pLength(0.0f){
}

dedaiSpaceGridEdge::~dedaiSpaceGridEdge(){
}



// Management
///////////////

void dedaiSpaceGridEdge::SetGrid(dedaiSpaceGrid *grid){
	pGrid = grid;
}

void dedaiSpaceGridEdge::SetIndex(unsigned short index){
	pIndex = index;
}

void dedaiSpaceGridEdge::SetVertex1(unsigned short vertex){
	pVertex1 = vertex;
}

void dedaiSpaceGridEdge::SetVertex2(unsigned short vertex){
	pVertex2 = vertex;
}

void dedaiSpaceGridEdge::SetTypeNumber1(unsigned short typeNumber){
	pTypeNumber1 = typeNumber;
}

void dedaiSpaceGridEdge::SetTypeNumber2(unsigned short typeNumber){
	pTypeNumber2 = typeNumber;
}

void dedaiSpaceGridEdge::SetLength(float length){
	pLength = length;
}
