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
#include "deEffectOverlayImage.h"
#include "deEffectFilterKernel.h"
#include "deEffectColorMatrix.h"
#include "deEffectDistortImage.h"
#include "deEffectVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deEffectVisitor
//////////////////////////

// constructor, destructor
////////////////////////////

deEffectVisitor::deEffectVisitor(){
}

deEffectVisitor::~deEffectVisitor(){
}



// Visiting
/////////////

void deEffectVisitor::VisitEffect( deEffect &effect ){
}

void deEffectVisitor::VisitFilterKernel( deEffectFilterKernel &effect ){
	VisitEffect( effect );
}

void deEffectVisitor::VisitOverlayImage( deEffectOverlayImage &effect ){
	VisitEffect( effect );
}

void deEffectVisitor::VisitColorMatrix( deEffectColorMatrix &effect ){
	VisitEffect( effect );
}

void deEffectVisitor::VisitDistortImage( deEffectDistortImage &effect ){
	VisitEffect( effect );
}
