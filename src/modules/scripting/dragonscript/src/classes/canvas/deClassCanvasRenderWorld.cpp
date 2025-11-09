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
#include <stdint.h>
#include <string.h>

#include "deClassCanvasRenderWorld.h"
#include "deClassCanvas.h"
#include "../math/deClassDVector.h"
#include "../math/deClassPoint.h"
#include "../math/deClassVector.h"
#include "../graphics/deClassCamera.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/camera/deCamera.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sCRenWNatDat{
	deCanvasRenderWorld *canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasRenderWorld::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCRenW,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasRenderWorld::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasRenderWorld* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.canvas = NULL;
	
	// super call
	deClassCanvas * const baseClass = ( deClassCanvas* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create canvas
	nd.canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasRenderWorld();
	baseClass->AssignCanvas( myself->GetRealObject(), nd.canvas );
}

// public func destructor()
deClassCanvasRenderWorld::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCRenW,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasRenderWorld::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( myself ) );
	
	if( nd.canvas ){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management all types
/////////////////////////

// public func Camera getCamera()
deClassCanvasRenderWorld::nfGetCamera::nfGetCamera( const sInitData &init ) : dsFunction( init.clsCRenW,
"getCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCamera ){
}
void deClassCanvasRenderWorld::nfGetCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasRenderWorld* )GetOwnerClass() )->GetDS();
	
	ds.GetClassCamera()->PushCamera( rt, nd.canvas->GetCamera() );
}

// public func void setCamera( Camera camera )
deClassCanvasRenderWorld::nfSetCamera::nfSetCamera( const sInitData &init ) : dsFunction( init.clsCRenW,
"setCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCamera ); // camera
}
void deClassCanvasRenderWorld::nfSetCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasRenderWorld* )GetOwnerClass() )->GetDS();
	
	nd.canvas->SetCamera( ds.GetClassCamera()->GetCamera( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func Point project( DVector position )
deClassCanvasRenderWorld::nfProject::nfProject( const sInitData &init ) : dsFunction( init.clsCRenW,
"project", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint ){
	p_AddParameter( init.clsDVector ); // position
}
void deClassCanvasRenderWorld::nfProject::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasRenderWorld* )GetOwnerClass() )->GetDS();
	const deCamera * const camera = nd.canvas->GetCamera();
	if( ! camera ){
		DSTHROW( dueInvalidParam );
	}
	
	const decPoint &viewportSize = nd.canvas->GetSize();
	if( viewportSize.x == 0 || viewportSize.y == 0 ){
		printf( "viewportSize=(%i,%i)\n", viewportSize.x, viewportSize.y );
		ds.GetClassPoint()->PushPoint( rt, decPoint() );
		return;
	}
	
	const decDVector &position = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const double halfHeight = ( double )viewportSize.y * 0.5;
	const double halfWidth = ( double )viewportSize.x * 0.5;
	const double aspectRatio = halfWidth / halfHeight;
	
	decDVector projected( camera->GetOrientation().Conjugate() * ( position - camera->GetPosition() ) );
	if( projected.z < FLOAT_SAFE_EPSILON ){
		printf( "projected.z=%g\n", projected.z );
		ds.GetClassPoint()->PushPoint( rt, decPoint() );
		return;
	}
	
	projected.x = projected.x / tan( ( double )camera->GetFov() * 0.5 ) / projected.z;
	projected.y = projected.y * aspectRatio / tan( ( double )camera->GetFov()
		* ( double )camera->GetFovRatio() * 0.5 ) / projected.z;
	
	projected.x = projected.x * halfWidth + halfWidth;
	projected.y = halfHeight - projected.y * halfHeight;
	
	const decPoint point( ( int )projected.x, ( int )projected.y );
	
	ds.GetClassPoint()->PushPoint( rt, point );
}

// public func Vector backProject( Point position )
deClassCanvasRenderWorld::nfBackProject::nfBackProject( const sInitData &init ) : dsFunction( init.clsCRenW,
"backProject", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
	p_AddParameter( init.clsPoint ); // position
}
void deClassCanvasRenderWorld::nfBackProject::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasRenderWorld* )GetOwnerClass() )->GetDS();
	const deCamera * const camera = nd.canvas->GetCamera();
	if( ! camera ){
		DSTHROW( dueInvalidParam );
	}
	
	const decPoint &viewportSize = nd.canvas->GetSize();
	if( viewportSize.x == 0 || viewportSize.y == 0 ){
		ds.GetClassVector()->PushVector( rt, camera->GetOrientation() * decVector( 0.0f, 0.0f, 1.0f ) );
		return;
	}
	
	const decPoint &position = ds.GetClassPoint()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const double halfHeight = ( double )viewportSize.y * 0.5;
	const double halfWidth = ( double )viewportSize.x * 0.5;
	const double aspectRatio = halfWidth / halfHeight;
	decDVector direction;
	
	direction.x = tan( ( double )camera->GetFov() * 0.5 )
		* ( ( position.x - ( double )halfWidth ) / halfWidth );
	direction.y = tan( ( double )camera->GetFov() * ( double )camera->GetFovRatio() * 0.5 )
		* ( ( halfHeight - ( double )position.y ) / halfHeight ) / aspectRatio;
	direction.z = 1.0;
	
	ds.GetClassVector()->PushVector( rt, ( camera->GetOrientation() * direction.Normalized() ).ToVector() );
}



// public func int hashCode()
deClassCanvasRenderWorld::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCRenW, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCanvasRenderWorld::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasRenderWorld * const canvas = ( ( sCRenWNatDat* )p_GetNativeData( myself ) )->canvas;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )canvas );
}

// public func bool equals( Object obj )
deClassCanvasRenderWorld::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCRenW, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassCanvasRenderWorld::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasRenderWorld * const canvas = ( ( sCRenWNatDat* )p_GetNativeData( myself ) )->canvas;
	deClassCanvasRenderWorld * const clsCRenW = ( deClassCanvasRenderWorld* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCRenW ) ){
		rt->PushBool( false );
		
	}else{
		deCanvasRenderWorld * const otherCanvas = ( ( sCRenWNatDat* )p_GetNativeData( obj ) )->canvas;
		rt->PushBool( canvas == otherCanvas );
	}
}



// Class deClassCanvasRenderWorld
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasRenderWorld::deClassCanvasRenderWorld( deScriptingDragonScript &ds ) :
dsClass( "CanvasRenderWorld", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Canvas" );
	
	p_SetNativeDataSize( sizeof( sCRenWNatDat ) );
}

deClassCanvasRenderWorld::~deClassCanvasRenderWorld(){
}



// Management
///////////////

void deClassCanvasRenderWorld::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCRenW = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsCamera = pDS.GetClassCamera();
	init.clsVector = pDS.GetClassVector();
	init.clsDVector = pDS.GetClassDVector();
	init.clsPoint = pDS.GetClassPoint();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCamera( init ) );
	AddFunction( new nfSetCamera( init ) );
	
	AddFunction( new nfProject( init ) );
	AddFunction( new nfBackProject( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasRenderWorld *deClassCanvasRenderWorld::GetCanvas( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCRenWNatDat* )p_GetNativeData( myself->GetBuffer() ) )->canvas;
}

void deClassCanvasRenderWorld::PushCanvas( dsRunTime *rt, deCanvasRenderWorld *canvas ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! canvas ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassCanvas * const baseClass = ( deClassCanvas* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sCRenWNatDat &nd = *( ( sCRenWNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.canvas = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.canvas = canvas;
		canvas->AddReference();
		
		baseClass->AssignCanvas( rt->GetValue( 0 )->GetRealObject(), canvas );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
