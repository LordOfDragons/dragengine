/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

deEffect::deEffect(deEffectManager *manager) : deResource(manager){
	pEnabled = true;
	pPeerGraphic = NULL;
}

deEffect::~deEffect(){
	if(pPeerGraphic) delete pPeerGraphic;
}



// Management
///////////////

void deEffect::SetEnabled(bool enabled){
	if(pEnabled != enabled){
		pEnabled = enabled;
		pNotifyEnabledChanged();
	}
}



// peers
//////////

void deEffect::SetPeerGraphic(deBaseGraphicEffect *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// visiting
/////////////

void deEffect::Visit(deEffectVisitor &visitor){
	visitor.VisitEffect(*this);
}



// Private Functions
//////////////////////

void deEffect::pNotifyEnabledChanged(){
	if(pPeerGraphic) pPeerGraphic->EnabledChanged();
}
