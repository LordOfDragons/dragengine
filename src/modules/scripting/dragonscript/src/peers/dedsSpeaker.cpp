/* 
 * Drag[en]gine DragonScript Script Module
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

#include "dedsSpeaker.h"
#include "../deScriptingDragonScript.h"
#include "../classes/sound/deClassSpeaker.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/sound/deSpeaker.h>



// Class dedsSpeaker
//////////////////////

// Constructor, destructor
////////////////////////////

dedsSpeaker::dedsSpeaker( deScriptingDragonScript &ds, deSpeaker *speaker ) :
pDS( ds ),
pSpeaker( speaker ),
pValueOwner( NULL )
{
	if( ! speaker ){
		DSTHROW( dueInvalidParam );
	}
	
	dsRunTime &rt = *ds.GetScriptEngine()->GetMainRunTime();
	
	pValueOwner = rt.CreateValue();
}

dedsSpeaker::~dedsSpeaker(){
	if( ! pValueOwner ){
		return;
	}
	
	// check if the speaker resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the speaker resource
	// being deleted due to links breaking while freeing the value. if this is the
	// case delay the deletion until a safe time
	if( pSpeaker && pSpeaker->GetRefCount() > 0 ){
		pDS.AddValueDeleteLater( pValueOwner );
		
	}else{
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValueOwner );
	}
	
	pValueOwner = NULL;
}



// Management
///////////////

dsRealObject *dedsSpeaker::GetOwner() const{
	return pValueOwner->GetRealObject();
}

void dedsSpeaker::SetOwner( dsRealObject *object ){
	if( ! pValueOwner ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValueOwner, object );
		
	}else{
		rt.ClearValue( pValueOwner );
	}
}
