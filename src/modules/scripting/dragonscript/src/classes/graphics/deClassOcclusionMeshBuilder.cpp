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

#include "deClassOcclusionMesh.h"
#include "deClassOcclusionMeshBuilder.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassPoint.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshBone.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshReference.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshBuilder.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshManager.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshWeight.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshVertex.h>


class deClassOcclusionMeshBuilder_Builder : public deOcclusionMeshBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deOcclusionMesh *pOcclusionMesh;
	
public:
	deClassOcclusionMeshBuilder_Builder( dsRunTime *rt, dsValue *myself ) :
	pRT( rt ), pMyself( myself ), pOcclusionMesh( NULL ){
	}
	
	virtual void BuildOcclusionMesh( deOcclusionMesh *occlusionMesh ){
		pOcclusionMesh = occlusionMesh;
		
		try{
			pRT->RunFunction( pMyself, "buildOcclusionMesh", 0 );
			
		}catch( const duException &e ){
			pOcclusionMesh = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW( deeInvalidParam );
			
		}catch( ... ){
			pOcclusionMesh = NULL;
			throw;
		}
		
		pOcclusionMesh = NULL;
	}
	
	inline deOcclusionMesh *GetOcclusionMesh() const{ return pOcclusionMesh; }
};


struct sMdlBldNatDat{
	deClassOcclusionMeshBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassOcclusionMeshBuilder::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassOcclusionMeshBuilder::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder = NULL;
}

// public destructor Destructor()
deClassOcclusionMeshBuilder::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassOcclusionMeshBuilder::nfDestructor::RunFunction( dsRunTime*, dsValue* ){
}



// public func OcclusionMesh build( String filename )
deClassOcclusionMeshBuilder::nfBuild::nfBuild( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsOcclusionMesh ){
	p_AddParameter( init.clsString ); // filename
}
void deClassOcclusionMeshBuilder::nfBuild::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMdlBldNatDat &nd = *( ( sMdlBldNatDat* )p_GetNativeData( myself ) );
	if( nd.builder ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassOcclusionMeshBuilder* )GetOwnerClass() )->GetDS();
	const char * const filename = rt->GetValue( 0 )->GetString();
	deClassOcclusionMeshBuilder_Builder builder( rt, myself );
	deOcclusionMeshReference occlusionMesh;
	
	nd.builder = &builder;
	
	try{
		occlusionMesh.TakeOver( ds.GetGameEngine()->GetOcclusionMeshManager()->CreateOcclusionMesh( filename, builder ) );
		
	}catch( ... ){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassOcclusionMesh()->PushOcclusionMesh( rt, occlusionMesh );
}



// abstract protected func void buildOcclusionMesh()
deClassOcclusionMeshBuilder::nfBuildOcclusionMesh::nfBuildOcclusionMesh( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "buildOcclusionMesh", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid ){
}
void deClassOcclusionMeshBuilder::nfBuildOcclusionMesh::RunFunction( dsRunTime*, dsValue* ){
}



// protected func void setBoneCount( int count )
deClassOcclusionMeshBuilder::nfSetBoneCount::nfSetBoneCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setBoneCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetBoneCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetBoneCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setBoneAt( int index, String name, int parent, Vector position, Quaternion orientation )
deClassOcclusionMeshBuilder::nfSetBoneAt::nfSetBoneAt( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setBoneAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsInteger ); // parent
	p_AddParameter( init.clsVector ); // position
	p_AddParameter( init.clsQuaternion ); // orientation
}
void deClassOcclusionMeshBuilder::nfSetBoneAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassOcclusionMeshBuilder* )GetOwnerClass() )->GetDS();
	deOcclusionMeshBone &bone = builder->GetOcclusionMesh()->GetBoneAt( rt->GetValue( 0 )->GetInt() );
	bone.SetName( rt->GetValue( 1 )->GetString() );
	bone.SetParent( rt->GetValue( 2 )->GetInt() );
	bone.SetPosition( ds.GetClassVector()->GetVector( rt->GetValue( 3 )->GetRealObject() ) );
	bone.SetOrientation( ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 4 )->GetRealObject() ) );
}

// protected func void setWeightCount( int count )
deClassOcclusionMeshBuilder::nfSetWeightCount::nfSetWeightCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setWeightCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetWeightCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetWeightCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setWeightAt( int index, int bone, float weight )
deClassOcclusionMeshBuilder::nfSetWeightAt::nfSetWeightAt( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setWeightAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // bone
	p_AddParameter( init.clsFloat ); // weight
}
void deClassOcclusionMeshBuilder::nfSetWeightAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	deOcclusionMeshWeight &weight = builder->GetOcclusionMesh()->GetWeightAt( rt->GetValue( 0 )->GetInt() );
	weight.SetBone( rt->GetValue( 1 )->GetInt() );
	weight.SetWeight( rt->GetValue( 2 )->GetFloat() );
}

