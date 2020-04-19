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
#include "gdeCategoryList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeCategory
//////////////////////

// Constructor, destructor
////////////////////////////

gdeCategoryList::gdeCategoryList(){
}

gdeCategoryList::gdeCategoryList( const gdeCategoryList &list ) :
pCategories( list.pCategories ){
}

gdeCategoryList::~gdeCategoryList(){
}



// Management
///////////////

int gdeCategoryList::GetCount() const{
	return pCategories.GetCount();
}

gdeCategory *gdeCategoryList::GetAt( int index ) const{
	return ( gdeCategory* )pCategories.GetAt( index );
}

gdeCategory *gdeCategoryList::GetNamed( const char *name ) const{
	const int count = pCategories.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeCategory * const category = ( gdeCategory* )pCategories.GetAt( i );
		
		if( category->GetName() == name ){
			return category;
		}
	}
	
	return NULL;
}

gdeCategory *gdeCategoryList::GetWithPath( const char *path ) const{
	const decStringList components( decString( path ).Split( '/' ) );
	const int count = components.GetCount();
	if( count == 0 ){
		return NULL;
	}
	
	gdeCategory *category = GetNamed( components.GetAt( 0 ) );
	if( ! category ){
		return NULL;
	}
	
	int i;
	for( i=1; i<count; i++ ){
		category = category->GetCategories().GetNamed( components.GetAt( i ) );
		if( ! category ){
			return NULL;
		}
	}
	
	return category;
}

int gdeCategoryList::IndexOf( gdeCategory *category ) const{
	return pCategories.IndexOf( category );
}

int gdeCategoryList::IndexOfNamed( const char *name ) const{
	const int count = pCategories.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeCategory* )pCategories.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool gdeCategoryList::Has( gdeCategory *category ) const{
	return pCategories.Has( category );
}

bool gdeCategoryList::HasNamed( const char *name ) const{
	const int count = pCategories.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeCategory* )pCategories.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void gdeCategoryList::Add( gdeCategory *category ){
	if( ! category || HasNamed( category->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pCategories.Add( category );
}

void gdeCategoryList::Remove( gdeCategory *category ){
	pCategories.Remove( category );
}

void gdeCategoryList::RemoveAll(){
	pCategories.RemoveAll();
}



gdeCategoryList &gdeCategoryList::operator=( const gdeCategoryList &list ){
	pCategories = list.pCategories;
	return *this;
}
