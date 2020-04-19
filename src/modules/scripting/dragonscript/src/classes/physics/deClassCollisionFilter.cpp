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

#include "deClassCollisionFilter.h"
#include "deClassLayerMask.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/utils/decCollisionFilter.h>

#include <libdscript/exceptions.h>



struct sCFNatDat{
	decCollisionFilter *layerMask;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassCollisionFilter::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCF,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCollisionFilter::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCFNatDat &nd = *( ( sCFNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.layerMask = NULL;
	
	// create layer mask
	nd.layerMask = new decCollisionFilter;
}

// public func new( CollisionFilter collisionFilter )
deClassCollisionFilter::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsCF,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassCollisionFilter::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCFNatDat &nd = *( ( sCFNatDat* )p_GetNativeData( myself ) );
	deClassCollisionFilter &clsCF = *( ( deClassCollisionFilter* )GetOwnerClass() );
	
	// clear ( important )
	nd.layerMask = NULL;
	
	// create layer mask
	const decCollisionFilter &collisionFilter = clsCF.GetCollisionFilter( rt->GetValue( 0 )->GetRealObject() );
	nd.layerMask = new decCollisionFilter( collisionFilter );
}

// public func new( LayerMask layerMask )
deClassCollisionFilter::nfNewMask::nfNewMask( const sInitData &init ) : dsFunction( init.clsCF,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLyM ); // layerMask
}
void deClassCollisionFilter::nfNewMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCFNatDat &nd = *( ( sCFNatDat* )p_GetNativeData( myself ) );
	deClassLayerMask &clsLyM = *( ( ( deClassCollisionFilter* )GetOwnerClass() )->GetDS()->GetClassLayerMask() );
	
	// clear ( important )
	nd.layerMask = NULL;
	
	// create layer mask
	const decLayerMask &layerMask = clsLyM.GetLayerMask( rt->GetValue( 0 )->GetRealObject() );
	nd.layerMask = new decCollisionFilter( layerMask );
}

// public func new( LayerMask category, LayerMask filter )
deClassCollisionFilter::nfNewCategoryFilter::nfNewCategoryFilter( const sInitData &init ) : dsFunction( init.clsCF,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLyM ); // category
	p_AddParameter( init.clsLyM ); // filter
}
void deClassCollisionFilter::nfNewCategoryFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCFNatDat &nd = *( ( sCFNatDat* )p_GetNativeData( myself ) );
	deClassLayerMask &clsLyM = *( ( ( deClassCollisionFilter* )GetOwnerClass() )->GetDS()->GetClassLayerMask() );
	
	// clear ( important )
	nd.layerMask = NULL;
	
	// create layer mask
	const decLayerMask &category = clsLyM.GetLayerMask( rt->GetValue( 0 )->GetRealObject() );
	const decLayerMask &filter = clsLyM.GetLayerMask( rt->GetValue( 1 )->GetRealObject() );
	nd.layerMask = new decCollisionFilter( category, filter );
}

// public func destructor()
deClassCollisionFilter::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCF,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCollisionFilter::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCFNatDat &nd = *( ( sCFNatDat* )p_GetNativeData( myself ) );
	
	if( nd.layerMask ){
		delete nd.layerMask;
		nd.layerMask = NULL;
	}
}



// Management
///////////////

// public func LayerMask getCategory()
deClassCollisionFilter::nfGetCategory::nfGetCategory( const sInitData &init ) : dsFunction( init.clsCF,
"getCategory", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM ){
}
void deClassCollisionFilter::nfGetCategory::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
	deClassLayerMask &clsLyM = *( ( ( deClassCollisionFilter* )GetOwnerClass() )->GetDS()->GetClassLayerMask() );
	
	clsLyM.PushLayerMask( rt, collisionFilter.GetCategory() );
}

// public func LayerMask getFilter()
deClassCollisionFilter::nfGetFilter::nfGetFilter( const sInitData &init ) : dsFunction( init.clsCF,
"getFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM ){
}
void deClassCollisionFilter::nfGetFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
	deClassLayerMask &clsLyM = *( ( ( deClassCollisionFilter* )GetOwnerClass() )->GetDS()->GetClassLayerMask() );
	
	clsLyM.PushLayerMask( rt, collisionFilter.GetFilter() );
}

