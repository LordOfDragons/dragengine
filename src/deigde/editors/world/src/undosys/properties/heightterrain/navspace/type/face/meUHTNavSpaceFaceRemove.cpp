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

#include "meUHTNavSpaceFaceRemove.h"
#include "../../../../../../world/terrain/meHeightTerrainNavSpaceType.h"
#include "../../../../../../world/terrain/meHeightTerrainNavSpaceFace.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceFaceRemove
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceFaceRemove::meUHTNavSpaceFaceRemove(const decObjectOrderedSet &faces) :
pFaces(NULL),
pFaceCount(0)
{
	if(faces.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	const int count = faces.GetCount();
	
	if(count == 1){
		SetShortInfo("Height terrain nav-space remove face");
		
	}else{
		SetShortInfo("Height terrain nav-space remove faces");
	}
	
	try{
		pFaces = new sFace[count];
		
		for(pFaceCount=0; pFaceCount<count; pFaceCount++){
			sFace &face = pFaces[pFaceCount];
			
			face.face = (meHeightTerrainNavSpaceFace*)faces.GetAt(pFaceCount);
			face.face->AddReference();
			
			face.type = face.face->GetType();
			if(!face.type){
				DETHROW(deeInvalidParam);
			}
			face.type->AddReference();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUHTNavSpaceFaceRemove::~meUHTNavSpaceFaceRemove(){
	pCleanUp();
}



// Management
///////////////

void meUHTNavSpaceFaceRemove::Undo(){
	int i;
	
	for(i=0; i<pFaceCount; i++){
		pFaces[i].type->AddFace(pFaces[i].face);
	}
}

void meUHTNavSpaceFaceRemove::Redo(){
	int i;
	
	for(i=0; i<pFaceCount; i++){
		pFaces[i].type->RemoveFace(pFaces[i].face);
	}
}



// Private functions
//////////////////////

void meUHTNavSpaceFaceRemove::pCleanUp(){
	if(!pFaces){
		return;
	}
	
	int i;
	
	for(i=0; i<pFaceCount; i++){
		if(pFaces[i].face){
			pFaces[i].face->FreeReference();
		}
		if(pFaces[i].type){
			pFaces[i].type->FreeReference();
		}
	}
}
