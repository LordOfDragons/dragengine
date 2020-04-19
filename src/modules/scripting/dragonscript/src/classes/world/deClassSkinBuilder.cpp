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

#include "deClassSkin.h"
#include "deClassSkinBuilder.h"
#include "../math/deClassVector2.h"
#include "../math/deClassPoint.h"
#include "../graphics/deClassColor.h"
#include "../graphics/deClassImage.h"
#include "../graphics/deClassFont.h"
#include "../video/deClassVideo.h"
#include "../curve/deClassCurveBezier.h"
#include "../math/deClassPoint3.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinBuilder.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitorIdentify.h>

#include <libdscript/packages/default/dsClassEnumeration.h>


class deClassSkinBuilder_Builder : public deSkinBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deSkin *pSkin;
	deSkinPropertyConstructed *pOpenConstructed;
	decPointerList pOpenNodes;
	bool pNextNodeAsMask;
	
public:
	deClassSkinBuilder_Builder( dsRunTime *rt, dsValue *myself ) :
	pRT( rt ), pMyself( myself ), pSkin( NULL ), pOpenConstructed( NULL ), pNextNodeAsMask( false ){
	}
	
	virtual void BuildSkin( deSkin *skin ){
		pSkin = skin;
		pOpenConstructed = NULL;
		pOpenNodes.RemoveAll();
		pNextNodeAsMask = false;
		
		try{
			pRT->RunFunction( pMyself, "buildSkin", 0 );
			
		}catch( const duException &e ){
			pSkin = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW( deeInvalidParam );
			
		}catch( ... ){
			pNextNodeAsMask = false;
			pOpenConstructed = NULL;
			pOpenNodes.RemoveAll();
			pSkin = NULL;
			throw;
		}
		
		pNextNodeAsMask = false;
		pOpenConstructed = NULL;
		pOpenNodes.RemoveAll();
		pSkin = NULL;
	}
	
	inline deSkin *GetSkin() const{ return pSkin; }
	
	inline deSkinPropertyConstructed *GetOpenConstructed() const{ return pOpenConstructed; }
	
	deSkinPropertyNodeGroup *OpenConstructed( int indexTexture, int indexProperty ){
		if( ! pSkin || pOpenConstructed ){
			DSTHROW( dueInvalidAction );
		}
		deSkinPropertyVisitorIdentify identify;
		pSkin->GetTextureAt( indexTexture )->GetPropertyAt( indexProperty )->Visit( identify );
		pOpenConstructed = &identify.CastToConstructed();
		pOpenNodes.Add( &pOpenConstructed->GetContent() );
		pNextNodeAsMask = false;
		return &pOpenConstructed->GetContent();
	}
	
	deSkinPropertyNodeGroup *GetOpenNode(){
		return ( deSkinPropertyNodeGroup* )pOpenNodes.GetAt( pOpenNodes.GetCount() - 1 );
	}
	
	void OpenGroup( deSkinPropertyNodeGroup *group ){
		if( ! pSkin || pOpenConstructed || ! group ){
			DSTHROW( dueInvalidAction );
		}
		pOpenNodes.Add( group );
		pNextNodeAsMask = false;
	}
	
	inline bool GetNextNodeAsMask() const{ return pNextNodeAsMask; }
	
	void SetNextNodeAsMask( bool nextNodeAsMask ){
		pNextNodeAsMask = nextNodeAsMask;
	}
	
	void CloseNode(){
		if( ! pSkin || ! pOpenConstructed || pOpenNodes.GetCount() == 0 ){
			DSTHROW( dueInvalidAction );
		}
		pOpenNodes.RemoveFrom( pOpenNodes.GetCount() - 1 );
		pNextNodeAsMask = false;
		if( pOpenNodes.GetCount() == 0 ){
			pOpenConstructed = NULL;
		}
	}
};


struct sMdlBldNatDat{
	deClassSkinBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassSkinBuilder::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkinBuilder::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder = NULL;
}

// public destructor Destructor()
deClassSkinBuilder::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkinBuilder::nfDestructor::RunFunction( dsRunTime*, dsValue* ){
}



