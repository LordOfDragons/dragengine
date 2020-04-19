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

#include "deDESynthesizer.h"
#include "desynCaches.h"
#include "desynConfiguration.h"
#include "desynCommandExecuter.h"
#include "buffer/desynSharedBufferList.h"
#include "parameters/desynParameter.h"
#include "parameters/desynParameterList.h"
#include "sound/desynDecodeBuffer.h"
#include "sound/desynSound.h"
#include "synthesizer/desynSynthesizer.h"
#include "synthesizer/desynSynthesizerInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/systems/modules/deModuleParameter.h>

#include <dragengine/dragengine_configuration.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DESynthesizerCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *DESynthesizerCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deDESynthesizer( *loadableModule );
		
	}catch( const deException & ){
	}
	
	return module;
}



// Class deDESynthesizer
//////////////////////////

// Constructor, destructor
////////////////////////////

deDESynthesizer::deDESynthesizer( deLoadableModule &loadableModule ) :
deBaseSynthesizerModule( loadableModule ),

pConfiguration( NULL ),
pCommandExecuter( NULL ),
pParameterList( NULL ),

pDecodeBuffer( NULL ),
pSharedBufferList( NULL ),
pCaches( NULL )
{
	try{
		pCommandExecuter = new desynCommandExecuter( *this );
		pConfiguration = new desynConfiguration( *this );
		
		pParameterList = new desynParameterList;
		
	}catch( const deException &e ){
		LogException( e );
		throw;
	}
}

deDESynthesizer::~deDESynthesizer(){
	CleanUp();
	
	if( pConfiguration ){
		delete pConfiguration;
	}
	if( pCommandExecuter ){
		delete pCommandExecuter;
	}
	if( pParameterList ){
		delete pParameterList;
	}
}



// Management
///////////////

bool deDESynthesizer::Init(){
	try{
		pCaches = new desynCaches( *this );
		pConfiguration->LoadConfig();
		pDecodeBuffer = new desynDecodeBuffer( ( 44100 / 10 ) * 4 );
		pSharedBufferList = new desynSharedBufferList;
		
	}catch( const deException &e ){
		LogException( e );
		return false;
	}
	
	// debug
	/*
	LogWarn( "" );
	LogWarn( "*********************** DESynthesizer TODO ************************" );
	LogWarn( "" );
	LogWarn( "Chain and Sound Sources need Streaming Support." );
	LogWarn( "" );
	LogWarn( "*******************************************************************" );
	LogWarn( "" );
	*/
	
	return true;
}

void deDESynthesizer::CleanUp(){
	if( pSharedBufferList ){
		delete pSharedBufferList;
		pSharedBufferList = NULL;
	}
	if( pDecodeBuffer ){
		delete pDecodeBuffer;
		pDecodeBuffer = NULL;
	}
	
	if( pConfiguration ){
		pConfiguration->SaveConfig();
	}
	
	if( pCaches ){
		delete pCaches;
		pCaches = NULL;
	}
}



// Synthesizer management
///////////////////////////

deBaseSynthesizerSound *deDESynthesizer::CreateSound( deSound *sound ){
	return new desynSound( *this, *sound );
}

deBaseSynthesizerSynthesizer *deDESynthesizer::CreateSynthesizer( deSynthesizer *synthesizer ){
	return new desynSynthesizer( *this, *synthesizer );
}

deBaseSynthesizerSynthesizerInstance *deDESynthesizer::CreateSynthesizerInstance(
deSynthesizerInstance *instance ){
	return new desynSynthesizerInstance( *this, *instance );
}



// Parameters
///////////////

int deDESynthesizer::GetParameterCount() const{
	return pParameterList->GetParameterCount();
}

void deDESynthesizer::GetParameterInfo( int index, deModuleParameter &info ) const{
	info = pParameterList->GetParameterAt( index );
}

int deDESynthesizer::IndexOfParameterNamed( const char *name ) const{
	return pParameterList->IndexOfParameterNamed( name );
}

decString deDESynthesizer::GetParameterValue( const char *name ) const{
	return pParameterList->GetParameterNamed( name ).GetParameterValue();
}

void deDESynthesizer::SetParameterValue( const char *name, const char *value ){
	pParameterList->GetParameterNamed( name ).SetParameterValue( value );
}

void deDESynthesizer::SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( pCommandExecuter ){
		pCommandExecuter->ExecuteCommand( command, answer );
		
	}else{
		answer.SetFromUTF8( "Internal Error!" );
	}
}
