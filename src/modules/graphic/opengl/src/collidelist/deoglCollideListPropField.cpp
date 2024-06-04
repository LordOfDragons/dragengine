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

#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropField
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropField::deoglCollideListPropField() :
pPropField( NULL ),
pTypeCount( 0 ){
}

deoglCollideListPropField::~deoglCollideListPropField(){
	const int count = pTypes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoglCollideListPropFieldType* )pTypes.GetAt( i );
	}
}



// Management
///////////////

void deoglCollideListPropField::Clear(){
	RemoveAllTypes();
	SetPropField( NULL );
}

void deoglCollideListPropField::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition ){
	if( ! pPropField ){
		DETHROW( deeInvalidParam );
	}
	
	const decVector offset( pPropField->GetPosition() - referencePosition );
	int i;
	for( i=0; i<pTypeCount; i++ ){
		( ( deoglCollideListPropFieldType* )pTypes.GetAt( i ) )->StartOcclusionTest( occlusionTest, offset );
	}
}

bool deoglCollideListPropField::GetIsEmpty() const{
	int i;
	for( i=0; i<pTypeCount; i++ ){
		if( ( ( deoglCollideListPropFieldType* )pTypes.GetAt( i ) )->GetIsNotEmpty() ){
			return false;
		}
	}
	
	return true;
}

void deoglCollideListPropField::SetPropField( deoglRPropField *propField ){
	RemoveAllTypes();
	pPropField = propField;
}



deoglCollideListPropFieldType *deoglCollideListPropField::GetTypeAt( int index ) const{
	return ( ( deoglCollideListPropFieldType* )pTypes.GetAt( index ) );
}

deoglCollideListPropFieldType *deoglCollideListPropField::AddType( deoglRPropFieldType *type ){
	deoglCollideListPropFieldType *cltype = NULL;
	
	if( pTypeCount < pTypes.GetCount() ){
		cltype = ( deoglCollideListPropFieldType* )pTypes.GetAt( pTypeCount );
		
	}else{
		cltype = new deoglCollideListPropFieldType( *this );
		pTypes.Add( cltype );
	}
	
	cltype->SetType( type );
	pTypeCount++;
	return cltype;
}

void deoglCollideListPropField::RemoveAllTypes(){
	while( pTypeCount > 0 ){
		( ( deoglCollideListPropFieldType* )pTypes.GetAt( --pTypeCount ) )->Clear();
	}
}