// protected func void setWeightGroupCount( int count )
deClassOcclusionMeshBuilder::nfSetWeightGroupCount::nfSetWeightGroupCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setWeightGroupCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetWeightGroupCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetWeightGroupCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setWeightGroupAt( int index, int count )
deClassOcclusionMeshBuilder::nfSetWeightGroupAt::nfSetWeightGroupAt( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setWeightGroupAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetWeightGroupAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetWeightGroupAt( rt->GetValue( 0 )->GetInt(), rt->GetValue( 1 )->GetInt() );
}

// protected func void setVertexCount( int count )
deClassOcclusionMeshBuilder::nfSetVertexCount::nfSetVertexCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setVertexCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetVertexCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetVertexCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setVertexAt( int index, Vector position, int weightSet )
deClassOcclusionMeshBuilder::nfSetVertexAt::nfSetVertexAt( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setVertexAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsVector ); // position
	p_AddParameter( init.clsInteger ); // weightSet
}
void deClassOcclusionMeshBuilder::nfSetVertexAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassOcclusionMeshBuilder* )GetOwnerClass() )->GetDS();
	deOcclusionMeshVertex &vertex = builder->GetOcclusionMesh()->GetVertexAt( rt->GetValue( 0 )->GetInt() );
	vertex.SetPosition( ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() ) );
	vertex.SetWeightSet( rt->GetValue( 2 )->GetInt() );
}

// protected func void setCornerCount( int count )
deClassOcclusionMeshBuilder::nfSetCornerCount::nfSetCornerCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setCornerCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetCornerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetCornerCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setCornerAt( int index, int vertex )
deClassOcclusionMeshBuilder::nfSetCornerAt::nfSetCornerAt( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setCornerAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // vertex
}
void deClassOcclusionMeshBuilder::nfSetCornerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetCornerAt( rt->GetValue( 0 )->GetInt(), rt->GetValue( 1 )->GetInt() );
}

// protected func void setFaceCount( int count )
deClassOcclusionMeshBuilder::nfSetFaceCount::nfSetFaceCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setFaceCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassOcclusionMeshBuilder::nfSetFaceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetFaceCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setFaceAt( int index, int cornerCount )
deClassOcclusionMeshBuilder::nfSetFaceAt::nfSetFaceAt( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setFaceAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // cornerCount
}
void deClassOcclusionMeshBuilder::nfSetFaceAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetFaceAt( rt->GetValue( 0 )->GetInt(), rt->GetValue( 1 )->GetInt() );
}

// protected func void setDoubleSidedFaceCount( int doubleSidedFaceCount )
deClassOcclusionMeshBuilder::nfSetDoubleSidedFaceCount::nfSetDoubleSidedFaceCount( const sInitData &init ) :
dsFunction( init.clsOcclusionMeshBuilder, "setDoubleSidedFaceCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // doubleSidedFaceCount
}
void deClassOcclusionMeshBuilder::nfSetDoubleSidedFaceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassOcclusionMeshBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetOcclusionMesh() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetOcclusionMesh()->SetDoubleSidedFaceCount( rt->GetValue( 0 )->GetInt() );
}



// Class deClassOcclusionMeshBuilder
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassOcclusionMeshBuilder::deClassOcclusionMeshBuilder( deScriptingDragonScript &ds ) :
dsClass( "OcclusionMeshBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMdlBldNatDat ) );
}

deClassOcclusionMeshBuilder::~deClassOcclusionMeshBuilder(){
}



// Management
///////////////

void deClassOcclusionMeshBuilder::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsOcclusionMeshBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsOcclusionMesh = pDS.GetClassOcclusionMesh();
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsPoint = pDS.GetClassPoint();
	init.clsVector2 = pDS.GetClassVector2();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfBuild( init ) );
	AddFunction( new nfBuildOcclusionMesh( init ) );
	AddFunction( new nfSetBoneCount( init ) );
	AddFunction( new nfSetBoneAt( init ) );
	AddFunction( new nfSetWeightCount( init ) );
	AddFunction( new nfSetWeightAt( init ) );
	AddFunction( new nfSetWeightGroupCount( init ) );
	AddFunction( new nfSetWeightGroupAt( init ) );
	AddFunction( new nfSetVertexCount( init ) );
	AddFunction( new nfSetVertexAt( init ) );
	AddFunction( new nfSetCornerCount( init ) );
	AddFunction( new nfSetCornerAt( init ) );
	AddFunction( new nfSetFaceCount( init ) );
	AddFunction( new nfSetFaceAt( init ) );
	AddFunction( new nfSetDoubleSidedFaceCount( init ) );
}