// public func Skin build( String filename )
deClassSkinBuilder::nfBuild::nfBuild( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
	p_AddParameter( init.clsString ); // filename
}
void deClassSkinBuilder::nfBuild::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMdlBldNatDat &nd = *( ( sMdlBldNatDat* )p_GetNativeData( myself ) );
	if( nd.builder ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	const char * const filename = rt->GetValue( 0 )->GetString();
	deClassSkinBuilder_Builder builder( rt, myself );
	deSkinReference skin;
	
	nd.builder = &builder;
	
	try{
		skin.TakeOver( ds.GetGameEngine()->GetSkinManager()->CreateSkin( filename, builder ) );
		
	}catch( ... ){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassSkin()->PushSkin( rt, skin );
}



// abstract protected func void buildSkin()
deClassSkinBuilder::nfBuildSkin::nfBuildSkin( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "buildSkin", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid ){
}
void deClassSkinBuilder::nfBuildSkin::RunFunction( dsRunTime*, dsValue* ){
}



// protected func void addTexture( String name )
deClassSkinBuilder::nfAddTexture::nfAddTexture( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addTexture", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
}
void deClassSkinBuilder::nfAddTexture::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const char * const name = rt->GetValue( 0 )->GetString();
	
	deSkinTexture * const texture = new deSkinTexture( name );
	try{
		builder->GetSkin()->AddTexture( texture );
		
	}catch( ... ){
		delete texture;
		throw;
	}
}

// protected func void addPropertyValue( int texture, String type, String texCoordSet,
// String renderable, float value )
deClassSkinBuilder::nfAddPropertyValue::nfAddPropertyValue( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addPropertyValue", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // type
	p_AddParameter( init.clsString ); // texCoordSet
	p_AddParameter( init.clsString ); // renderable
	p_AddParameter( init.clsFloat ); // value
}
void deClassSkinBuilder::nfAddPropertyValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const char * const type = rt->GetValue( 1 )->GetString();
	const char * const texCoordSet = rt->GetValue( 2 )->GetString();
	const char * const renderable = rt->GetValue( 3 )->GetString();
	const float value = rt->GetValue( 4 )->GetFloat();
	
	deSkinPropertyValue * const property = new deSkinPropertyValue( type );
	try{
		property->SetTexCoordSet( texCoordSet );
		property->SetRenderable( renderable );
		property->SetValue( value );
		texture.AddProperty( property );
		
	}catch( ... ){
		delete property;
		throw;
	}
}

// protected func void addPropertyColor( int texture, String type, String texCoordSet,
// String renderable, Color color )
deClassSkinBuilder::nfAddPropertyColor::nfAddPropertyColor( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addPropertyColor", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // type
	p_AddParameter( init.clsString ); // texCoordSet
	p_AddParameter( init.clsString ); // renderable
	p_AddParameter( init.clsColor ); // color
}
void deClassSkinBuilder::nfAddPropertyColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const char * const type = rt->GetValue( 1 )->GetString();
	const char * const texCoordSet = rt->GetValue( 2 )->GetString();
	const char * const renderable = rt->GetValue( 3 )->GetString();
	const decColor &color = ds.GetClassColor()->GetColor( rt->GetValue( 4 )->GetRealObject() );
	
	deSkinPropertyColor * const property = new deSkinPropertyColor( type );
	try{
		property->SetTexCoordSet( texCoordSet );
		property->SetRenderable( renderable );
		property->SetColor( color );
		texture.AddProperty( property );
		
	}catch( ... ){
		delete property;
		throw;
	}
}

// protected func void addPropertyImage( int texture, String type, String texCoordSet,
// String renderable, String pathImage, Image image )
deClassSkinBuilder::nfAddPropertyImage::nfAddPropertyImage( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addPropertyImage", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // type
	p_AddParameter( init.clsString ); // texCoordSet
	p_AddParameter( init.clsString ); // renderable
	p_AddParameter( init.clsString ); // pathImage
	p_AddParameter( init.clsImage ); // image
}
void deClassSkinBuilder::nfAddPropertyImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const char * const type = rt->GetValue( 1 )->GetString();
	const char * const texCoordSet = rt->GetValue( 2 )->GetString();
	const char * const renderable = rt->GetValue( 3 )->GetString();
	const char * const pathImage = rt->GetValue( 4 )->GetString();
	deImage * const image = ds.GetClassImage()->GetImage( rt->GetValue( 5 )->GetRealObject() );
	
	deSkinPropertyImage * const property = new deSkinPropertyImage( type );
	try{
		property->SetTexCoordSet( texCoordSet );
		property->SetRenderable( renderable );
		property->SetPath( pathImage );
		property->SetImage( image );
		texture.AddProperty( property );
		
	}catch( ... ){
		delete property;
		throw;
	}
}

