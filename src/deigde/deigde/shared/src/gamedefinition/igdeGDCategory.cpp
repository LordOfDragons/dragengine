/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCategory.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class igdeGDCategory
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCategory::igdeGDCategory( const char *name ) :
pName( name ),
pHidden( false ),
pParent( NULL )
{
	if( pName.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
}

igdeGDCategory::~igdeGDCategory(){
	RemoveAllCategories();
}



// Management
///////////////

void igdeGDCategory::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGDCategory::SetParent( igdeGDCategory *parent ){
	pParent = parent;
}

void igdeGDCategory::SetHidden( bool hidden ){
	pHidden = hidden;
}

void igdeGDCategory::GetFullPath( decPath &path ) const{
	if( pParent ){
		pParent->GetFullPath( path );
		path.AddComponent( pName );
		
	}else{
		path.SetEmpty();
	}
}

decString igdeGDCategory::GetFullPathString() const{
	decPath path;
	GetFullPath( path );
	return path.GetPathUnix();
}

void igdeGDCategory::SetAutoCategorizePattern( const decStringSet &patternList ){
	pAutoCategorizePattern = patternList;
}

igdeGDCategory *igdeGDCategory::AutoCategorize( const decString &path ) const{
	igdeGDCategory *category = NULL;
	int longestMatch = 0;
	pAutoCategorize( path, longestMatch, category );
	return category;
}



// Children Categories
////////////////////////

int igdeGDCategory::GetCategoryCount() const{
	return pCategories.GetCount();
}

bool igdeGDCategory::HasCategory( igdeGDCategory *category ) const{
	return pCategories.Has( category );
}

bool igdeGDCategory::HasCategoryNamed( const char *name ) const{
	return IndexOfCategoryNamed( name ) != -1;
}

int igdeGDCategory::IndexOfCategory( igdeGDCategory *category ) const{
	return pCategories.IndexOf( category );
}

int igdeGDCategory::IndexOfCategoryNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	const int count = pCategories.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDCategory* )pCategories.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

igdeGDCategory *igdeGDCategory::GetCategoryAt( int index ) const{
	return ( igdeGDCategory* )pCategories.GetAt( index );
}

igdeGDCategory *igdeGDCategory::GetCategoryNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	const int count = pCategories.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDCategory * const category = ( igdeGDCategory* )pCategories.GetAt( i );
		if( category->GetName() == name ){
			return category;
		}
	}
	
	return NULL;
}

igdeGDCategory *igdeGDCategory::GetCategoryWithPath( const decPath &path ) const{
	const int count = path.GetComponentCount();
	igdeGDCategory *category = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		if( category ){
			category = category->GetCategoryNamed( path.GetComponentAt( i ) );
			
		}else{
			category = GetCategoryNamed( path.GetComponentAt( i ) );
		}
		
		if( ! category ){
			break;
		}
	}
	
	return category;
}

void igdeGDCategory::AddCategory( igdeGDCategory *category ){
	if( ! category || HasCategoryNamed( category->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pCategories.Add( category );
	category->SetParent( this );
}

void igdeGDCategory::RemoveCategory( igdeGDCategory *category ){
	const int index = pCategories.IndexOf( category );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	category->SetParent( NULL );
	pCategories.RemoveFrom( index );
}

void igdeGDCategory::RemoveAllCategories(){
	while( pCategories.GetCount() > 0 ){
		const int index = pCategories.GetCount() - 1;
		( ( igdeGDCategory* )pCategories.GetAt( index ) )->SetParent( NULL );
		pCategories.RemoveFrom( index );
	}
}



void igdeGDCategory::UpdateWith( const igdeGDCategory &category ){
	const int count = category.GetCategoryCount();
	int i;
	
	pDescription = category.GetDescription();
	pAutoCategorizePattern += category.pAutoCategorizePattern;
	pHidden = category.GetHidden();
	
	for( i=0; i<count; i++ ){
		const igdeGDCategory &childCategory = *category.GetCategoryAt( i );
		igdeGDCategory *matchingCategory = GetCategoryNamed( childCategory.GetName() );
		
		if( ! matchingCategory ){
			deObjectReference newCategory;
			newCategory.TakeOver( new igdeGDCategory( childCategory.GetName() ) );
			pCategories.Add( newCategory );
			matchingCategory = ( igdeGDCategory* )( deObject* )newCategory;
			matchingCategory->SetParent( this );
		}
		
		matchingCategory->UpdateWith( childCategory );
	}
}



// Private Functions
//////////////////////

void igdeGDCategory::pAutoCategorize( const decString &path, int &longestMatch,
igdeGDCategory *&category ) const{
	const int count = pCategories.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		igdeGDCategory * const checkCategory = ( igdeGDCategory* )pCategories.GetAt( i );
		const decStringSet &patternList = checkCategory->GetAutoCategorizePattern();
		const int patternCount = patternList.GetCount();
		
		for( j=0; j<patternCount; j++ ){
			const decString &pattern = patternList.GetAt( j );
			const int patternLen = pattern.GetLength();
			if( patternLen > longestMatch && path.MatchesPattern( pattern ) ){
				category = checkCategory;
				longestMatch = patternLen;
				break;
			}
		}
		
		checkCategory->pAutoCategorize( path, longestMatch, category );
	}
}
