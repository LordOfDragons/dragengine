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

#include "gdeCategory.h"

#include <dragengine/common/exceptions.h>



// Class gdeCategory
//////////////////////

// Constructor, destructor
////////////////////////////

gdeCategory::gdeCategory() :
pParent( NULL ),
pHidden( false ){
}

gdeCategory::gdeCategory( const char *name ) :
pName( name ),
pParent( NULL ),
pHidden( false ){
}

gdeCategory::gdeCategory( const gdeCategory &category ) :
pName( category.pName ),
pDescription( category.pDescription ),
pParent( NULL ),
pAutoCategorizePattern( category.pAutoCategorizePattern ),
pHidden( category.pHidden )
{
	const int count = category.pCategories.GetCount();
	gdeCategory *subCategory = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			subCategory = new gdeCategory( *category.pCategories.GetAt( i ) );
			subCategory->pParent = this;
			pCategories.Add( subCategory );
			subCategory->FreeReference();
			subCategory = NULL;
		}
		
	}catch( const deException & ){
		if( subCategory ){
			subCategory->FreeReference();
		}
		throw;
	}
}

gdeCategory::~gdeCategory(){
}



// Management
///////////////

void gdeCategory::SetName( const char *name ){
	pName = name;
}

void gdeCategory::SetDescription( const char *description ){
	pDescription = description;
}

void gdeCategory::SetParent( gdeCategory *parent ){
	pParent = parent;
}

decString gdeCategory::GetPath() const{
	if( pParent ){
		return pParent->GetPath() + "/" + pName;
		
	}else{
		return pName;
	}
}

void gdeCategory::SetAutoCategorizePattern( const decStringSet &patternList ){
	pAutoCategorizePattern = patternList;
}

void gdeCategory::SetHidden( bool hidden ){
	pHidden = hidden;
}



void gdeCategory::AddCategory( gdeCategory *category ){
	if( ! category || category->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	pCategories.Add( category );
	category->SetParent( this );
}

void gdeCategory::RemoveCategory( gdeCategory *category ){
	if( ! pCategories.Has( category ) ){
		DETHROW( deeInvalidParam );
	}
	
	category->SetParent( NULL );
	pCategories.Remove( category );
}

void gdeCategory::RemoveAllCategories(){
	const int count = pCategories.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pCategories.GetAt( i )->SetParent( NULL );
	}
	pCategories.RemoveAll();
}



gdeCategory &gdeCategory::operator=( const gdeCategory &category ){
	pName = category.pName;
	pDescription = category.pDescription;
	pAutoCategorizePattern = category.pAutoCategorizePattern;
	
	const int count = category.pCategories.GetCount();
	gdeCategory *subCategory = NULL;
	int i;
	
	pCategories.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			subCategory = new gdeCategory( *category.pCategories.GetAt( i ) );
			subCategory->pParent = this;
			pCategories.Add( subCategory );
			subCategory->FreeReference();
			subCategory = NULL;
		}
		
	}catch( const deException & ){
		if( subCategory ){
			subCategory->FreeReference();
		}
		throw;
	}
	
	return *this;
}
