/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglBVHNode.h"

#include <dragengine/common/exceptions.h>


// Class deoglBVHNode
///////////////////////

// Constructor, destructor
////////////////////////////

deoglBVHNode::deoglBVHNode(){
}

deoglBVHNode::~deoglBVHNode(){
}



// Management
///////////////

void deoglBVHNode::SetExtends( const decVector &minExtend, const decVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglBVHNode::SetFirstIndex( int index ){
	pFirstIndex = index;
}

void deoglBVHNode::SetPrimitiveCount( int count ){
	pPrimitiveCount = count;
}

void deoglBVHNode::Clear(){
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	pFirstIndex = 0;
	pPrimitiveCount = 0;
}
