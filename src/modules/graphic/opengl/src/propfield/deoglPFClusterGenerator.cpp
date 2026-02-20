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

#include "deoglPFClusterGenerator.h"

#include <dragengine/common/exceptions.h>



// Class deoglPFClusterGenerator
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPFClusterGenerator::deoglPFClusterGenerator(const decVector2 &minExtend,
const decVector2 &maxExtend, int maxEntryCount){
	DEASSERT_TRUE(maxEntryCount >= 0)
	
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
	pMaxEntryCount = maxEntryCount;
	
	pNode1 = nullptr;
	pNode2 = nullptr;
}

deoglPFClusterGenerator::~deoglPFClusterGenerator(){
	ClearTree();
}



// Management
///////////////

void deoglPFClusterGenerator::AddEntry(int index, const decVector2 &position){
	//printf( "add entry %i (%g,%g) to node %p (entrycount %i)\n", index, position.x, position.y, this, pEntryCount );
	if(pEntries.GetCount() == pMaxEntryCount){
		decVector2 entryMinExtend, entryMaxExtend;
		
		GetEntryExtends(entryMinExtend, entryMaxExtend);
		
		if((entryMaxExtend.x - entryMinExtend.x) > (entryMaxExtend.y - entryMinExtend.y)){
			SplitAtX((entryMaxExtend.x + entryMinExtend.x) * 0.5f);
			
		}else{
			SplitAtY((entryMaxExtend.y + entryMinExtend.y) * 0.5f);
		}
		
		GetNodeContaining(position)->AddEntry(index, position);
		
	}else{
		DEASSERT_TRUE(index >= 0)
		pEntries.Add({index, position});
	}
}

void deoglPFClusterGenerator::RemoveAllEntries(){
	pEntries.SetCountDiscard(0);
}

void deoglPFClusterGenerator::GetEntryExtends(decVector2 &minExtend, decVector2 &maxExtend){
	if(pEntries.IsEmpty()){
		minExtend = (pMaxExtend - pMinExtend) * 0.5f;
		maxExtend = minExtend;
		return;
	}
	
	minExtend = pEntries[0].position;
	maxExtend = minExtend;
	
	pEntries.Visit(1, [&](const sEntry &entry){
		const decVector2 &position = entry.position;
		minExtend.SetSmallest(position);
		maxExtend.SetLargest(position);
	});
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
		return const_cast<deoglPFClusterGenerator*>(this);
	}
}

void deoglPFClusterGenerator::SplitAtX(float position){
	//printf( "SplitAtX %p (%i entries, position %g)\n", this, pEntryCount, position );
	if(pNode1 || pNode2 || pEntries.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	pNode1 = new deoglPFClusterGenerator(pMinExtend, decVector2(position, pMaxExtend.y), pMaxEntryCount);
	pNode2 = new deoglPFClusterGenerator(decVector2(position, pMinExtend.y), pMaxExtend, pMaxEntryCount);
	//printf( "  creating nodes %p and %p\n", pNode1, pNode2 );
	
	pEntries.Visit([&](const sEntry &entry){
		if(entry.position.x < position){
			pNode1->AddEntry(entry.index, entry.position);
			
		}else{
			pNode2->AddEntry(entry.index, entry.position);
		}
	});
	
	RemoveAllEntries();
}

void deoglPFClusterGenerator::SplitAtY(float position){
	//printf( "SplitAtY %p (%i entries, position %g)\n", this, pEntryCount, position );
	if(pNode1 || pNode2 || pEntries.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	pNode1 = new deoglPFClusterGenerator(pMinExtend, decVector2(pMaxExtend.x, position), pMaxEntryCount);
	pNode2 = new deoglPFClusterGenerator(decVector2(pMinExtend.x, position), pMaxExtend, pMaxEntryCount);
	//printf( "  creating nodes %p and %p\n", pNode1, pNode2 );
	
	pEntries.Visit([&](const sEntry &entry){
		if(entry.position.y < position){
			pNode1->AddEntry(entry.index, entry.position);
			
		}else{
			pNode2->AddEntry(entry.index, entry.position);
		}
	});
	
	RemoveAllEntries();
}

void deoglPFClusterGenerator::ClearTree(){
	if(pNode1){
		pNode1->ClearTree();
		delete pNode1;
		pNode1 = nullptr;
	}
	
	if(pNode2){
		pNode2->ClearTree();
		delete pNode2;
		pNode2 = nullptr;
	}
	
	RemoveAllEntries();
}
