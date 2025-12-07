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

#include "deoglPFClusterGenerator.h"

#include <dragengine/common/exceptions.h>



// Class deoglPFClusterGenerator
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPFClusterGenerator::deoglPFClusterGenerator(const decVector2 &minExtend, const decVector2 &maxExtend, int maxEntryCount){
	if(maxEntryCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
	pMaxEntryCount = maxEntryCount;
	
	pEntries = NULL;
	pEntryCount = 0;
	pEntrySize = 0;
	
	pNode1 = NULL;
	pNode2 = NULL;
}

deoglPFClusterGenerator::~deoglPFClusterGenerator(){
	ClearTree();
	
	if(pEntries){
		delete [] pEntries;
	}
}



// Management
///////////////

void deoglPFClusterGenerator::AddEntry(int index, const decVector2 &position){
	//printf( "add entry %i (%g,%g) to node %p (entrycount %i)\n", index, position.x, position.y, this, pEntryCount );
	if(pEntryCount == pMaxEntryCount){
		decVector2 entryMinExtend, entryMaxExtend;
		
		GetEntryExtends(entryMinExtend, entryMaxExtend);
		
		if((entryMaxExtend.x - entryMinExtend.x) > (entryMaxExtend.y - entryMinExtend.y)){
			SplitAtX((entryMaxExtend.x + entryMinExtend.x) * 0.5f);
			
		}else{
			SplitAtY((entryMaxExtend.y + entryMinExtend.y) * 0.5f);
		}
		
		GetNodeContaining(position)->AddEntry(index, position);
		
	}else{
		if(index < 0){
			DETHROW(deeInvalidParam);
		}
		
		if(pEntryCount == pEntrySize){
			const int newSize = pEntryCount + 10;
			sEntry * const newArray = new sEntry[newSize];
			
			if(pEntries){
				memcpy(newArray, pEntries, sizeof(sEntry) * pEntryCount);
				delete [] pEntries;
			}
			
			pEntries = newArray;
			pEntrySize = newSize;
		}
		
		pEntries[pEntryCount].index = index;
		pEntries[pEntryCount].position = position;
		pEntryCount++;
	}
}

void deoglPFClusterGenerator::RemoveAllEntries(){
	pEntryCount = 0;
}

void deoglPFClusterGenerator::GetEntryExtends(decVector2 &minExtend, decVector2 &maxExtend){
	if(pEntryCount == 0){
		minExtend = (pMaxExtend - pMinExtend) * 0.5f;
		maxExtend = minExtend;
		
	}else{
		int i;
		
		minExtend = pEntries[0].position;
		maxExtend = minExtend;
		
		for(i=1; i<pEntryCount; i++){
			const decVector2 &position = pEntries[i].position;
			
			if(position.x < minExtend.x){
				minExtend.x = position.x;
				
			}else if(position.x > maxExtend.x){
				maxExtend.x = position.x;
			}
			
			if(position.y < minExtend.y){
				minExtend.y = position.y;
				
			}else if(position.y > maxExtend.y){
				maxExtend.y = position.y;
			}
		}
	}
}



void deoglPFClusterGenerator::AddEntryToTree(int index, const decVector2 &position){
	GetNodeContaining(position)->AddEntry(index, position);
}

deoglPFClusterGenerator *deoglPFClusterGenerator::GetNodeContaining(const decVector2 &position) const{
	if(pNode1 && position >= pNode1->GetMinExtend() && position <= pNode1->GetMaxExtend()){
		return pNode1->GetNodeContaining(position);
	}
	
	if(pNode2 && position >= pNode2->GetMinExtend() && position <= pNode2->GetMaxExtend()){
		return pNode2->GetNodeContaining(position);
	}
	
	if(pNode1){
		/*printf( "!!! this(%g,%g)(%g,%g) n1(%g,%g)(%g,%g) n2(%g,%g)(%g,%g)\n", pMinExtend.x, pMinExtend.y, pMaxExtend.x, pMaxExtend.y,
			pNode1->pMinExtend.x, pNode1->pMinExtend.y, pNode1->pMaxExtend.x, pNode1->pMaxExtend.y,
			pNode2->pMinExtend.x, pNode2->pMinExtend.y, pNode2->pMaxExtend.x, pNode2->pMaxExtend.y);*/
		return pNode1->GetNodeContaining(position);
		
	}else{
		return (deoglPFClusterGenerator*)this;
	}
}

void deoglPFClusterGenerator::SplitAtX(float position){
	//printf( "SplitAtX %p (%i entries, position %g)\n", this, pEntryCount, position );
	if(pNode1 || pNode2 || pEntryCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	pNode1 = new deoglPFClusterGenerator(pMinExtend, decVector2(position, pMaxExtend.y), pMaxEntryCount);
	pNode2 = new deoglPFClusterGenerator(decVector2(position, pMinExtend.y), pMaxExtend, pMaxEntryCount);
	//printf( "  creating nodes %p and %p\n", pNode1, pNode2 );
	
	for(i=0; i<pEntryCount; i++){
		const sEntry &entry = pEntries[i];
		
		if(entry.position.x < position){
			pNode1->AddEntry(entry.index, entry.position);
			
		}else{
			pNode2->AddEntry(entry.index, entry.position);
		}
	}
	
	RemoveAllEntries();
}

void deoglPFClusterGenerator::SplitAtY(float position){
	//printf( "SplitAtY %p (%i entries, position %g)\n", this, pEntryCount, position );
	if(pNode1 || pNode2 || pEntryCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	pNode1 = new deoglPFClusterGenerator(pMinExtend, decVector2(pMaxExtend.x, position), pMaxEntryCount);
	pNode2 = new deoglPFClusterGenerator(decVector2(pMinExtend.x, position), pMaxExtend, pMaxEntryCount);
	//printf( "  creating nodes %p and %p\n", pNode1, pNode2 );
	
	for(i=0; i<pEntryCount; i++){
		const sEntry &entry = pEntries[i];
		
		if(entry.position.y < position){
			pNode1->AddEntry(entry.index, entry.position);
			
		}else{
			pNode2->AddEntry(entry.index, entry.position);
		}
	}
	
	RemoveAllEntries();
}

void deoglPFClusterGenerator::ClearTree(){
	if(pNode1){
		pNode1->ClearTree();
		delete pNode1;
		pNode1 = NULL;
	}
	
	if(pNode2){
		pNode2->ClearTree();
		delete pNode2;
		pNode2 = NULL;
	}
	
	RemoveAllEntries();
}
