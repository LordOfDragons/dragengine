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
#include "../graphics/deClassColor.h"
#include "../graphics/deClassImage.h"
#include "../resources/deClassResourceListener.h"
#include "../video/deClassVideo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>

struct sSkinNatDat{
	deSkin *skin;
};



// Native Functions
/////////////////////

// public constructor Load( string name )
deClassSkin::nfLoad::nfLoad( const sInitData &init ) :
dsFunction( init.clsSkin, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
}
void deClassSkin::nfLoad::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSkinNatDat &nd = *( ( sSkinNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassSkin* )GetOwnerClass() )->GetDS();
	deSkinManager &skinMgr = *ds.GetGameEngine()->GetSkinManager();
	
	// reset all
	nd.skin = NULL;
	
	// load skin
	nd.skin = skinMgr.LoadSkin( rt->GetValue( 0 )->GetString(), "/" );
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassSkin::nfLoadAsynchron::nfLoadAsynchron( const sInitData &init ) :
dsFunction( init.clsSkin, "loadAsynchron", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // filename
	p_AddParameter( init.clsResourceListener ); // listener
}
void deClassSkin::nfLoadAsynchron::RunFunction( dsRunTime *rt, dsValue *myself ){
	deScriptingDragonScript &ds = ( ( deClassSkin* )GetOwnerClass() )->GetDS();
	
	const char * const filename = rt->GetValue( 0 )->GetString();
	dsRealObject * const listener = rt->GetValue( 1 )->GetRealObject();
	if( ! listener ){
		DSTHROW( dueNullPointer );
	}
	
	ds.GetResourceLoader()->AddRequest( filename, deResourceLoader::ertSkin, listener );
}

// public destructor Destructor()
deClassSkin::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSkin, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSkin::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSkinNatDat &nd = *( ( sSkinNatDat* )p_GetNativeData( myself ) );
	if( nd.skin ){
		nd.skin->FreeReference();
		nd.skin = NULL;
	}
}



// public func string GetFilename()
deClassSkin::nfGetFilename::nfGetFilename( const sInitData &init ) :
dsFunction( init.clsSkin, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassSkin::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	rt->PushString( skin.GetFilename() );
}

// public func int getTextureCount()
deClassSkin::nfGetTextureCount::nfGetTextureCount( const sInitData &init ) :
dsFunction( init.clsSkin, "getTextureCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassSkin::nfGetTextureCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	rt->PushInt( skin.GetTextureCount() );
}

// public func int indexOfTextureNamed( String name )
deClassSkin::nfIndexOfTextureNamed::nfIndexOfTextureNamed( const sInitData &init ) :
dsFunction( init.clsSkin, "indexOfTextureNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsString ); // name
}
void deClassSkin::nfIndexOfTextureNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const char * const name = rt->GetValue( 0 )->GetString();
	
	rt->PushInt( skin.IndexOfTextureNamed( name ) );
}



// public func String textureGetName( int texture )
deClassSkin::nfTextureGetName::nfTextureGetName( const sInitData &init ) :
dsFunction( init.clsSkin, "textureGetName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
	p_AddParameter( init.clsInteger ); // texture
}
void deClassSkin::nfTextureGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushString( texture.GetName() );
}

// public func int textureGetPropertyCount( int texture )
deClassSkin::nfTextureGetPropertyCount::nfTextureGetPropertyCount( const sInitData &init ) :
dsFunction( init.clsSkin, "textureGetPropertyCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // texture
}
void deClassSkin::nfTextureGetPropertyCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushInt( texture.GetPropertyCount() );
}

// public func int textureIndexOfPropertyNamed( int texture, String property )
deClassSkin::nfTextureIndexOfPropertyNamed::nfTextureIndexOfPropertyNamed( const sInitData &init ) :
dsFunction( init.clsSkin, "textureIndexOfPropertyNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsString ); // property
}
void deClassSkin::nfTextureIndexOfPropertyNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	
	const char * const name = rt->GetValue( 1 )->GetString();
	const int count = texture.GetPropertyCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( texture.GetPropertyAt( i )->GetType() == name ){
			rt->PushInt( i );
			return;
		}
	}
	
	rt->PushInt( -1 );
}



// public func String propertyGetName( int texture, int property )
deClassSkin::nfPropertyGetName::nfPropertyGetName( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	
	rt->PushString( property.GetType() );
}

// public func String propertyGetRenderable( int texture, int property )
deClassSkin::nfPropertyGetRenderable::nfPropertyGetRenderable( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetRenderable", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetRenderable::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	const deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	
	rt->PushString( property.GetRenderable() );
}

// public func SkinPropertyType propertyGetType( int texture, int property )
deClassSkin::nfPropertyGetType::nfPropertyGetType( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkinPropertyType ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit( visitor );
	rt->PushValue( ( ( deClassSkin* )GetOwnerClass() )->GetClassSkinPropertyType()
		->GetVariable( visitor.GetType() )->GetStaticValue() );
}

