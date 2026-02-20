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

#include "deoglOcclusionQuery.h"
#include "deoglOcclusionQueryManager.h"

#include <dragengine/common/exceptions.h>


// Class deoglOcclusionQueryManager
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionQueryManager::deoglOcclusionQueryManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pFreeQueryCount(0),
pActiveQuery(nullptr){
}


// Management
///////////////

deoglOcclusionQuery *deoglOcclusionQueryManager::ClaimQuery(){
	if(pFreeQueryCount == 0){
		pQueries.Add(deTUniqueReference<deoglOcclusionQuery>::New(pRenderThread));
		return pQueries.Last();
		
	}else{
		pFreeQueryCount--;
		return pQueries.GetAt(pFreeQueryCount);
	}
}

void deoglOcclusionQueryManager::ReleaseQuery(deoglOcclusionQuery *query){
	DEASSERT_NOTNULL(query)
	
	const int index = IndexOfQuery(query, pFreeQueryCount, pQueries.GetCount());
	DEASSERT_TRUE(index != -1)
	
	if(index > pFreeQueryCount){
		pQueries.Swap(index, pFreeQueryCount);
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
	return pQueries.IndexOf(query);
}
