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

#include "deVideoWebm.h"
#include "dewmVideoDecoder.h"
#include "dewmWebmReader.h"
#include "dewmVPXTrackCallback.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>



// Class dewmVideoDecoder
///////////////////////////

// Constructor, destructor
////////////////////////////

dewmVideoDecoder::dewmVideoDecoder(deVideoWebm &module, decBaseFileReader *file) :
deBaseVideoDecoder(file),
pModule(module),
pCallback(nullptr),
pReader(nullptr),
pParser(nullptr),
pCurFrame(0)
{
	try{
		pCallback = new dewmVPXTrackCallback(module);
		pReader = new dewmWebmReader(*file);
		pParser = new webm::WebmParser;
		
	}catch(const deException &e){
		pCleanUp();
		pModule.LogException(e);
		throw;
	}
}

dewmVideoDecoder::~dewmVideoDecoder(){
	pCleanUp();
}



// Management
///////////////

int dewmVideoDecoder::GetPosition(){
	return pCurFrame;
}

void dewmVideoDecoder::SetPosition(int position){
	if(position == pCurFrame){
		return;
	}
	
	pCallback->SetResBuffer(nullptr); // process frames but do not output frame data
	
	if(position < pCurFrame){
		pReader->SetPosition(0);
		pCallback->Rewind();
		pParser->DidSeek();
		pCurFrame = 0;
	}
	
	while(pCurFrame < position){
		DEASSERT_TRUE(pParser->Feed(pCallback, pReader).ok())
		pCurFrame++;
	}
}

bool dewmVideoDecoder::DecodeFrame(void *buffer, int){
	pCallback->SetResBuffer(buffer);
	if(! pParser->Feed(pCallback, pReader).ok()){
		return false;
	}
	
	pCurFrame++;
	return true;
}



// Private Functions
//////////////////////

void dewmVideoDecoder::pCleanUp(){
	if(pParser){
		delete pParser;
	}
	if(pReader){
		delete pReader;
	}
	if(pCallback){
		delete pCallback;
	}
}
