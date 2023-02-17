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

#include "deClassCurveDistanceMapping.h"
#include "deClassCurveBezier3D.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/common/curve/decCurveDistanceMapping.h>
#include <dragengine/common/curve/decCurveDistanceMapping.h>

#include <libdscript/exceptions.h>



// Native Structure
/////////////////////

struct sDistMapNatDat{
	decCurveDistanceMapping *mapping;
};



// Native functions
/////////////////////

// public func new()
deClassCurveDistanceMapping::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCDistMap,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCurveDistanceMapping::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sDistMapNatDat &nd = *( ( sDistMapNatDat* )p_GetNativeData( myself ) );
	
	nd.mapping = NULL;
	
	nd.mapping = new decCurveDistanceMapping;
}

// public func new( CurveDistanceMapping copy )
deClassCurveDistanceMapping::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsCDistMap,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCDistMap ); // copy
}
void deClassCurveDistanceMapping::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sDistMapNatDat &nd = *( ( sDistMapNatDat* )p_GetNativeData( myself ) );
	deClassCurveDistanceMapping &clsCDistMap = *( ( deClassCurveDistanceMapping* )GetOwnerClass() );
	
	nd.mapping = NULL;
	
	const decCurveDistanceMapping &copy = clsCDistMap.GetMapping( rt->GetValue( 0 )->GetRealObject() );
	
	nd.mapping = new decCurveDistanceMapping( copy );
}

// public func destructor()
deClassCurveDistanceMapping::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCDistMap,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCurveDistanceMapping::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sDistMapNatDat &nd = *( ( sDistMapNatDat* )p_GetNativeData( myself ) );
	
	if( nd.mapping ){
		delete nd.mapping;
		nd.mapping = NULL;
	}
}



// Management
///////////////

// public func float getResolution()
deClassCurveDistanceMapping::nfGetResolution::nfGetResolution( const sInitData &init ) : dsFunction( init.clsCDistMap,
"getResolution", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCurveDistanceMapping::nfGetResolution::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCurveDistanceMapping &mapping = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	
	rt->PushFloat( mapping.GetResolution() );
}

// public func float getLength()
deClassCurveDistanceMapping::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsCDistMap,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCurveDistanceMapping::nfGetLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCurveDistanceMapping &mapping = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	
	rt->PushFloat( mapping.GetLength() );
}

// public func float map( float distance )
deClassCurveDistanceMapping::nfMap::nfMap( const sInitData &init ) : dsFunction( init.clsCDistMap,
"map", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsFloat ); // distance
}
void deClassCurveDistanceMapping::nfMap::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCurveDistanceMapping &mapping = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	
	const float distance = rt->GetValue( 0 )->GetFloat();
	
	int segment;
	float blend;
	mapping.Map( distance, segment, blend );
	
	rt->PushFloat( ( float )segment + blend );
}

// public func void clear()
deClassCurveDistanceMapping::nfClear::nfClear( const sInitData &init ) : dsFunction( init.clsCDistMap,
"clear", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCurveDistanceMapping::nfClear::RunFunction( dsRunTime *rt, dsValue *myself ){
	decCurveDistanceMapping &mapping = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	
	mapping.Clear();
}

// public func void init( CurveBezier3D curve )
deClassCurveDistanceMapping::nfInit::nfInit( const sInitData &init ) : dsFunction( init.clsCDistMap,
"init", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCBezier3D ); // curve
}
void deClassCurveDistanceMapping::nfInit::RunFunction( dsRunTime *rt, dsValue *myself ){
	decCurveDistanceMapping &mapping = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	deScriptingDragonScript &ds = ( ( deClassCurveDistanceMapping* )GetOwnerClass() )->GetDS();
	
	const decCurveBezier3D &curve = ds.GetClassCurveBezier3D()->GetCurve( rt->GetValue( 0 )->GetRealObject() );
	
	mapping.Init( curve );
}

// public func void init( CurveBezier3D curve, float resolution, int segmentSamples )
deClassCurveDistanceMapping::nfInit2::nfInit2( const sInitData &init ) : dsFunction( init.clsCDistMap,
"init", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCBezier3D ); // curve
	p_AddParameter( init.clsFloat ); // resolution
	p_AddParameter( init.clsInteger ); // segmentSamples
}
void deClassCurveDistanceMapping::nfInit2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decCurveDistanceMapping &mapping = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	deScriptingDragonScript &ds = ( ( deClassCurveDistanceMapping* )GetOwnerClass() )->GetDS();
	
	const decCurveBezier3D &curve = ds.GetClassCurveBezier3D()->GetCurve( rt->GetValue( 0 )->GetRealObject() );
	const float resolution = rt->GetValue( 1 )->GetFloat();
	const int segmentSamples = rt->GetValue( 2 )->GetInt();
	
	mapping.Init( curve, resolution, segmentSamples );
}



// public func int hashCode()
deClassCurveDistanceMapping::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCDistMap, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassCurveDistanceMapping::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCurveDistanceMapping * const curve = ( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	
	rt->PushInt( ( int )( intptr_t )curve );
}

// public func bool equals( Object obj )
deClassCurveDistanceMapping::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCDistMap, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassCurveDistanceMapping::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCurveDistanceMapping &curve = *( ( sDistMapNatDat* )p_GetNativeData( myself ) )->mapping;
	deClassCurveDistanceMapping *clsCDistMap = ( deClassCurveDistanceMapping* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCDistMap ) ){
		rt->PushBool( false );
		
	}else{
		const decCurveDistanceMapping &other = *( ( sDistMapNatDat* )p_GetNativeData( obj ) )->mapping;
		rt->PushBool( curve == other );
	}
}




// Class deClassCurveDistanceMapping
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassCurveDistanceMapping::deClassCurveDistanceMapping( deScriptingDragonScript &ds ) :
dsClass( "CurveDistanceMapping", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sDistMapNatDat ) );
}

deClassCurveDistanceMapping::~deClassCurveDistanceMapping(){
}



// Management
///////////////

void deClassCurveDistanceMapping::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	memset( &init, '\0', sizeof( init ) );
	
	init.clsCDistMap = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsCBezier3D = pDS.GetClassCurveBezier3D();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetResolution( init ) );
	AddFunction( new nfGetLength( init ) );
	AddFunction( new nfMap( init ) );
	AddFunction( new nfClear( init ) );
	AddFunction( new nfInit( init ) );
	AddFunction( new nfInit2( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

decCurveDistanceMapping &deClassCurveDistanceMapping::GetMapping( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return *( ( sDistMapNatDat* )p_GetNativeData( myself->GetBuffer() ) )->mapping;
}

void deClassCurveDistanceMapping::PushMapping( dsRunTime *rt, const decCurveDistanceMapping &mapping ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sDistMapNatDat &nd = *( ( sDistMapNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.mapping = NULL;
	
	try{
		nd.mapping = new decCurveDistanceMapping( mapping );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
