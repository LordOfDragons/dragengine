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
#include <stdlib.h>

#include "deoglPDefRenUsePOTs.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../rendering/defren/deoglDeferredRendering.h"

#include <dragengine/common/exceptions.h>



// Class deoglPDefRenUsePOTs
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPDefRenUsePOTs::deoglPDefRenUsePOTs( deGraphicOpenGl &ogl ) : deoglParameterBool( ogl ){
	SetName( "defRenUsePOTs" );
	SetDescription( "Use Power-of-2 (POT) textures instead of Non-power-of-2 (NPOT) textures"
		" for deferred rendering although the hardware supports this. NPOTs have been"
		" introduced to remove the size restrictions on textures as well as reducing the"
		" amount of dead texture space required to fit an NPOT into a POT. NPOTs can though"
		" cause huge slowdown on some hardware especially ATI. In this case deferred rendering"
		" can be forced to use POTs at all times. Using POTs though GPU memory consumption"
		" tends to be a lot higher than with NPOTs. By default this is enabled to be safe."
		" If you do not have enough GPU memory you can try toggling this parameter for"
		" playing games. For multi-window applications like the editor this most probably"
		" back fires though on you on ATI." );
	SetCategory( ecExpert );
}

deoglPDefRenUsePOTs::~deoglPDefRenUsePOTs(){
}



// Management
///////////////

bool deoglPDefRenUsePOTs::GetParameterBool(){
	return pOgl.GetConfiguration().GetDefRenUsePOTs();
}

void deoglPDefRenUsePOTs::SetParameterBool( bool value ){
	pOgl.GetConfiguration().SetDefRenUsePOTs( value );
}
