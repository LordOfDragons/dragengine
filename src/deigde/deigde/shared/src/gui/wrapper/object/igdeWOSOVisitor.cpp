/* 
 * Drag[en]gine IGDE
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
