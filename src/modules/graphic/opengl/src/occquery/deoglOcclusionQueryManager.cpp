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

#include "deoglOcclusionQuery.h"
#include "deoglOcclusionQueryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionQueryManager
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionQueryManager::deoglOcclusionQueryManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
	pQueries = NULL;
	pQueryCount = 0;
	pQuerySize = 0;
	
	pFreeQueryCount = 0;
	
	pActiveQuery = NULL;
}

deoglOcclusionQueryManager::~deoglOcclusionQueryManager(){
	if(pQueries){
		while(pQueryCount > 0){
			pQueryCount--;
			delete pQueries[pQueryCount];
		}
		
		delete [] pQueries;
	}
}



// Management
///////////////

deoglOcclusionQuery *deoglOcclusionQueryManager::ClaimQuery(){
	if(pFreeQueryCount == 0){
		if(pQueryCount == pQuerySize){
			int newSize = pQuerySize + 10;
			deoglOcclusionQuery **newArray = new deoglOcclusionQuery*[newSize];
			if(! newArray) DETHROW(deeOutOfMemory);
			if(pQueries){
				memcpy(newArray, pQueries, sizeof(deoglOcclusionQuery*) * pQuerySize);
				delete [] pQueries;
			}
			memset(newArray + pQuerySize, '\0', sizeof(deoglOcclusionQuery*) * (newSize - pQuerySize));
			pQueries = newArray;
			pQuerySize = newSize;
		}
		
		if(! pQueries[pQueryCount]){
			pQueries[pQueryCount] = new deoglOcclusionQuery(pRenderThread);
			if(! pQueries[pQueryCount]) DETHROW(deeOutOfMemory);
		}
		
		return pQueries[pQueryCount++];
		
	}else{
		pFreeQueryCount--;
		return pQueries[pFreeQueryCount];
	}
}

void deoglOcclusionQueryManager::ReleaseQuery(deoglOcclusionQuery *query){
	if(! query) DETHROW(deeInvalidParam);
	
	int index = IndexOfQuery(query, pFreeQueryCount, pQueryCount);
	
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(index > pFreeQueryCount){
		deoglOcclusionQuery *exchange = pQueries[index];
		pQueries[index] = pQueries[pFreeQueryCount];
		pQueries[pFreeQueryCount] = exchange;
	}
	
	pFreeQueryCount++;
}

void deoglOcclusionQueryManager::SetActiveQuery(deoglOcclusionQuery *query){
	pActiveQuery = query;
}

void deoglOcclusionQueryManager::EndActiveQuery(){
	if(pActiveQuery){
		pActiveQuery->EndQuery();
	}
}



// Private Functions
//////////////////////

int deoglOcclusionQueryManager::IndexOfQuery(deoglOcclusionQuery *query, int from, int to) const{
	int i;
	
	for(i=from; i<to; i++){
		if(pQueries[i] == query){
			return i;
		}
	}
	
	return -1;
}
