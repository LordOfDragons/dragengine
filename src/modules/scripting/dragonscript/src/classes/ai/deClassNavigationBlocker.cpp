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

#include "deClassNavigationBlocker.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../utils/deClassShapeList.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlockerManager.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sNavBlockerNatDat{
	deNavigationBlocker *blocker;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassNavigationBlocker::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsNavBlocker,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigationBlocker::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sNavBlockerNatDat &nd = *( ( sNavBlockerNatDat* )p_GetNativeData( myself ) );
	const deClassNavigationBlocker &clsNavBlocker = *( ( deClassNavigationBlocker* )GetOwnerClass() );
	deNavigationBlockerManager &navBlockerMgr = *clsNavBlocker.GetDS()->GetGameEngine()->GetNavigationBlockerManager();
	
	// clear ( important )
	nd.blocker = NULL;
	
	// create navigation blocker
	nd.blocker = navBlockerMgr.CreateNavigationBlocker();
}

// public func destructor()
deClassNavigationBlocker::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsNavBlocker,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigationBlocker::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sNavBlockerNatDat &nd = *( ( sNavBlockerNatDat* )p_GetNativeData( myself ) );
	
	if( nd.blocker ){
		nd.blocker->FreeReference();
		nd.blocker = NULL;
	}
}



// Management
///////////////

// public func DVector getPosition()
deClassNavigationBlocker::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassNavigationBlocker::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	const deScriptingDragonScript &ds = *( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassDVector()->PushDVector( rt, blocker.GetPosition() );
}

// public func void setPosition( DVector position )
deClassNavigationBlocker::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassNavigationBlocker::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	const deScriptingDragonScript &ds = *( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetDS() );
	
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	
	if( ! objPosition ){
		DSTHROW( dueNullPointer );
	}
	
	blocker.SetPosition( ds.GetClassDVector()->GetDVector( objPosition ) );
}

// public func Quaternion getOrientation()
deClassNavigationBlocker::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassNavigationBlocker::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	const deScriptingDragonScript &ds = *( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassQuaternion()->PushQuaternion( rt, blocker.GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassNavigationBlocker::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassNavigationBlocker::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	const deScriptingDragonScript &ds = *( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetDS() );
	
	dsRealObject * const objOrientation = rt->GetValue( 0 )->GetRealObject();
	
	if( ! objOrientation ){
		DSTHROW( dueNullPointer );
	}
	
	blocker.SetOrientation( ds.GetClassQuaternion()->GetQuaternion( objOrientation ) );
}

// public func void int getLayer()
deClassNavigationBlocker::nfGetLayer::nfGetLayer( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"getLayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassNavigationBlocker::nfGetLayer::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	
	rt->PushInt( blocker.GetLayer() );
}

// public func void setLayer( int layer )
deClassNavigationBlocker::nfSetLayer::nfSetLayer( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"setLayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // layer
}
void deClassNavigationBlocker::nfSetLayer::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	
	blocker.SetLayer( rt->GetValue( 0 )->GetInt() );
}

// public func NavigationSpaceType getSpaceType()
deClassNavigationBlocker::nfGetSpaceType::nfGetSpaceType( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"getSpaceType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavigationSpaceType ){
}
void deClassNavigationBlocker::nfGetSpaceType::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	
	rt->PushValue( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetClassNavigationSpaceType()
		->GetVariable( blocker.GetSpaceType() )->GetStaticValue() );
}

// public func void setSpaceType( NavigationSpaceType spaceType )
deClassNavigationBlocker::nfSetSpaceType::nfSetSpaceType( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"setSpaceType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavigationSpaceType ); // spaceType
}
void deClassNavigationBlocker::nfSetSpaceType::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	
	blocker.SetSpaceType( ( deNavigationSpace::eSpaceTypes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func int getBlockingPriority()
deClassNavigationBlocker::nfGetBlockingPriority::nfGetBlockingPriority( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"getBlockingPriority", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassNavigationBlocker::nfGetBlockingPriority::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	
	rt->PushInt( blocker.GetBlockingPriority() );
}

// public func void setBlockingPriority( int priority )
deClassNavigationBlocker::nfSetBlockingPriority::nfSetBlockingPriority( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"setBlockingPriority", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // priority
}
void deClassNavigationBlocker::nfSetBlockingPriority::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	
	blocker.SetBlockingPriority( rt->GetValue( 0 )->GetInt() );
}

// public func void bool getEnabled()
deClassNavigationBlocker::nfGetEnabled::nfGetEnabled( const sInitData &init ) :
dsFunction( init.clsNavBlocker, "getEnabled", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassNavigationBlocker::nfGetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	rt->PushBool( blocker.GetEnabled() );
}

// public func void setEnabled( bool enabled )
deClassNavigationBlocker::nfSetEnabled::nfSetEnabled( const sInitData &init ) :
dsFunction( init.clsNavBlocker, "setEnabled", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassNavigationBlocker::nfSetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	blocker.SetEnabled( rt->GetValue( 0 )->GetBool() );
}

// func World getParentWorld()
deClassNavigationBlocker::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsNavBlocker, "getParentWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}

void deClassNavigationBlocker::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationBlocker &blocker = *((const sNavBlockerNatDat *)p_GetNativeData(myself))->blocker;
	const deScriptingDragonScript &ds = *((deClassNavigationBlocker*)GetOwnerClass())->GetDS();
	ds.GetClassWorld()->PushWorld(rt, blocker.GetParentWorld());
}

