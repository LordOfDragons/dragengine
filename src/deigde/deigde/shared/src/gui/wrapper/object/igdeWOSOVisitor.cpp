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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeWOSOVisitor.h"
#include "igdeWOSOBillboard.h"
#include "igdeWOSOLight.h"
#include "igdeWOSOSpeaker.h"
#include "igdeWOSOParticleEmitter.h"
#include "igdeWOSOForceField.h"
#include "igdeWOSOEnvMapProbe.h"
#include "igdeWOSONavigationSpace.h"
#include "igdeWOSONavigationBlocker.h"
#include "igdeWOSOComponent.h"
#include "igdeWOSOWorld.h"


// Class igdeWOSOVisitor
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOVisitor::igdeWOSOVisitor(){
}

igdeWOSOVisitor::~igdeWOSOVisitor(){
}



// Visiting
/////////////

void igdeWOSOVisitor::VisitSubObject( igdeWOSubObject& ){
}

void igdeWOSOVisitor::VisitBillboard( igdeWOSOBillboard &billboard ){
	VisitSubObject( billboard );
}

void igdeWOSOVisitor::VisitLight( igdeWOSOLight &light ){
	VisitSubObject( light );
}

void igdeWOSOVisitor::VisitSpeaker( igdeWOSOSpeaker &speaker ){
	VisitSubObject( speaker );
}

void igdeWOSOVisitor::VisitParticleEmitter( igdeWOSOParticleEmitter &particleEmitter ){
	VisitSubObject( particleEmitter );
}

void igdeWOSOVisitor::VisitForceField( igdeWOSOForceField &forceField ){
	VisitSubObject( forceField );
}

void igdeWOSOVisitor::VisitEnvMapProbe( igdeWOSOEnvMapProbe &envMapProbe ){
	VisitSubObject( envMapProbe );
}

void igdeWOSOVisitor::VisitNavigationSpace( igdeWOSONavigationSpace &navigationSpace ){
	VisitSubObject( navigationSpace );
}

void igdeWOSOVisitor::VisitNavigationBlocker( igdeWOSONavigationBlocker &navigationBlocker ){
	VisitSubObject( navigationBlocker );
}

void igdeWOSOVisitor::VisitComponent( igdeWOSOComponent &component ){
	VisitSubObject( component );
}

void igdeWOSOVisitor::VisitWorld(igdeWOSOWorld &world){
	VisitSubObject(world);
}
