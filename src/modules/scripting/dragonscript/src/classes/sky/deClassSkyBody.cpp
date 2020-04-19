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
#include "deClassSkyBody.h"
#include "../graphics/deClassColor.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector2.h"
#include "../world/deClassSkin.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyLayerBody.h>



// Native Structure
/////////////////////

struct sSkyBodyNatDat{
	deSky *sky;
	int layer;
	int index;
};



// Native functions
/////////////////////

// private func new()
deClassSkyBody::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsSkyBody, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkyBody::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	DSTHROW( dueInvalidParam );
}

// public func destructor()
deClassSkyBody::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSkyBody, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkyBody::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSkyBodyNatDat &nd = *( ( sSkyBodyNatDat* )p_GetNativeData( myself ) );
	if( nd.sky ){
		nd.sky->FreeReference();
		nd.sky = NULL;
	}
	nd.layer = -1;
	nd.index = -1;
}



// Management
///////////////

// public func Sky getSky()
deClassSkyBody::nfGetSky::nfGetSky( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky ){
}
void deClassSkyBody::nfGetSky::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSky()->PushSky( rt, nd.sky );
}

// public func int getLayerIndex()
deClassSkyBody::nfGetLayerIndex::nfGetLayerIndex( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getLayerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSkyBody::nfGetLayerIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.layer );
}

// public func int getBodyIndex()
deClassSkyBody::nfGetBodyIndex::nfGetBodyIndex( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getBodyIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSkyBody::nfGetBodyIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.index );
}



// public func Quaternion getOrientation()
deClassSkyBody::nfGetOrientation::nfGetOrientation( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion ){
}
void deClassSkyBody::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	const deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, body.GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassSkyBody::nfSetOrientation::nfSetOrientation( const sInitData &init ) :
dsFunction( init.clsSkyBody, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuaternion ); // orientation
}
void deClassSkyBody::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	body.SetOrientation( ds.GetClassQuaternion()->GetQuaternion(
		rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Vector2 getSize()
deClassSkyBody::nfGetSize::nfGetSize( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector2 ){
}
void deClassSkyBody::nfGetSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	const deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector2()->PushVector2( rt, body.GetSize() );
}

// public func void setSize( Vector2 size )
deClassSkyBody::nfSetSize::nfSetSize( const sInitData &init ) :
dsFunction( init.clsSkyBody, "setSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVector2 ); // size
}
void deClassSkyBody::nfSetSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	body.SetSize( ds.GetClassVector2()->GetVector2( rt->GetValue( 0 )->GetRealObject() ) * DEG2RAD );
}

// public func Color getColor()
deClassSkyBody::nfGetColor::nfGetColor( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor ){
	
}
void deClassSkyBody::nfGetColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	const deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	ds.GetClassColor()->PushColor( rt, body.GetColor() );
}

// public func void setColor( Color color )
deClassSkyBody::nfSetColor::nfSetColor( const sInitData &init ) :
dsFunction( init.clsSkyBody, "setColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsColor ); // color
}
void deClassSkyBody::nfSetColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	body.SetColor( ds.GetClassColor()->GetColor( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Skin getSkin()
deClassSkyBody::nfGetSkin::nfGetSkin( const sInitData &init ) :
dsFunction( init.clsSkyBody, "getSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
}
void deClassSkyBody::nfGetSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	const deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSkin()->PushSkin( rt, body.GetSkin() );
}

// public func void setSkin( Skin skin )
deClassSkyBody::nfSetSkin::nfSetSkin( const sInitData &init ) :
dsFunction( init.clsSkyBody, "setSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSkin ); // skin
}
void deClassSkyBody::nfSetSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	deSkyLayerBody &body = nd.sky->GetLayerAt( nd.layer ).GetBodyAt( nd.index );
	deScriptingDragonScript &ds = ( ( deClassSkyBody* )GetOwnerClass() )->GetDS();
	
	body.SetSkin( ds.GetClassSkin()->GetSkin( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func bool equals( Object obj )
deClassSkyBody::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsSkyBody, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassSkyBody::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkyBody * const clsSkyBody = ( deClassSkyBody* )GetOwnerClass();
	const sSkyBodyNatDat &nd = *( ( const sSkyBodyNatDat* )p_GetNativeData( myself ) );
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSkyBody ) ){
		rt->PushBool( false );
		
	}else{
		const sSkyBodyNatDat &other = *( ( const sSkyBodyNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.sky == other.sky && nd.layer == other.layer && nd.index == other.index );
	}
}



// Class deClassSkyBody
/////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyBody::deClassSkyBody( deScriptingDragonScript &ds ) :
dsClass( "SkyBody", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSkyBodyNatDat ) );
}

deClassSkyBody::~deClassSkyBody(){
}



// Management
///////////////

void deClassSkyBody::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsSkyBody = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsVector2 = pDS.GetClassVector2();
	init.clsColor = pDS.GetClassColor();
	init.clsSkin = pDS.GetClassSkin();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetSky( init ) );
	AddFunction( new nfGetLayerIndex( init ) );
	AddFunction( new nfGetBodyIndex( init ) );
	
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetSize( init ) );
	AddFunction( new nfSetSize( init ) );
	AddFunction( new nfGetColor( init ) );
	AddFunction( new nfSetColor( init ) );
	AddFunction( new nfGetSkin( init ) );
	AddFunction( new nfSetSkin( init ) );
	
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

void deClassSkyBody::PushBody( dsRunTime *rt, deSky *sky, int layer, int index ){
	if( ! rt || ! sky ){
		DSTHROW( dueInvalidParam );
	}
	
	const deSkyLayer &skyLayer = sky->GetLayerAt( layer );
	if( index < 0 || index > skyLayer.GetBodyCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sSkyBodyNatDat &nd = *( ( sSkyBodyNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.sky = sky;
	sky->AddReference();
	nd.layer = layer;
	nd.index = index;
}
