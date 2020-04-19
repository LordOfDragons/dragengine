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

#include "deClassSky.h"
#include "deClassSkyLink.h"
#include "../curve/deClassCurveBezier.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLink.h>



// Native Structure
/////////////////////

struct sSkyLinkNatDat{
	deSky *sky;
	int index;
};



// Native functions
/////////////////////

// private func new()
deClassSkyLink::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkyLink::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	DSTHROW( dueInvalidParam );
}

// public func destructor()
deClassSkyLink::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkyLink::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	if( nd.sky ){
		nd.sky->FreeReference();
		nd.sky = NULL;
	}
	nd.index = -1;
}



// Management
///////////////

// public func Sky getSky()
deClassSkyLink::nfGetSky::nfGetSky( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky ){
}
void deClassSkyLink::nfGetSky::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassSkyLink* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSky()->PushSky( rt, nd.sky );
}

// public func int getLinkIndex()
deClassSkyLink::nfGetLinkIndex::nfGetLinkIndex( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "getLinkIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSkyLink::nfGetLinkIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.index );
}



// public func int getController()
deClassSkyLink::nfGetController::nfGetController( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "getController", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSkyLink::nfGetController::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	const deSkyLink &link = nd.sky->GetLinkAt( nd.index );
	
	rt->PushInt( link.GetController() );
}

// public func void setController( int controller )
deClassSkyLink::nfSetController::nfSetController( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "setController", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // controller
}
void deClassSkyLink::nfSetController::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	deSkyLink &link = nd.sky->GetLinkAt( nd.index );
	
	link.SetController( rt->GetValue( 0 )->GetInt() );
}

// public func CurveBezier getCurve()
deClassSkyLink::nfGetCurve::nfGetCurve( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "getCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassSkyLink::nfGetCurve::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	const deSkyLink &link = nd.sky->GetLinkAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyLink* )GetOwnerClass() )->GetDS();
	
	ds.GetClassCurveBezier()->PushCurve( rt, link.GetCurve() );
}

// public func void setCurve( CurveBezier curve )
deClassSkyLink::nfSetCurve::nfSetCurve( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "setCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassSkyLink::nfSetCurve::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	deSkyLink &link = nd.sky->GetLinkAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyLink* )GetOwnerClass() )->GetDS();
	
	dsRealObject * const objCurve = rt->GetValue( 0 )->GetRealObject();
	
	if( objCurve ){
		link.GetCurve() = ds.GetClassCurveBezier()->GetCurve( objCurve );
		
	}else{
		link.GetCurve().RemoveAllPoints();
	}
}

// public func int getRepeat()
deClassSkyLink::nfGetRepeat::nfGetRepeat( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "getRepeat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSkyLink::nfGetRepeat::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	const deSkyLink &link = nd.sky->GetLinkAt( nd.index );
	
	rt->PushInt( link.GetRepeat() );
}

// public func void setRepeat( int repeat )
deClassSkyLink::nfSetRepeat::nfSetRepeat( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "setRepeat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // repeat
}
void deClassSkyLink::nfSetRepeat::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	deSkyLink &link = nd.sky->GetLinkAt( nd.index );
	
	link.SetRepeat( rt->GetValue( 0 )->GetInt() );
}



// public func bool equals( Object obj )
deClassSkyLink::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsSkyCtrl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassSkyLink::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkyLink * const clsSkyCtrl = ( deClassSkyLink* )GetOwnerClass();
	const sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( myself ) );
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSkyCtrl ) ){
		rt->PushBool( false );
		
	}else{
		const sSkyLinkNatDat &other = *( ( sSkyLinkNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.sky == other.sky && nd.index == other.index );
	}
}



// Class deClassSkyLink
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyLink::deClassSkyLink( deScriptingDragonScript &ds ) :
dsClass( "SkyLink", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSkyLinkNatDat ) );
}

deClassSkyLink::~deClassSkyLink(){
}



// Management
///////////////

void deClassSkyLink::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsSkyCtrl = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	init.clsCurveBezier = pDS.GetClassCurveBezier();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetSky( init ) );
	AddFunction( new nfGetLinkIndex( init ) );
	
	AddFunction( new nfGetController( init ) );
	AddFunction( new nfSetController( init ) );
	AddFunction( new nfGetCurve( init ) );
	AddFunction( new nfSetCurve( init ) );
	AddFunction( new nfGetRepeat( init ) );
	AddFunction( new nfSetRepeat( init ) );
	
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

void deClassSkyLink::PushLink( dsRunTime *rt, deSky *sky, int index ){
	if( ! rt || ! sky || index < 0 || index >= sky->GetLinkCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sSkyLinkNatDat &nd = *( ( sSkyLinkNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.sky = sky;
	sky->AddReference();
	nd.index = index;
}