// protected func void addPropertyVideo( int texture, String type, String texCoordSet,
// String renderable, String pathVideo, Video video, bool sharedTime )
deClassSkinBuilder::nfAddPropertyVideo::nfAddPropertyVideo( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addPropertyVideo", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // type
	p_AddParameter( init.clsString ); // texCoordSet
	p_AddParameter( init.clsString ); // renderable
	p_AddParameter( init.clsString ); // pathVideo
	p_AddParameter( init.clsVideo ); // video
	p_AddParameter( init.clsBoolean ); // sharedTime
}
void deClassSkinBuilder::nfAddPropertyVideo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const char * const type = rt->GetValue( 1 )->GetString();
	const char * const texCoordSet = rt->GetValue( 2 )->GetString();
	const char * const renderable = rt->GetValue( 3 )->GetString();
	const char * const pathVideo = rt->GetValue( 4 )->GetString();
	deVideo * const video = ds.GetClassVideo()->GetVideo( rt->GetValue( 5 )->GetRealObject() );
	const bool sharedTime = rt->GetValue( 6 )->GetBool();
	
	deSkinPropertyVideo * const property = new deSkinPropertyVideo( type );
	try{
		property->SetTexCoordSet( texCoordSet );
		property->SetRenderable( renderable );
		property->SetPath( pathVideo );
		property->SetVideo( video );
		property->SetSharedTime( sharedTime );
		texture.AddProperty( property );
		
	}catch( ... ){
		delete property;
		throw;
	}
}

