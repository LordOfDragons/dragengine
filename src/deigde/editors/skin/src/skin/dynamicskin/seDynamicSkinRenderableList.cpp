/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seDynamicSkinRenderable.h"
#include "seDynamicSkinRenderableList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seDynamicSkinRenderable
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seDynamicSkinRenderableList::seDynamicSkinRenderableList(){
}

seDynamicSkinRenderableList::~seDynamicSkinRenderableList(){
}



// Management
///////////////

int seDynamicSkinRenderableList::GetCount() const{
	return pRenderables.GetCount();
}

seDynamicSkinRenderable *seDynamicSkinRenderableList::GetAt( int index ) const{
	return ( seDynamicSkinRenderable* )pRenderables.GetAt( index );
}

seDynamicSkinRenderable *seDynamicSkinRenderableList::GetNamed( const char *name ) const{
	const int count = pRenderables.GetCount();
	seDynamicSkinRenderable *renderable;
	int p;
	
	for( p=0; p<count; p++ ){
		renderable = ( seDynamicSkinRenderable* )pRenderables.GetAt( p );
		
		if( renderable->GetName().Equals( name ) ){
			return renderable;
		}
	}
	
	return NULL;
}

int seDynamicSkinRenderableList::IndexOf( seDynamicSkinRenderable *renderable ) const{
	return pRenderables.IndexOf( renderable );
}

int seDynamicSkinRenderableList::IndexOfNamed( const char *name ) const{
	const int count = pRenderables.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seDynamicSkinRenderable* )pRenderables.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool seDynamicSkinRenderableList::Has( seDynamicSkinRenderable *renderable ) const{
	return pRenderables.Has( renderable );
}

bool seDynamicSkinRenderableList::HasNamed( const char *name ) const{
	const int count = pRenderables.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seDynamicSkinRenderable* )pRenderables.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void seDynamicSkinRenderableList::Add( seDynamicSkinRenderable *renderable ){
	if( ! renderable || HasNamed( renderable->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pRenderables.Add( renderable );
}

void seDynamicSkinRenderableList::InsertAt( seDynamicSkinRenderable *renderable, int index ){
	if( ! renderable || HasNamed( renderable->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pRenderables.Insert( renderable, index );
}

void seDynamicSkinRenderableList::MoveTo( seDynamicSkinRenderable *renderable, int index ){
	pRenderables.Move( renderable, index );
}

void seDynamicSkinRenderableList::Remove( seDynamicSkinRenderable *renderable ){
	pRenderables.Remove( renderable );
}

void seDynamicSkinRenderableList::RemoveAll(){
	pRenderables.RemoveAll();
}



seDynamicSkinRenderableList &seDynamicSkinRenderableList::operator=( const seDynamicSkinRenderableList &list ){
	pRenderables = list.pRenderables;
	return *this;
}
