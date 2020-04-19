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

#include "deClassPropField.h"
#include "deClassPropFieldListener.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../physics/deClassCollisionFilter.h"
#include "../world/deClassModel.h"
#include "../world/deClassSkin.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../peers/dedsPropField.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include "dragengine/resources/skin/deSkin.h"
#include <libdscript/exceptions.h>



// Native Structure
/////////////////////

struct sPFNatDat{
	dePropField *propfield;
};



// Native functions
/////////////////////

// public func new()
deClassPropField::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsPF,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassPropField::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sPFNatDat *nd = ( sPFNatDat* )p_GetNativeData( myself );
	deClassPropField *clsPF = ( deClassPropField* )GetOwnerClass();
	dePropFieldManager *pfmgr = clsPF->GetDS()->GetGameEngine()->GetPropFieldManager();
	
	// clear ( important )
	nd->propfield = NULL;
	
	// create propfield
	nd->propfield = pfmgr->CreatePropField();
	if( ! nd->propfield ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassPropField::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsPF,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassPropField::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sPFNatDat *nd = ( sPFNatDat* )p_GetNativeData( myself );
	
	if( nd->propfield ){
		nd->propfield->FreeReference();
		nd->propfield = NULL;
	}
}



// Management
///////////////

// public func void setPosition( DVector position )
deClassPropField::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsPF,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassPropField::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	deClassPropField *clsPF = ( deClassPropField* )GetOwnerClass();
	deClassDVector *clsDVec = clsPF->GetDS()->GetClassDVector();
	
	dsRealObject *objPos = rt->GetValue( 0 )->GetRealObject();
	if( ! objPos ) DSTHROW( dueNullPointer );
	
	propfield->SetPosition( clsDVec->GetDVector( objPos ) );
}

// public func int addType( Model model, Skin skin, float rotPerForce, float restitution,
// CollisionFilter collisionFilter, int instanceCount )
deClassPropField::nfAddType::nfAddType( const sInitData &init ) : dsFunction( init.clsPF,
"addType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsMdl ); // model
	p_AddParameter( init.clsSkin ); // skin
	p_AddParameter( init.clsFlt ); // rotPerForce
	p_AddParameter( init.clsFlt ); // restitution
	p_AddParameter( init.clsCF ); // collisionFilter
	p_AddParameter( init.clsInt ); // instanceCount
}
void deClassPropField::nfAddType::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField &propfield = *( ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield );
	deScriptingDragonScript &ds = *( ( ( deClassPropField* )GetOwnerClass() )->GetDS() );
	deClassModel &clsMdl = *ds.GetClassModel();
	deClassSkin &clsSkin = *ds.GetClassSkin();
	
	dsRealObject *objMdl = rt->GetValue( 0 )->GetRealObject();
	dsRealObject *objSkin = rt->GetValue( 1 )->GetRealObject();
	float rotPerForce = rt->GetValue( 2 )->GetFloat();
	float restitution = rt->GetValue( 3 )->GetFloat();
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter( rt->GetValue( 4 )->GetRealObject() );
	int instanceCount = rt->GetValue( 5 )->GetInt();
	int index = propfield.GetTypeCount();
	
	dePropFieldType *type = NULL;
	
	try{
		type = new dePropFieldType;
		
		if( objMdl ){
			type->SetModel( clsMdl.GetModel( objMdl ) );
		}
		if( objSkin ){
			type->SetSkin( clsSkin.GetSkin( objSkin ) );
		}
		type->SetRotationPerForce( rotPerForce * DEG2RAD );
		type->SetRestitution( restitution );
		type->SetCollisionFilter( collisionFilter );
		type->SetInstanceCount( instanceCount );
		
		propfield.AddType( type );
		
	}catch( ... ){
		if( type ) delete type;
		throw;
	}
	
	rt->PushInt( index );
}

// public func void removeAllTypes()
deClassPropField::nfRemoveAllTypes::nfRemoveAllTypes( const sInitData &init ) : dsFunction( init.clsPF,
"removeAllTypes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassPropField::nfRemoveAllTypes::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	
	propfield->RemoveAllTypes();
}