// protected func void addPropertyMapped( int texture, String type, String texCoordSet, String renderable,
// CurveBezier redCurve, SkinPropertyMappedInputType redInputType, Vector2 redInputRange, bool redInputClamped, String redBone,
// CurveBezier greenCurve, SkinPropertyMappedInputType greenInputType, Vector2 greenInputRange, bool greenInputClamped, String greenBone,
// CurveBezier blueCurve, SkinPropertyMappedInputType blueInputType, Vector2 blueInputRange, bool blueInputClamped, String blueBone,
// CurveBezier alphaCurve, SkinPropertyMappedInputType alphaInputType, Vector2 alphaInputRange, bool alphaInputClamped, String alphaBone,
// )
deClassSkinBuilder::nfAddPropertyMapped::nfAddPropertyMapped( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addPropertyMapped", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // type
	p_AddParameter( init.clsString ); // texCoordSet
	p_AddParameter( init.clsString ); // renderable
	p_AddParameter( init.clsCurveBezier ); // redCurve
	p_AddParameter( init.clsSkinPropertyMappedInputType ); // redInputType
	p_AddParameter( init.clsVector2 ); // redInputRange
	p_AddParameter( init.clsBoolean ); // redInputClamped
	p_AddParameter( init.clsString ); // redBone
	p_AddParameter( init.clsCurveBezier ); // greenCurve
	p_AddParameter( init.clsSkinPropertyMappedInputType ); // greenInputType
	p_AddParameter( init.clsVector2 ); // greenInputRange
	p_AddParameter( init.clsBoolean ); // greenInputClamped
	p_AddParameter( init.clsString ); // greenBone
	p_AddParameter( init.clsCurveBezier ); // blueCurve
	p_AddParameter( init.clsSkinPropertyMappedInputType ); // blueInputType
	p_AddParameter( init.clsVector2 ); // blueInputRange
	p_AddParameter( init.clsBoolean ); // blueInputClamped
	p_AddParameter( init.clsString ); // blueBone
	p_AddParameter( init.clsCurveBezier ); // alphaCurve
	p_AddParameter( init.clsSkinPropertyMappedInputType ); // alphaInputType
	p_AddParameter( init.clsVector2 ); // alphaInputRange
	p_AddParameter( init.clsBoolean ); // alphaInputClamped
	p_AddParameter( init.clsString ); // alphaBone
}
void deClassSkinBuilder::nfAddPropertyMapped::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deClassCurveBezier &clsCurveBezier = *ds.GetClassCurveBezier();
	deClassVector2 &clsVector2 = *ds.GetClassVector2();
	dsClassEnumeration &clsEnum = *( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() );
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const char * const type = rt->GetValue( 1 )->GetString();
	const char * const texCoordSet = rt->GetValue( 2 )->GetString();
	const char * const renderable = rt->GetValue( 3 )->GetString();
	
	const decCurveBezier &redCurve = clsCurveBezier.GetCurve( rt->GetValue( 4 )->GetRealObject() );
	const deSkinPropertyMapped::eInputTypes redInputType = ( deSkinPropertyMapped::eInputTypes )
		clsEnum.GetConstantOrder( *rt->GetValue( 5 )->GetRealObject() );
	const decVector2 &redInputRange = clsVector2.GetVector2( rt->GetValue( 6 )->GetRealObject() );
	const bool redInputClamped = rt->GetValue( 7 )->GetBool();
	const char * const redBone = rt->GetValue( 8 )->GetString();
	
	const decCurveBezier &greenCurve = clsCurveBezier.GetCurve( rt->GetValue( 9 )->GetRealObject() );
	const deSkinPropertyMapped::eInputTypes greenInputType = ( deSkinPropertyMapped::eInputTypes )
		clsEnum.GetConstantOrder( *rt->GetValue( 10 )->GetRealObject() );
	const decVector2 &greenInputRange = clsVector2.GetVector2( rt->GetValue( 11 )->GetRealObject() );
	const bool greenInputClamped = rt->GetValue( 12 )->GetBool();
	const char * const greenBone = rt->GetValue( 13 )->GetString();
	
	const decCurveBezier &blueCurve = clsCurveBezier.GetCurve( rt->GetValue( 14 )->GetRealObject() );
	const deSkinPropertyMapped::eInputTypes blueInputType = ( deSkinPropertyMapped::eInputTypes )
		clsEnum.GetConstantOrder( *rt->GetValue( 15 )->GetRealObject() );
	const decVector2 &blueInputRange = clsVector2.GetVector2( rt->GetValue( 16 )->GetRealObject() );
	const bool blueInputClamped = rt->GetValue( 17 )->GetBool();
	const char * const blueBone = rt->GetValue( 18 )->GetString();
	
	const decCurveBezier &alphaCurve = clsCurveBezier.GetCurve( rt->GetValue( 19 )->GetRealObject() );
	const deSkinPropertyMapped::eInputTypes alphaInputType = ( deSkinPropertyMapped::eInputTypes )
		clsEnum.GetConstantOrder( *rt->GetValue( 20 )->GetRealObject() );
	const decVector2 &alphaInputRange = clsVector2.GetVector2( rt->GetValue( 21 )->GetRealObject() );
	const bool alphaInputClamped = rt->GetValue( 22 )->GetBool();
	const char * const alphaBone = rt->GetValue( 23 )->GetString();
	
	deSkinPropertyMapped * const property = new deSkinPropertyMapped( type );
	try{
		property->SetTexCoordSet( texCoordSet );
		property->SetRenderable( renderable );
		property->GetRed().GetCurve() = redCurve;
		property->GetRed().SetInputType( redInputType );
		property->GetRed().SetInputLower( redInputRange.x );
		property->GetRed().SetInputUpper( redInputRange.y );
		property->GetRed().SetInputClamped( redInputClamped );
		property->GetRed().SetBone( redBone );
		property->GetGreen().GetCurve() = greenCurve;
		property->GetGreen().SetInputType( greenInputType );
		property->GetGreen().SetInputLower( greenInputRange.x );
		property->GetGreen().SetInputUpper( greenInputRange.y );
		property->GetGreen().SetInputClamped( greenInputClamped );
		property->GetGreen().SetBone( greenBone );
		property->GetBlue().GetCurve() = blueCurve;
		property->GetBlue().SetInputType( blueInputType );
		property->GetBlue().SetInputLower( blueInputRange.x );
		property->GetBlue().SetInputUpper( blueInputRange.y );
		property->GetBlue().SetInputClamped( blueInputClamped );
		property->GetBlue().SetBone( blueBone );
		property->GetAlpha().GetCurve() = alphaCurve;
		property->GetAlpha().SetInputType( alphaInputType );
		property->GetAlpha().SetInputLower( alphaInputRange.x );
		property->GetAlpha().SetInputUpper( alphaInputRange.y );
		property->GetAlpha().SetInputClamped( alphaInputClamped );
		property->GetAlpha().SetBone( alphaBone );
		texture.AddProperty( property );
		
	}catch( ... ){
		delete property;
		throw;
	}
}