// public func float propertyGetValue( int texture, int property )
deClassSkin::nfPropertyGetValue::nfPropertyGetValue( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit( visitor );
	rt->PushFloat( visitor.CastToValue().GetValue() );
}

// public func Color propertyGetColor( int texture, int property )
deClassSkin::nfPropertyGetColor::nfPropertyGetColor( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetColor::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	deScriptingDragonScript &ds = ( ( deClassSkin* )GetOwnerClass() )->GetDS();
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit( visitor );
	ds.GetClassColor()->PushColor( rt, visitor.CastToColor().GetColor() );
}

// public func Image propertyGetImage( int texture, int property )
deClassSkin::nfPropertyGetImage::nfPropertyGetImage( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	deScriptingDragonScript &ds = ( ( deClassSkin* )GetOwnerClass() )->GetDS();
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit( visitor );
	ds.GetClassImage()->PushImage( rt, visitor.CastToImage().GetImage() );
}

// public func Video propertyGetVideo( int texture, int property )
deClassSkin::nfPropertyGetVideo::nfPropertyGetVideo( const sInitData &init ) :
dsFunction( init.clsSkin, "propertyGetVideo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVideo ){
	p_AddParameter( init.clsInteger ); // texture
	p_AddParameter( init.clsInteger ); // property
}
void deClassSkin::nfPropertyGetVideo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSkin &skin = *( ( sSkinNatDat* )p_GetNativeData( myself ) )->skin;
	const deSkinTexture &texture = *skin.GetTextureAt( rt->GetValue( 0 )->GetInt() );
	deSkinProperty &property = *texture.GetPropertyAt( rt->GetValue( 1 )->GetInt() );
	deScriptingDragonScript &ds = ( ( deClassSkin* )GetOwnerClass() )->GetDS();
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit( visitor );
	ds.GetClassVideo()->PushVideo( rt, visitor.CastToVideo().GetVideo() );
}



// public func int hashCode()
deClassSkin::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction(init.clsSkin, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassSkin::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkinNatDat &nd = *( ( sSkinNatDat* )p_GetNativeData( myself ) );
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )nd.skin );
}

// public func bool equals( Object obj )
deClassSkin::nfEquals::nfEquals( const sInitData &init ) :
dsFunction(init.clsSkin, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassSkin::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSkinNatDat &nd = *( ( sSkinNatDat* )p_GetNativeData( myself ) );
	deClassSkin * const clsSkin = ( deClassSkin* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( object->GetType()->GetPrimitiveType() != DSPT_OBJECT || ! object->GetRealObject()
	|| object->GetRealObject()->GetType() != clsSkin ){
		rt->PushBool( false );
		
	}else{
		const sSkinNatDat &other = *( ( sSkinNatDat* )p_GetNativeData( object ) );
		rt->PushBool( nd.skin == other.skin );
	}
}



// Class deClassSkin
///////////////////////

// Constructor, destructor
////////////////////////////

deClassSkin::deClassSkin( deScriptingDragonScript &ds ) :
dsClass( "Skin", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSkinNatDat ) );
}

deClassSkin::~deClassSkin(){
}



// Management
///////////////

void deClassSkin::CreateClassMembers( dsEngine *engine ){
	pClsSkinPropertyType = engine->GetClass( "Dragengine.Scenery.SkinPropertyType" );
	
	sInitData init;
	init.clsSkin = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsResourceListener = pDS.GetClassResourceListener();
	init.clsColor = pDS.GetClassColor();
	init.clsImage = pDS.GetClassImage();
	init.clsVideo = pDS.GetClassVideo();
	init.clsSkinPropertyType = pClsSkinPropertyType;
	
	// functions
	AddFunction( new nfLoad( init ) );
	AddFunction( new nfLoadAsynchron( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetFilename( init ) );
	
	AddFunction( new nfGetTextureCount( init ) );
	AddFunction( new nfIndexOfTextureNamed( init ) );
	
	AddFunction( new nfTextureGetName( init ) );
	AddFunction( new nfTextureGetPropertyCount( init ) );
	AddFunction( new nfTextureIndexOfPropertyNamed( init ) );
	
	AddFunction( new nfPropertyGetName( init ) );
	AddFunction( new nfPropertyGetRenderable( init ) );
	AddFunction( new nfPropertyGetType( init ) );
	AddFunction( new nfPropertyGetValue( init ) );
	AddFunction( new nfPropertyGetColor( init ) );
	AddFunction( new nfPropertyGetImage( init ) );
	AddFunction( new nfPropertyGetVideo( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	CalcMemberOffsets();
}

deSkin *deClassSkin::GetSkin( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	return ( ( sSkinNatDat* )p_GetNativeData( myself->GetBuffer() ) )->skin;
}

void deClassSkin::PushSkin( dsRunTime *rt, deSkin *skin ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! skin ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sSkinNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->skin = skin;
	skin->AddReference();
}