// public func ShapeList getShapeList()
deClassNavigationBlocker::nfGetShapeList::nfGetShapeList( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"getShapeList", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShaList ){
}
void deClassNavigationBlocker::nfGetShapeList::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	const deScriptingDragonScript &ds = *( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassShapeList()->PushShapeList( rt, blocker.GetShapeList() );
}

// public func void setShapeList( ShapeList shapeList )
deClassNavigationBlocker::nfSetShapeList::nfSetShapeList( const sInitData &init ) : dsFunction( init.clsNavBlocker,
"setShapeList", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShaList ); // shapeList
}
void deClassNavigationBlocker::nfSetShapeList::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigationBlocker &blocker = *( ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker );
	const deScriptingDragonScript &ds = *( ( ( deClassNavigationBlocker* )GetOwnerClass() )->GetDS() );
	dsRealObject * const objShapeList = rt->GetValue( 0 )->GetRealObject();
	
	blocker.GetShapeList() = ds.GetClassShapeList()->GetShapeList( objShapeList );
	blocker.NotifyShapeListChanged();
}



// public func int hashCode()
deClassNavigationBlocker::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsNavBlocker, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassNavigationBlocker::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigationBlocker * const blocker = ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker;
	
	rt->PushInt( ( int )( intptr_t )blocker );
}

// public func bool equals( Object object )
deClassNavigationBlocker::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsNavBlocker, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassNavigationBlocker::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigationBlocker * const blocker = ( ( const sNavBlockerNatDat * )p_GetNativeData( myself ) )->blocker;
	deClassNavigationBlocker * const clsNavBlocker = ( deClassNavigationBlocker* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsNavBlocker ) ){
		rt->PushBool( false );
		
	}else{
		const deNavigationBlocker * const otherNavBlocker = ( ( const sNavBlockerNatDat * )p_GetNativeData( object ) )->blocker;
		rt->PushBool( blocker == otherNavBlocker );
	}
}



// Class deClassNavigationBlocker
////////////////////////////////

// Constructor
////////////////

deClassNavigationBlocker::deClassNavigationBlocker( deScriptingDragonScript *ds ) :
dsClass( "NavigationBlocker", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sNavBlockerNatDat ) );
}

deClassNavigationBlocker::~deClassNavigationBlocker(){
}



// Management
///////////////

void deClassNavigationBlocker::CreateClassMembers( dsEngine *engine ){
	pClsNavigationSpaceType = engine->GetClass( "Dragengine.Scenery.NavigationSpaceType" );
	
	sInitData init;
	init.clsNavBlocker = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec = pDS->GetClassVector();
	init.clsDVec = pDS->GetClassDVector();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsShaList = pDS->GetClassShapeList();
	init.clsNavigationSpaceType = pClsNavigationSpaceType;
	init.clsWorld = pDS->GetClassWorld();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetLayer( init ) );
	AddFunction( new nfSetLayer( init ) );
	AddFunction( new nfGetSpaceType( init ) );
	AddFunction( new nfSetSpaceType( init ) );
	AddFunction( new nfGetBlockingPriority( init ) );
	AddFunction( new nfSetBlockingPriority( init ) );
	AddFunction( new nfGetEnabled( init ) );
	AddFunction( new nfSetEnabled( init ) );
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction( new nfGetShapeList( init ) );
	AddFunction( new nfSetShapeList( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

deNavigationBlocker *deClassNavigationBlocker::GetNavigationBlocker( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( const sNavBlockerNatDat * )p_GetNativeData( myself->GetBuffer() ) )->blocker;
}

void deClassNavigationBlocker::PushNavigationBlocker( dsRunTime *rt, deNavigationBlocker *blocker ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! blocker ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sNavBlockerNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->blocker = blocker;
	blocker->AddReference();
}
