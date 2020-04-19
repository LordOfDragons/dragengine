/* 
 * Drag[en]gine IGDE Synthesizer Editor
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
#include <stdio.h>

#include "seLink.h"
#include "../seSynthesizer.h"
#include "../controller/seController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>



// Class seLink
/////////////////

// Constructor, destructor
////////////////////////////

seLink::seLink() :
pSynthesizer( NULL ),
pEngLink( NULL ),
pName( "Link" ),
pController( NULL ),
pRepeat( 1 ){
}

seLink::seLink( const seLink &copy ) :
pSynthesizer( NULL ),
pEngLink( NULL ),
pName( copy.pName ),
pController( NULL ),
pRepeat( copy.pRepeat ),
pCurve( copy.pCurve )
{
	pController = copy.pController;
	if( pController ){
		pController->AddReference();
	}
}

seLink::~seLink(){
	SetSynthesizer( NULL );
	
	if( pController ){
		pController->FreeReference();
	}
}



// Management
///////////////

void seLink::SetSynthesizer( seSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	if( pSynthesizer && pEngLink ){
		pSynthesizer->GetEngineSynthesizer()->RemoveLink( pEngLink );
		pEngLink = NULL;
	}
	
	pSynthesizer = synthesizer;
	
	if( synthesizer ){
		deSynthesizerLink *link = NULL;
		
		try{
			link = new deSynthesizerLink;
			synthesizer->GetEngineSynthesizer()->AddLink( link );
			pEngLink = link;
			
		}catch( const deException & ){
			if( link ){
				link->FreeReference();
			}
			throw;
		}
		
		UpdateController();
		
		pEngLink->SetRepeat( pRepeat );
		NotifyLinkChanged();
		
		UpdateCurve();
	}
}

void seLink::SetName( const char *name ){
	if( pName.Equals( name ) ){
		return;
	}
	
	pName = name;
	
	if( pSynthesizer ){
		pSynthesizer->NotifyLinkNameChanged( this );
	}
}

void seLink::SetController( seController *controller ){
	if( controller == pController ){
		return;
	}
	
	if( pController ){
		pController->FreeReference();
	}
	
	pController = controller;
	
	if( controller ){
		controller->AddReference();
	}
	
	UpdateController();
	
	if( pSynthesizer ){
		pSynthesizer->NotifyLinkChanged( this );
	}
}

void seLink::SetRepeat( int repeat ){
	if( repeat < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( repeat == pRepeat ){
		return;
	}
	
	pRepeat = repeat;
	
	if( pEngLink ){
		pEngLink->SetRepeat( pRepeat );
		NotifyLinkChanged();
	}
	
	if( pSynthesizer ){
		pSynthesizer->NotifyLinkChanged( this );
	}
}



void seLink::NotifyLinkChanged(){
	if( pEngLink && pSynthesizer ){
		pSynthesizer->GetEngineSynthesizer()->NotifyLinksChanged();
	}
}



void seLink::UpdateController(){
	if( ! pEngLink ){
		return;
	}
	
	deSynthesizer *engSynthesizer = pSynthesizer->GetEngineSynthesizer();
	int indexController = -1;
	
	if( pController && engSynthesizer ){
		indexController = pController->GetEngineControllerIndex();
	}
	
	pEngLink->SetController( indexController );
	
	NotifyLinkChanged();
}

void seLink::UpdateCurve(){
	if( ! pEngLink ){
		return;
	}
	
	pEngLink->GetCurve() = pCurve;
	NotifyLinkChanged();
}



// Operators
//////////////

seLink &seLink::operator=( const seLink &copy ){
	SetName( copy.pName );
	SetController( copy.pController );
	SetRepeat( copy.pRepeat );
	pCurve = copy.pCurve;
	return *this;
}
