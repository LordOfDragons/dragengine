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

#include "deClassOcclusionMesh.h"
#include "../resources/deClassResourceListener.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshManager.h>
#include <libdscript/exceptions.h>
#include <dragengine/deEngine.h>



// Native Structure
/////////////////////

struct sOccMNatDat{
	deOcclusionMesh *occlusionMesh;
};



// Native Functions
/////////////////////

// public func new( String filename )
deClassOcclusionMesh::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsOccM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
}
void deClassOcclusionMesh::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sOccMNatDat &nd = *( ( sOccMNatDat* )p_GetNativeData( myself ) );
	deClassOcclusionMesh &clsOccM = *( ( deClassOcclusionMesh* )GetOwnerClass() );
	deOcclusionMeshManager &occmgr = *clsOccM.GetDS()->GetGameEngine()->GetOcclusionMeshManager();
	
	const char *filename = rt->GetValue( 0 )->GetString();
	
	// clear ( important )
	nd.occlusionMesh = NULL;
	
	// load occlusion mesh
	nd.occlusionMesh = occmgr.LoadOcclusionMesh( filename, "/" );
	if( ! nd.occlusionMesh ){
		DSTHROW( dueOutOfMemory );
	}
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassOcclusionMesh::nfLoadAsynchron::nfLoadAsynchron( const sInitData &init ) : dsFunction( init.clsOccM,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
	p_AddParameter( init.clsRN ); // listener
}
void deClassOcclusionMesh::nfLoadAsynchron::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMesh &clsOccM = *( ( deClassOcclusionMesh* )GetOwnerClass() );
	
	const char *filename = rt->GetValue( 0 )->GetString();
	dsRealObject * const listener = rt->GetValue( 1 )->GetRealObject();
	
	if( ! listener ){
		DSTHROW( dueInvalidParam );
	}
	
	clsOccM.GetDS()->GetResourceLoader()->AddRequest( filename,
		deResourceLoader::ertOcclusionMesh, listener );
}

// public func destructor()
deClassOcclusionMesh::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsOccM,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassOcclusionMesh::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sOccMNatDat &nd = *( ( sOccMNatDat* )p_GetNativeData( myself ) );
	
	if( nd.occlusionMesh ){
		nd.occlusionMesh->FreeReference();
		nd.occlusionMesh = NULL;
	}
}



// public func String getFilename()
deClassOcclusionMesh::nfGetFilename::nfGetFilename( const sInitData &init ) : dsFunction( init.clsOccM,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassOcclusionMesh::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deOcclusionMesh &occlusionMesh = *( ( ( sOccMNatDat* )p_GetNativeData( myself ) )->occlusionMesh );
	
	rt->PushString( occlusionMesh.GetFilename() );
}



// public func int hashCode()
deClassOcclusionMesh::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsOccM,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassOcclusionMesh::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deOcclusionMesh * const occlusionMesh = ( ( sOccMNatDat* )p_GetNativeData( myself ) )->occlusionMesh;
	
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )occlusionMesh );
}

// public func bool equals( Object obj )
deClassOcclusionMesh::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsOccM,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassOcclusionMesh::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deOcclusionMesh * const occlusionMesh = ( ( sOccMNatDat* )p_GetNativeData( myself ) )->occlusionMesh;
	deClassOcclusionMesh * const clsOccM = ( deClassOcclusionMesh* )GetOwnerClass();
	
	dsValue *object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsOccM ) ){
		rt->PushBool( false );
		
	}else{
		const deOcclusionMesh * const otherOcclusionMesh = ( ( sOccMNatDat* )p_GetNativeData( object ) )->occlusionMesh;
		
		rt->PushBool( occlusionMesh == otherOcclusionMesh );
	}
}

// public static func bool equals( OcclusionMesh occlusionMesh1, OcclusionMesh occlusionMesh2 )
deClassOcclusionMesh::nfEquals2::nfEquals2( const sInitData &init ) :
dsFunction( init.clsOccM, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsOccM ); // occlusionMesh1
	p_AddParameter( init.clsOccM ); // occlusionMesh2
}
void deClassOcclusionMesh::nfEquals2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMesh &clsOccM = *( ( deClassOcclusionMesh* )GetOwnerClass() );
	
	const deOcclusionMesh * const occlusionMesh1 = clsOccM.GetOcclusionMesh( rt->GetValue( 0 )->GetRealObject() );
	const deOcclusionMesh * const occlusionMesh2 = clsOccM.GetOcclusionMesh( rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushBool( occlusionMesh1 == occlusionMesh2 );
}



// Class deClassOcclusionMesh
///////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassOcclusionMesh::deClassOcclusionMesh( deScriptingDragonScript *ds ) :
dsClass( "OcclusionMesh", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sOccMNatDat ) );
}

deClassOcclusionMesh::~deClassOcclusionMesh(){
}



// Management
///////////////

void deClassOcclusionMesh::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsOccM = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsRN = pDS->GetClassResourceListener();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfLoadAsynchron( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetFilename( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfEquals2( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deOcclusionMesh *deClassOcclusionMesh::GetOcclusionMesh( dsRealObject *object ) const{
	if( ! object ){
		return NULL;
	}
	
	return ( ( sOccMNatDat* )p_GetNativeData( object->GetBuffer() ) )->occlusionMesh;
}

void deClassOcclusionMesh::PushOcclusionMesh( dsRunTime *rt, deOcclusionMesh *occlusionMesh ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! occlusionMesh ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sOccMNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->occlusionMesh = occlusionMesh;
	occlusionMesh->AddReference();
}
