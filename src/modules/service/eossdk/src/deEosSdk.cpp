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

#include <string.h>

#include "deEosSdk.h"
#include "deEosSdkServiceEos.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>

#ifdef OS_UNIX
	#include <unistd.h>
	#include <linux/limits.h>
#elif defined OS_W32
	#include <dragengine/app/deOSWindows.h>
#endif



// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *EosSdkCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *SsdkCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deEosSdk( *loadableModule );
	}catch( ... ){
		return nullptr;
	}
}


// Class deEosSdk
/////////////////////

deEosSdk *deEosSdk::globalModule = nullptr;


// Constructor, destructor
////////////////////////////

deEosSdk::deEosSdk( deLoadableModule& loadableModule ) :
deBaseServiceModule( loadableModule ),
pSdkInited( false ){
	globalModule = this;
}

deEosSdk::~deEosSdk(){
	if( pSdkInited ){
		LogInfo( "Shut down EOS SDK" );
	}
	
	globalModule = nullptr;
}


// Management
///////////////

decStringSet deEosSdk::GetSupportedServices(){
	decStringSet names;
	names.Add( deEosSdkServiceEos::serviceName );
	return names;
}

void deEosSdk::InitSdk( const deServiceObject::Ref &data ){
	if( pSdkInited ){
		return;
	}
	
	LogInfo( "Initialize EOS SDK" );
}

deBaseServiceService* deEosSdk::CreateService( deService *service,
const char *name, const deServiceObject::Ref &data ){
	DEASSERT_NOTNULL( service )
	
	if( strcmp( name, deEosSdkServiceEos::serviceName ) == 0 ){
		return new deEosSdkServiceEos( *this, service, data );
	}
	
	return nullptr;
}

void deEosSdk::FrameUpdate( float ){
	if( pSdkInited ){
	}
}