// protected func void addPropertyConstructed( int texture, String type, String texCoordSet,
// String renderable, Color color, bool tileX, bool tileY, Point3 size )
deClassSkinBuilder::nfAddPropertyConstructed::nfAddPropertyConstructed( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addPropertyConstructed", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // type
	p_AddParameter( init.clsString ); // texCoordSet
	p_AddParameter( init.clsString ); // renderable
	p_AddParameter( init.clsColor ); // color
	p_AddParameter( init.clsBoolean ); // tileX
	p_AddParameter( init.clsBoolean ); // tileY
	p_AddParameter( init.clsPoint3 ); // size
}
void deClassSkinBuilder::nfAddPropertyConstructed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const char * const type = rt->GetValue( 1 )->GetString();
	const char * const texCoordSet = rt->GetValue( 2 )->GetString();
	const char * const renderable = rt->GetValue( 3 )->GetString();
	const decColor &color = ds.GetClassColor()->GetColor( rt->GetValue( 4 )->GetRealObject() );
	const bool tileX = rt->GetValue( 5 )->GetBool();
	const bool tileY = rt->GetValue( 6 )->GetBool();
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint( rt->GetValue( 7 )->GetRealObject() );
	
	deSkinPropertyConstructed * const property = new deSkinPropertyConstructed( type );
	try{
		property->SetTexCoordSet( texCoordSet );
		property->SetRenderable( renderable );
		property->SetColor( color );
		property->SetTileX( tileX );
		property->SetTileY( tileY );
		property->GetContent().SetSize( size );
		texture.AddProperty( property );
		
	}catch( ... ){
		delete property;
		throw;
	}
}

// protected func void constructedOpenContent( int texture, int property )
deClassSkinBuilder::nfConstructedOpenContent::nfConstructedOpenContent( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "constructedOpenContent", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
	p_AddParameter( init.clsPoint3 ); // size
	p_AddParameter( init.clsColor ); // backgroundColor
}
void deClassSkinBuilder::nfConstructedOpenContent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->OpenConstructed( rt->GetValue( 0 )->GetInt(), rt->GetValue( 1 )->GetInt() );
}

