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

#include "deClassEnvMapProbe.h"
#include "../resources/deClassResourceListener.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../utils/deClassShapeList.h"
#include "../physics/deClassLayerMask.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/probe/deEnvMapProbeManager.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/deEngine.h>



// Native Structure
/////////////////////

struct sEmpNatDat{
	deEnvMapProbe *envMapProbe;
};



// Native Functions
/////////////////////

// public func new()
deClassEnvMapProbe::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsEmp,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEnvMapProbe::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sEmpNatDat &nd = *( ( sEmpNatDat* )p_GetNativeData( myself ) );
	const deClassEnvMapProbe &clsEmp = *( ( deClassEnvMapProbe* )GetOwnerClass() );
	deEnvMapProbeManager &empmgr = *clsEmp.GetDS()->GetGameEngine()->GetEnvMapProbeManager();
	
	nd.envMapProbe = NULL;
	nd.envMapProbe = empmgr.CreateEnvMapProbe();
}

// public func destructor()
deClassEnvMapProbe::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsEmp,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEnvMapProbe::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sEmpNatDat &nd = *( ( sEmpNatDat* )p_GetNativeData( myself ) );
	
	if( nd.envMapProbe ){
		nd.envMapProbe->FreeReference();
		nd.envMapProbe = NULL;
	}
}



// public func DVector getPosition()
deClassEnvMapProbe::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsEmp,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassEnvMapProbe::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassDVector()->PushDVector( rt, envMapProbe.GetPosition() );
}

// public func void setPosition( DVector position )
deClassEnvMapProbe::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsEmp,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassEnvMapProbe::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	
	envMapProbe.SetPosition( ds.GetClassDVector()->GetDVector( objPosition ) );
}

// public func Quaternion getOrientation()
deClassEnvMapProbe::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsEmp,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassEnvMapProbe::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassQuaternion()->PushQuaternion( rt, envMapProbe.GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassEnvMapProbe::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsEmp,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassEnvMapProbe::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objOrientation = rt->GetValue( 0 )->GetRealObject();
	
	envMapProbe.SetOrientation( ds.GetClassQuaternion()->GetQuaternion( objOrientation ) );
}

// public func Vector getScaling()
deClassEnvMapProbe::nfGetScaling::nfGetScaling( const sInitData &init ) : dsFunction( init.clsEmp,
"getScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassEnvMapProbe::nfGetScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassVector()->PushVector( rt, envMapProbe.GetScaling() );
}

// public func void setScaling( Vector scaling )
deClassEnvMapProbe::nfSetScaling::nfSetScaling( const sInitData &init ) : dsFunction( init.clsEmp,
"setScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // scaling
}
void deClassEnvMapProbe::nfSetScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	
	envMapProbe.SetScaling( ds.GetClassVector()->GetVector( objScaling ) );
}



// public func LayerMask getLayerMask()
deClassEnvMapProbe::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) : dsFunction( init.clsEmp,
"getLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassEnvMapProbe::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassLayerMask()->PushLayerMask( rt, envMapProbe.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassEnvMapProbe::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) : dsFunction( init.clsEmp,
"setLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassEnvMapProbe::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	envMapProbe.SetLayerMask( ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func ShapeList getShapeListInfluence()
deClassEnvMapProbe::nfGetShapeListInfluence::nfGetShapeListInfluence( const sInitData &init ) : dsFunction( init.clsEmp,
"getShapeListInfluence", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShaList ){
}
void deClassEnvMapProbe::nfGetShapeListInfluence::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassShapeList()->PushShapeList( rt, envMapProbe.GetShapeListInfluence() );
}

// public func void setShapeListInfluence( ShapeList shapeList )
deClassEnvMapProbe::nfSetShapeListInfluence::nfSetShapeListInfluence( const sInitData &init ) : dsFunction( init.clsEmp,
"setShapeListInfluence", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShaList ); // shapeList
}
void deClassEnvMapProbe::nfSetShapeListInfluence::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objShapeList = rt->GetValue( 0 )->GetRealObject();
	
	envMapProbe.GetShapeListInfluence() = ds.GetClassShapeList()->GetShapeList( objShapeList );
	envMapProbe.NotifyShapeListInfluenceChanged();
}

