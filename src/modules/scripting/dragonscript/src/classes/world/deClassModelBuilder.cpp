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

#include "deClassModel.h"
#include "deClassModelBuilder.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassPoint.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelReference.h>
#include <dragengine/resources/model/deModelBuilder.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>


class deClassModelBuilder_Builder : public deModelBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deModel *pModel;
	
public:
	deClassModelBuilder_Builder( dsRunTime *rt, dsValue *myself ) :
	pRT( rt ), pMyself( myself ), pModel( NULL ){
	}
	
	virtual void BuildModel( deModel *model ){
		pModel = model;
		
		try{
			pRT->RunFunction( pMyself, "buildModel", 0 );
			
		}catch( const duException &e ){
			pModel = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW( deeInvalidParam );
			
		}catch( ... ){
			pModel = NULL;
			throw;
		}
		
		pModel = NULL;
	}
	
	inline deModel *GetModel() const{ return pModel; }
};


struct sMdlBldNatDat{
	deClassModelBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassModelBuilder::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsModelBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassModelBuilder::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder = NULL;
}

// public destructor Destructor()
deClassModelBuilder::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsModelBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassModelBuilder::nfDestructor::RunFunction( dsRunTime*, dsValue* ){
}



// public func Model build( String filename )
deClassModelBuilder::nfBuild::nfBuild( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModel ){
	p_AddParameter( init.clsString ); // filename
}
void deClassModelBuilder::nfBuild::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMdlBldNatDat &nd = *( ( sMdlBldNatDat* )p_GetNativeData( myself ) );
	if( nd.builder ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassModelBuilder* )GetOwnerClass() )->GetDS();
	const char * const filename = rt->GetValue( 0 )->GetString();
	deClassModelBuilder_Builder builder( rt, myself );
	deModelReference model;
	
	nd.builder = &builder;
	
	try{
		model.TakeOver( ds.GetGameEngine()->GetModelManager()->CreateModel( filename, builder ) );
		
	}catch( ... ){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassModel()->PushModel( rt, model );
}



// abstract protected func void buildModel()
deClassModelBuilder::nfBuildModel::nfBuildModel( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "buildModel", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid ){
}
void deClassModelBuilder::nfBuildModel::RunFunction( dsRunTime*, dsValue* ){
}



// protected func void addBone( String name, int parent, Vector position, Quaternion orientation )
deClassModelBuilder::nfAddBone::nfAddBone( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "addBone", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsInteger ); // parent
	p_AddParameter( init.clsVector ); // position
	p_AddParameter( init.clsQuaternion ); // orientation
}
void deClassModelBuilder::nfAddBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassModelBuilder* )GetOwnerClass() )->GetDS();
	const char * const name = rt->GetValue( 0 )->GetString();
	const int parent = rt->GetValue( 1 )->GetInt();
	const decVector &position = ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() );
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 3 )->GetRealObject() );
	
	deModelBone * const bone = new deModelBone( name );
	try{
		bone->SetParent( parent );
		bone->SetPosition( position );
		bone->SetOrientation( orientation );
		builder->GetModel()->AddBone( bone );
		
	}catch( ... ){
		delete bone;
		throw;
	}
}

// protected func void addTexture( String name, Point size, bool doubleSided, bool decal, int decalOffset )
deClassModelBuilder::nfAddTexture::nfAddTexture( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "addTexture", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsPoint ); // size
	p_AddParameter( init.clsBoolean ); // doubleSided
	p_AddParameter( init.clsBoolean ); // decal
	p_AddParameter( init.clsInteger ); // decalOffset
}
void deClassModelBuilder::nfAddTexture::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassModelBuilder* )GetOwnerClass() )->GetDS();
	const char * const name = rt->GetValue( 0 )->GetString();
	const decPoint &size = ds.GetClassPoint()->GetPoint( rt->GetValue( 1 )->GetRealObject() );
	const bool doubleSided = rt->GetValue( 2 )->GetBool();
	const bool decal = rt->GetValue( 3 )->GetBool();
	const int decalOffset = rt->GetValue( 4 )->GetInt();
	
	deModelTexture * const texture = new deModelTexture( name, size.x, size.y );
	try{
		texture->SetDoubleSided( doubleSided );
		texture->SetDecal( decal );
		texture->SetDecalOffset( decalOffset );
		builder->GetModel()->AddTexture( texture );
		
	}catch( ... ){
		delete texture;
		throw;
	}
}

// protected func void addTextureCoordinatesSet( String name )
deClassModelBuilder::nfAddTextureCoordinatesSet::nfAddTextureCoordinatesSet( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "addTextureCoordinatesSet", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
}
void deClassModelBuilder::nfAddTextureCoordinatesSet::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetModel()->GetTextureCoordinatesSetList().Add( rt->GetValue( 0 )->GetString() );
}

