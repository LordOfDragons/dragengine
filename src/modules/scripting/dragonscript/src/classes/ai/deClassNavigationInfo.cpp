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

#include "deClassNavigationInfo.h"
#include "../math/deClassDVector.h"
#include "../../utils/dedsNavigationInfo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>



struct sNavInfoNatDat{
	dedsNavigationInfo *navinfo;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassNavigationInfo::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsNavInfo,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigationInfo::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sNavInfoNatDat &nd = *( ( sNavInfoNatDat* )p_GetNativeData( myself ) );
	const deClassNavigationInfo &clsNavInfo = *( ( deClassNavigationInfo* )GetOwnerClass() );
	
	// clear ( important )
	nd.navinfo = NULL;
	
	// create navigation information
	nd.navinfo = new dedsNavigationInfo( clsNavInfo.GetDS() );
}

// public func new( NavigationInfo copy )
deClassNavigationInfo::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsNavInfo,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavInfo ); // copy
}
void deClassNavigationInfo::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sNavInfoNatDat &nd = *( ( sNavInfoNatDat* )p_GetNativeData( myself ) );
	deClassNavigationInfo &clsNavInfo = *( ( deClassNavigationInfo* )GetOwnerClass() );
	
	// clear ( important )
	nd.navinfo = NULL;
	
	// create navigation information
	const dedsNavigationInfo * const copy = clsNavInfo.GetNavigationInfo( rt->GetValue( 0 )->GetRealObject() );
	if( ! copy ){
		DSTHROW( dueNullPointer );
	}
	
	nd.navinfo = new dedsNavigationInfo( *copy );
}

// public func destructor()
deClassNavigationInfo::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsNavInfo,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigationInfo::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sNavInfoNatDat &nd = *( ( sNavInfoNatDat* )p_GetNativeData( myself ) );
	
	if( nd.navinfo ){
		nd.navinfo->FreeReference();
		nd.navinfo = NULL;
	}
}



// Management
///////////////

// public func DVector getPosition()
deClassNavigationInfo::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsNavInfo,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector ){
}
void deClassNavigationInfo::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	const deScriptingDragonScript &ds = ( ( deClassNavigationInfo* )GetOwnerClass() )->GetDS();
	
	ds.GetClassDVector()->PushDVector( rt, navinfo.GetPosition() );
}

// public func void setPosition( DVector position )
deClassNavigationInfo::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsNavInfo,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVector ); // position
}
void deClassNavigationInfo::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	const deScriptingDragonScript &ds = ( ( deClassNavigationInfo* )GetOwnerClass() )->GetDS();
	
	const decDVector &position = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	navinfo.SetPosition( position );
}

// public func int getCostType()
deClassNavigationInfo::nfGetCostType::nfGetCostType( const sInitData &init ) : dsFunction( init.clsNavInfo,
"getCostType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassNavigationInfo::nfGetCostType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	
	rt->PushInt( navinfo.GetCostType() );
}

// public func void setCostType( int costType )
deClassNavigationInfo::nfSetCostType::nfSetCostType( const sInitData &init ) : dsFunction( init.clsNavInfo,
"setCostType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // costType
}
void deClassNavigationInfo::nfSetCostType::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	
	navinfo.SetCostType( rt->GetValue( 0 )->GetInt() );
}



// public func int getPathIndex()
deClassNavigationInfo::nfGetPathIndex::nfGetPathIndex( const sInitData &init ) : dsFunction( init.clsNavInfo,
"getPathIndex", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassNavigationInfo::nfGetPathIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	
	rt->PushInt( navinfo.GetPathIndex() );
}

// public func void setPathIndex( int index )
deClassNavigationInfo::nfSetPathIndex::nfSetPathIndex( const sInitData &init ) : dsFunction( init.clsNavInfo,
"setPathIndex", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassNavigationInfo::nfSetPathIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	
	navinfo.SetPathIndex( rt->GetValue( 0 )->GetInt() );
}

// public func float getPathFactor()
deClassNavigationInfo::nfGetPathFactor::nfGetPathFactor( const sInitData &init ) : dsFunction( init.clsNavInfo,
"getPathFactor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassNavigationInfo::nfGetPathFactor::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	
	rt->PushFloat( navinfo.GetPathFactor() );
}

// public func void setPathFactor( float factor )
deClassNavigationInfo::nfSetPathFactor::nfSetPathFactor( const sInitData &init ) : dsFunction( init.clsNavInfo,
"setPathFactor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // factor
}
void deClassNavigationInfo::nfSetPathFactor::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsNavigationInfo &navinfo = *( ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo );
	
	navinfo.SetPathFactor( rt->GetValue( 0 )->GetFloat() );
}



// public func int hashCode()
deClassNavigationInfo::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsNavInfo, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassNavigationInfo::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsNavigationInfo * const navinfo = ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo;
	
	rt->PushInt( ( int )( intptr_t )navinfo );
}

// public func bool equals( Object object )
deClassNavigationInfo::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsNavInfo, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // object
}
void deClassNavigationInfo::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsNavigationInfo * const navinfo = ( ( const sNavInfoNatDat * )p_GetNativeData( myself ) )->navinfo;
	deClassNavigationInfo * const clsNavInfo = ( deClassNavigationInfo* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsNavInfo ) ){
		rt->PushBool( false );
		
	}else{
		const dedsNavigationInfo * const otherNavInfo = ( ( const sNavInfoNatDat * )p_GetNativeData( object ) )->navinfo;
		rt->PushBool( navinfo == otherNavInfo );
	}
}



// Class deClassNavigationInfo
////////////////////////////////

// Constructor
////////////////

deClassNavigationInfo::deClassNavigationInfo( deScriptingDragonScript &ds ) :
dsClass( "NavigationInfo", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sNavInfoNatDat ) );
}

deClassNavigationInfo::~deClassNavigationInfo(){
}



// Management
///////////////

void deClassNavigationInfo::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsNavInfo = this;
	
	init.clsBool = engine->GetClassBool();
	init.clsDVector = pDS.GetClassDVector();
	init.clsFloat = engine->GetClassFloat();
	init.clsInteger = engine->GetClassInt();
	init.clsObject = engine->GetClassObject();
	init.clsString = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetCostType( init ) );
	AddFunction( new nfSetCostType( init ) );
	
	AddFunction( new nfGetPathIndex( init ) );
	AddFunction( new nfSetPathIndex( init ) );
	AddFunction( new nfGetPathFactor( init ) );
	AddFunction( new nfSetPathFactor( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

dedsNavigationInfo *deClassNavigationInfo::GetNavigationInfo( dsRealObject *myself ) const{
	if( myself ){
		return ( ( const sNavInfoNatDat * )p_GetNativeData( myself->GetBuffer() ) )->navinfo;
		
	}else{
		return NULL;
	}
}

void deClassNavigationInfo::PushNavigationInfo( dsRunTime *rt, dedsNavigationInfo *info ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! info ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sNavInfoNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->navinfo = info;
	info->AddReference();
}
