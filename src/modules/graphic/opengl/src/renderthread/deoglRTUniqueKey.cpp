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

#include "deoglRTUniqueKey.h"

#include <dragengine/threading/deMutexGuard.h>


// Class deoglRTUniqueKey
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTUniqueKey::deoglRTUniqueKey() :
pNextKey( 0 ){
}

deoglRTUniqueKey::~deoglRTUniqueKey(){
}



// Management
///////////////

unsigned int deoglRTUniqueKey::Get(){
	const deMutexGuard lock( pMutex );
	const int index = pFreeKeys.GetCount() - 1;
	unsigned int key;
	
	if( index >= 0 ){
		key = ( unsigned int )pFreeKeys.GetAt( index );
		pFreeKeys.RemoveFrom( index );
		
	}else{
		key = pNextKey++;
	}
	
	return key;
}

void deoglRTUniqueKey::Return( unsigned int key ){
	const deMutexGuard lock( pMutex );
	pFreeKeys.Add( ( int )key );
}
