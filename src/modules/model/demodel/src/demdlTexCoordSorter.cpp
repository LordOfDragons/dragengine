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

#include "demdlTexCoordSorter.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////

//#define TCBUCKET_COUNT 4096
//#define TCBUCKET_COUNT 8192
#define TCBUCKET_COUNT 16384


// Class demdlTexCoordSorter
//////////////////////////////

// Constructor, destructor
////////////////////////////

demdlTexCoordSorter::demdlTexCoordSorter() :
pFaceCount(0),
pTexCoordSetCount(0),
pTCBuckets(decTList<sBucketEntry*>(TCBUCKET_COUNT, nullptr)){
}


// Management
///////////////

void demdlTexCoordSorter::Resize(int faceCount, int texCoordSetCount){
	DEASSERT_TRUE(faceCount >= 0)
	DEASSERT_TRUE(texCoordSetCount >= 0)
	
	pTCBucketEntries.RemoveAll();
	pTexCoords.RemoveAll();
	pFaceTexCoords.RemoveAll();
	pFaceCorners.RemoveAll();
	
	if(faceCount > 0){
		pFaceCorners.AddRange(faceCount * 3, -1);
		pTCBucketEntries.AddRange(faceCount * 3, {});
		pTexCoords.EnlargeCapacityDiscard(faceCount * 3);
		
		if(texCoordSetCount > 0){
			pFaceTexCoords.AddRange(faceCount * 3 * texCoordSetCount, {});
		}
	}
	
	pFaceCount = faceCount;
	pTexCoordSetCount = texCoordSetCount;
}

void demdlTexCoordSorter::Sort(){
	if(pFaceCount == 0){
		return;
	}
	
	const int strideTexCoords = pTexCoordSetCount * 3; // 3 corners per face
	
	pTCBuckets.SetRangeAt(0, pTCBuckets.GetCount(), nullptr);
	
	int nextBucketEntry = 0;
	int nextFaceCorner = 0;
	int f;
	
	for(f=0; f<pFaceCount; f++){
		const int faceTexCoords = strideTexCoords * f;
		
		int c;
		for(c=0; c<3; c++){
			// calculate hash code over all texture coordinates belonging to corner
			const int texCoords = faceTexCoords + pTexCoordSetCount * c;
			const int hash = HashTexCoords(texCoords);
			
			// find matching entry in bucket with hash code
			sBucketEntry *tail = nullptr;
			sBucketEntry *entry = pTCBuckets.GetAt(hash);
			
			while(entry){
				if(TexCoordsAreEqual(entry->texCoords, texCoords)){
					// entry found. store index and stop the search
					pFaceCorners.SetAt(nextFaceCorner++, entry->index);
					break;
				}
				tail = entry;
				entry = entry->next;
			}
			
			// entry not found. add new entry and use next index
			if(!entry){
				const int nextTexCoords = pTexCoords.GetCount();
				pTexCoords.Add(texCoords);
				pFaceCorners.SetAt(nextFaceCorner++, nextTexCoords);
				
				pTCBucketEntries.SetAt(nextBucketEntry, {texCoords, nextTexCoords});
				if(tail){
					tail->next = &pTCBucketEntries.GetAt(nextBucketEntry);
					
				}else{
					pTCBuckets[hash] = &pTCBucketEntries.GetAt(nextBucketEntry);
				}
				nextBucketEntry++;
			}
		}
	}
	
	/*
	int *faceCorners = pFaceCorners;
	int f, s, c, t;
	
	pTexCoordCount = 0;
	
	for(f=0; f<pFaceCount; f++){
		const int baseIndexFace = f * 3 * pTexCoordSetCount;
		
		for(c=0; c<3; c++){
			const decVector2 * const ptrTexCoord1 = pFaceTexCoords + (baseIndexFace + pTexCoordSetCount * c);
			
			for(t=0; t<pTexCoordCount; t++){
				const decVector2 * const ptrTexCoord2 = pTexCoords + pTexCoordSetCount * t;
				
				for(s=0; s<pTexCoordSetCount; s++){
					if(!ptrTexCoord2[s].IsEqualTo(ptrTexCoord1[s])){
						break; // no match possible, skip the rest
					}
				}
				
				if(s == pTexCoordSetCount){
					break; // all entries match
				}
			}
			
			*faceCorners++ = t;
			
			if(t == pTexCoordCount){
				decVector2 * const ptrTexCoord2 = pTexCoords + pTexCoordSetCount * t;
				
				for(s=0; s<pTexCoordSetCount; s++){
					ptrTexCoord2[s] = ptrTexCoord1[s];
				}
				
				pTexCoordCount++;
			}
		}
	}
	*/
}