// public func bool collides( CollisionFilter collisionFilter )
deClassCollisionFilter::nfCollides::nfCollides( const sInitData &init ) : dsFunction( init.clsCF,
"collides", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassCollisionFilter::nfCollides::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
	deClassCollisionFilter &clsCF = *( ( deClassCollisionFilter* )GetOwnerClass() );
	
	const decCollisionFilter &other = clsCF.GetCollisionFilter( rt->GetValue( 0 )->GetRealObject() );
	rt->PushBool( collisionFilter.Collides( other ) );
}

// public func bool collidesNot( CollisionFilter collisionFilter )
deClassCollisionFilter::nfCollidesNot::nfCollidesNot( const sInitData &init ) : dsFunction( init.clsCF,
"collidesNot", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassCollisionFilter::nfCollidesNot::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
	deClassCollisionFilter &clsCF = *( ( deClassCollisionFilter* )GetOwnerClass() );
	
	const decCollisionFilter &other = clsCF.GetCollisionFilter( rt->GetValue( 0 )->GetRealObject() );
	rt->PushBool( collisionFilter.CollidesNot( other ) );
}



// File Handling
//////////////////

// static public func ShapeList readFromFile( FileReader reader )
deClassCollisionFilter::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsCF,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCF ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassCollisionFilter::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassCollisionFilter &clsCF = *( ( deClassCollisionFilter* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsCF.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	decCollisionFilter filter;
	filter.GetCategory() = decLayerMask::ReadFromFile( *reader );
	filter.GetFilter() = decLayerMask::ReadFromFile( *reader );
	
	clsCF.PushCollisionFilter( rt, filter );
}

// public func void writeToFile( FileWriter writer )
deClassCollisionFilter::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsCF,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassCollisionFilter::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
	deClassCollisionFilter &clsCF = *( ( deClassCollisionFilter* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsCF.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	collisionFilter.GetCategory().WriteToFile( *writer );
	collisionFilter.GetFilter().WriteToFile( *writer );
}



// Special
////////////

// public func int hashCode()
deClassCollisionFilter::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCF, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCollisionFilter::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter * const collisionFilter = ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask;
	rt->PushInt( ( intptr_t )collisionFilter );
}

// public func bool equals( Object object )
deClassCollisionFilter::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCF, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassCollisionFilter::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassCollisionFilter * const clsCF = ( deClassCollisionFilter* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsCF ) ){
		rt->PushBool( false );
		
	}else{
		const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
		const decCollisionFilter &other = *( ( ( sCFNatDat* )p_GetNativeData( object ) )->layerMask );
		rt->PushBool( collisionFilter == other );
	}
}

// public func String toString()
deClassCollisionFilter::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsCF,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassCollisionFilter::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decCollisionFilter &collisionFilter = *( ( ( sCFNatDat* )p_GetNativeData( myself ) )->layerMask );
	
	decString text;
	text += "[";
	text += collisionFilter.GetCategory().ToHexString();
	text += ",";
	text += collisionFilter.GetFilter().ToHexString();
	text += "]";
	
	rt->PushString( text );
}



// Class deClassCollisionFilter
/////////////////////////////////

// Constructor
////////////////

deClassCollisionFilter::deClassCollisionFilter( deScriptingDragonScript *ds ) :
dsClass( "CollisionFilter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sCFNatDat ) );
}

deClassCollisionFilter::~deClassCollisionFilter(){
}



// Management
///////////////

void deClassCollisionFilter::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCF = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsLyM = pDS->GetClassLayerMask();
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfNewMask( init ) );
	AddFunction( new nfNewCategoryFilter( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCategory( init ) );
	AddFunction( new nfGetFilter( init ) );
	
	AddFunction( new nfCollides( init ) );
	AddFunction( new nfCollidesNot( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

const decCollisionFilter &deClassCollisionFilter::GetCollisionFilter( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return *( ( ( sCFNatDat* )p_GetNativeData( myself->GetBuffer() ) )->layerMask );
}

void deClassCollisionFilter::PushCollisionFilter( dsRunTime *rt, const decCollisionFilter &collisionFilter ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sCFNatDat &nd = *( ( sCFNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.layerMask = NULL;
	
	try{
		nd.layerMask = new decCollisionFilter( collisionFilter );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
