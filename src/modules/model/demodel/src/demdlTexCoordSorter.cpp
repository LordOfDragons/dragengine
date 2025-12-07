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
#include <stdlib.h>
#include <string.h>

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
pFaceCorners(NULL),
pFaceTexCoords(NULL),

pTCBuckets(NULL),
pTCBucketEntries(NULL),
pTexCoords(NULL),
pTexCoordCount(0)
{
	pTCBuckets = new sBucketEntry*[TCBUCKET_COUNT];
}

demdlTexCoordSorter::~demdlTexCoordSorter(){
	if(pTCBuckets){
		delete [] pTCBuckets;
	}
	if(pTCBucketEntries){
		delete [] pTCBucketEntries;
	}
	if(pTexCoords){
		delete [] pTexCoords;
	}
	if(pFaceTexCoords){
		delete [] pFaceTexCoords;
	}
	if(pFaceCorners){
		delete [] pFaceCorners;
	}
}



// Management
///////////////

void demdlTexCoordSorter::Resize(int faceCount, int texCoordSetCount){
	if(faceCount < 0 || texCoordSetCount < 0){
		DETHROW(deeInvalidParam);
	}
	
	pTexCoordCount = 0;
	pTexCoordSetCount = 0;
	pFaceCount = 0;
	
	if(pTCBucketEntries){
		delete [] pTCBucketEntries;
		pTCBucketEntries = NULL;
	}
	if(pTexCoords){
		delete [] pTexCoords;
		pTexCoords = NULL;
	}
	if(pFaceTexCoords){
		delete [] pFaceTexCoords;
		pFaceTexCoords = NULL;
	}
	if(pFaceCorners){
		delete [] pFaceCorners;
		pFaceCorners = NULL;
	}
	
	if(faceCount > 0){
		pFaceCorners = new int[faceCount * 3];
		
		if(texCoordSetCount > 0){
			pFaceTexCoords = new decVector2[faceCount * 3 * texCoordSetCount];
			pTCBucketEntries = new sBucketEntry[faceCount * 3];
			pTexCoords = new const decVector2*[faceCount * 3];
		}
	}
	
	pFaceCount = faceCount;
	pTexCoordSetCount = texCoordSetCount;
}

void demdlTexCoordSorter::Sort(){
	const int strideTexCoords = pTexCoordSetCount * 3; // 3 corners per face
	sBucketEntry *nextEntry = pTCBucketEntries;
	int *corners = pFaceCorners;
	int f, c;
	
	memset(pTCBuckets, 0, sizeof(sBucketEntry*) * TCBUCKET_COUNT);
	
	for(f=0; f<pFaceCount; f++){
		const decVector2 * const baseTexCoords = pFaceTexCoords + strideTexCoords * f;
		
		for(c=0; c<3; c++){
			// calculate hash code over all texture coordinates belonging to corner
			const decVector2 * const texCoords = baseTexCoords + pTexCoordSetCount * c;
			const int hash = HashTexCoords(texCoords);
			
			// find matching entry in bucket with hash code
			sBucketEntry *tail = NULL;
			sBucketEntry *entry = pTCBuckets[hash];
			
			while(entry){
				if(TexCoordsAreEqual(entry->texCoords, texCoords)){
					// entry found. store index and stop the search
					*corners++ = entry->index;
					break;
				}
				tail = entry;
				entry = entry->next;
			}
			
			// entry not found. add new entry and use next index
			if(! entry){
				pTexCoords[pTexCoordCount] = texCoords; // for GetTexCoordAt()
				*corners++ = pTexCoordCount;
				
				nextEntry->texCoords = texCoords;
				nextEntry->index = pTexCoordCount;
				nextEntry->next = NULL;
				
				if(tail){
					tail->next = nextEntry;
					
				}else{
					pTCBuckets[hash] = nextEntry;
				}
				
				pTexCoordCount++;
				nextEntry++;
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
					if(! ptrTexCoord2[s].IsEqualTo(ptrTexCoord1[s])){
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
	if(face < 0 || face >= pFaceCount || corner < 0 || corner > 2){
		DETHROW(deeInvalidParam);
	}
	
	return pFaceCorners[face * 3 + corner];
}

void demdlTexCoordSorter::SetFaceCornerAt(int face, int corner, int texCoord){
	if(face < 0 || face >= pFaceCount || corner < 0 || corner > 2 || texCoord < 0){
		DETHROW(deeInvalidParam);
	}
	
	pFaceCorners[face * 3 + corner] = texCoord;
}

const decVector2 &demdlTexCoordSorter::GetFaceTexCoordAt(int face, int corner, int texCoordSet) const{
	if(face < 0 || face >= pFaceCount || corner < 0 || corner > 2 || texCoordSet < 0 || texCoordSet >= pTexCoordSetCount){
		DETHROW(deeInvalidParam);
	}
	
	return pFaceTexCoords[face * 3 * pTexCoordSetCount + pTexCoordSetCount * corner + texCoordSet];
}

void demdlTexCoordSorter::SetFaceTexCoordAt(int face, int corner, int texCoordSet, const decVector2 &texCoord){
	if(face < 0 || face >= pFaceCount || corner < 0 || corner > 2 || texCoordSet < 0 || texCoordSet >= pTexCoordSetCount){
		DETHROW(deeInvalidParam);
	}
	
	pFaceTexCoords[face * 3 * pTexCoordSetCount + pTexCoordSetCount * corner + texCoordSet] = texCoord;
}

const decVector2 &demdlTexCoordSorter::GetTexCoordAt(int index, int texCoordSet) const{
	if(index < 0 || index >= pTexCoordCount || texCoordSet < 0 || texCoordSet >= pTexCoordSetCount){
		DETHROW(deeInvalidParam);
	}
	
	return pTexCoords[index][texCoordSet];
}



// Private Functions
//////////////////////

int demdlTexCoordSorter::HashTexCoords(const decVector2 *texCoords) const{
	float accum = 0.0f;
	int i;
	
	for(i=0; i<pTexCoordSetCount; i++){
		accum += texCoords[i].x;
		accum += texCoords[i].y;
	}
	
	return ((int)((accum - (int)accum) * TCBUCKET_COUNT) + TCBUCKET_COUNT) % TCBUCKET_COUNT;
}

bool demdlTexCoordSorter::TexCoordsAreEqual(const decVector2 *texCoords1, const decVector2 *texCoords2) const{
	int i;
	for(i=0; i<pTexCoordSetCount; i++){
		if(! texCoords1[i].IsEqualTo(texCoords2[i])){
			// one texture coordinate does not match. we can stop checking here
			return false;
		}
	}
	
	// entire texture coordinates match
	return true;
}
