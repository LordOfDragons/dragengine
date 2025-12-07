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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglCombinedTexture.h"
#include "deoglCombinedTextureList.h"

#include <dragengine/common/exceptions.h>



// Class deoglCombinedTextureList
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCombinedTextureList::deoglCombinedTextureList(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pRoot(NULL),
pTail(NULL),
pCount(0){
}

deoglCombinedTextureList::~deoglCombinedTextureList(){
	while(pRoot){
		pTail = pRoot;
		pRoot = pRoot->GetLLNext();
		delete pTail;
	}
	pTail = NULL;
}



// Management
///////////////

deoglCombinedTexture *deoglCombinedTextureList::GetWith(const decColor &color, deoglRImage *images[4]){
	deoglCombinedTexture *combinedTexture = pRoot;
	
	// if there exists already a combined texture with these properties add a usage and return it
	while(combinedTexture){
		if(combinedTexture->Equals(color, images)){
			combinedTexture->AddUsage();
			return combinedTexture;
		}
		
		combinedTexture = combinedTexture->GetLLNext();
	}
	
	// otherwise create a new combined texture with these parameters and return it
	combinedTexture = new deoglCombinedTexture(pRenderThread, color, images);
	
	try{
		combinedTexture->CalcHashCode();
		
		if(pTail){
			pTail->SetLLNext(combinedTexture);
			combinedTexture->SetLLPrev(pTail);
			pTail = combinedTexture;
			
		}else{
			pRoot = combinedTexture;
			pTail = combinedTexture;
		}
		pCount++;
		
	}catch(const deException &){
		delete combinedTexture;
		throw;
	}
	
	return combinedTexture;
}



void deoglCombinedTextureList::Remove(deoglCombinedTexture *config){
	// WARNING remove usage typically happens during main thread. if the combined texture
	//         would be removed from it can be potentially deleted. combined texture
	//         though holds references to render images (which can cause deletion loop)
	//         as well as opengl objects (which would incorrectly try to delete them in
	//         the opengl context free main thread). if removing is desired the combined
	//         texture list has to take care of delaying deletion of the opengl objects
	//         until a later time. this makes this call main thread safe
	
	if(!config){
		DETHROW(deeInvalidParam);
	}
	
	if(config->GetLLPrev()){
		config->GetLLPrev()->SetLLNext(config->GetLLNext());
	}
	if(config->GetLLNext()){
		config->GetLLNext()->SetLLPrev(config->GetLLPrev());
	}
	if(pRoot == config){
		pRoot = config->GetLLNext();
	}
	if(pTail == config){
		pTail = config->GetLLPrev();
	}
	pCount--;
	
	delete config;
}
