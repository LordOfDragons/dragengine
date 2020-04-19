/* 
 * Drag[en]gine Game Engine
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deEffect.h"
#include "deEffectManager.h"
#include "deEffectVisitor.h"
#include "../../systems/modules/graphic/deBaseGraphicEffect.h"
//#include "../../systems/modules/physics/deBasePhysicsEffect.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deEffect
///////////////////

// Constructor, destructor
////////////////////////////

deEffect::deEffect( deEffectManager *manager ) : deResource( manager ){
	pEnabled = true;
	pPeerGraphic = NULL;
}

deEffect::~deEffect(){
	if( pPeerGraphic ) delete pPeerGraphic;
}



// Management
///////////////

void deEffect::SetEnabled( bool enabled ){
	if( pEnabled != enabled ){
		pEnabled = enabled;
		pNotifyEnabledChanged();
	}
}



// peers
//////////

void deEffect::SetPeerGraphic( deBaseGraphicEffect *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// visiting
/////////////

void deEffect::Visit( deEffectVisitor &visitor ){
	visitor.VisitEffect( *this );
}



// Private Functions
//////////////////////

void deEffect::pNotifyEnabledChanged(){
	if( pPeerGraphic ) pPeerGraphic->EnabledChanged();
}