// protected func void addLOD()
deClassModelBuilder::nfAddLOD::nfAddLOD( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "addLOD", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
}
void deClassModelBuilder::nfAddLOD::RunFunction( dsRunTime*, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD * const lod = new deModelLOD;
	try{
		builder->GetModel()->AddLOD( lod );
		
	}catch( ... ){
		delete lod;
		throw;
	}
}

// protected func void setLodError( int lod, float error )
deClassModelBuilder::nfSetLodError::nfSetLodError( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setLodError", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsFloat ); // error
}
void deClassModelBuilder::nfSetLodError::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetLodError( rt->GetValue( 1 )->GetFloat() );
	lod.SetHasLodError( true );
}

// protected func void setWeightCount( int lod, int count )
deClassModelBuilder::nfSetWeightCount::nfSetWeightCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setWeightCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetWeightCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetWeightCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setWeightAt( int lod, int index, int bone, float weight )
deClassModelBuilder::nfSetWeightAt::nfSetWeightAt( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setWeightAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // bone
	p_AddParameter( init.clsFloat ); // weight
}
void deClassModelBuilder::nfSetWeightAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	deModelWeight &weight = lod.GetWeightAt( rt->GetValue( 1 )->GetInt() );
	weight.SetBone( rt->GetValue( 2 )->GetInt() );
	weight.SetWeight( rt->GetValue( 3 )->GetFloat() );
}

// protected func void setWeightGroupCount( int lod, int count )
deClassModelBuilder::nfSetWeightGroupCount::nfSetWeightGroupCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setWeightGroupCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetWeightGroupCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetWeightGroupCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setWeightGroupAt( int lod, int index, int count )
deClassModelBuilder::nfSetWeightGroupAt::nfSetWeightGroupAt( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setWeightGroupAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetWeightGroupAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetWeightGroupAt( rt->GetValue( 1 )->GetInt(), rt->GetValue( 2 )->GetInt() );
}

// protected func void setVertexCount( int lod, int count )
deClassModelBuilder::nfSetVertexCount::nfSetVertexCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setVertexCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetVertexCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetVertexCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setVertexAt( int lod, int index, Vector position, int weightSet )
deClassModelBuilder::nfSetVertexAt::nfSetVertexAt( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setVertexAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsVector ); // position
	p_AddParameter( init.clsInteger ); // weightSet
}
void deClassModelBuilder::nfSetVertexAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassModelBuilder* )GetOwnerClass() )->GetDS();
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	deModelVertex &vertex = lod.GetVertexAt( rt->GetValue( 1 )->GetInt() );
	vertex.SetPosition( ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() ) );
	vertex.SetWeightSet( rt->GetValue( 3 )->GetInt() );
}

// protected func void setNormalCount( int lod, int count )
deClassModelBuilder::nfSetNormalCount::nfSetNormalCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setNormalCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetNormalCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetNormalCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setTangentCount( int lod, int count )
deClassModelBuilder::nfSetTangentCount::nfSetTangentCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setTangentCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetTangentCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetTangentCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setFaceCount( int lod, int count )
deClassModelBuilder::nfSetFaceCount::nfSetFaceCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setFaceCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetFaceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetFaceCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setFaceAt( int lod, int index, int texture, int vertex1, int vertex2,
//    int vertex3, int normal1, int normal2, int normal3, int tangent1, int tangent2,
//    int tangent3, int texCoord1, int texCoord2, int texCoord3, Vector faceNormal,
//    Vector faceTangent )
deClassModelBuilder::nfSetFaceAt::nfSetFaceAt( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setFaceAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // vertex1
	p_AddParameter( init.clsInteger ); // vertex2
	p_AddParameter( init.clsInteger ); // vertex3
	p_AddParameter( init.clsInteger ); // normal1
	p_AddParameter( init.clsInteger ); // normal2
	p_AddParameter( init.clsInteger ); // normal3
	p_AddParameter( init.clsInteger ); // tangent1
	p_AddParameter( init.clsInteger ); // tangent2
	p_AddParameter( init.clsInteger ); // tangent3
	p_AddParameter( init.clsInteger ); // texCoord1
	p_AddParameter( init.clsInteger ); // texCoord2
	p_AddParameter( init.clsInteger ); // texCoord3
	p_AddParameter( init.clsVector ); // faceNormal
	p_AddParameter( init.clsVector ); // faceTanget
}
void deClassModelBuilder::nfSetFaceAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassModelBuilder* )GetOwnerClass() )->GetDS();
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	deModelFace &face = lod.GetFaceAt( rt->GetValue( 1 )->GetInt() );
	face.SetTexture( rt->GetValue( 2 )->GetInt() );
	face.SetVertex1( rt->GetValue( 3 )->GetInt() );
	face.SetVertex2( rt->GetValue( 4 )->GetInt() );
	face.SetVertex3( rt->GetValue( 5 )->GetInt() );
	face.SetNormal1( rt->GetValue( 6 )->GetInt() );
	face.SetNormal2( rt->GetValue( 7 )->GetInt() );
	face.SetNormal3( rt->GetValue( 8 )->GetInt() );
	face.SetTangent1( rt->GetValue( 9 )->GetInt() );
	face.SetTangent2( rt->GetValue( 10 )->GetInt() );
	face.SetTangent3( rt->GetValue( 11 )->GetInt() );
	face.SetTextureCoordinates1( rt->GetValue( 12 )->GetInt() );
	face.SetTextureCoordinates2( rt->GetValue( 13 )->GetInt() );
	face.SetTextureCoordinates3( rt->GetValue( 14 )->GetInt() );
	face.SetFaceNormal( ds.GetClassVector()->GetVector( rt->GetValue( 15 )->GetRealObject() ) );
	face.SetFaceTangent( ds.GetClassVector()->GetVector( rt->GetValue( 16 )->GetRealObject() ) );
}

