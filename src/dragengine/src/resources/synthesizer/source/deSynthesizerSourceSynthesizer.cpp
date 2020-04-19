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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSynthesizerSourceSynthesizer.h"
#include "deSynthesizerSourceVisitor.h"
#include "../deSynthesizer.h"
#include "../../sound/deSound.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSourceSynthesizer
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceSynthesizer::deSynthesizerSourceSynthesizer() :
pSynthesizer( NULL ),
pConnections( NULL ),
pConnectionCount( 0 ){
}

deSynthesizerSourceSynthesizer::~deSynthesizerSourceSynthesizer(){
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
	if( pConnections ){
		delete [] pConnections;
	}
}



// Management
///////////////

void deSynthesizerSourceSynthesizer::SetSynthesizer( deSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
	
	pSynthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddReference();
	}
	
	UpdateConnectionCount();
}



void deSynthesizerSourceSynthesizer::UpdateConnectionCount(){
	if( pConnections ){
		delete [] pConnections;
		pConnections = NULL;
		pConnectionCount = 0;
	}
	
	if( ! pSynthesizer ){
		return;
	}
	
	const int controllerCount = pSynthesizer->GetControllerCount();
	if( controllerCount == 0 ){
		return;
	}
	
	pConnections = new int[ controllerCount ];
	for( pConnectionCount=0; pConnectionCount<controllerCount; pConnectionCount++ ){
		pConnections[ pConnectionCount ] = -1;
	}
}

int deSynthesizerSourceSynthesizer::GetConnectionAt( int childController ) const{
	if( childController < 0 || childController >= pConnectionCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pConnections[ childController ];
}

void deSynthesizerSourceSynthesizer::SetConnectionAt( int childController, int localController ){
	if( childController < 0 || childController >= pConnectionCount ){
		DETHROW( deeInvalidParam );
	}
	if( localController < -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pConnections[ childController ] = localController;
}



// Visiting
/////////////

void deSynthesizerSourceSynthesizer::Visit( deSynthesizerSourceVisitor &visitor ){
	visitor.VisitSynthesizer( *this );
}
