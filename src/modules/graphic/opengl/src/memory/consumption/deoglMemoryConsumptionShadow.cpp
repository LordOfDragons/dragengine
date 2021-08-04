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

#include "deoglMemoryConsumptionShadow.h"


// Class deoglMemoryConsumptionShadow
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionShadow::deoglMemoryConsumptionShadow() :
all( "Shadow" ),
allStatic( "Shadow Static", all ),
allDynamic( "Shadow Dynamic", all ),

skyStatic( "Shadow Sky Static", allStatic ),
skyDynamic( "Shadow Sky Dynamic", allDynamic ),

solidStaticMap( "Shadow Solid Static Map", allStatic ),
solidStaticCube( "Shadow Solid Static Cube", allStatic ),
solidStaticArray( "Shadow Solid Static Array", allStatic ),
solidDynamicMap( "Shadow Solid Dynamic Map", allDynamic ),
solidDynamicCube( "Shadow Solid Dynamic Cube", allDynamic ),
solidDynamicArray( "Shadow Solid Dynamic Array", allDynamic ),

transparentStaticMapDepth( "Shadow Transparent Static Map Depth", allStatic ),
transparentStaticMapColor( "Shadow Transparent Static Map Color", allStatic ),
transparentStaticCubeDepth( "Shadow Transparent Static Cube Depth", allStatic ),
transparentStaticCubeColor( "Shadow Transparent Static Cube Color", allStatic ),
transparentDynamicMapDepth( "Shadow Transparent Dynamic Map Depth", allDynamic ),
transparentDynamicMapColor( "Shadow Transparent Dynamic Map Color", allDynamic ),
transparentDynamicCubeDepth( "Shadow Transparent Dynamic Cube Depth", allDynamic ),
transparentDynamicCubeColor( "Shadow Transparent Dynamic Cube Color", allDynamic ),

ambientStaticMap( "Shadow Solid Static Ambient Map", allStatic ),
ambientStaticCube( "Shadow Solid Static Ambient Cube", allStatic ),
ambientDynamicMap( "Shadow Solid Dynamic Ambient Map", allDynamic ),
ambientDynamicCube( "Shadow Solid Dynamic Ambient Cube", allDynamic ){
}

deoglMemoryConsumptionShadow::~deoglMemoryConsumptionShadow(){
}
