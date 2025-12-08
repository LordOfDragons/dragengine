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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seClipboardDataSource.h"
#include "../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seClipboardDataSource
//////////////////////////////

const char * const seClipboardDataSource::TYPE_NAME = "source";

// Constructor, destructor
////////////////////////////

seClipboardDataSource::seClipboardDataSource(seSource *source) :
igdeClipboardData(TYPE_NAME)
{
	seSource *copySource = NULL;
	
	try{
		copySource = source->CreateCopy();
		pSources.Add(copySource);
		copySource->FreeReference();
		
	}catch(const deException &){
		throw;
	}
}

seClipboardDataSource::seClipboardDataSource(const seSourceList &sources) :
igdeClipboardData(TYPE_NAME)
{
	const int count = sources.GetCount();
	seSource *source = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			source = sources.GetAt(i)->CreateCopy();
			pSources.Add(source);
			source->FreeReference();
			source = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
}

seClipboardDataSource::~seClipboardDataSource(){
	pSources.RemoveAll();
}
