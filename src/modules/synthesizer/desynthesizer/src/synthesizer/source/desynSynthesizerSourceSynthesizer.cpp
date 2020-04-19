/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynSynthesizerSourceSynthesizer.h"
#include "../desynSynthesizer.h"
#include "../desynCreateSynthesizerSource.h"
#include "../desynSynthesizerLink.h"
#include "../desynSynthesizerInstance.h"
#include "../../deDESynthesizer.h"
#include "../../buffer/desynSharedBuffer.h"
#include "../../buffer/desynSharedBufferList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>



// Class desynSynthesizerSourceSynthesizer
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSourceSynthesizer::desynSynthesizerSourceSynthesizer( desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceSynthesizer &source ) :
desynSynthesizerSource( synthesizer, firstLink, source ),
pChildSynthesizer( NULL ),
pSources( NULL ),
pSourceCount( 0 )
{
	SetSilent( ! source.GetEnabled() );
	if( GetSilent() ){
		return;
	}
	
	try{
		pCreateSources( synthesizer, firstLink, source );
		
	}catch( const deException & ){
		pClearSources();
		throw;
	}
}

desynSynthesizerSourceSynthesizer::~desynSynthesizerSourceSynthesizer(){
	pClearSources();
}



// Management
///////////////

int desynSynthesizerSourceSynthesizer::StateDataSizeSource( int offset ){
	int i, count = 0;
	for( i=0; i<pSourceCount; i++ ){
		count += pSources[ i ]->StateDataSize( offset + count );
	}
	
	return count;
}

void desynSynthesizerSourceSynthesizer::InitStateDataSource( char *stateData ){
	int i;
	for( i=0; i<pSourceCount; i++ ){
		pSources[ i ]->InitStateData( stateData );
	}
}



void desynSynthesizerSourceSynthesizer::GenerateSourceSound( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	int i;
	
	GenerateSilence( instance, buffer, samples );
	for( i=0; i<pSourceCount; i++ ){
		pSources[ i ]->GenerateSound( instance, stateData, buffer, samples, curveOffset, curveFactor );
	}
}

void desynSynthesizerSourceSynthesizer::SkipSourceSound( const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor ){
	int i;
	for( i=0; i<pSourceCount; i++ ){
		pSources[ i ]->SkipSound( instance, stateData, samples, curveOffset, curveFactor );
	}
}



// Private Functions
//////////////////////

void desynSynthesizerSourceSynthesizer::pCreateSources( desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceSynthesizer &source ){
	pClearSources();
	
	const deSynthesizer * const childSynthesizer = source.GetSynthesizer();
	if( ! childSynthesizer ){
		SetSilent( true );
		return;
	}
	
	const int count = childSynthesizer->GetSourceCount();
	if( count == 0 ){
		SetSilent( true );
		return;
	}
	
	// create controller mapping
	const int controllerCount = childSynthesizer->GetControllerCount();
	const int connectionCount = source.GetConnectionCount();
	decIntList controllerMapping;
	int i;
	
	for( i=0; i<controllerCount; i++ ){
		if( i >= connectionCount ){
			controllerMapping.Add( -1 );
			continue;
		}
		
		const int localController = source.GetConnectionAt( i );
		if( localController == -1 ){
			controllerMapping.Add( -1 );
			continue;
		}
		
		controllerMapping.Add( localController );
	}
	
	// add sub links
	const int linkCount = childSynthesizer->GetLinkCount();
	const int childFirstLink = synthesizer.GetLinkCount();
	
	if( linkCount > 0 ){
		desynSynthesizerLink *link = NULL;
		
		try{
			for( i=0; i<linkCount; i++ ){
				link = new desynSynthesizerLink( *childSynthesizer->GetLinkAt( i ), controllerMapping );
				synthesizer.AddLink( link );
				link = NULL;
			}
			
		}catch( const deException &exception ){
			if( link ){
				delete link;
			}
			throw;
		}
	}
	
	// create rules
	desynCreateSynthesizerSource createSource( synthesizer, childFirstLink );
	
	pSources = new desynSynthesizerSource*[ count ];
	
	for( pSourceCount=0; pSourceCount<count; pSourceCount++ ){
		createSource.Reset();
		
		try{
			childSynthesizer->GetSourceAt( pSourceCount )->Visit( createSource );
			pSources[ pSourceCount ] = createSource.GetSource();
			
		}catch( const deException & ){
			if( createSource.GetSource() ){
				delete createSource.GetSource();
			}
			throw;
		}
	}
}

void desynSynthesizerSourceSynthesizer::pClearSources(){
	if( ! pSources ){
		return;
	}
	
	int i;
	for( i=0; i<pSourceCount; i++ ){
		delete pSources[ i ];
	}
	delete [] pSources;
	
	pSources = NULL;
	pSourceCount = 0;
}
