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

#include "deoglPAsyncRenderSkipSyncTimeRatio.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPAsyncRenderSkipSyncTimeRatio
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPAsyncRenderSkipSyncTimeRatio::deoglPAsyncRenderSkipSyncTimeRatio( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "asyncRenderSkipSyncTimeRatio" );
	SetDescription( "Ratio between game time and render time required to skip synchronization."
		" Used to judge if estimated render time is long enough to do another game frame update"
		" before rendering again. Decouples render frame rate from game frame rate."
		" Lower values make judging bias towards doing more game frame updates per render frames."
		" Using lower values can improve the game responsiveness while reducing render frame rate."
	);
	SetType( deModuleParameter::eptRanged );
	SetMinimumValue( 0.0f );
	SetMaximumValue( 1.0f );
	SetValueStepSize( 0.1f );
	SetCategory( ecAdvanced );
	SetDisplayName( "Render Sync Skip Time Ratio" );
}

deoglPAsyncRenderSkipSyncTimeRatio::~deoglPAsyncRenderSkipSyncTimeRatio(){
}



// Parameter Value
////////////////////

float deoglPAsyncRenderSkipSyncTimeRatio::GetParameterFloat(){
	return pOgl.GetConfiguration().GetAsyncRenderSkipSyncTimeRatio();
}

void deoglPAsyncRenderSkipSyncTimeRatio::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetAsyncRenderSkipSyncTimeRatio( value );
}
