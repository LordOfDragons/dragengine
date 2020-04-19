/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
