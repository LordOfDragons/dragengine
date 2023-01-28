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

#include "deClassForceField.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassCollisionFilter.h"
#include "../utils/deClassShapeList.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/forcefield/deForceFieldManager.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sFFNatDat{
	deForceField *forcefield;
};



// Native functions
/////////////////////

// public func new()
deClassForceField::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsFF,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassForceField::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sFFNatDat *nd = ( sFFNatDat* )p_GetNativeData( myself );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd->forcefield = NULL;
	
	// create forcefield
	nd->forcefield = ds.GetGameEngine()->GetForceFieldManager()->CreateForceField();
}

// public func destructor()
deClassForceField::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsFF,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassForceField::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sFFNatDat &nd = *( ( sFFNatDat* )p_GetNativeData( myself ) );
	
	if( nd.forcefield ){
		nd.forcefield->FreeReference();
		nd.forcefield = NULL;
	}
}



// Management
///////////////

// public func DVector getPosition()
deClassForceField::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsFF,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassForceField::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	ds.GetClassDVector()->PushDVector( rt, forcefield.GetPosition() );
}

// public func void setPosition( DVector position )
deClassForceField::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsFF,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassForceField::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	const decDVector &position = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	forcefield.SetPosition( position );
}

// public func Quaternion getOrientation()
deClassForceField::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsFF,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassForceField::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, forcefield.GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassForceField::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsFF,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassForceField::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 0 )->GetRealObject() );
	forcefield.SetOrientation( orientation );
}



// public func ShapeList getInfluenceArea()
deClassForceField::nfGetInfluenceArea::nfGetInfluenceArea( const sInitData &init ) : dsFunction( init.clsFF,
"getInfluenceArea", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList ){
}
void deClassForceField::nfGetInfluenceArea::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	ds.GetClassShapeList()->PushShapeList( rt, forcefield.GetInfluenceArea() );
}

// public func void setInfluenceArea( ShapeList area )
deClassForceField::nfSetInfluenceArea::nfSetInfluenceArea( const sInitData &init ) : dsFunction( init.clsFF,
"setInfluenceArea", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShapeList ); // area
}
void deClassForceField::nfSetInfluenceArea::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	const decShapeList &area = ds.GetClassShapeList()->GetShapeList( rt->GetValue( 0 )->GetRealObject() );
	forcefield.SetInfluenceArea( area );
}



// public func float getRadius()
deClassForceField::nfGetRadius::nfGetRadius( const sInitData &init ) : dsFunction( init.clsFF,
"getRadius", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassForceField::nfGetRadius::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushFloat( forcefield.GetRadius() );
}

// public func void setRadius( float radius )
deClassForceField::nfSetRadius::nfSetRadius( const sInitData &init ) : dsFunction( init.clsFF,
"setRadius", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // radius
}
void deClassForceField::nfSetRadius::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	forcefield.SetRadius( rt->GetValue( 0 )->GetFloat() );
}

// public func float getExponent()
deClassForceField::nfGetExponent::nfGetExponent( const sInitData &init ) : dsFunction( init.clsFF,
"getExponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassForceField::nfGetExponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushFloat( forcefield.GetExponent() );
}

// public func void setExponent( float exponent )
deClassForceField::nfSetExponent::nfSetExponent( const sInitData &init ) : dsFunction( init.clsFF,
"setExponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // radius
}
void deClassForceField::nfSetExponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	forcefield.SetExponent( rt->GetValue( 0 )->GetFloat() );
}



// public func ForceFieldType getFieldType()
deClassForceField::nfGetFieldType::nfGetFieldType( const sInitData &init ) : dsFunction( init.clsFF,
"getFieldType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsForceFieldType ){
}
void deClassForceField::nfGetFieldType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushValue( ( ( deClassForceField* )GetOwnerClass() )->GetClassForceFieldType()
		->GetVariable( forcefield.GetFieldType() )->GetStaticValue() );
}

