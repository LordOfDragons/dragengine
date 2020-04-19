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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeTaskSyncGameDefinition.h"
#include "aeWindowMain.h"
#include "../animator/aeAnimator.h"
#include "../animator/attachment/aeAttachment.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/exceptions.h>



// Class aeTaskSyncGameDefinition
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeTaskSyncGameDefinition::aeTaskSyncGameDefinition( aeWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	SetMessage( "Animator Editor: Synchronize Game Definition" );
}

aeTaskSyncGameDefinition::~aeTaskSyncGameDefinition(){
}



// Management
///////////////

bool aeTaskSyncGameDefinition::Step(){
	if( ! pWindowMain.GetAnimator() ){
		return false;
	}
	
	aeAnimator &animator = *pWindowMain.GetAnimator();
	
	animator.GetSky()->OnGameDefinitionChanged();
	animator.GetEnvObject()->OnGameDefinitionChanged();
	
	const int attachmentCount = animator.GetAttachmentCount();
	int i;
	
	for( i=0; i<attachmentCount; i++ ){
		animator.GetAttachmentAt( i )->GetObjectWrapper()->OnGameDefinitionChanged();
	}
	
	return false;
}