// protected func void addNodeImage( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float constrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode,
// String path, Image image, Point repeat )
deClassSkinBuilder::nfAddNodeImage::nfAddNodeImage( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addNodeImage", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPoint3 ); // position
	p_AddParameter( init.clsPoint3 ); // size
	p_AddParameter( init.clsFloat ); // rotation
	p_AddParameter( init.clsFloat ); // shear
	p_AddParameter( init.clsFloat ); // brightness
	p_AddParameter( init.clsFloat ); // constrast
	p_AddParameter( init.clsFloat ); // gamma
	p_AddParameter( init.clsColor ); // colorize
	p_AddParameter( init.clsFloat ); // transparency
	p_AddParameter( init.clsSkinPropertyNodeCombineMode ); // combineMode
	p_AddParameter( init.clsString ); // path
	p_AddParameter( init.clsImage ); // image
	p_AddParameter( init.clsPoint ); // repeat
}
void deClassSkinBuilder::nfAddNodeImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint( rt->GetValue( 1 )->GetRealObject() );
	const float rotation = rt->GetValue( 2 )->GetFloat();
	const float shear = rt->GetValue( 3 )->GetFloat();
	const float brightness = rt->GetValue( 4 )->GetFloat();
	const float contrast = rt->GetValue( 5 )->GetFloat();
	const float gamma = rt->GetValue( 6 )->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor( rt->GetValue( 7 )->GetRealObject() );
	const float transparency = rt->GetValue( 8 )->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = ( deSkinPropertyNode::eCombineModes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	const char * const path = rt->GetValue( 10 )->GetString();
	deImage * const image = ds.GetClassImage()->GetImage( rt->GetValue( 11 )->GetRealObject() );
	const decPoint &repeat = ds.GetClassPoint()->GetPoint( rt->GetValue( 12 )->GetRealObject() );
	
	deSkinPropertyNodeImage * const node = new deSkinPropertyNodeImage;
	try{
		node->SetPosition( position );
		node->SetSize( size );
		node->SetRotation( rotation );
		node->SetShear( shear );
		node->SetBrightness( brightness );
		node->SetContrast( contrast );
		node->SetGamma( gamma );
		node->SetColorize( colorize );
		node->SetTransparency( transparency );
		node->SetPath( path );
		node->SetImage( image );
		node->SetRepeat( repeat );
		node->SetCombineMode( combineMode );
		
		if( builder->GetNextNodeAsMask() ){
			group->SetMask( node );
			
		}else{
			group->AddNode( node );
		}
		
	}catch( ... ){
		delete node;
		throw;
	}
}

// protected func void addNodeText( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float constrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode,
// String path, Font font, float fontSize, String text, Color color )
deClassSkinBuilder::nfAddNodeText::nfAddNodeText( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addNodeText", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPoint3 ); // position
	p_AddParameter( init.clsPoint3 ); // size
	p_AddParameter( init.clsFloat ); // rotation
	p_AddParameter( init.clsFloat ); // shear
	p_AddParameter( init.clsFloat ); // brightness
	p_AddParameter( init.clsFloat ); // constrast
	p_AddParameter( init.clsFloat ); // gamma
	p_AddParameter( init.clsColor ); // colorize
	p_AddParameter( init.clsFloat ); // transparency
	p_AddParameter( init.clsSkinPropertyNodeCombineMode ); // combineMode
	p_AddParameter( init.clsString ); // path
	p_AddParameter( init.clsFont ); // font
	p_AddParameter( init.clsFloat ); // fontSize
	p_AddParameter( init.clsString ); // text
	p_AddParameter( init.clsColor ); // color
}
void deClassSkinBuilder::nfAddNodeText::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint( rt->GetValue( 1 )->GetRealObject() );
	const float rotation = rt->GetValue( 2 )->GetFloat();
	const float shear = rt->GetValue( 3 )->GetFloat();
	const float brightness = rt->GetValue( 4 )->GetFloat();
	const float contrast = rt->GetValue( 5 )->GetFloat();
	const float gamma = rt->GetValue( 6 )->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor( rt->GetValue( 7 )->GetRealObject() );
	const float transparency = rt->GetValue( 8 )->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = ( deSkinPropertyNode::eCombineModes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	const char * const path = rt->GetValue( 10 )->GetString();
	deFont * const font = ds.GetClassFont()->GetFont( rt->GetValue( 11 )->GetRealObject() );
	const float fontSize = rt->GetValue( 12 )->GetFloat();
	const char * const text = rt->GetValue( 13 )->GetString();
	const decColor &color = ds.GetClassColor()->GetColor( rt->GetValue( 14 )->GetRealObject() );
	
	deSkinPropertyNodeText * const node = new deSkinPropertyNodeText;
	try{
		node->SetPosition( position );
		node->SetSize( size );
		node->SetRotation( rotation );
		node->SetShear( shear );
		node->SetBrightness( brightness );
		node->SetContrast( contrast );
		node->SetGamma( gamma );
		node->SetColorize( colorize );
		node->SetTransparency( transparency );
		node->SetCombineMode( combineMode );
		node->SetPath( path );
		node->SetFont( font );
		node->SetFontSize( fontSize );
		node->SetText( text );
		node->SetColor( color );
		
		if( builder->GetNextNodeAsMask() ){
			group->SetMask( node );
			
		}else{
			group->AddNode( node );
		}
		
	}catch( ... ){
		delete node;
		throw;
	}
}