// public func void setFieldType( ForceFieldType type )
deClassForceField::nfSetFieldType::nfSetFieldType( const sInitData &init ) : dsFunction( init.clsFF,
"setFieldType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsForceFieldType ); // type
}
void deClassForceField::nfSetFieldType::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	forcefield.SetFieldType( ( deForceField::eFieldTypes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func ForceFieldApplication getApplicationType()
deClassForceField::nfGetApplicationType::nfGetApplicationType( const sInitData &init ) : dsFunction( init.clsFF,
"getApplicationType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsForceFieldApplication ){
}
void deClassForceField::nfGetApplicationType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushValue( ( ( deClassForceField* )GetOwnerClass() )->GetClassForceFieldApplication()
		->GetVariable( forcefield.GetApplicationType() )->GetStaticValue() );
}

// public func void setApplicationType( ForceFieldApplication type )
deClassForceField::nfSetApplicationType::nfSetApplicationType( const sInitData &init ) : dsFunction( init.clsFF,
"setApplicationType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsForceFieldApplication ); // type
}
void deClassForceField::nfSetApplicationType::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	forcefield.SetApplicationType( ( deForceField::eApplicationTypes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Vector getDirection()
deClassForceField::nfGetDirection::nfGetDirection( const sInitData &init ) : dsFunction( init.clsFF,
"getDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassForceField::nfGetDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector()->PushVector( rt, forcefield.GetDirection() );
}

// public func void setDirection( Vector direction )
deClassForceField::nfSetDirection::nfSetDirection( const sInitData &init ) : dsFunction( init.clsFF,
"setDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // direction
}
void deClassForceField::nfSetDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	const decVector &direction = ds.GetClassVector()->GetVector( rt->GetValue( 0 )->GetRealObject() );
	forcefield.SetDirection( direction );
}

// public func float getForce()
deClassForceField::nfGetForce::nfGetForce( const sInitData &init ) : dsFunction( init.clsFF,
"getForce", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassForceField::nfGetForce::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushFloat( forcefield.GetForce() );
}

// public void setForce( float force )
deClassForceField::nfSetForce::nfSetForce( const sInitData &init ) : dsFunction( init.clsFF,
"setForce", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // force
}
void deClassForceField::nfSetForce::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	forcefield.SetForce( rt->GetValue( 0 )->GetFloat() );
}

// public func float getFluctuationDirection()
deClassForceField::nfGetFluctuationDirection::nfGetFluctuationDirection( const sInitData &init ) : dsFunction( init.clsFF,
"getFluctuationDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassForceField::nfGetFluctuationDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushFloat( forcefield.GetFluctuationDirection() );
}

// public func void setFluctuationDirection( float fluctuation )
deClassForceField::nfSetFluctuationDirection::nfSetFluctuationDirection( const sInitData &init ) : dsFunction( init.clsFF,
"setFluctuationDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // fluctuation
}
void deClassForceField::nfSetFluctuationDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	forcefield.SetFluctuationDirection( rt->GetValue( 0 )->GetFloat() );
}

// public func float getFluctuationForce()
deClassForceField::nfGetFluctuationForce::nfGetFluctuationForce( const sInitData &init ) : dsFunction( init.clsFF,
"getFluctuationForce", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassForceField::nfGetFluctuationForce::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushFloat( forcefield.GetFluctuationForce() );
}

// public func void setFluctuationForce( float fluctuation )
deClassForceField::nfSetFluctuationForce::nfSetFluctuationForce( const sInitData &init ) : dsFunction( init.clsFF,
"setFluctuationForce", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // fluctuation
}
void deClassForceField::nfSetFluctuationForce::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	forcefield.SetFluctuationForce( rt->GetValue( 0 )->GetFloat() );
}



// public func ShapeList getShape()
deClassForceField::nfGetShape::nfGetShape( const sInitData &init ) :
dsFunction( init.clsFF, "getShape", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList ){
}
void deClassForceField::nfGetShape::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	ds.GetClassShapeList()->PushShapeList( rt, forcefield.GetShape() );
}

