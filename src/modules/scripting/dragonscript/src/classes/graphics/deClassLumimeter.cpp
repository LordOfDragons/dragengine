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

// includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassLumimeter.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../graphics/deClassColor.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/resources/sensor/deLumimeter.h"
#include "dragengine/resources/sensor/deLumimeterManager.h"
#include "dragengine/resources/skin/deSkin.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// native structure
struct sLumimeterNatDat{
	deLumimeter *lumimeter;
};



// Constructors, destructor
/////////////////////////////

// public func new()
deClassLumimeter::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsLM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLumimeter::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sLumimeterNatDat *nd = ( sLumimeterNatDat* )p_GetNativeData( myself );
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	deLumimeterManager *lumimeterMgr = clsLumimeter->GetGameEngine()->GetLumimeterManager();
	
	// clear ( important )
	nd->lumimeter = NULL;
	
	// create lumimeter
	nd->lumimeter = lumimeterMgr->CreateLumimeter();
	if( ! nd->lumimeter ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassLumimeter::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsLM,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLumimeter::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sLumimeterNatDat *nd = ( sLumimeterNatDat* )p_GetNativeData( myself );
	
	if( nd->lumimeter ){
		nd->lumimeter->FreeReference();
		nd->lumimeter = NULL;
	}
}



// Management
///////////////

// public func DVector getPosition()
deClassLumimeter::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsLM,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassLumimeter::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	
	clsLumimeter->GetClassDVector()->PushDVector( rt, lumimeter->GetPosition() );
}

// public func void setPosition( DVector position )
deClassLumimeter::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsLM,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassLumimeter::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	lumimeter->SetPosition( clsLumimeter->GetClassDVector()->GetDVector( obj ) );
}

// public func Vector getDirection()
deClassLumimeter::nfGetDirection::nfGetDirection( const sInitData &init ) : dsFunction( init.clsLM,
"getDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassLumimeter::nfGetDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	
	clsLumimeter->GetClassVector()->PushVector( rt, lumimeter->GetDirection() );
}

// public func void setDirection( Vector direction )
deClassLumimeter::nfSetDirection::nfSetDirection( const sInitData &init ) : dsFunction( init.clsLM,
"setDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // direction
}
void deClassLumimeter::nfSetDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	lumimeter->SetDirection( clsLumimeter->GetClassVector()->GetVector( obj ) );
}

// public func float getConeInnerAngle()
deClassLumimeter::nfGetConeInnerAngle::nfGetConeInnerAngle( const sInitData &init ) : dsFunction( init.clsLM,
"getConeInnerAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassLumimeter::nfGetConeInnerAngle::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	rt->PushFloat( lumimeter->GetConeInnerAngle() / DEG2RAD );
}

// public func void setConeInnerAngle( float angle )
deClassLumimeter::nfSetConeInnerAngle::nfSetConeInnerAngle( const sInitData &init ) : dsFunction( init.clsLM,
"setConeInnerAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // angle
}
void deClassLumimeter::nfSetConeInnerAngle::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	lumimeter->SetConeInnerAngle( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
}

// public func float getConeOuterAngle()
deClassLumimeter::nfGetConeOuterAngle::nfGetConeOuterAngle( const sInitData &init ) : dsFunction( init.clsLM,
"getConeOuterAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassLumimeter::nfGetConeOuterAngle::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	rt->PushFloat( lumimeter->GetConeOuterAngle() / DEG2RAD );
}

// public func void setConeOuterAngle( float angle )
deClassLumimeter::nfSetConeOuterAngle::nfSetConeOuterAngle( const sInitData &init ) : dsFunction( init.clsLM,
"setConeOuterAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // angle
}
void deClassLumimeter::nfSetConeOuterAngle::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	lumimeter->SetConeOuterAngle( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
}

// public func float getConeExponent()
deClassLumimeter::nfGetConeExponent::nfGetConeExponent( const sInitData &init ) : dsFunction( init.clsLM,
"getConeExponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassLumimeter::nfGetConeExponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	rt->PushFloat( lumimeter->GetConeExponent() );
}

// public func void setConeExponent( float exponent )
deClassLumimeter::nfSetConeExponent::nfSetConeExponent( const sInitData &init ) : dsFunction( init.clsLM,
"setConeExponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // exponent
}
void deClassLumimeter::nfSetConeExponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	lumimeter->SetConeExponent( rt->GetValue( 0 )->GetFloat() );
}



// Measuring
//////////////

// public func float measureLuminance()
deClassLumimeter::nfMeasureLuminance::nfMeasureLuminance( const sInitData &init ) : dsFunction( init.clsLM,
"measureLuminance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassLumimeter::nfMeasureLuminance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	rt->PushFloat( lumimeter->MeasureLuminance() );
}

// public func Color measureColor()
deClassLumimeter::nfMeasureColor::nfMeasureColor( const sInitData &init ) : dsFunction( init.clsLM,
"measureColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
}
void deClassLumimeter::nfMeasureColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	
	clsLumimeter->GetClassColor()->PushColor( rt, lumimeter->MeasureColor() );
}



// Misc
/////////

// public func int hashCode()
deClassLumimeter::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsLM, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassLumimeter::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	
	rt->PushInt( ( int )( intptr_t )lumimeter );
}

// public func bool equals( Object obj )
deClassLumimeter::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsLM, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassLumimeter::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deLumimeter *lumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( myself ) )->lumimeter;
	deClassLumimeter *clsLumimeter = ( deClassLumimeter* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsLumimeter ) ){
		rt->PushBool( false );
		
	}else{
		deLumimeter *otherLumimeter = ( ( sLumimeterNatDat* )p_GetNativeData( obj ) )->lumimeter;
		rt->PushBool( lumimeter == otherLumimeter );
	}
}



// Class deClassLumimeter
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassLumimeter::deClassLumimeter( deEngine *gameEngine, deScriptingDragonScript *scrMgr ) :
dsClass( "Lumimeter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sLumimeterNatDat ) );
}

deClassLumimeter::~deClassLumimeter(){
}



// Management
///////////////

void deClassLumimeter::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	pClsVec = pScrMgr->GetClassVector();
	pClsDVec = pScrMgr->GetClassDVector();
	pClsClr = pScrMgr->GetClassColor();
	
	init.clsLM = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pClsVec;
	init.clsDVec = pClsDVec;
	init.clsClr = pClsClr;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetDirection( init ) );
	AddFunction( new nfSetDirection( init ) );
	AddFunction( new nfGetConeInnerAngle( init ) );
	AddFunction( new nfSetConeInnerAngle( init ) );
	AddFunction( new nfGetConeOuterAngle( init ) );
	AddFunction( new nfSetConeOuterAngle( init ) );
	AddFunction( new nfGetConeExponent( init ) );
	AddFunction( new nfSetConeExponent( init ) );
	
	AddFunction( new nfMeasureLuminance( init ) );
	AddFunction( new nfMeasureColor( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deLumimeter *deClassLumimeter::GetLumimeter( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sLumimeterNatDat* )p_GetNativeData( myself->GetBuffer() ) )->lumimeter;
}

void deClassLumimeter::PushLumimeter( dsRunTime *rt, deLumimeter *lumimeter ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! lumimeter ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sLumimeterNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->lumimeter = lumimeter;
	lumimeter->AddReference();
}
