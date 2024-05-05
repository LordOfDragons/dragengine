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

#include "deClassSky.h"
#include "deClassSkyController.h"
#include "deClassSkyLayer.h"
#include "deClassSkyLink.h"
#include "../graphics/deClassColor.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyLink.h>
#include <dragengine/resources/sky/deSkyLayer.h>



// Native Structure
/////////////////////

struct sSkyNatDat{
	deSky *sky;
};



// Native functions
/////////////////////


// Constructors, Destructor
/////////////////////////////

// public func new()
deClassSky::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSky,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSky::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	deSkyManager &skyMgr = *ds.GetGameEngine()->GetSkyManager();
	sSkyNatDat &nd = *( ( sSkyNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.sky = NULL;
	
	// create sky
	nd.sky = skyMgr.CreateSky();
}

// public func destructor()
deClassSky::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSky,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSky::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSkyNatDat &nd = *( ( sSkyNatDat* )p_GetNativeData( myself ) );
	if( ! nd.sky ){
		return;
	}
	
	nd.sky->FreeReference();
	nd.sky = NULL;
}



// Management
///////////////

// public func Color getBgColor()
deClassSky::nfGetBgColor::nfGetBgColor( const sInitData &init ) :
dsFunction( init.clsSky, "getBgColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor ){
}
void deClassSky::nfGetBgColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	
	ds.GetClassColor()->PushColor( rt, sky.GetBgColor() );
}

// public func void setBgColor( Color color )
deClassSky::nfSetBgColor::nfSetBgColor(const sInitData &init) : dsFunction( init.clsSky,
"setBgColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsColor ); // color
}
void deClassSky::nfSetBgColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	
	sky.SetBgColor( ds.GetClassColor()->GetColor( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func int getControllerCount()
deClassSky::nfGetControllerCount::nfGetControllerCount( const sInitData &init ) :
dsFunction( init.clsSky, "getControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSky::nfGetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	rt->PushInt( sky.GetControllerCount() );
}

// public func void setControllerCount( int count )
deClassSky::nfSetControllerCount::nfSetControllerCount( const sInitData &init ) :
dsFunction( init.clsSky, "setControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSky::nfSetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	sky.SetControllerCount( rt->GetValue( 0 )->GetInt() );
}

// public func SkyController getControllerAt( int index )
deClassSky::nfGetControllerAt::nfGetControllerAt( const sInitData &init ) :
dsFunction( init.clsSky, "getControllerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyCtrl ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSky::nfGetControllerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	const int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 ){
		ds.GetClassSkyController()->PushController( rt, sky, sky->GetControllerCount() + index );
		
	}else{
		ds.GetClassSkyController()->PushController( rt, sky, index );
	}
}

// public func SkyController getControllerNamed( String name )
deClassSky::nfGetControllerNamed::nfGetControllerNamed( const sInitData &init ) :
dsFunction( init.clsSky, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyCtrl ){
	p_AddParameter( init.clsStr ); // name
}
void deClassSky::nfGetControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	const deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	const int index = sky->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() );
	
	if( index == -1 ){
		rt->PushObject( NULL, ds.GetClassSkyController() );
		
	}else{
		ds.GetClassSkyController()->PushController( rt, sky, index );
	}
}

// public func int indexOfControllerNamed( String name )
deClassSky::nfIndexOfControllerNamed::nfIndexOfControllerNamed( const sInitData &init ) :
dsFunction( init.clsSky, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassSky::nfIndexOfControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	rt->PushInt( sky->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() ) );
}



// public func int getLinkCount()
deClassSky::nfGetLinkCount::nfGetLinkCount( const sInitData &init ) :
dsFunction( init.clsSky, "getLinkCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSky::nfGetLinkCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	rt->PushInt( sky.GetLinkCount() );
}

// public func void setLinkCount( int count )
deClassSky::nfSetLinkCount::nfSetLinkCount( const sInitData &init ) :
dsFunction( init.clsSky, "setLinkCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSky::nfSetLinkCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	sky.SetLinkCount( rt->GetValue( 0 )->GetInt() );
}

