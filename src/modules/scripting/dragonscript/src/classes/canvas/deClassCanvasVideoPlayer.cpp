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

#include "deClassCanvasVideoPlayer.h"
#include "deClassCanvas.h"
#include "../video/deClassVideoPlayer.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasVideoPlayer.h>
#include <dragengine/resources/canvas/deCanvasManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sCVidPNatDat{
	deCanvasVideoPlayer *canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasVideoPlayer::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCVidP,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasVideoPlayer::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasVideoPlayer* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.canvas = NULL;
	
	// super call
	deClassCanvas * const baseClass = ( deClassCanvas* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create canvas
	nd.canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasVideoPlayer();
	baseClass->AssignCanvas( myself->GetRealObject(), nd.canvas );
}

// public func destructor()
deClassCanvasVideoPlayer::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCVidP,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasVideoPlayer::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	
	if( nd.canvas ){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management all types
/////////////////////////

// public func VideoPlayer getVideoPlayer()
deClassCanvasVideoPlayer::nfGetVideoPlayer::nfGetVideoPlayer( const sInitData &init ) : dsFunction( init.clsCVidP,
"getVideoPlayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVidPlay ){
}
void deClassCanvasVideoPlayer::nfGetVideoPlayer::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasVideoPlayer* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVideoPlayer()->PushVideoPlayer( rt, nd.canvas->GetVideoPlayer() );
}

// public func void setVideoPlayer( VideoPlayer videoPlayer )
deClassCanvasVideoPlayer::nfSetVideoPlayer::nfSetVideoPlayer( const sInitData &init ) : dsFunction( init.clsCVidP,
"setVideoPlayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVidPlay ); // videoPlayer
}
void deClassCanvasVideoPlayer::nfSetVideoPlayer::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasVideoPlayer* )GetOwnerClass() )->GetDS();
	
	nd.canvas->SetVideoPlayer( ds.GetClassVideoPlayer()->GetVideoPlayer( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func int getRepeatX()
deClassCanvasVideoPlayer::nfGetRepeatX::nfGetRepeatX( const sInitData &init ) : dsFunction( init.clsCVidP,
"getRepeatX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCanvasVideoPlayer::nfGetRepeatX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.canvas->GetRepeatX() );
}

// public func void setRepeatX( int count )
deClassCanvasVideoPlayer::nfSetRepeatX::nfSetRepeatX( const sInitData &init ) : dsFunction( init.clsCVidP,
"setRepeatX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassCanvasVideoPlayer::nfSetRepeatX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	nd.canvas->SetRepeatX( rt->GetValue( 0 )->GetInt() );
}

// public func int getRepeatY()
deClassCanvasVideoPlayer::nfGetRepeatY::nfGetRepeatY( const sInitData &init ) : dsFunction( init.clsCVidP,
"getRepeatY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCanvasVideoPlayer::nfGetRepeatY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.canvas->GetRepeatY() );
}

// public func void setRepeatY( int count )
deClassCanvasVideoPlayer::nfSetRepeatY::nfSetRepeatY( const sInitData &init ) : dsFunction( init.clsCVidP,
"setRepeatY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassCanvasVideoPlayer::nfSetRepeatY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( myself ) );
	nd.canvas->SetRepeatY( rt->GetValue( 0 )->GetInt() );
}



// public func int hashCode()
deClassCanvasVideoPlayer::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCVidP, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCanvasVideoPlayer::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasVideoPlayer * const canvas = ( ( sCVidPNatDat* )p_GetNativeData( myself ) )->canvas;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )canvas );
}

// public func bool equals( Object obj )
deClassCanvasVideoPlayer::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCVidP, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassCanvasVideoPlayer::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasVideoPlayer * const canvas = ( ( sCVidPNatDat* )p_GetNativeData( myself ) )->canvas;
	deClassCanvasVideoPlayer * const clsCVidP = ( deClassCanvasVideoPlayer* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCVidP ) ){
		rt->PushBool( false );
		
	}else{
		deCanvasVideoPlayer * const otherCanvas = ( ( sCVidPNatDat* )p_GetNativeData( obj ) )->canvas;
		rt->PushBool( canvas == otherCanvas );
	}
}



// Class deClassCanvasVideoPlayer
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasVideoPlayer::deClassCanvasVideoPlayer( deScriptingDragonScript &ds ) :
dsClass( "CanvasVideoPlayer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Canvas" );
	
	p_SetNativeDataSize( sizeof( sCVidPNatDat ) );
}

deClassCanvasVideoPlayer::~deClassCanvasVideoPlayer(){
}



// Management
///////////////

void deClassCanvasVideoPlayer::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCVidP = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsVidPlay = pDS.GetClassVideoPlayer();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetVideoPlayer( init ) );
	AddFunction( new nfSetVideoPlayer( init ) );
	AddFunction( new nfGetRepeatX( init ) );
	AddFunction( new nfSetRepeatX( init ) );
	AddFunction( new nfGetRepeatY( init ) );
	AddFunction( new nfSetRepeatY( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasVideoPlayer *deClassCanvasVideoPlayer::GetCanvas( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCVidPNatDat* )p_GetNativeData( myself->GetBuffer() ) )->canvas;
}

void deClassCanvasVideoPlayer::PushCanvas( dsRunTime *rt, deCanvasVideoPlayer *canvas ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! canvas ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassCanvas * const baseClass = ( deClassCanvas* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sCVidPNatDat &nd = *( ( sCVidPNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
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
