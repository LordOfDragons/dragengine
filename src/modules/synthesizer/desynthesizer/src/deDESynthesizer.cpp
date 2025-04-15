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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class desynModuleInternal : public deInternalModule{
public:
	desynModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DESynthesizer");
		SetDescription("Generate sound using synthesizers.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtSynthesizer);
		SetDirectoryName("desynthesizer");
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(DESynthesizerCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

DE_FUNC_USED deInternalModule *desynRegisterInternalModule(deModuleSystem *system){
	return new desynModuleInternal(system);
}
#endif
