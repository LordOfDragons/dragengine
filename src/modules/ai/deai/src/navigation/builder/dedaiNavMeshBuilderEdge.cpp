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

#include "dedaiNavMeshBuilderEdge.h"

#include <dragengine/common/exceptions.h>



// Class dedaiNavMeshBuilderEdge
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavMeshBuilderEdge::dedaiNavMeshBuilderEdge(){
	pIndex = 0;
	pVertex1 = 0;
	pVertex2 = 0;
	pFace1 = NULL;
	pFace2 = NULL;
}

dedaiNavMeshBuilderEdge::dedaiNavMeshBuilderEdge( int vertex1, int vertex2 ){
	pIndex = 0;
	pVertex1 = vertex1;
	pVertex2 = vertex2;
	pFace1 = NULL;
	pFace2 = NULL;
}

dedaiNavMeshBuilderEdge::~dedaiNavMeshBuilderEdge(){
}



// Management
///////////////

void dedaiNavMeshBuilderEdge::SetIndex( int index ){
	pIndex = index;
}

void dedaiNavMeshBuilderEdge::SetVertex1( int vertex ){
	pVertex1 = vertex;
}

void dedaiNavMeshBuilderEdge::SetVertex2( int vertex ){
	pVertex2 = vertex;
}

void dedaiNavMeshBuilderEdge::SetFace1( dedaiNavMeshBuilderFace *face ){
	pFace1 = face;
}

void dedaiNavMeshBuilderEdge::SetFace2( dedaiNavMeshBuilderFace *face ){
	pFace2 = face;
}
