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
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassCanvasCanvasView.h"
#include "deClassCanvasView.h"
#include "deClassCanvas.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasCanvasView.h>
#include <dragengine/resources/canvas/deCanvasView.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sCCViewNatDat{
	deCanvasCanvasView *canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasCanvasView::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCCView,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasCanvasView::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasCanvasView* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.canvas = NULL;
	
	// super call
	deClassCanvas * const baseClass = ( deClassCanvas* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create canvas
	nd.canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasCanvasView();
	baseClass->AssignCanvas( myself->GetRealObject(), nd.canvas );
}

// public func destructor()
deClassCanvasCanvasView::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCCView,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasCanvasView::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	
	if( nd.canvas ){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management all types
/////////////////////////

// public func Canvas getCanvasView()
deClassCanvasCanvasView::nfGetCanvasView::nfGetCanvasView( const sInitData &init ) : dsFunction( init.clsCCView,
"getCanvasView", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCView ){
}
void deClassCanvasCanvasView::nfGetCanvasView::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasCanvasView* )GetOwnerClass() )->GetDS();
	
	ds.GetClassCanvasView()->PushCanvas( rt, nd.canvas->GetCanvasView() );
}

// public func void setCanvasView( Canvas canvasView )
deClassCanvasCanvasView::nfSetCanvasView::nfSetCanvasView( const sInitData &init ) : dsFunction( init.clsCCView,
"setCanvasView", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCView ); // canvasView
}
void deClassCanvasCanvasView::nfSetCanvasView::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasCanvasView* )GetOwnerClass() )->GetDS();
	
	deCanvasView * const canvasView = ds.GetClassCanvasView()->GetCanvas( rt->GetValue( 0 )->GetRealObject() );
	nd.canvas->SetCanvasView( canvasView );
}

// public func int getRepeatX()
deClassCanvasCanvasView::nfGetRepeatX::nfGetRepeatX( const sInitData &init ) : dsFunction( init.clsCCView,
"getRepeatX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCanvasCanvasView::nfGetRepeatX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.canvas->GetRepeatX() );
}

// public func void setRepeatX( int count )
deClassCanvasCanvasView::nfSetRepeatX::nfSetRepeatX( const sInitData &init ) : dsFunction( init.clsCCView,
"setRepeatX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassCanvasCanvasView::nfSetRepeatX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	nd.canvas->SetRepeatX( rt->GetValue( 0 )->GetInt() );
}

// public func int getRepeatY()
deClassCanvasCanvasView::nfGetRepeatY::nfGetRepeatY( const sInitData &init ) : dsFunction( init.clsCCView,
"getRepeatY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCanvasCanvasView::nfGetRepeatY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.canvas->GetRepeatY() );
}

// public func void setRepeatY( int count )
deClassCanvasCanvasView::nfSetRepeatY::nfSetRepeatY( const sInitData &init ) : dsFunction( init.clsCCView,
"setRepeatY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassCanvasCanvasView::nfSetRepeatY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( myself ) );
	nd.canvas->SetRepeatY( rt->GetValue( 0 )->GetInt() );
}



// public func int hashCode()
deClassCanvasCanvasView::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCCView, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCanvasCanvasView::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasCanvasView * const canvas = ( ( sCCViewNatDat* )p_GetNativeData( myself ) )->canvas;
	// hash code = memory location
	rt->PushInt( ( intptr_t )canvas );
}

// public func bool equals( Object obj )
deClassCanvasCanvasView::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCCView, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassCanvasCanvasView::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasCanvasView * const canvas = ( ( sCCViewNatDat* )p_GetNativeData( myself ) )->canvas;
	deClassCanvasCanvasView * const clsCCView = ( deClassCanvasCanvasView* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCCView ) ){
		rt->PushBool( false );
		
	}else{
		deCanvasCanvasView * const otherCanvas = ( ( sCCViewNatDat* )p_GetNativeData( obj ) )->canvas;
		rt->PushBool( canvas == otherCanvas );
	}
}



// Class deClassCanvasCanvasView
//////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasCanvasView::deClassCanvasCanvasView( deScriptingDragonScript &ds ) :
dsClass( "CanvasCanvasView", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Canvas" );
	
	p_SetNativeDataSize( sizeof( sCCViewNatDat ) );
}

deClassCanvasCanvasView::~deClassCanvasCanvasView(){
}



// Management
///////////////

void deClassCanvasCanvasView::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCCView = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsCView = pDS.GetClassCanvasView();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCanvasView( init ) );
	AddFunction( new nfSetCanvasView( init ) );
	AddFunction( new nfGetRepeatX( init ) );
	AddFunction( new nfSetRepeatX( init ) );
	AddFunction( new nfGetRepeatY( init ) );
	AddFunction( new nfSetRepeatY( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasCanvasView *deClassCanvasCanvasView::GetCanvas( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCCViewNatDat* )p_GetNativeData( myself->GetBuffer() ) )->canvas;
}

void deClassCanvasCanvasView::PushCanvas( dsRunTime *rt, deCanvasCanvasView *canvas ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! canvas ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassCanvas * const baseClass = ( deClassCanvas* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sCCViewNatDat &nd = *( ( sCCViewNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.canvas = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		canvas->AddReference();
		nd.canvas = canvas;
		
		baseClass->AssignCanvas( rt->GetValue( 0 )->GetRealObject(), canvas );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
