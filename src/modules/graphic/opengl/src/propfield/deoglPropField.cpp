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

#include "deoglPropField.h"
#include "deoglPropFieldType.h"
#include "deoglPropFieldCluster.h"
#include "deoglRPropField.h"

#include "../deoglBasics.h"
#include "../deGraphicOpenGl.h"
#include "../texture/deoglTextureStageManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>



// Class deoglPropField
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglPropField::deoglPropField( deGraphicOpenGl &ogl, dePropField &propField ) :
pOgl( ogl ),
pPropField( propField ),

pRPropField( NULL ),

pLODLevel( 0 ),
pBestLOD( 0 ),
pCheckLOD( false ),

pDirtyPosition( true ),
pDirtyExtends( true ),
pDirtyTypes( true ),
pDirtyInstances( true )
{
	const int typeCount = propField.GetTypeCount();
	int t;
	
	try{
		pRPropField = new deoglRPropField( ogl.GetRenderThread() );
		
		for( t=0; t<typeCount; t++ ){
			TypeAdded( t, propField.GetTypeAt( t ) );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglPropField::~deoglPropField(){
	pCleanUp();
}



// Management
///////////////

void deoglPropField::SyncToRender(){
	const int count = pTypes.GetCount();
	int i;
	
// 	if( pCheckLOD ){
// 		SetLODLevel( pBestLOD ); // sets pCheckLOD to false
// 	}
	SetLODLevel( 1 ); // HACK use static lod level
	
	if( pDirtyPosition ){
		pRPropField->SetPosition( pPropField.GetPosition() );
		pDirtyPosition = false;
	}
	
	if( pDirtyTypes ){
		pRPropField->RemoveAllTypes();
		
		for( i=0; i<count; i++ ){
			deoglPropFieldType &type = *( ( deoglPropFieldType* )pTypes.GetAt( i ) );
			type.SyncToRender();
			pRPropField->AddType( type.GetRType() );
		}
		
		pDirtyTypes = false;
		pDirtyInstances = true;
		
	}else{
		for( i=0; i<count; i++ ){
			( ( deoglPropFieldType* )pTypes.GetAt( i ) )->SyncToRender();
		}
	}
	
	if( pDirtyInstances ){
		UpdateInstanceCounts();
		pDirtyInstances = false;
	}
	
	if( pDirtyExtends ){
		pRPropField->UpdateExtends( pPropField );
		pDirtyExtends = false;
	}
}



void deoglPropField::SetLODLevel( int level ){
	if( level != pLODLevel ){
		pLODLevel = level;
		pDirtyInstances = true;
	}
	
	pCheckLOD = false;
}

void deoglPropField::TestLODLevel( const decDVector &camera ){
	// to get things started we use a simple distance formula without much
	// brain. anything closer than 100m is set to lod level 1 and anything
	// beyond is set to lod level 0. better solution would be to calculate
	// the bounding sphere and use the distance minus the sphere radius as
	// the distance to test with
	double distance = ( camera - pPropField.GetPosition() ).Length();
	int lodLevel = 0;
	
	if( distance < 100.0 ){
		lodLevel = 1;
		
	}else{
		lodLevel = 0;
	}
	
//	if( pPropField->GetPosition().x > 0.0 && pPropField->GetPosition().x < 50.0 && pPropField->GetPosition().z > 0.0 && pPropField->GetPosition().z < 50.0 )
//	pOgl->LogInfoFormat( "PropField.TestLODLevel: pf=(%g,%g,%g) cp=(%g,%g,%g) ll=%i bll=%i cl=%i",
//		pPropField->GetPosition().x, pPropField->GetPosition().y, pPropField->GetPosition().z,
//		camera.x, camera.y, camera.z, lodLevel, pBestLOD, pCheckLOD?1:0 );
	// this call can be done from various cameras but we can have only one
	// value in the end. we keep the smallest lod level. to figure out when
	// to start looking for this winner lod value we use the best lod
	// variable. if this is -1 then this is the first candidate for the best
	// lod level and is stored. otherwise the new lod level is compared
	// against the best one. the real change is carried out during the update
	// call later on
	if( ! pCheckLOD || lodLevel < pBestLOD ){
		pBestLOD = lodLevel;
		pCheckLOD = true;
	}
}

void deoglPropField::UpdateInstanceCounts(){
	const int count = pTypes.GetCount();
	int i;
	
	// if the lod level is 0 clear the prop field of all instances but keep the
	// types intact
	if( pLODLevel == 0 ){
		for( i=0; i<count; i++ ){
			pPropField.GetTypeAt( i )->SetInstanceCount( 0 );
		}
		
	// otherwise ask the game scripts to update the instances. we do not touch
	// types nor instances. it is up to the game scripts to set the instance
	// count. we do not set it to 0 first as this would cause notifications
	// which have no effect at all except potentially costing time
	}else{
		float density;
		
		// the density is based on the lod level. currently there is only one
		// lod level hence we use 1 all the time
		density = 1.0f;
		
		// tell the game scripts to update the instances
		pPropField.NotifyCreateInstances( density );
	}
	
	// instance counts most probably changed. force an update
	for( i=0; i<count; i++ ){
		( ( deoglPropFieldType* )pTypes.GetAt( i ) )->InstanceCountChanged();
	}
	
	// extends need to be updated now
	pDirtyExtends = true;
}




void deoglPropField::Update(){
	// next SyncToRender resets lod level. all other SyncToRender only update it
	// keeping the smallest. required since multiple cameras can call SyncToRender
	// but only only lod value can be stored. we keep the smallest one
	
	// TODO lod level is a problem. the calculation has to be done during main
	//      thread since type instance counts can  be adjusted. for synching
	//      though we use one lod level. this has to be the smallest lod level
	//      required by any camera seeing this prop field and being included
	//      for rendering. this is currently not working so we use the lowest
	//      lod level always. to fix this problem a master render plan is
	//      required which is not yet existing.
// 	pCheckLOD = false;
}



int deoglPropField::GetTypeCount() const{
	return pTypes.GetCount();
}

deoglPropFieldType &deoglPropField::GetTypeAt( int index ) const{
	return *( ( deoglPropFieldType* )pTypes.GetAt( index ) );
}



// Notifications
//////////////////

void deoglPropField::GroundChanged(){
	pDirtyInstances = true;
}

void deoglPropField::PositionChanged(){
	pDirtyPosition = true;
	pDirtyInstances = true; // required?
	pDirtyExtends = true;
}

void deoglPropField::TypeAdded( int index, dePropFieldType *type ){
	pTypes.Add( new deoglPropFieldType( *this, *type ) );
	
	pDirtyTypes = true;
	pDirtyExtends = true;
}

void deoglPropField::TypeRemoved( int index, dePropFieldType *type ){
	delete ( deoglPropFieldType* )pTypes.GetAt( index );
	pTypes.RemoveFrom( index );
	
	pDirtyTypes = true;
	pDirtyExtends = true;
}

void deoglPropField::AllTypesRemoved(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglPropFieldType* )pTypes.GetAt( i );
	}
	pTypes.RemoveAll();
	
	pDirtyTypes = true;
	pDirtyExtends = true;
}

void deoglPropField::TypeChanged( int index, dePropFieldType *type ){
	( ( deoglPropFieldType* )pTypes.GetAt( index ) )->TypeChanged();
	
	pDirtyExtends = true;
}

void deoglPropField::InstancesChanged( int index, dePropFieldType *type ){
	( ( deoglPropFieldType* )pTypes.GetAt( index ) )->InstancesChanged();
	
	pDirtyExtends = true;
}

void deoglPropField::AssignmentsChanged( int index, dePropFieldType *type ){
	( ( deoglPropFieldType* )pTypes.GetAt( index ) )->AssignmentsChanged();
}

void deoglPropField::BendStatesChanged( int index, dePropFieldType *type ){
	( ( deoglPropFieldType* )pTypes.GetAt( index ) )->BendStatesChanged();
}



// Private functions
//////////////////////

void deoglPropField::pCleanUp(){
	if( pRPropField ){
		pRPropField->FreeReference();
		pRPropField = NULL;
	}
	
	AllTypesRemoved();
}
