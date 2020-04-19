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
#include <string.h>

#include <libdscript/libdscript.h>
// #include <libdscript/exceptions.h>

#include "dedsResourceLoaderTask.h"
#include "../deScriptingDragonScript.h"
#include "../classes/animation/deClassAnimation.h"
#include "../classes/graphics/deClassFont.h"
#include "../classes/graphics/deClassImage.h"
#include "../classes/graphics/deClassOcclusionMesh.h"
#include "../classes/resources/deClassResourceListener.h"
#include "../classes/sound/deClassSound.h"
#include "../classes/translation/deClassLanguagePack.h"
#include "../classes/video/deClassVideo.h"
#include "../classes/world/deClassModel.h"
#include "../classes/world/deClassRig.h"
#include "../classes/world/deClassSkin.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/deFileResource.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/video/deVideo.h>



// Class dedsResourceLoaderTask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dedsResourceLoaderTask::dedsResourceLoaderTask( deScriptingDragonScript *ds,
const char *filename, deResourceLoader::eResourceType resourceType ){
	if( ! ds || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	pDS = ds;
	pFilename = NULL;
	pResourceType = resourceType;
	
	pListeners = NULL;
	pListenerCount = 0;
	pListenerSize = 0;
	
	pFilename = new char[ strlen( filename ) + 1 ];
	strcpy( pFilename, filename );
}

dedsResourceLoaderTask::~dedsResourceLoaderTask(){
	pClearListeners();
	if( pListeners ) delete [] pListeners;
}



// Management
///////////////

bool dedsResourceLoaderTask::Matches( const char *filename,
deResourceLoader::eResourceType resourceType ) const{
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	return strcmp( filename, pFilename ) == 0 && resourceType == pResourceType;
}

void dedsResourceLoaderTask::AddListener( dsRealObject *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	
	if( pListenerCount == pListenerSize ){
		int newSize = pListenerSize * 3 / 2 + 1;
		dsValue **newArray = new dsValue*[ newSize ];
		if( pListeners ){
			memcpy( newArray, pListeners, sizeof( dsValue* ) * pListenerSize );
			delete [] pListeners;
		}
		pListeners = newArray;
		pListenerSize = newSize;
	}
	
	pListeners[ pListenerCount ] = rt->CreateValue( pDS->GetClassResourceListener() );
	rt->SetObject( pListeners[ pListenerCount ], listener );
	rt->CastValueTo( pListeners[ pListenerCount ], pListeners[ pListenerCount ], pDS->GetClassResourceListener() );
	pListenerCount++;
}

void dedsResourceLoaderTask::RemoveListener( dsRealObject *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	int i, j;
	
	for( i=0; i<pListenerCount; i++ ){
		if( listener == pListeners[ i ]->GetRealObject() ){
			rt->FreeValue( pListeners[ i ] );
			for( j=i+1; j<pListenerCount; j++ ){
				pListeners[ i - 1 ] = pListeners[ i ];
			}
			return;
		}
	}
}

void dedsResourceLoaderTask::NotifyLoadingFinished( deFileResource *resource ){
	if( ! resource ){
		DETHROW( deeInvalidParam );
	}
	
	const int funcIndex = pDS->GetClassResourceListener()->GetFuncIndexFinishedLoading();
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	int i;
	
	for( i=0; i<pListenerCount; i++ ){
		// finishedLoading( filename, resourceType, resource )
		switch( pResourceType ){
		case deResourceLoader::ertAnimation:
			pDS->GetClassAnimation()->PushAnimation( rt, ( deAnimation* )resource );
			break;
			
		case deResourceLoader::ertFont:
			pDS->GetClassFont()->PushFont( rt, ( deFont* )resource, 10 ); // problem, font has a size
			break;
			
		case deResourceLoader::ertImage:
			pDS->GetClassImage()->PushImage( rt, ( deImage* )resource );
			break;
			
		case deResourceLoader::ertModel:
			pDS->GetClassModel()->PushModel( rt, ( deModel* )resource );
			break;
			
		case deResourceLoader::ertRig:
			pDS->GetClassRig()->PushRig( rt, ( deRig* )resource );
			break;
			
		case deResourceLoader::ertSkin:
			pDS->GetClassSkin()->PushSkin( rt, ( deSkin* )resource );
			break;
			
		case deResourceLoader::ertSound:
			pDS->GetClassSound()->PushSound( rt, ( deSound* )resource );
			break;
			
		case deResourceLoader::ertOcclusionMesh:
			pDS->GetClassOcclusionMesh()->PushOcclusionMesh( rt, ( deOcclusionMesh* )resource );
			break;
			
		case deResourceLoader::ertLanguagePack:
			pDS->GetClassLanguagePack()->PushLanguagePack( rt, ( deLanguagePack* )resource );
			break;
			
		case deResourceLoader::ertVideo:
			pDS->GetClassVideo()->PushVideo( rt, ( deVideo* )resource );
			break;
			
		default:
			DETHROW( deeInvalidParam ); // TODO do something more smart here
		}
		rt->PushInt( pResourceType );
		rt->PushString( pFilename );
		rt->RunFunctionFast( pListeners[ i ], funcIndex );
	}
	
	pClearListeners();
}

void dedsResourceLoaderTask::NotifyLoadingFailed(){
	const int funcIndex = pDS->GetClassResourceListener()->GetFuncIndexFailedLoading();
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	int i;
	
	for( i=0; i<pListenerCount; i++ ){
		// failedLoading( filename, resourceType )
		rt->PushInt( pResourceType );
		rt->PushString( pFilename );
		rt->RunFunctionFast( pListeners[ i ], funcIndex );
	}
	
	pClearListeners();
}



// Private Functions
//////////////////////

void dedsResourceLoaderTask::pClearListeners(){
	if( pListeners ){
		dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
		
		while( pListenerCount > 0 ){
			pListenerCount--;
			rt->FreeValue( pListeners[ pListenerCount ] );
		}
	}
}
