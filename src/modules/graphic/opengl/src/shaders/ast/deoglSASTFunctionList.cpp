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

#include "deoglSASTFunction.h"
#include "deoglSASTFunctionList.h"

#include <dragengine/common/exceptions.h>



// Class deoglSASTFunction
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSASTFunctionList::deoglSASTFunctionList(){
}

deoglSASTFunctionList::~deoglSASTFunctionList(){
	RemoveAll();
}



// Management
///////////////

int deoglSASTFunctionList::GetCount() const{
	return pFunctions.GetCount();
}

int deoglSASTFunctionList::IndexOf( deoglSASTFunction *function ) const{
	return pFunctions.IndexOf( function );
}

int deoglSASTFunctionList::IndexOfNamed( const char *name ) const{
	const int count = pFunctions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglSASTFunction* )pFunctions.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool deoglSASTFunctionList::Has( deoglSASTFunction *function ) const{
	return pFunctions.Has( function );
}

bool deoglSASTFunctionList::HasNamed( const char *name ) const{
	const int count = pFunctions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglSASTFunction* )pFunctions.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

deoglSASTFunction *deoglSASTFunctionList::GetAt( int index ) const{
	return ( deoglSASTFunction* )pFunctions.GetAt( index );
}

deoglSASTFunction *deoglSASTFunctionList::GetNamed( const char *name ) const{
	const int count = pFunctions.GetCount();
	deoglSASTFunction *function;
	int i;
	
	for( i=0; i<count; i++ ){
		function = ( deoglSASTFunction* )pFunctions.GetAt( i );
		
		if( function->GetName().Equals( name ) ){
			return function;
		}
	}
	
	return NULL;
}

void deoglSASTFunctionList::Add( deoglSASTFunction *function ){
	pFunctions.Add( function );
}

void deoglSASTFunctionList::Remove( deoglSASTFunction *function ){
	pFunctions.Remove( function );
}

void deoglSASTFunctionList::RemoveAll(){
	pFunctions.RemoveAll();
}