int demdlTexCoordSorter::GetFaceCornerAt(int face, int corner) const{
	DEASSERT_TRUE(face >= 0)
	DEASSERT_TRUE(face < pFaceCount)
	DEASSERT_TRUE(corner >= 0)
	DEASSERT_TRUE(corner <= 2)
	
	return pFaceCorners.GetAt(face * 3 + corner);
}

void demdlTexCoordSorter::SetFaceCornerAt(int face, int corner, int texCoord){
	DEASSERT_TRUE(face >= 0)
	DEASSERT_TRUE(face < pFaceCount)
	DEASSERT_TRUE(corner >= 0)
	DEASSERT_TRUE(corner <= 2)
	DEASSERT_TRUE(texCoord >= 0)
	
	pFaceCorners.SetAt(face * 3 + corner, texCoord);
}

const decVector2 &demdlTexCoordSorter::GetFaceTexCoordAt(int face, int corner, int texCoordSet) const{
	DEASSERT_TRUE(face >= 0)
	DEASSERT_TRUE(face < pFaceCount)
	DEASSERT_TRUE(corner >= 0)
	DEASSERT_TRUE(corner <= 2)
	DEASSERT_TRUE(texCoordSet >= 0)
	DEASSERT_TRUE(texCoordSet < pTexCoordSetCount)
	
	return pFaceTexCoords.GetAt(face * 3 * pTexCoordSetCount + pTexCoordSetCount * corner + texCoordSet);
}

void demdlTexCoordSorter::SetFaceTexCoordAt(int face, int corner, int texCoordSet, const decVector2 &texCoord){
	DEASSERT_TRUE(face >= 0)
	DEASSERT_TRUE(face < pFaceCount)
	DEASSERT_TRUE(corner >= 0)
	DEASSERT_TRUE(corner <= 2)
	DEASSERT_TRUE(texCoordSet >= 0)
	DEASSERT_TRUE(texCoordSet < pTexCoordSetCount)
	
	pFaceTexCoords.SetAt(face * 3 * pTexCoordSetCount + pTexCoordSetCount * corner + texCoordSet, texCoord);
}

const decVector2 &demdlTexCoordSorter::GetTexCoordAt(int index, int texCoordSet) const{
	DEASSERT_TRUE(texCoordSet >= 0)
	DEASSERT_TRUE(texCoordSet < pTexCoordSetCount)
	
	return pFaceTexCoords.GetAt(pTexCoords.GetAt(index) + texCoordSet);
}



// Private Functions
//////////////////////

int demdlTexCoordSorter::HashTexCoords(int baseIndex) const{
	const float accum = pFaceTexCoords.Inject(0.0f, baseIndex, baseIndex + pTexCoordSetCount,
		[&](float acc, const decVector2 &tc){
			return acc + tc.x + tc.y;
		});
	return ((int)((accum - (int)accum) * TCBUCKET_COUNT) + TCBUCKET_COUNT) % TCBUCKET_COUNT;
}

bool demdlTexCoordSorter::TexCoordsAreEqual(int texCoords1, int texCoords2) const{
	int i;
	for(i=0; i<pTexCoordSetCount; i++){
		if(pFaceTexCoords.GetAt(texCoords1 + i) != pFaceTexCoords.GetAt(texCoords2 + i)){
			return false;
		}
	}
	return true;
}