// public func void setInstanceAt( int type, int instance, Vector position, Vector rotation, float scaling )
deClassPropField::nfSetInstanceAt::nfSetInstanceAt( const sInitData &init ) : dsFunction( init.clsPF,
"setInstanceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsInt ); // instance
	p_AddParameter( init.clsVec ); // position
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsFlt ); // scaling
}
void deClassPropField::nfSetInstanceAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	deClassPropField *clsPF = ( deClassPropField* )GetOwnerClass();
	deClassVector *clsVec = clsPF->GetDS()->GetClassVector();
	
	int typeIndex = rt->GetValue( 0 )->GetInt();
	int instanceIndex = rt->GetValue( 1 )->GetInt();
	dsRealObject *objPos = rt->GetValue( 2 )->GetRealObject();
	dsRealObject *objRot = rt->GetValue( 3 )->GetRealObject();
	float scaling = rt->GetValue( 4 )->GetFloat();
	
	if( ! objPos || ! objRot ) DSTHROW( dueNullPointer );
	
	dePropFieldInstance &instance = propfield->GetTypeAt( typeIndex )->GetInstanceAt( instanceIndex );
	
	instance.SetPosition( clsVec->GetVector( objPos ) );
	instance.SetRotation( clsVec->GetVector( objRot ) * DEG2RAD );
	instance.SetScaling( scaling );
}



// Listening
//////////////

// public func PropFieldListener getListener()
deClassPropField::nfGetListener::nfGetListener( const sInitData &init ) : dsFunction( init.clsPF,
"getListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPFL ){
}
void deClassPropField::nfGetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	deClassPropField *clsPF = ( deClassPropField* )GetOwnerClass();
	dedsPropField *peer = ( dedsPropField* )propfield->GetPeerScripting();
	
	rt->PushObject( peer->GetCallback(), clsPF->GetDS()->GetClassPropFieldListener() );
}

// public func void setListener( PropFieldListener listener )
deClassPropField::nfSetListener::nfSetListener( const sInitData &init ) : dsFunction( init.clsPF,
"setListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPFL ); // listener
}
void deClassPropField::nfSetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	dedsPropField *peer = ( dedsPropField* )propfield->GetPeerScripting();
	
	peer->SetCallback( rt->GetValue( 0 )->GetRealObject() );
}



// public func int hashCode()
deClassPropField::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsPF, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassPropField::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	
	rt->PushInt( ( intptr_t )propfield );
}

// public func bool equals( Object obj )
deClassPropField::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsPF, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassPropField::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	dePropField *propfield = ( ( sPFNatDat* )p_GetNativeData( myself ) )->propfield;
	deClassPropField *clsPF = ( deClassPropField* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsPF ) ){
		rt->PushBool( false );
		
	}else{
		dePropField *other = ( ( sPFNatDat* )p_GetNativeData( obj ) )->propfield;
		
		rt->PushBool( propfield == other );
	}
}




// Class deClassPropField
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassPropField::deClassPropField( deScriptingDragonScript *ds ) :
dsClass( "PropField", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sPFNatDat ) );
}

deClassPropField::~deClassPropField(){
}



// Management
///////////////

void deClassPropField::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	memset( &init, '\0', sizeof( init ) );
	
	init.clsPF = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS->GetClassVector();
	init.clsDVec = pDS->GetClassDVector();
	init.clsMdl = pDS->GetClassModel();
	init.clsSkin = pDS->GetClassSkin();
	init.clsPFL = pDS->GetClassPropFieldListener();
	init.clsCF = pDS->GetClassCollisionFilter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetPosition( init ) );
	
	AddFunction( new nfAddType( init ) );
	AddFunction( new nfRemoveAllTypes( init ) );
	AddFunction( new nfSetInstanceAt( init ) );
	
	AddFunction( new nfGetListener( init ) );
	AddFunction( new nfSetListener( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

dePropField *deClassPropField::GetPropField( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sPFNatDat* )p_GetNativeData( myself->GetBuffer() ) )->propfield;
}

void deClassPropField::PushPropField( dsRunTime *rt, dePropField *propfield ){
	if( ! rt ) DSTHROW( dueInvalidParam );
	
	if( ! propfield ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sPFNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->propfield = propfield;
	propfield->AddReference();
}
