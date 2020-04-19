/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSASTDeclaration.h"
#include "deoglSASTDeclarationList.h"

#include <dragengine/common/exceptions.h>



// Class deoglSASTDeclaration
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTDeclarationList::deoglSASTDeclarationList(){
}

deoglSASTDeclarationList::~deoglSASTDeclarationList(){
	RemoveAll();
}



// Management
///////////////

int deoglSASTDeclarationList::GetCount() const{
	return pDeclarations.GetCount();
}

int deoglSASTDeclarationList::IndexOf( deoglSASTDeclaration *declaration ) const{
	return pDeclarations.IndexOf( declaration );
}

int deoglSASTDeclarationList::IndexOfNamed( const char *name ) const{
	const int count = pDeclarations.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglSASTDeclaration* )pDeclarations.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool deoglSASTDeclarationList::Has( deoglSASTDeclaration *declaration ) const{
	return pDeclarations.Has( declaration );
}

bool deoglSASTDeclarationList::HasNamed( const char *name ) const{
	const int count = pDeclarations.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglSASTDeclaration* )pDeclarations.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

deoglSASTDeclaration *deoglSASTDeclarationList::GetAt( int index ) const{
	return ( deoglSASTDeclaration* )pDeclarations.GetAt( index );
}

deoglSASTDeclaration *deoglSASTDeclarationList::GetNamed( const char *name ) const{
	const int count = pDeclarations.GetCount();
	deoglSASTDeclaration *declaration;
	int i;
	
	for( i=0; i<count; i++ ){
		declaration = ( deoglSASTDeclaration* )pDeclarations.GetAt( i );
		
		if( declaration->GetName().Equals( name ) ){
			return declaration;
		}
	}
	
	return NULL;
}

void deoglSASTDeclarationList::Add( deoglSASTDeclaration *declaration ){
	pDeclarations.Add( declaration );
}

void deoglSASTDeclarationList::Remove( deoglSASTDeclaration *declaration ){
	pDeclarations.Remove( declaration );
}

void deoglSASTDeclarationList::RemoveAll(){
	pDeclarations.RemoveAll();
}