// public func void setShape( ShapeList shape )
deClassForceField::nfSetShape::nfSetShape( const sInitData &init ) :
dsFunction( init.clsFF, "setShape", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShapeList ); // shape
}
void deClassForceField::nfSetShape::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	forcefield.SetShape( ds.GetClassShapeList()->GetShapeList( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func CollisionFilter getCollisionFilter()
deClassForceField::nfGetCollisionFilter::nfGetCollisionFilter( const sInitData &init ) : dsFunction( init.clsFF,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF ){
}
void deClassForceField::nfGetCollisionFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	ds.GetClassCollisionFilter()->PushCollisionFilter( rt, forcefield.GetCollisionFilter() );
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassForceField::nfSetCollisionFilter::nfSetCollisionFilter( const sInitData &init ) : dsFunction( init.clsFF,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassForceField::nfSetCollisionFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	const deScriptingDragonScript &ds = ( ( deClassForceField* )GetOwnerClass() )->GetDS();
	
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter( rt->GetValue( 0 )->GetRealObject() );
	forcefield.SetCollisionFilter( collisionFilter );
}

// public func bool getEnabled()
deClassForceField::nfGetEnabled::nfGetEnabled( const sInitData &init ) : dsFunction( init.clsFF,
"getEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassForceField::nfGetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	rt->PushBool( forcefield.GetEnabled() );
}

// public func void setEnabled( bool enabled )
deClassForceField::nfSetEnabled::nfSetEnabled( const sInitData &init ) : dsFunction( init.clsFF,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassForceField::nfSetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	deForceField &forcefield = *( ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield );
	forcefield.SetEnabled( rt->GetValue( 0 )->GetBool() );
}



// public func int hashCode()
deClassForceField::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsFF, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassForceField::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField * const forcefield = ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield;
	rt->PushInt( ( int )( intptr_t )forcefield );
}

// public func bool equals( Object obj )
deClassForceField::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsFF, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassForceField::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deForceField * const forcefield = ( ( sFFNatDat* )p_GetNativeData( myself ) )->forcefield;
	deClassForceField * const clsFF = ( deClassForceField* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsFF ) ){
		rt->PushBool( false );
		
	}else{
		const deForceField * const other = ( ( sFFNatDat* )p_GetNativeData( obj ) )->forcefield;
		rt->PushBool( forcefield == other );
	}
}



// Class deClassForceField
////////////////////////////

// Constructor, destructor
////////////////////////////

deClassForceField::deClassForceField( deScriptingDragonScript &ds ) :
dsClass( "ForceField", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sFFNatDat ) );
}

deClassForceField::~deClassForceField(){
}



// Management
///////////////

void deClassForceField::CreateClassMembers( dsEngine *engine ){
	pClsForceFieldType = engine->GetClass( "Dragengine.Scenery.ForceFieldType" );
	pClsForceFieldApplication = engine->GetClass( "Dragengine.Scenery.ForceFieldApplication" );
	
	sInitData init;
	init.clsFF = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsDVec = pDS.GetClassDVector();
	init.clsVec = pDS.GetClassVector();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsCF = pDS.GetClassCollisionFilter();
	init.clsShapeList = pDS.GetClassShapeList();
	init.clsForceFieldType = pClsForceFieldType;
	init.clsForceFieldApplication = pClsForceFieldApplication;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	
	AddFunction( new nfGetInfluenceArea( init ) );
	AddFunction( new nfSetInfluenceArea( init ) );
	AddFunction( new nfGetRadius( init ) );
	AddFunction( new nfSetRadius( init ) );
	AddFunction( new nfGetExponent( init ) );
	AddFunction( new nfSetExponent( init ) );
	
	AddFunction( new nfGetFieldType( init ) );
	AddFunction( new nfSetFieldType( init ) );
	AddFunction( new nfGetApplicationType( init ) );
	AddFunction( new nfSetApplicationType( init ) );
	AddFunction( new nfGetDirection( init ) );
	AddFunction( new nfSetDirection( init ) );
	AddFunction( new nfGetForce( init ) );
	AddFunction( new nfSetForce( init ) );
	AddFunction( new nfGetFluctuationDirection( init ) );
	AddFunction( new nfSetFluctuationDirection( init ) );
	AddFunction( new nfGetFluctuationForce( init ) );
	AddFunction( new nfSetFluctuationForce( init ) );
	
	AddFunction( new nfGetShape( init ) );
	AddFunction( new nfSetShape( init ) );
	AddFunction( new nfGetCollisionFilter( init ) );
	AddFunction( new nfSetCollisionFilter( init ) );
	AddFunction( new nfGetEnabled( init ) );
	AddFunction( new nfSetEnabled( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deForceField *deClassForceField::GetForceField( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sFFNatDat* )p_GetNativeData( myself->GetBuffer() ) )->forcefield;
}

void deClassForceField::PushForceField( dsRunTime *rt, deForceField *forcefield ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! forcefield ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sFFNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->forcefield = forcefield;
	forcefield->AddReference();
}