// public func ShapeList getShapeReflection()
deClassEnvMapProbe::nfGetShapeReflection::nfGetShapeReflection( const sInitData &init ) : dsFunction( init.clsEmp,
"getShapeReflection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShaList ){
}
void deClassEnvMapProbe::nfGetShapeReflection::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	decShapeList shapeList;
	
	if( envMapProbe.GetShapeReflection() ){
		shapeList.Add( envMapProbe.GetShapeReflection()->Copy() );
	}
	
	ds.GetClassShapeList()->PushShapeList( rt, shapeList );
}

// public func void setShapeReflection( ShapeList shape )
deClassEnvMapProbe::nfSetShapeReflection::nfSetShapeReflection( const sInitData &init ) : dsFunction( init.clsEmp,
"setShapeReflection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShaList ); // shape
}
void deClassEnvMapProbe::nfSetShapeReflection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objShape = rt->GetValue( 0 )->GetRealObject();
	
	if( objShape ){
		const decShapeList &shapeList = ds.GetClassShapeList()->GetShapeList( objShape );
		
		if( shapeList.GetCount() > 0 ){
			envMapProbe.SetShapeReflection( shapeList.GetAt( 0 )->Copy() );
			
		}else{
			envMapProbe.SetShapeReflection( NULL );
		}
		
	}else{
		envMapProbe.SetShapeReflection( NULL );
	}
}

// public func ShapeList getShapeListReflectionMask()
deClassEnvMapProbe::nfGetShapeListReflectionMask::nfGetShapeListReflectionMask( const sInitData &init ) : dsFunction( init.clsEmp,
"getShapeListReflectionMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShaList ){
}
void deClassEnvMapProbe::nfGetShapeListReflectionMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassShapeList()->PushShapeList( rt, envMapProbe.GetShapeListReflectionMask() );
}

// public func void setShapeListReflectionMask( ShapeList shapeList )
deClassEnvMapProbe::nfSetShapeListReflectionMask::nfSetShapeListReflectionMask( const sInitData &init ) : dsFunction( init.clsEmp,
"setShapeListReflectionMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShaList ); // shapeList
}
void deClassEnvMapProbe::nfSetShapeListReflectionMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const deScriptingDragonScript &ds = *( ( ( deClassEnvMapProbe* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objShapeList = rt->GetValue( 0 )->GetRealObject();
	
	envMapProbe.GetShapeListReflectionMask() = ds.GetClassShapeList()->GetShapeList( objShapeList );
	envMapProbe.NotifyShapeReflectionChanged();
}

// public func float getInfluenceBorderSize()
deClassEnvMapProbe::nfGetInfluenceBorderSize::nfGetInfluenceBorderSize( const sInitData &init ) : dsFunction( init.clsEmp,
"getInfluenceBorderSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassEnvMapProbe::nfGetInfluenceBorderSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	
	rt->PushFloat( envMapProbe.GetInfluenceBorderSize() );
}

// public func void setInfluenceBorderSize( float borderSize )
deClassEnvMapProbe::nfSetInfluenceBorderSize::nfSetInfluenceBorderSize( const sInitData &init ) : dsFunction( init.clsEmp,
"setInfluenceBorderSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // borderSize
}
void deClassEnvMapProbe::nfSetInfluenceBorderSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const float borderSize = rt->GetValue( 0 )->GetFloat();
	
	envMapProbe.SetInfluenceBorderSize( borderSize );
}

// public func int getInfluencePriority()
deClassEnvMapProbe::nfGetInfluencePriority::nfGetInfluencePriority( const sInitData &init ) : dsFunction( init.clsEmp,
"getInfluencePriority", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassEnvMapProbe::nfGetInfluencePriority::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	
	rt->PushInt( envMapProbe.GetInfluencePriority() );
}

// public func void setInfluencePriority( int priority )
deClassEnvMapProbe::nfSetInfluencePriority::nfSetInfluencePriority( const sInitData &init ) : dsFunction( init.clsEmp,
"setInfluencePriority", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // priority
}
void deClassEnvMapProbe::nfSetInfluencePriority::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEnvMapProbe &envMapProbe = *( ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe );
	const int priority = rt->GetValue( 0 )->GetInt();
	
	envMapProbe.SetInfluencePriority( priority );
}



