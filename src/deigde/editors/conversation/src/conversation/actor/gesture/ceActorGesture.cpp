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

#include "ceActorGesture.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/loadsave/igdeLoadAnimator.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceActorGesture
/////////////////////////

// Constructor, destructor
////////////////////////////

ceActorGesture::ceActorGesture( igdeEnvironment &environment, const char *name ) :
pEnvironment( environment ),
pEngAnimator( NULL ),
pName( name ){
}

ceActorGesture::ceActorGesture( const ceActorGesture &gesture ) :
pEnvironment( gesture.pEnvironment ),
pEngAnimator( NULL ),
pName( gesture.pName ),
pPathAnimator( gesture.pPathAnimator )
{
	pEngAnimator = gesture.pEngAnimator;
	if( pEngAnimator ){
		pEngAnimator->AddReference();
	}
}

ceActorGesture::~ceActorGesture(){
	if( pEngAnimator ){
		pEngAnimator->SetRig( NULL );
		pEngAnimator->FreeReference();
	}
}



// Management
///////////////

void ceActorGesture::SetName( const char *name ){
	pName = name;
}

void ceActorGesture::SetPathAnimator( const char *path ){
	if( pPathAnimator.Equals( path ) ){
		return;
	}
	
	pPathAnimator = path;
	pLoadAnimator();
}



// Private Functions
//////////////////////

void ceActorGesture::pLoadAnimator(){
	if( pPathAnimator.IsEmpty() ){
		return;
	}
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	decBaseFileReaderReference reader;
	deAnimator *animator = NULL;
	
	try{
		reader.TakeOver( engine.GetVirtualFileSystem()->OpenFileForReading(
			decPath::CreatePathUnix( pPathAnimator ) ) );
		animator = engine.GetAnimatorManager()->CreateAnimator();
		
		igdeLoadAnimator( pEnvironment, pEnvironment.GetLogger(), LOGSOURCE ).
			Load( pPathAnimator, *animator, reader );
		
		if( pEngAnimator ){
			pEngAnimator->FreeReference();
		}
		pEngAnimator = animator;
		
	}catch( const deException &e ){
		if( animator ){
			animator->FreeReference();
		}
		pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
		
		// ignore missing or broken animators. this can easily happen during development
		if( pEngAnimator ){
			pEngAnimator->FreeReference();
		}
		pEngAnimator = nullptr;
	}
}
