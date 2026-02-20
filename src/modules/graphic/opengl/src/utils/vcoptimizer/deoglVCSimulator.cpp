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

#include "deoglVCSimulator.h"

#include <dragengine/common/exceptions.h>


// Class deoglVCSimulator
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVCSimulator::deoglVCSimulator(){
	pCacheHitCount = 0;
	pCacheMissCount = 0;
	pFaceCount = 0;
	
	pCache.SetAll(32, -1);
}

deoglVCSimulator::~deoglVCSimulator() = default;


// Management
///////////////

void deoglVCSimulator::SetCacheSize(int cacheSize){
	DEASSERT_TRUE(cacheSize > 3)
	
	pCache.SetAll(cacheSize, -1);
}


void deoglVCSimulator::Reset(){
	pCache.SetRangeAt(0, pCache.GetCount(), -1);
	
	pCacheHitCount = 0;
	pCacheMissCount = 0;
	pFaceCount = 0;
}

void deoglVCSimulator::ProcessFace(int vertex1, int vertex2, int vertex3){
	DEASSERT_TRUE(vertex1 >= 0)
	DEASSERT_TRUE(vertex2 >= 0)
	DEASSERT_TRUE(vertex3 >= 0)
	
	pFaceCount++;
	TestVertexAgainstCache(vertex1);
	TestVertexAgainstCache(vertex2);
	TestVertexAgainstCache(vertex3);
}



float deoglVCSimulator::GetAvgCacheMissRatio() const{
	if(pFaceCount == 0){
		return 0.0f;
		
	}else{
		return (float)pCacheMissCount / (float)pFaceCount;
	}
}


void deoglVCSimulator::TestVertexAgainstCache(int vertex){
	int * const cache = pCache.GetArrayPointer();
	
	int i;
	for(i=0; i<pCache.GetCount(); i++){
		if(cache[i] == vertex){
			pCacheHitCount++;
			return;
		}
	}
	
	pCacheMissCount++;
	for(i=pCache.GetCount()-1; i>0; i--){
		cache[i] = cache[i - 1];
	}
	cache[0] = vertex;
}