// protected func void addNodeShape( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float constrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode,
// int shapeType, Color fillColor, Color lineColor, float thickness )
deClassSkinBuilder::nfAddNodeShape::nfAddNodeShape( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addNodeShape", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPoint3 ); // position
	p_AddParameter( init.clsPoint3 ); // size
	p_AddParameter( init.clsFloat ); // rotation
	p_AddParameter( init.clsFloat ); // shear
	p_AddParameter( init.clsFloat ); // brightness
	p_AddParameter( init.clsFloat ); // constrast
	p_AddParameter( init.clsFloat ); // gamma
	p_AddParameter( init.clsColor ); // colorize
	p_AddParameter( init.clsFloat ); // transparency
	p_AddParameter( init.clsSkinPropertyNodeCombineMode ); // combineMode
	p_AddParameter( init.clsInteger ); // shapeType
	p_AddParameter( init.clsColor ); // fillColor
	p_AddParameter( init.clsColor ); // lineColor
	p_AddParameter( init.clsFloat ); // thickness
}
void deClassSkinBuilder::nfAddNodeShape::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint( rt->GetValue( 1 )->GetRealObject() );
	const float rotation = rt->GetValue( 2 )->GetFloat();
	const float shear = rt->GetValue( 3 )->GetFloat();
	const float brightness = rt->GetValue( 4 )->GetFloat();
	const float contrast = rt->GetValue( 5 )->GetFloat();
	const float gamma = rt->GetValue( 6 )->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor( rt->GetValue( 7 )->GetRealObject() );
	const float transparency = rt->GetValue( 8 )->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = ( deSkinPropertyNode::eCombineModes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	const deSkinPropertyNodeShape::eShapeTypes shapeType =
		( deSkinPropertyNodeShape::eShapeTypes )rt->GetValue( 10 )->GetInt();
	const decColor &fillColor = ds.GetClassColor()->GetColor( rt->GetValue( 11 )->GetRealObject() );
	const decColor &lineColor = ds.GetClassColor()->GetColor( rt->GetValue( 12 )->GetRealObject() );
	const float thickness = rt->GetValue( 13 )->GetFloat();
	
	deSkinPropertyNodeShape * const node = new deSkinPropertyNodeShape;
	try{
		node->SetPosition( position );
		node->SetSize( size );
		node->SetRotation( rotation );
		node->SetShear( shear );
		node->SetBrightness( brightness );
		node->SetContrast( contrast );
		node->SetGamma( gamma );
		node->SetColorize( colorize );
		node->SetTransparency( transparency );
		node->SetCombineMode( combineMode );
		node->SetShapeType( shapeType );
		node->SetFillColor( fillColor );
		node->SetLineColor( lineColor );
		node->SetThickness( thickness );
		
		if( builder->GetNextNodeAsMask() ){
			group->SetMask( node );
			
		}else{
			group->AddNode( node );
		}
		
	}catch( ... ){
		delete node;
		throw;
	}
}

