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

#include "deSynthesizer.h"
#include "deSynthesizerManager.h"
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deSynthesizerSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizer.h"



// Class deSynthesizerManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerManager::deSynthesizerManager( deEngine *engine ) :
deResourceManager( engine, ertSynthesizer ){
	SetLoggingName( "synthesizer" );
}

deSynthesizerManager::~deSynthesizerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSynthesizerManager::GetSynthesizerCount() const{
	return pSynthesizers.GetCount();
}

deSynthesizer *deSynthesizerManager::GetRootSynthesizer() const{
	return ( deSynthesizer* )pSynthesizers.GetRoot();
}

deSynthesizer *deSynthesizerManager::CreateSynthesizer(){
	deSynthesizer *synthesizer = NULL;
	
	try{
		synthesizer = new deSynthesizer( this );
		GetSynthesizerSystem()->LoadSynthesizer( synthesizer );
		
		pSynthesizers.Add( synthesizer );
		
	}catch( const deException & ){
		if( synthesizer ){
			synthesizer->FreeReference();
		}
		throw;
	}
	
	return synthesizer;
}



void deSynthesizerManager::ReleaseLeakingResources(){
	const int count = GetSynthesizerCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking synthesizers", count );
		pSynthesizers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deSynthesizerManager::SystemSynthesizerLoad(){
	deSynthesizer *synthesizer = ( deSynthesizer* )pSynthesizers.GetRoot();
	deSynthesizerSystem &synthSys = *GetSynthesizerSystem();
	
	while( synthesizer ){
		if( ! synthesizer->GetPeerSynthesizer() ){
			synthSys.LoadSynthesizer( synthesizer );
		}
		
		synthesizer = ( deSynthesizer* )synthesizer->GetLLManagerNext();
	}
}

void deSynthesizerManager::SystemSynthesizerUnload(){
	deSynthesizer *synthesizer = ( deSynthesizer* )pSynthesizers.GetRoot();
	
	while( synthesizer ){
		synthesizer->SetPeerSynthesizer( NULL );
		synthesizer = ( deSynthesizer* )synthesizer->GetLLManagerNext();
	}
}



void deSynthesizerManager::RemoveResource( deResource *resource ){
	pSynthesizers.RemoveIfPresent( resource );
}
