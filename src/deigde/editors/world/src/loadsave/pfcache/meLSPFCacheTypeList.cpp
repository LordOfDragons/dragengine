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

#include "meLSPFCacheTypeList.h"

#include <dragengine/common/exceptions.h>



// Class meLSPFCacheTypeList
//////////////////////////////

// Constructor, destructor
////////////////////////////

meLSPFCacheTypeList::meLSPFCacheTypeList(){
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
}

meLSPFCacheTypeList::~meLSPFCacheTypeList(){
	if( pTypes ) delete [] pTypes;
}



// Management
///////////////

meLSPFCacheTypeList::sType &meLSPFCacheTypeList::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ) DETHROW( deeInvalidParam );
	
	return pTypes[ index ];
}

int meLSPFCacheTypeList::IndexOfTypeWith( int vlayer, int variation ) const{
	int t;
	
	for( t=0; t<pTypeCount; t++ ){
		if( pTypes[ t ].vlayer == vlayer && pTypes[ t ].variation == variation ){
			return t;
		}
	}
	
	return -1;
}

void meLSPFCacheTypeList::AddType( int vlayer, int variation ){
	if( IndexOfTypeWith( vlayer, variation ) == -1 ){
		if( pTypeCount == pTypeSize ){
			int newSize = pTypeSize * 3 / 2 + 1;
			sType *newArray = new sType[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pTypes ){
				memcpy( newArray, pTypes, sizeof( sType ) * pTypeSize );
				delete [] pTypes;
			}
			pTypes = newArray;
			pTypeSize = newSize;
		}
		
		pTypes[ pTypeCount ].vlayer = vlayer;
		pTypes[ pTypeCount ].variation = variation;
		pTypeCount++;
	}
}

void meLSPFCacheTypeList::RemoveAllTypes(){
	pTypeCount = 0;
}
