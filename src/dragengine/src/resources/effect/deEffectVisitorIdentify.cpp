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
#include "deEffectFilterKernel.h"
#include "deEffectOverlayImage.h"
#include "deEffectColorMatrix.h"
#include "deEffectDistortImage.h"
#include "deEffectVisitorIdentify.h"
#include "../../common/exceptions.h"



// Class deEffectVisitorIdentify
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deEffectVisitorIdentify::deEffectVisitorIdentify(){
	Reset();
}

deEffectVisitorIdentify::~deEffectVisitorIdentify(){
}



// Management
///////////////

deEffectFilterKernel &deEffectVisitorIdentify::CastToFilterKernel() const{
	if( pType != eetFilterKernel ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectFilterKernel* )pEffect );
}

deEffectOverlayImage &deEffectVisitorIdentify::CastToOverlayImage() const{
	if( pType != eetOverlayImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectOverlayImage* )pEffect );
}

deEffectColorMatrix &deEffectVisitorIdentify::CastToColorMatrix() const{
	if( pType != eetColorMatrix ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectColorMatrix* )pEffect );
}

deEffectDistortImage &deEffectVisitorIdentify::CastToDistortImage() const{
	if( pType != eetDistortImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deEffectDistortImage* )pEffect );
}

void deEffectVisitorIdentify::Reset(){
	pEffect = NULL;
	pType = eetUnknown;
}



// Visiting
/////////////

void deEffectVisitorIdentify::VisitEffect( deEffect &effect ){
	pEffect = &effect;
	pType = eetUnknown;
}

void deEffectVisitorIdentify::VisitFilterKernel( deEffectFilterKernel &effect ){
	pEffect = &effect;
	pType = eetFilterKernel;
}

void deEffectVisitorIdentify::VisitOverlayImage( deEffectOverlayImage &effect ){
	pEffect = &effect;
	pType = eetOverlayImage;
}

void deEffectVisitorIdentify::VisitColorMatrix( deEffectColorMatrix &effect ){
	pEffect = &effect;
	pType = eetColorMatrix;
}

void deEffectVisitorIdentify::VisitDistortImage( deEffectDistortImage &effect ){
	pEffect = &effect;
	pType = eetDistortImage;
}
