/* 
 * Drag[en]gine IGDE Animator Editor
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

#include <stdlib.h>
#include <string.h>

#include "aeWindowMain.h"
#include "aeWindowMainListener.h"
#include "../animator/aeAnimator.h"

#include <deigde/environment/igdeEnvironment.h>

#include "dragengine/common/exceptions.h"



// Class aeWindowMainListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeWindowMainListener::aeWindowMainListener( aeWindowMain &windowMain ) :
pWindowMain( windowMain ){
}

aeWindowMainListener::~aeWindowMainListener(){
}



// Notifications
//////////////////

void aeWindowMainListener::StateChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::UndoChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::AnimatorChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::ViewChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::ModelChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::AnimationChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}



void aeWindowMainListener::ControllerChanged( aeAnimator*, aeController* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::ControllerStructureChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}



void aeWindowMainListener::ActiveRuleChanged( aeAnimator*, aeRule* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::RuleChanged( aeAnimator*, aeRule* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::RuleNameChanged( aeAnimator*, aeRule* ){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::RuleStructureChanged( aeAnimator* ){
	pWindowMain.UpdateAllActions();
}