// protected func void setTextureCoordinateSetCount( int lod, int count )
deClassModelBuilder::nfSetTextureCoordinateSetCount::nfSetTextureCoordinateSetCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setTextureCoordinatesCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetTextureCoordinateSetCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	lod.SetTextureCoordinatesCount( rt->GetValue( 1 )->GetInt() );
}

// protected func void setTextureCoordinateSetAtSetCount( int lod, int index, int count )
deClassModelBuilder::nfSetTextureCoordinateSetAtSetCount::nfSetTextureCoordinateSetAtSetCount( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setTextureCoordinatesAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // count
}
void deClassModelBuilder::nfSetTextureCoordinateSetAtSetCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	deModelTextureCoordinatesSet &tcs = lod.GetTextureCoordinatesSetAt( rt->GetValue( 1 )->GetInt() );
	tcs.SetTextureCoordinatesCount( rt->GetValue( 2 )->GetInt() );
}

// protected func void setTextureCoordinateSetAtSetAt( int lod, int index, int index2, Vector2 tc )
deClassModelBuilder::nfSetTextureCoordinateSetAtSetAt::nfSetTextureCoordinateSetAtSetAt( const sInitData &init ) :
dsFunction( init.clsModelBuilder, "setTextureCoordinatesAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // lod
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // index2
	p_AddParameter( init.clsVector2 ); // tc
}
void deClassModelBuilder::nfSetTextureCoordinateSetAtSetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassModelBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetModel() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassModelBuilder* )GetOwnerClass() )->GetDS();
	deModelLOD &lod = *builder->GetModel()->GetLODAt( rt->GetValue( 0 )->GetInt() );
	deModelTextureCoordinatesSet &tcs = lod.GetTextureCoordinatesSetAt( rt->GetValue( 1 )->GetInt() );
	tcs.SetTextureCoordinatesAt( rt->GetValue( 2 )->GetInt(),
		ds.GetClassVector2()->GetVector2( rt->GetValue( 3 )->GetRealObject() ) );
}



// Class deClassModelBuilder
//////////////////////////////

// Constructor, destructor
////////////////////////////

deClassModelBuilder::deClassModelBuilder( deScriptingDragonScript &ds ) :
dsClass( "ModelBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMdlBldNatDat ) );
}

deClassModelBuilder::~deClassModelBuilder(){
}



// Management
///////////////

void deClassModelBuilder::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsModelBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsModel = pDS.GetClassModel();
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsPoint = pDS.GetClassPoint();
	init.clsVector2 = pDS.GetClassVector2();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfBuild( init ) );
	AddFunction( new nfBuildModel( init ) );
	AddFunction( new nfAddBone( init ) );
	AddFunction( new nfAddTexture( init ) );
	AddFunction( new nfAddLOD( init ) );
	AddFunction( new nfSetLodError( init ) );
	AddFunction( new nfAddTextureCoordinatesSet( init ) );
	AddFunction( new nfSetWeightCount( init ) );
	AddFunction( new nfSetWeightAt( init ) );
	AddFunction( new nfSetWeightGroupCount( init ) );
	AddFunction( new nfSetWeightGroupAt( init ) );
	AddFunction( new nfSetVertexCount( init ) );
	AddFunction( new nfSetVertexAt( init ) );
	AddFunction( new nfSetNormalCount( init ) );
	AddFunction( new nfSetTangentCount( init ) );
	AddFunction( new nfSetFaceCount( init ) );
	AddFunction( new nfSetFaceAt( init ) );
	AddFunction( new nfSetTextureCoordinateSetCount( init ) );
	AddFunction( new nfSetTextureCoordinateSetAtSetCount( init ) );
	AddFunction( new nfSetTextureCoordinateSetAtSetAt( init ) );
}
