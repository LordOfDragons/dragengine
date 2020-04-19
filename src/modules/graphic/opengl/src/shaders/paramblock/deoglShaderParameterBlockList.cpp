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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglShaderParameterBlock.h"
#include "deoglShaderParameterBlockList.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderParameterBlockList
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderParameterBlockList::deoglShaderParameterBlockList(){
}

deoglShaderParameterBlockList::~deoglShaderParameterBlockList(){
	pList.RemoveAll();
}



// Management
///////////////

int deoglShaderParameterBlockList::GetCount() const{
	return pList.GetCount();
}

deoglShaderParameterBlock *deoglShaderParameterBlockList::GetAt( int index ) const{
	return ( deoglShaderParameterBlock* )pList.GetAt( index );
}

int deoglShaderParameterBlockList::IndexOf( deoglShaderParameterBlock *block ) const{
	return pList.IndexOf( block );
}

bool deoglShaderParameterBlockList::Has( deoglShaderParameterBlock *block ) const{
	return pList.Has( block );
}

void deoglShaderParameterBlockList::Add( deoglShaderParameterBlock *block ){
	pList.Add( block );
}

void deoglShaderParameterBlockList::Remove( deoglShaderParameterBlock *block ){
	pList.RemoveFrom( pList.IndexOf( block ) );
}

void deoglShaderParameterBlockList::RemoveAll(){
	pList.RemoveAll();
}