// protected func void addNodeGroup( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float constrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode )
deClassSkinBuilder::nfAddNodeGroup::nfAddNodeGroup( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "addNodeGroup", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPoint3 ); // position
	p_AddParameter( init.clsPoint3 ); // size
	p_AddParameter( init.clsFloat ); // rotation
	p_AddParameter( init.clsFloat ); // shear
	p_AddParameter( init.clsFloat ); // brightness
	p_AddParameter( init.clsFloat ); // constrast
	p_AddParameter( init.clsFloat ); // gamma
	p_AddParameter( init.clsColor ); // colorize
	p_AddParameter( init.clsFloat ); // transparency
	p_AddParameter( init.clsSkinPropertyNodeCombineMode ); // combineMode
}
void deClassSkinBuilder::nfAddNodeGroup::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassSkinBuilder* )GetOwnerClass() )->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint( rt->GetValue( 1 )->GetRealObject() );
	const float rotation = rt->GetValue( 2 )->GetFloat();
	const float shear = rt->GetValue( 3 )->GetFloat();
	const float brightness = rt->GetValue( 4 )->GetFloat();
	const float contrast = rt->GetValue( 5 )->GetFloat();
	const float gamma = rt->GetValue( 6 )->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor( rt->GetValue( 7 )->GetRealObject() );
	const float transparency = rt->GetValue( 8 )->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = ( deSkinPropertyNode::eCombineModes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	
	deSkinPropertyNodeGroup * const node = new deSkinPropertyNodeGroup;
	try{
		node->SetPosition( position );
		node->SetSize( size );
		node->SetRotation( rotation );
		node->SetShear( shear );
		node->SetBrightness( brightness );
		node->SetContrast( contrast );
		node->SetGamma( gamma );
		node->SetColorize( colorize );
		node->SetTransparency( transparency );
		node->SetCombineMode( combineMode );
		
		if( builder->GetNextNodeAsMask() ){
			group->SetMask( node );
			
		}else{
			group->AddNode( node );
		}
		
	}catch( ... ){
		delete node;
		throw;
	}
	
	builder->OpenGroup( node );
}

// protected func void nextNodeAsMask()
deClassSkinBuilder::nfNextNodeAsMask::nfNextNodeAsMask( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "nextNodeAsMask", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkinBuilder::nfNextNodeAsMask::RunFunction( dsRunTime*, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->SetNextNodeAsMask( true );
}

// protected func void closeNode()
deClassSkinBuilder::nfCloseNode::nfCloseNode( const sInitData &init ) :
dsFunction( init.clsSkinBuilder, "closeNode", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkinBuilder::nfCloseNode::RunFunction( dsRunTime*, dsValue *myself ){
	deClassSkinBuilder_Builder * const builder = ( ( sMdlBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetSkin() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->CloseNode();
}



// Class deClassSkinBuilder
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkinBuilder::deClassSkinBuilder( deScriptingDragonScript &ds ) :
dsClass( "SkinBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMdlBldNatDat ) );
}

deClassSkinBuilder::~deClassSkinBuilder(){
}



// Management
///////////////

void deClassSkinBuilder::CreateClassMembers( dsEngine *engine ){
	pClsSkinPropertyNodeCombineMode = engine->GetClass( "Dragengine.Scenery.SkinPropertyNodeCombineMode" );
	pClsSkinPropertyMappedInputType = engine->GetClass( "Dragengine.Scenery.SkinPropertyMappedInputType" );
	
	sInitData init;
	init.clsSkinBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsSkin = pDS.GetClassSkin();
	init.clsColor = pDS.GetClassColor();
	init.clsImage = pDS.GetClassImage();
	init.clsVideo = pDS.GetClassVideo();
	init.clsPoint = pDS.GetClassPoint();
	init.clsPoint3 = pDS.GetClassPoint3();
	init.clsVector2 = pDS.GetClassVector2();
	init.clsFont = pDS.GetClassFont();
	init.clsCurveBezier = pDS.GetClassCurveBezier();
	init.clsSkinPropertyNodeCombineMode = pClsSkinPropertyNodeCombineMode;
	init.clsSkinPropertyMappedInputType = pClsSkinPropertyMappedInputType;
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfBuild( init ) );
	AddFunction( new nfBuildSkin( init ) );
	AddFunction( new nfAddTexture( init ) );
	AddFunction( new nfAddPropertyValue( init ) );
	AddFunction( new nfAddPropertyColor( init ) );
	AddFunction( new nfAddPropertyImage( init ) );
	AddFunction( new nfAddPropertyVideo( init ) );
	AddFunction( new nfAddPropertyConstructed( init ) );
	AddFunction( new nfAddPropertyMapped( init ) );
	AddFunction( new nfConstructedOpenContent( init ) );
	AddFunction( new nfAddNodeImage( init ) );
	AddFunction( new nfAddNodeText( init ) );
	AddFunction( new nfAddNodeShape( init ) );
	AddFunction( new nfAddNodeGroup( init ) );
	AddFunction( new nfNextNodeAsMask( init ) );
	AddFunction( new nfCloseNode( init ) );
}
