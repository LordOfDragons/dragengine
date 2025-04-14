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

#include "deCRSimplyQuit.h"
#include "decrsqCoreFault.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/systems/modules/deLoadableModule.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *SQCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Point
////////////////

deBaseModule *SQCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deCRSimplyQuit( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deCRSimplyQuit
/////////////////////////

// Constructor, destructor
////////////////////////////

deCRSimplyQuit::deCRSimplyQuit( deLoadableModule &loadableModule ) :
deBaseCrashRecoveryModule( loadableModule ),
pCoreFault( NULL ){
}

deCRSimplyQuit::~deCRSimplyQuit(){
}



// Management
///////////////

bool deCRSimplyQuit::Init(){
	pCoreFault = new decrsqCoreFault( *this );
	return true;
}

void deCRSimplyQuit::CleanUp(){
	if( pCoreFault ){
		delete pCoreFault;
		pCoreFault = NULL;
	}
}



// Crash Management
/////////////////////

bool deCRSimplyQuit::RecoverFromError(){
	pPrintTrace();
	
	return false;
}



// Private Functions
//////////////////////

void deCRSimplyQuit::pPrintTrace(){
	deErrorTrace *trace = GetGameEngine()->GetErrorTrace();
	int i, count;
	
	LogError( "Error Trace:" );
	
	count = trace->GetPointCount();
	for( i=0; i<count; i++ ){
		pPrintTracePoint( i + 1, trace->GetPoint( i ) );
	}
}

void deCRSimplyQuit::pPrintTracePoint(int number, deErrorTracePoint *point ){
	int i, count = point->GetValueCount();
	
	if( point->GetSourceModule() ){
		LogErrorFormat( "%i) %s %s at %i\n", number, point->GetSourceModule()->GetName().GetString(),
			point->GetSourceFunction().GetString(), point->GetSourceLine() );
		
	}else{
		LogErrorFormat( "%i) game engine %s at %i\n", number,
			point->GetSourceFunction().GetString(), point->GetSourceLine() );
	}
	
	for( i=0; i<count; i++ ){
		pPrintTraceValue( 1, point->GetValue( i ) );
	}
}

void deCRSimplyQuit::pPrintTraceValue( int level, deErrorTraceValue *value ){
	int i, count = value->GetSubValueCount();
	decString text;
	
	for( i=0; i<level; i++ ) text.Append( "  " );
	LogErrorFormat( "%s- %s = '%s'", text.GetString(),
		value->GetName().GetString(), value->GetValue().GetString() );
	
	for( i=0; i<count; i++ ){
		pPrintTraceValue( level + 1, value->GetSubValue( i ) );
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deCRModuleInternal : public deInternalModule{
public:
	deCRModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("SimplyQuit");
		SetDescription("Simply quits the engine gracefully on system failures.");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtCrashRecovery);
		SetDirectoryName("simplyquit");
		SetPriority(0);
		SetIsFallback(true);
	}
	
	void CreateModule() override{
		SetModule(SQCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deCRRegisterInternalModule(deModuleSystem *system){
	return new deCRModuleInternal(system);
}
#endif
