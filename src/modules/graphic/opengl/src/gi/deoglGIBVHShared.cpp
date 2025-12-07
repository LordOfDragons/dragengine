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

#include "deoglGIBVHShared.h"
#include "../capabilities/deoglCapabilities.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOShared.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIBVHShared
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVHShared::deoglGIBVHShared(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pTBONodeBox(NULL),
pTBOIndex(NULL),
pTBOFace(NULL),
pTBOVertex(NULL),
pTBOTexCoord(NULL),
pTBOMaterial(NULL),
pTBOMaterial2(NULL)
{
	try{
		pTBONodeBox = new deoglDynamicTBOFloat32(renderThread, 4);
		pTBOIndex = new deoglDynamicTBOUInt16(renderThread, 2);
		pTBOFace = new deoglDynamicTBOUInt16(renderThread, 4);
		pTBOVertex = new deoglDynamicTBOFloat32(renderThread, 4);
		pTBOTexCoord = new deoglDynamicTBOFloat16(renderThread, 2);
		pTBOMaterial = new deoglDynamicTBOUInt32(renderThread, 4);
		pTBOMaterial2 = new deoglDynamicTBOFloat16(renderThread, 4);
		
		pSharedTBONode = new deoglDynamicTBOShared(pTBOIndex, 1, pTBONodeBox, 2);
		pSharedTBOFace = new deoglDynamicTBOShared(pTBOFace, 1, pTBOTexCoord, 3);
		pSharedTBOVertex = new deoglDynamicTBOShared(pTBOVertex, 1);
		pSharedTBOMaterial = new deoglDynamicTBOShared(pTBOMaterial, 1, pTBOMaterial2, 3);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglGIBVHShared::~deoglGIBVHShared(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglGIBVHShared::pCleanUp(){
	if(pSharedTBONode){
		pSharedTBONode->FreeReference();
	}
	if(pSharedTBOFace){
		pSharedTBOFace->FreeReference();
	}
	if(pSharedTBOVertex){
		pSharedTBOVertex->FreeReference();
	}
	if(pSharedTBOMaterial){
		pSharedTBOMaterial->FreeReference();
	}
	
	if(pTBONodeBox){
		pTBONodeBox->FreeReference();
	}
	if(pTBOIndex){
		pTBOIndex->FreeReference();
	}
	if(pTBOFace){
		pTBOFace->FreeReference();
	}
	if(pTBOVertex){
		pTBOVertex->FreeReference();
	}
	if(pTBOTexCoord){
		pTBOTexCoord->FreeReference();
	}
	if(pTBOMaterial){
		pTBOMaterial->FreeReference();
	}
	if(pTBOMaterial2){
		pTBOMaterial2->FreeReference();
	}
}
