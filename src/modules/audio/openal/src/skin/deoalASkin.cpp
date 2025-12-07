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

#include "deoalASkin.h"
#include "deoalSkinTexture.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/common/exceptions.h>



// Class deoalASkin
/////////////////////

// Constructor, destructor
////////////////////////////

deoalASkin::deoalASkin(deoalAudioThread &audioThread, const deSkin &skin) :
pAudioThread(audioThread),
pFilename(skin.GetFilename()),
pTextures(NULL),
pTextureCount(0),
pAffectsSound(false)
{
	try{
		pCreateTextures(skin);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	LEAK_CHECK_CREATE(audioThread, Skin);
}

deoalASkin::~deoalASkin(){
	LEAK_CHECK_FREE(pAudioThread, Skin);
	
	pCleanUp();
}



// Management
///////////////

const deoalSkinTexture &deoalASkin::GetTextureAt(int index) const{
	if(index < 0 || index >= pTextureCount){
		DETHROW(deeInvalidParam);
	}
	return pTextures[index];
}



// Private Functions
//////////////////////

void deoalASkin::pCleanUp(){
	if(pTextures){
		delete [] pTextures;
	}
}

void deoalASkin::pCreateTextures(const deSkin &skin){
	const int count = skin.GetTextureCount();
	if(count == 0){
		return;
	}
	
	pTextures = new deoalSkinTexture[count];
	for(pTextureCount=0; pTextureCount<count; pTextureCount++){
		pTextures[pTextureCount].ProcessProperties(*skin.GetTextureAt(pTextureCount));
		pAffectsSound |= pTextures[pTextureCount].GetAffectsSound();
	}
}