// public func int hashCode()
deClassEnvMapProbe::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsEmp,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassEnvMapProbe::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe * const envMapProbe = ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe;
	
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )envMapProbe );
}

// public func bool equals( Object obj )
deClassEnvMapProbe::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsEmp,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassEnvMapProbe::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEnvMapProbe * const envMapProbe = ( ( sEmpNatDat* )p_GetNativeData( myself ) )->envMapProbe;
	deClassEnvMapProbe * const clsEmp = ( deClassEnvMapProbe* )GetOwnerClass();
	
	dsValue *object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsEmp ) ){
		rt->PushBool( false );
		
	}else{
		const deEnvMapProbe * const otherEnvMapProbe = ( ( sEmpNatDat* )p_GetNativeData( object ) )->envMapProbe;
		
		rt->PushBool( envMapProbe == otherEnvMapProbe );
	}
}

// public static func bool equals( EnvMapProbe envMapProbe1, EnvMapProbe envMapProbe2 )
deClassEnvMapProbe::nfEquals2::nfEquals2( const sInitData &init ) :
dsFunction( init.clsEmp, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsEmp ); // envMapProbe1
	p_AddParameter( init.clsEmp ); // envMapProbe2
}
void deClassEnvMapProbe::nfEquals2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassEnvMapProbe &clsEmp = *( ( deClassEnvMapProbe* )GetOwnerClass() );
	
	const deEnvMapProbe * const envMapProbe1 = clsEmp.GetEnvMapProbe( rt->GetValue( 0 )->GetRealObject() );
	const deEnvMapProbe * const envMapProbe2 = clsEmp.GetEnvMapProbe( rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushBool( envMapProbe1 == envMapProbe2 );
}



// Class deClassEnvMapProbe
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEnvMapProbe::deClassEnvMapProbe( deScriptingDragonScript *ds ) :
dsClass( "EnvMapProbe", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sEmpNatDat ) );
}

deClassEnvMapProbe::~deClassEnvMapProbe(){
}



// Management
///////////////

void deClassEnvMapProbe::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsEmp = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	
	init.clsDVec = pDS->GetClassDVector();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsVec = pDS->GetClassVector();
	init.clsShaList = pDS->GetClassShapeList();
	init.clsLayerMask = pDS->GetClassLayerMask();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetScaling( init ) );
	AddFunction( new nfSetScaling( init ) );
	
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	
	AddFunction( new nfGetShapeListInfluence( init ) );
	AddFunction( new nfSetShapeListInfluence( init ) );
	
	AddFunction( new nfGetShapeReflection( init ) );
	AddFunction( new nfSetShapeReflection( init ) );
	AddFunction( new nfGetShapeListReflectionMask( init ) );
	AddFunction( new nfSetShapeListReflectionMask( init ) );
	
	AddFunction( new nfGetInfluenceBorderSize( init ) );
	AddFunction( new nfSetInfluenceBorderSize( init ) );
	AddFunction( new nfGetInfluencePriority( init ) );
	AddFunction( new nfSetInfluencePriority( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfEquals2( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deEnvMapProbe *deClassEnvMapProbe::GetEnvMapProbe( dsRealObject *object ) const{
	if( ! object ){
		return NULL;
	}
	
	return ( ( sEmpNatDat* )p_GetNativeData( object->GetBuffer() ) )->envMapProbe;
}

void deClassEnvMapProbe::PushEnvMapProbe( dsRunTime *rt, deEnvMapProbe *envMapProbe ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! envMapProbe ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sEmpNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->envMapProbe = envMapProbe;
	envMapProbe->AddReference();
}
