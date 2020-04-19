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

#include "deClassModel.h"
#include "../resources/deClassResourceListener.h"
#include "../math/deClassVector.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <libdscript/exceptions.h>


struct sMdlNatDat{
	deModel *model;
};



// Native Functions
/////////////////////

// public constructor Load(string filename)
deClassModel::nfLoad::nfLoad( const sInitData &init ) : dsFunction( init.clsMdl,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
}
void deClassModel::nfLoad::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMdlNatDat &nd = *( ( sMdlNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = *( ( deClassModel* )GetOwnerClass() )->GetDS();
	deModelManager &mdlMgr = *ds.GetGameEngine()->GetModelManager();
	
	// prepare
	nd.model = NULL;
	
	// load model
	const char * const filename = rt->GetValue( 0 )->GetString();
	nd.model = mdlMgr.LoadModel( filename, "/" );
	if( ! nd.model ){
		DSTHROW( dueInvalidParam );
	}
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassModel::nfLoadAsynchron::nfLoadAsynchron( const sInitData &init ) : dsFunction( init.clsMdl,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
	p_AddParameter( init.clsRN ); // listener
}
void deClassModel::nfLoadAsynchron::RunFunction( dsRunTime *rt, dsValue *myself ){
	deScriptingDragonScript &ds = *( ( deClassModel* )GetOwnerClass() )->GetDS();
	
	const char * const filename = rt->GetValue( 0 )->GetString();
	dsRealObject * const listener = rt->GetValue( 1 )->GetRealObject();
	
	if( ! listener ){
		DSTHROW( dueNullPointer );
	}
	
	ds.GetResourceLoader()->AddRequest( filename, deResourceLoader::ertModel, listener );
}

// public destructor Destructor()
deClassModel::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsMdl,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassModel::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sMdlNatDat &nd = *( ( sMdlNatDat* )p_GetNativeData( myself ) );
	
	if( nd.model ){
		nd.model->FreeReference();
		nd.model = NULL;
	}
}



// public func string GetFilename()
deClassModel::nfGetFilename::nfGetFilename( const sInitData &init ) : dsFunction( init.clsMdl,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassModel::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModel &model = *( ( sMdlNatDat* )p_GetNativeData( myself ) )->model;
	
	rt->PushString( model.GetFilename() );
}



// public func int getLodCount()
deClassModel::nfGetLodCount::nfGetLodCount( const sInitData &init ) : dsFunction( init.clsMdl,
"getLodCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassModel::nfGetLodCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModel &model = *( ( sMdlNatDat* )p_GetNativeData( myself ) )->model;
	
	rt->PushInt( model.GetLODCount() );
}

// public func int getTextureCount()
deClassModel::nfGetTextureCount::nfGetTextureCount( const sInitData &init ) : dsFunction( init.clsMdl,
"getTextureCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassModel::nfGetTextureCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModel &model = *( ( sMdlNatDat* )p_GetNativeData( myself ) )->model;
	
	rt->PushInt( model.GetTextureCount() );
}

// public func String getTextureNameAt( int texture )
deClassModel::nfGetTextureNameAt::nfGetTextureNameAt( const sInitData &init ) : dsFunction( init.clsMdl,
"getTextureNameAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // texture
}
void deClassModel::nfGetTextureNameAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModel &model = *( ( sMdlNatDat* )p_GetNativeData( myself ) )->model;
	const int texture = rt->GetValue( 0 )->GetInt();
	
	rt->PushString( model.GetTextureAt( texture )->GetName() );
}

// public func int getFaceCount( int lod )
deClassModel::nfGetFaceCount::nfGetFaceCount( const sInitData &init ) : dsFunction( init.clsMdl,
"getFaceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // lod
}
void deClassModel::nfGetFaceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModel &model = *( ( sMdlNatDat* )p_GetNativeData( myself ) )->model;
	const int lod = rt->GetValue( 0 )->GetInt();
	
	rt->PushInt( model.GetLODAt( lod )->GetFaceCount() );
}

// public func int getVertexCount( int lod )
deClassModel::nfGetVertexCount::nfGetVertexCount( const sInitData &init ) : dsFunction( init.clsMdl,
"getVertexCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // lod
}
void deClassModel::nfGetVertexCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModel &model = *( ( sMdlNatDat* )p_GetNativeData( myself ) )->model;
	const int lod = rt->GetValue( 0 )->GetInt();
	
	rt->PushInt( model.GetLODAt( lod )->GetVertexCount() );
}



// public func int hashCode()
deClassModel::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsMdl, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassModel::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMdlNatDat &nd = *( ( sMdlNatDat* )p_GetNativeData( myself ) );
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )nd.model );
}

// public func bool equals( Object obj )
deClassModel::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsMdl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassModel::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sMdlNatDat &nd = *( ( sMdlNatDat* )p_GetNativeData( myself ) );
	deClassModel * const clsModel = ( deClassModel* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( obj->GetType()->GetPrimitiveType() != DSPT_OBJECT || ! obj->GetRealObject()
	|| obj->GetRealObject()->GetType() != clsModel ){
		rt->PushBool( false );
		
	}else{
		const sMdlNatDat &other = *( ( sMdlNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.model == other.model );
	}
}




// Class deClassModel
///////////////////////

// Constructor, destructor
////////////////////////////

deClassModel::deClassModel( deScriptingDragonScript *ds ) : dsClass( "Model",
DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMdlNatDat ) );
}

deClassModel::~deClassModel(){
}



// Management
///////////////

void deClassModel::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsMdl = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS->GetClassVector();
	init.clsRN = pDS->GetClassResourceListener();
	
	// add functions
	AddFunction( new nfLoad( init ) );
	AddFunction( new nfLoadAsynchron( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetFilename( init ) );
	
	AddFunction( new nfGetLodCount( init ) );
	AddFunction( new nfGetTextureCount( init ) );
	AddFunction( new nfGetTextureNameAt( init ) );
	AddFunction( new nfGetFaceCount( init ) );
	AddFunction( new nfGetVertexCount( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
}

deModel *deClassModel::GetModel( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sMdlNatDat* )p_GetNativeData( myself->GetBuffer() ) )->model;
}

void deClassModel::PushModel( dsRunTime *rt, deModel *model ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! model ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sMdlNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->model = model;
	model->AddReference();
}
