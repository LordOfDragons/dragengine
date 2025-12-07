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

#include "meUDecalSkin.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalList.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalSkin
///////////////////////

// Constructor, destructor
////////////////////////////

meUDecalSkin::meUDecalSkin(meDecal *decal, const char *newskin){
	if(!decal || !newskin){
		DETHROW(deeInvalidParam);
	}
	
	meWorld * const world = decal->GetWorld();
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Set Decal Skin");
	
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		pDecals = new sDecal[1];
		
		pDecals[0].oldskin = decal->GetSkinPath();
		pDecals[0].newskin = newskin;
		pDecals[0].decal = decal;
		decal->AddReference();
		
		pDecalCount = 1;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUDecalSkin::meUDecalSkin(meDecalList &decals, const char *newskin){
	const int count = decals.GetCount();
	meDecal *decal;
	
	if(count == 0 || !newskin){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Set Decal Skins");
	
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		pDecals = new sDecal[count];
		
		for(pDecalCount=0; pDecalCount<count; pDecalCount++){
			decal = decals.GetAt(pDecalCount);
			
			pDecals[pDecalCount].oldskin = decal->GetSkinPath();
			pDecals[pDecalCount].newskin = newskin;
			pDecals[pDecalCount].decal = decal;
			decal->AddReference();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUDecalSkin::~meUDecalSkin(){
	pCleanUp();
}



// Management
///////////////

void meUDecalSkin::Undo(){
	meDecal *decal;
	int d;
	
	for(d=0; d<pDecalCount; d++){
		decal = pDecals[d].decal;
		decal->SetSkinPath(pDecals[d].oldskin.GetString());
	}
	
	pDecals[0].decal->GetWorld()->GetGuiParameters().SetElementMode(meWorldGuiParameters::eemDecal);
}

void meUDecalSkin::Redo(){
	meDecal *decal;
	int d;
	
	for(d=0; d<pDecalCount; d++){
		decal = pDecals[d].decal;
		decal->SetSkinPath(pDecals[d].newskin.GetString());
	}
	
	pDecals[0].decal->GetWorld()->GetGuiParameters().SetElementMode(meWorldGuiParameters::eemDecal);
}



// Private Functions
//////////////////////

void meUDecalSkin::pCleanUp(){
	if(pDecals){
		while(pDecalCount > 0){
			pDecalCount--;
			pDecals[pDecalCount].decal->FreeReference();
		}
		
		delete [] pDecals;
	}
}