// public func SkyLink getLinkAt( int index )
deClassSky::nfGetLinkAt::nfGetLinkAt( const sInitData &init ) :
dsFunction( init.clsSky, "getLinkAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyLink ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSky::nfGetLinkAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	
	const int index = rt->GetValue( 0 )->GetInt();
	ds.GetClassSkyLink()->PushLink( rt, sky, index );
}



// public func int getLayerCount()
deClassSky::nfGetLayerCount::nfGetLayerCount( const sInitData &init ) :
dsFunction( init.clsSky, "getLayerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSky::nfGetLayerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	rt->PushInt( sky.GetLayerCount() );
}

// public func void setLayerCount( int count )
deClassSky::nfSetLayerCount::nfSetLayerCount( const sInitData &init ) :
dsFunction( init.clsSky, "setLayerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSky::nfSetLayerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	sky.SetLayerCount( rt->GetValue( 0 )->GetInt() );
}

// public func SkyLayer getLayerAt( int index )
deClassSky::nfGetLayerAt::nfGetLayerAt( const sInitData &init ) :
dsFunction( init.clsSky, "getLayerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyLayer ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSky::nfGetLayerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	deScriptingDragonScript &ds = ( ( deClassSky* )GetOwnerClass() )->GetDS();
	
	const int index = rt->GetValue( 0 )->GetInt();
	ds.GetClassSkyLayer()->PushLayer( rt, sky, index );
}



// public func void contentChanged()
deClassSky::nfContentChanged::nfContentChanged( const sInitData &init ) :
dsFunction( init.clsSky, "contentChanged", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSky::nfContentChanged::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky &sky = *( ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky );
	
	sky.NotifyParametersChanged();
}



// public func int hashCode()
deClassSky::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsSky, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassSky::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	
	rt->PushInt( ( int )( intptr_t )sky );
}

// public func bool equals( Object obj )
deClassSky::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsSky, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassSky::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSky * const sky = ( ( sSkyNatDat* )p_GetNativeData( myself ) )->sky;
	deClassSky * const clsSky = ( deClassSky* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSky ) ){
		rt->PushBool( false );
		
	}else{
		deSky * const other = ( ( sSkyNatDat* )p_GetNativeData( obj ) )->sky;
		rt->PushBool( sky == other );
	}
}



// Class deClassSky
/////////////////////

// Constructor, destructor
////////////////////////////

deClassSky::deClassSky( deScriptingDragonScript &ds ) :
dsClass( "Sky", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSkyNatDat ) );
}

deClassSky::~deClassSky(){
}



// Management
///////////////

void deClassSky::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsSky = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsColor = pDS.GetClassColor();
	init.clsSkyCtrl = pDS.GetClassSkyController();
	init.clsSkyLayer = pDS.GetClassSkyLayer();
	init.clsSkyLink = pDS.GetClassSkyLink();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetBgColor( init ) );
	AddFunction( new nfSetBgColor( init ) );
	
	AddFunction( new nfGetControllerCount( init ) );
	AddFunction( new nfSetControllerCount( init ) );
	AddFunction( new nfGetControllerAt( init ) );
	AddFunction( new nfGetControllerNamed( init ) );
	AddFunction( new nfIndexOfControllerNamed( init ) );
	
	AddFunction( new nfGetLinkCount( init ) );
	AddFunction( new nfSetLinkCount( init ) );
	AddFunction( new nfGetLinkAt( init ) );
	
	AddFunction( new nfGetLayerCount( init ) );
	AddFunction( new nfSetLayerCount( init ) );
	AddFunction( new nfGetLayerAt( init ) );
	
	AddFunction( new nfContentChanged( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	CalcMemberOffsets();
}

deSky *deClassSky::GetSky( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSkyNatDat* )p_GetNativeData( myself->GetBuffer() ) )->sky;
}

void deClassSky::PushSky( dsRunTime *rt, deSky *sky ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! sky ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sSkyNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->sky = sky;
	sky->AddReference();
}
