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
#include <string.h>

#include "deSkinModule.h"
#include "desmWriteProperty.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>


// export definition
#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DESkinCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
#endif


// entry function
// used to create a dragengine skin module.
// returns NULL on error.
/////////////////////////////////////////////////////////
deBaseModule *DESkinCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deSkinModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// class deSkinModule
//////////////////////

// Constructor, destructor
////////////////////////////

deSkinModule::deSkinModule( deLoadableModule &loadableModule ) :
deBaseSkinModule( loadableModule ){
}

deSkinModule::~deSkinModule(){
}



// Loading and Saving
///////////////////////

void deSkinModule::LoadSkin( decBaseFileReader &file, deSkin &skin ){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser( GetGameEngine()->GetLogger() ).ParseXml( &file, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "skin" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pParseSkin( *root, skin );
}

void deSkinModule::SaveSkin(decBaseFileWriter &file, const deSkin &skin){
	decXmlWriter xmlWriter( &file );
	xmlWriter.WriteXMLDeclaration();
	pWriteSkin( xmlWriter, skin );
}



// Private functions
//////////////////////

decXmlElementTag *deSkinModule::pGetTagAt( const decXmlElementTag &tag, int index ){
	decXmlElement &element = *tag.GetElementAt( index );
	
	if( element.CanCastToElementTag() ){
		return element.CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *deSkinModule::pFindAttribute( const decXmlElementTag &tag, const char *name ){
	decXmlAttValue *value;
	int i;
	
	for( i=0; i<tag.GetElementCount(); i++ ){
		decXmlElement &element = *tag.GetElementAt( i );
		
		if( element.CanCastToAttValue() ){
			value = element.CastToAttValue();
			
			if( strcmp( value->GetName(), name ) == 0 ){
				return value;
			}
		}
	}
	
	return NULL;
}

const char *deSkinModule::pGetAttributeString( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return value->GetValue();
		
	}else{
		LogErrorFormat( "Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int deSkinModule::pGetAttributeInt( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return ( int )strtol( value->GetValue(), NULL, 10 );
		
	}else{
		LogErrorFormat( "Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float deSkinModule::pGetAttributeFloat( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return strtof( value->GetValue(), NULL );
		
	}else{
		LogErrorFormat( "Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

bool deSkinModule::pGetAttributeBool( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = tag.FindAttribute( name );
	if( ! value ){
		LogErrorFormat( "Missing Attribute %s in tag %s", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
	
	return strcmp( value->GetValue(), "true" ) == 0
	|| strcmp( value->GetValue(), "yes" ) == 0
	|| strcmp( value->GetValue(), "1" ) == 0;
}



void deSkinModule::pParseSkin( const decXmlElementTag &root, deSkin &skin ){
	deSkinTexture *texture = NULL;
	decXmlElementTag *tag;
	decPath basePath;
	int i;
	
	// build base path
	basePath.SetFromUnix( skin.GetFilename() );
	
	if( basePath.GetComponentCount() > 1 ){
		basePath.RemoveLastComponent();
		
	}else{
		basePath.SetFromUnix( "/" );
	}
	
	try{
		for( i=0; i<root.GetElementCount(); i++ ){
			tag = pGetTagAt( root, i );
			
			if( tag ){
				if( tag->GetName() == "texture" ){
					texture = pParseTexture( *tag, basePath, skin );
					
					if( skin.IndexOfTextureNamed( texture->GetName() ) != -1 ){
						LogErrorFormat( "texture(%i:%i): Duplicate texture!",
							tag->GetLineNumber(), tag->GetPositionNumber() );
						DETHROW( deeInvalidParam );
					}
					
					skin.AddTexture( texture );
					texture = nullptr;
					
				}else if( tag->GetName() == "mapped" ){
					const deSkinMapped::Ref mapped( pParseMapped( *tag ) );
					
					if( skin.HasMappedNamed( mapped->GetName() ) ){
						LogErrorFormat( "mapped(%i:%i): Duplicate mapped!",
							tag->GetLineNumber(), tag->GetPositionNumber() );
						DETHROW( deeInvalidParam );
					}
					
					skin.AddMapped( mapped );
					
				}else{
					LogWarnFormat( "skin(%i:%i): Unknown Tag %s, ignoring", tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString() );
				}
			}
		}
		
	}catch( const deException &e ){
		if( texture ){
			delete texture;
		}
		e.PrintError();
		throw;
	}
}



deSkinMapped::Ref deSkinModule::pParseMapped( const decXmlElementTag &root, const char *forceName ){
	const deSkinMapped::Ref mapped( deSkinMapped::Ref::NewWith(forceName ? forceName : pGetAttributeString( root, "name" )) );
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "curve" ){
			pParseMappedCurve( *tag, mapped->GetCurve() );
			
		}else if( tag->GetName() == "inputType" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				continue;
			}
			
			const decString &type = cdata->GetData();
			
			if( type == "time" ){
				mapped->SetInputType( deSkinMapped::eitTime );
				
			}else if( type == "bonePositionX" ){
				mapped->SetInputType( deSkinMapped::eitBonePositionX );
				
			}else if( type == "bonePositionY" ){
				mapped->SetInputType( deSkinMapped::eitBonePositionY );
				
			}else if( type == "bonePositionZ" ){
				mapped->SetInputType( deSkinMapped::eitBonePositionZ );
				
			}else if( type == "boneRotationX" ){
				mapped->SetInputType( deSkinMapped::eitBoneRotationX );
				
			}else if( type == "boneRotationY" ){
				mapped->SetInputType( deSkinMapped::eitBoneRotationY );
				
			}else if( type == "boneRotationZ" ){
				mapped->SetInputType( deSkinMapped::eitBoneRotationZ );
				
			}else if( type == "boneScaleX" ){
				mapped->SetInputType( deSkinMapped::eitBoneScaleX );
				
			}else if( type == "boneScaleY" ){
				mapped->SetInputType( deSkinMapped::eitBoneScaleY );
				
			}else if( type == "boneScaleZ" ){
				mapped->SetInputType( deSkinMapped::eitBoneScaleZ );
				
			}else if( type == "renderable" ){
				mapped->SetInputType( deSkinMapped::eitRenderable );
			}
			
		}else if( tag->GetName() == "inputLower" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetInputLower( cdata->GetData().ToFloat() );
			}
			
		}else if( tag->GetName() == "inputUpper" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetInputUpper( cdata->GetData().ToFloat() );
			}
			
		}else if( tag->GetName() == "inputClamped" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetInputClamped( cdata->GetData() == "true"
					|| cdata->GetData() == "yes" || cdata->GetData() == "1" );
			}
			
		}else if( tag->GetName() == "outputLower" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetOutputLower( cdata->GetData().ToFloat() );
			}
			
		}else if( tag->GetName() == "outputUpper" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetOutputUpper( cdata->GetData().ToFloat() );
			}
			
		}else if( tag->GetName() == "bone" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetBone( cdata->GetData() );
			}
			
		}else if( tag->GetName() == "renderable" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				mapped->SetRenderable( cdata->GetData() );
			}
			
		}else if( tag->GetName() == "renderableComponent" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				continue;
			}
			
			const decString &type = cdata->GetData();
			
			if( type == "red" ){
				mapped->SetRenderableComponent( deSkinMapped::ercRed );
				
			}else if( type == "green" ){
				mapped->SetRenderableComponent( deSkinMapped::ercGreen );
				
			}else if( type == "blue" ){
				mapped->SetRenderableComponent( deSkinMapped::ercBlue );
				
			}else if( type == "alpha" ){
				mapped->SetRenderableComponent( deSkinMapped::ercAlpha );
			}
			
		}else{
			LogWarnFormat( "mapped(%i:%i): Unknown Tag %s, ignoring", tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
	
	return mapped;
}

void deSkinModule::pParseMappedCurve( const decXmlElementTag &root, decCurveBezier &curve ){
	int i;
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "interpolation" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				continue;
			}
			
			const decString &type = cdata->GetData();
			
			if( type == "constant" ){
				curve.SetInterpolationMode( decCurveBezier::eimConstant );
				
			}else if( type == "linear" ){
				curve.SetInterpolationMode( decCurveBezier::eimLinear );
				
			}else if( type == "bezier" ){
				curve.SetInterpolationMode( decCurveBezier::eimBezier );
			}
			
		}else if( tag->GetName() == "point" ){
			pParseMappedCurvePoint ( *tag, curve );
			
		}else{
			LogWarnFormat( "curve(%i:%i): Unknown Tag %s, ignoring", tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParseMappedCurvePoint( const decXmlElementTag &root, decCurveBezier &curve ){
	decVector2 point, handle1, handle2;
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "coordinates" ){
			pReadVector2( *tag, point );
			
		}else if( tag->GetName() == "handle1" ){
			pReadVector2( *tag, handle1 );
			
		}else if( tag->GetName() == "handle2" ){
			pReadVector2( *tag, handle2 );
			
		}else{
			LogWarnFormat( "point(%i:%i): Unknown Tag %s, ignoring", tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
	
	curve.AddPoint( decCurveBezierPoint( point, handle1, handle2 ) );
}



deSkinTexture *deSkinModule::pParseTexture( const decXmlElementTag &root, decPath &basePath, deSkin &skin ){
	deSkinPropertyValue *propertyValue = NULL;
	deSkinPropertyColor *propertyColor = NULL;
	deSkinPropertyImage *propertyImage = NULL;
	deSkinPropertyVideo *propertyVideo = NULL;
	deSkinPropertyMapped *propertyMapped = NULL;
	deSkinPropertyConstructed *propertyConstructed = NULL;
	deSkinTexture *texture = NULL;
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	const char *name = NULL;
	decColor color;
	int i;
	
	// attributes
	name = pGetAttributeString( root, "name" );
	
	// tags
	try{
		texture = new deSkinTexture( name );
		
		// parse propertys
		for( i=0; i<root.GetElementCount(); i++ ){
			tag = pGetTagAt( root, i );
			
			if( tag ){
				if( tag->GetName() == "value" ){
					name = pGetAttributeString( *tag, "property" );
					
					if( texture->HasPropertyWithType( name ) ){
						LogErrorFormat( "value(%i:%i): Duplicate property '%s'!",
							tag->GetLineNumber(), tag->GetPositionNumber(), name );
						DETHROW( deeInvalidParam );
					}
					
					propertyValue = new deSkinPropertyValue( name );
					
					cdata = tag->GetFirstData();
					if( cdata ){
						propertyValue->SetValue( strtof( cdata->GetData(), NULL ) );
						
					}else{
						propertyValue->SetValue( 0.0f );
					}
					
					if( pFindAttribute( *tag, "renderable" ) ){
						propertyValue->SetRenderable( pGetAttributeString( *tag, "renderable" ) );
					}
					if( pFindAttribute( *tag, "bone" ) ){
						propertyValue->SetBone( pGetAttributeString( *tag, "bone" ) );
					}
					
					texture->AddProperty( propertyValue );
					propertyValue = NULL;
					
				}else if( tag->GetName() == "color" ){
					name = pGetAttributeString( *tag, "property" );
					
					if( texture->HasPropertyWithType( name ) ){
						LogErrorFormat( "color(%i:%i): Duplicate property '%s'!",
							tag->GetLineNumber(), tag->GetPositionNumber(), name );
						DETHROW( deeInvalidParam );
					}
					
					propertyColor = new deSkinPropertyColor( name );
					
					color.Set( 0.0f, 0.0f, 0.0f, 1.0f );
					if( pFindAttribute( *tag, "r" ) ){
						color.r = pGetAttributeFloat( *tag, "r" );
					}
					if( pFindAttribute( *tag, "g" ) ){
						color.g = pGetAttributeFloat( *tag, "g" );
					}
					if( pFindAttribute( *tag, "b" ) ){
						color.b = pGetAttributeFloat( *tag, "b" );
					}
					if( pFindAttribute( *tag, "a" ) ){
						color.a = pGetAttributeFloat( *tag, "a" );
					}
					color.Normalize();
					propertyColor->SetColor( color );
					
					if( pFindAttribute( *tag, "renderable" ) ){
						propertyColor->SetRenderable( pGetAttributeString( *tag, "renderable" ) );
					}
					if( pFindAttribute( *tag, "bone" ) ){
						propertyColor->SetBone( pGetAttributeString( *tag, "bone" ) );
					}
					
					texture->AddProperty( propertyColor );
					propertyColor = NULL;
					
				}else if( tag->GetName() == "image" ){
					name = pGetAttributeString( *tag, "property" );
					
					if( texture->HasPropertyWithType( name ) ){
						LogErrorFormat( "image(%i:%i): Duplicate property '%s'!",
							tag->GetLineNumber(), tag->GetPositionNumber(), name );
						DETHROW( deeInvalidParam );
					}
					
					propertyImage = new deSkinPropertyImage( name );
					
					cdata = tag->GetFirstData();
					if( cdata ){
						propertyImage->SetPath( cdata->GetData() );
					}
					
					if( pFindAttribute( *tag, "renderable" ) ){
						propertyImage->SetRenderable( pGetAttributeString( *tag, "renderable" ) );
					}
					if( pFindAttribute( *tag, "bone" ) ){
						propertyImage->SetBone( pGetAttributeString( *tag, "bone" ) );
					}
					
					texture->AddProperty( propertyImage );
					propertyImage = NULL;
					
				}else if( tag->GetName() == "video" ){
					name = pGetAttributeString( *tag, "property" );
					
					if( texture->HasPropertyWithType( name ) ){
						LogErrorFormat( "video(%i:%i): Duplicate property '%s'!",
							tag->GetLineNumber(), tag->GetPositionNumber(), name );
						DETHROW( deeInvalidParam );
					}
					
					propertyVideo = new deSkinPropertyVideo( name );
					
					cdata = tag->GetFirstData();
					if( cdata ){
						propertyVideo->SetPath( cdata->GetData() );
					}
					
					if( pFindAttribute( *tag, "renderable" ) ){
						propertyVideo->SetRenderable( pGetAttributeString( *tag, "renderable" ) );
					}
					if( pFindAttribute( *tag, "bone" ) ){
						propertyVideo->SetBone( pGetAttributeString( *tag, "bone" ) );
					}
					if( pFindAttribute( *tag, "sharedTime" ) ){
						propertyVideo->SetSharedTime( pGetAttributeBool( *tag, "sharedTime" ) );
					}
					
					texture->AddProperty( propertyVideo );
					propertyVideo = NULL;
					
				}else if( tag->GetName() == "mapped" ){
					name = pGetAttributeString( *tag, "property" );
					
					if( texture->HasPropertyWithType( name ) ){
						LogErrorFormat( "mapped(%i:%i): Duplicate property '%s'!",
							tag->GetLineNumber(), tag->GetPositionNumber(), name );
						DETHROW( deeInvalidParam );
					}
					
					propertyMapped = new deSkinPropertyMapped( name );
					
					if( pFindAttribute( *tag, "renderable" ) ){
						propertyMapped->SetRenderable( pGetAttributeString( *tag, "renderable" ) );
					}
					if( pFindAttribute( *tag, "bone" ) ){
						propertyMapped->SetBone( pGetAttributeString( *tag, "bone" ) );
					}
					
					pParsePropertyMapped( *tag, skin, *propertyMapped );
					
					texture->AddProperty( propertyMapped );
					propertyMapped = NULL;
					
				}else if( tag->GetName() == "constructed" ){
					name = pGetAttributeString( *tag, "property" );
					
					if( texture->HasPropertyWithType( name ) ){
						LogErrorFormat( "constructed(%i:%i): Duplicate property '%s'!",
							tag->GetLineNumber(), tag->GetPositionNumber(), name );
						DETHROW( deeInvalidParam );
					}
					
					propertyConstructed = new deSkinPropertyConstructed( name );
					
					if( pFindAttribute( *tag, "renderable" ) ){
						propertyConstructed->SetRenderable( pGetAttributeString( *tag, "renderable" ) );
					}
					if( pFindAttribute( *tag, "bone" ) ){
						propertyConstructed->SetBone( pGetAttributeString( *tag, "bone" ) );
					}
					
					pParsePropertyConstructed( *tag, skin, *propertyConstructed );
					
					texture->AddProperty( propertyConstructed );
					propertyConstructed = NULL;
					
				}else if( tag->GetName() == "fullAlpha" ){
					LogWarnFormat( "%s: texture(%i:%i): tag fullAlpha is deprecated. adding transparency.type=1 property instead",
						skin.GetFilename().GetString(), tag->GetLineNumber(), tag->GetPositionNumber() );
					
					name = "transparency.type";
					
					if( ! texture->HasPropertyWithType( name ) ){
						propertyValue = new deSkinPropertyValue( name );
						propertyValue->SetValue( 1.0f );
						texture->AddProperty( propertyValue );
						propertyValue = NULL;
					}
					
				}else{
					LogWarnFormat( "texture(%i:%i): Unknown Tag %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(),
						tag->GetName().GetString() );
				}
			}
		}
		
	}catch( const deException &e ){
		if( propertyConstructed ){
			delete propertyConstructed;
		}
		if( propertyMapped ){
			delete propertyMapped;
		}
		if( propertyVideo ){
			delete propertyVideo;
		}
		if( propertyImage ){
			delete propertyImage;
		}
		if( propertyColor ){
			delete propertyColor;
		}
		if( propertyValue ){
			delete propertyValue;
		}
		if( texture ){
			delete texture;
		}
		e.PrintError();
		throw;
	}
	
	return texture;
}

void deSkinModule::pParsePropertyMapped( const decXmlElementTag &root, deSkin &skin, deSkinPropertyMapped &property ){
	deSkinMapped::Ref mapped;
	int i, index;
	decString name;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "red" ){
			// deprecated
			index = skin.GetMappedCount();
			name.Format( "#generated%d", index );
			mapped = pParseMapped( *tag, name );
			if( mapped->GetCurve().GetPointCount() > 0 ){
				skin.AddMapped( mapped );
				property.SetRed( index );
			}
			
		}else if( tag->GetName() == "green" ){
			// deprecated
			index = skin.GetMappedCount();
			name.Format( "#generated%d", index );
			mapped = pParseMapped( *tag, name );
			if( mapped->GetCurve().GetPointCount() > 0 ){
				skin.AddMapped( mapped );
				property.SetGreen( index );
			}
			
		}else if( tag->GetName() == "blue" ){
			// deprecated
			index = skin.GetMappedCount();
			name.Format( "#generated%d", index );
			mapped = pParseMapped( *tag, name );
			if( mapped->GetCurve().GetPointCount() > 0 ){
				skin.AddMapped( mapped );
				property.SetBlue( index );
			}
			
		}else if( tag->GetName() == "alpha" ){
			// deprecated
			index = skin.GetMappedCount();
			name.Format( "#generated%d", index );
			mapped = pParseMapped( *tag, name );
			if( mapped->GetCurve().GetPointCount() > 0 ){
				skin.AddMapped( mapped );
				property.SetAlpha( index );
			}
			
		}else if( tag->GetName() == "mappedRed" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				property.SetRed( skin.IndexOfMappedNamed( cdata->GetData() ) );
			}
			
		}else if( tag->GetName() == "mappedGreen" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				property.SetGreen( skin.IndexOfMappedNamed( cdata->GetData() ) );
			}
			
		}else if( tag->GetName() == "mappedBlue" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				property.SetBlue( skin.IndexOfMappedNamed( cdata->GetData() ) );
			}
			
		}else if( tag->GetName() == "mappedAlpha" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				property.SetAlpha( skin.IndexOfMappedNamed( cdata->GetData() ) );
			}
			
		}else{
			LogWarnFormat( "mapped(%i:%i): Unknown Tag %s, ignoring", tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyConstructed( const decXmlElementTag& root,
const deSkin &skin, deSkinPropertyConstructed& property ){
	deSkinPropertyNode *node = NULL;
	int i;
	
	try{
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = pGetTagAt( root, i );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "color" ) == 0 ){
				decColor color( 0.0f, 0.0f, 0.0f, 1.0f );
				
				if( pFindAttribute( *tag, "r" ) ){
					color.r = pGetAttributeFloat( *tag, "r" );
				}
				if( pFindAttribute( *tag, "g" ) ){
					color.g = pGetAttributeFloat( *tag, "g" );
				}
				if( pFindAttribute( *tag, "b" ) ){
					color.b = pGetAttributeFloat( *tag, "b" );
				}
				if( pFindAttribute( *tag, "a" ) ){
					color.a = pGetAttributeFloat( *tag, "a" );
				}
				color.Normalize();
				property.SetColor( color );
				
			}else if( strcmp( tag->GetName(), "size" ) == 0 ){
				decPoint3 size( 256, 256, 1 );
				
				size.x = pGetAttributeInt( *tag, "x" );
				size.y = pGetAttributeInt( *tag, "y" );
				if( pFindAttribute( *tag, "z" ) ){
					size.z = pGetAttributeInt( *tag, "z" );
				}
				
				property.GetContent().SetSize( size );
				
			}else if( strcmp( tag->GetName(), "tileX" ) == 0 ){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if( cdata ){
					property.SetTileX( strcmp( cdata->GetData(), "true" ) == 0 || strcmp( cdata->GetData(), "1" ) == 0 );
				}
				
			}else if( strcmp( tag->GetName(), "tileY" ) == 0 ){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if( cdata ){
					property.SetTileY( strcmp( cdata->GetData(), "true" ) == 0 || strcmp( cdata->GetData(), "1" ) == 0 );
				}
				
			}else if( strcmp( tag->GetName(), "bitCount" ) == 0 ){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if( cdata ){
					property.SetBitCount( decString( cdata->GetData() ).ToInt() );
				}
				
			}else{
				node = pParsePropertyNode( *tag, skin );
				
				if( node ){
					property.GetContent().AddNode( node );
					node = NULL;
					
				}else{
					LogWarnFormat( "constructed(%i:%i): Unknown Tag %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(),
						tag->GetName().GetString() );
				}
			}
		}
		
	}catch( const deException &e ){
		if( node ){
			delete node;
		}
		e.PrintError();
		throw;
	}
}

deSkinPropertyNode *deSkinModule::pParsePropertyNode( const decXmlElementTag &tag, const deSkin &skin ){
	deSkinPropertyNode *node = NULL;
	
	try{
		const decString tagName( tag.GetName() );
		if( tagName == "group" ){
			node = new deSkinPropertyNodeGroup;
			pParsePropertyNodeGroup( tag, skin, *( ( deSkinPropertyNodeGroup* )node ) );
			
		}else if( tagName == "image" ){
			node = new deSkinPropertyNodeImage;
			pParsePropertyNodeImage( tag, skin, *( ( deSkinPropertyNodeImage* )node ) );
			
		}else if( tagName == "shape" ){
			node = new deSkinPropertyNodeShape;
			pParsePropertyNodeShape( tag, skin, *( ( deSkinPropertyNodeShape* )node ) );
			
		}else if( tagName == "text" ){
			node = new deSkinPropertyNodeText;
			pParsePropertyNodeText( tag, skin, *( ( deSkinPropertyNodeText* )node ) );
		}
		
	}catch( const deException &e ){
		if( node ){
			delete node;
		}
		e.PrintError();
		throw;
	}
	
	return node;
}

bool deSkinModule::pParsePropertyNodeCommon( const decXmlElementTag &tag,
const deSkin &skin, deSkinPropertyNode &node ){
	const decString tagName( tag.GetName() );
	
	if( tagName == "position" ){
		decPoint3 position;
		if( pFindAttribute( tag, "x" ) ){
			position.x = pGetAttributeInt( tag, "x" );
		}
		if( pFindAttribute( tag, "y" ) ){
			position.y = pGetAttributeInt( tag, "y" );
		}
		if( pFindAttribute( tag, "z" ) ){
			position.z = pGetAttributeInt( tag, "z" );
		}
		node.SetPosition( position );
		return true;
		
	}else if( tagName == "size" ){
		decPoint3 size( 256, 256, 1 );
		if( pFindAttribute( tag, "x" ) ){
			size.x = pGetAttributeInt( tag, "x" );
		}
		if( pFindAttribute( tag, "y" ) ){
			size.y = pGetAttributeInt( tag, "y" );
		}
		if( pFindAttribute( tag, "z" ) ){
			size.z = pGetAttributeInt( tag, "z" );
		}
		node.SetSize( size );
		return true;
		
	}else if( tagName == "rotation" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			node.SetRotation( strtof( cdata->GetData(), NULL ) * DEG2RAD );
		}
		return true;
		
	}else if( tagName == "shear" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			node.SetShear( strtof( cdata->GetData(), NULL ) * DEG2RAD );
		}
		return true;
		
	}else if( tagName == "brightness" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			node.SetBrightness( strtof( cdata->GetData(), NULL ) );
		}
		return true;
		
	}else if( tagName == "contrast" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			node.SetContrast( strtof( cdata->GetData(), NULL ) );
		}
		return true;
		
	}else if( tagName == "gamma" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			node.SetGamma( strtof( cdata->GetData(), NULL ) );
		}
		return true;
		
	}else if( tagName == "colorize" ){
		decColor colorize( 1.0f, 1.0f, 1.0f );
		
		if( pFindAttribute( tag, "r" ) ){
			colorize.r = pGetAttributeFloat( tag, "r" );
		}
		if( pFindAttribute( tag, "g" ) ){
			colorize.g = pGetAttributeFloat( tag, "g" );
		}
		if( pFindAttribute( tag, "b" ) ){
			colorize.b = pGetAttributeFloat( tag, "b" );
		}
		
		node.SetColorize( colorize );
		return true;
		
	}else if( tagName == "transparency" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			node.SetTransparency( strtof( cdata->GetData(), NULL ) );
		}
		return true;
		
	}else if( tagName == "combineMode" ){
		decXmlCharacterData * const cdata = tag.GetFirstData();
		if( cdata ){
			if( cdata->GetData() == "blend" ){
				node.SetCombineMode( deSkinPropertyNode::ecmBlend );
				
			}else if( cdata->GetData() == "overlay" ){
				node.SetCombineMode( deSkinPropertyNode::ecmOverlay );
				
			}else{
				LogWarnFormat( "combineMode(%d:%d): Unknown value %s, ignoring",
					tag.GetLineNumber(), tag.GetPositionNumber(), cdata->GetData().GetString() );
			}
		}
		return true;
		
	}else if( tagName == "mask" ){
		const int count = tag.GetElementCount();
		deSkinPropertyNode *nodeMask = NULL;
		int i;
		
		try{
			for( i=0; i<count; i++ ){
				const decXmlElementTag * const tagMask = pGetTagAt( tag, i );
				if( ! tagMask ){
					continue;
				}
				
				if( nodeMask ){
					LogWarnFormat( "mask(%i:%i): Only one mask node allowed, ignoring superfluous tags",
						tag.GetLineNumber(), tag.GetPositionNumber() );
					break;
				}		
				
				const decString tagName2( tagMask->GetName() );
				if( tagName2 == "group" ){
					nodeMask = new deSkinPropertyNodeGroup;
					pParsePropertyNodeGroup( *tagMask, skin, *( ( deSkinPropertyNodeGroup* )nodeMask ) );
					
				}else if( tagName2 == "image" ){
					nodeMask = new deSkinPropertyNodeImage;
					pParsePropertyNodeImage( *tagMask, skin, *( ( deSkinPropertyNodeImage* )nodeMask ) );
					
				}else if( tagName2 == "shape" ){
					nodeMask = new deSkinPropertyNodeShape;
					pParsePropertyNodeShape( *tagMask, skin, *( ( deSkinPropertyNodeShape* )nodeMask ) );
					
				}else if( tagName2 == "text" ){
					nodeMask = new deSkinPropertyNodeText;
					pParsePropertyNodeText( *tagMask, skin, *( ( deSkinPropertyNodeText* )nodeMask ) );
					
				}else{
					LogWarnFormat( "mask(%i:%i): Unknown Tag %s, ignoring",
						tagMask->GetLineNumber(), tagMask->GetPositionNumber(),
						tagMask->GetName().GetString() );
					break;
				}
			}
			
			node.SetMask( nodeMask );
			
		}catch( const deException &e ){
			if( nodeMask ){
				delete nodeMask;
			}
			e.PrintError();
			throw;
		}
		
		return true;
		
	}else if( tag.GetName() == "mapped" ){
		const decXmlCharacterData * const cdata = tag.GetFirstData();
		if( ! cdata ){
			return true;
		}
		
		const int index = skin.IndexOfMappedNamed( cdata->GetData() );
		const char * const name = pGetAttributeString( tag, "name" );
		
		if( strcmp( name, "positionX" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emPositionX, index );
			
		}else if( strcmp( name, "positionY" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emPositionY, index );
			
		}else if( strcmp( name, "positionZ" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emPositionZ, index );
			
		}else if( strcmp( name, "sizeX" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emSizeX, index );
			
		}else if( strcmp( name, "sizeY" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emSizeY, index );
			
		}else if( strcmp( name, "sizeZ" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emSizeZ, index );
			
		}else if( strcmp( name, "rotation" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emRotation, index );
			
		}else if( strcmp( name, "shear" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emShear, index );
			
		}else if( strcmp( name, "brightness" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emBrightness, index );
			
		}else if( strcmp( name, "contrast" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emContrast, index );
			
		}else if( strcmp( name, "gamma" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emGamma, index );
			
		}else if( strcmp( name, "colorizeR" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emColorizeRed, index );
			
		}else if( strcmp( name, "colorizeG" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emColorizeGreen, index );
			
		}else if( strcmp( name, "colorizeB" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emColorizeBlue, index );
			
		}else if( strcmp( name, "transparency" ) == 0 ){
			node.SetMappedFor( deSkinPropertyNode::emTransparency, index );
			
		}else{
			LogWarnFormat( "node(%i:%i): Unknown Tag %s, ignoring",
				tag.GetLineNumber(), tag.GetPositionNumber(), tag.GetName().GetString() );
		}
		
		return true;
		
	}else{
		return false;
	}
}

void deSkinModule::pParsePropertyNodeGroup( const decXmlElementTag &root,
const deSkin &skin, deSkinPropertyNodeGroup &group ){
	const int count = root.GetElementCount();
	deSkinPropertyNode *node = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			const decXmlElementTag * const tag = pGetTagAt( root, i );
			if( ! tag ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			node = pParsePropertyNode( *tag, skin );
			if( node ){
				group.AddNode( node );
				node = NULL;
				
			}else if( ! pParsePropertyNodeCommon( *tag, skin, group ) ){
				LogWarnFormat( "group(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(),
					tag->GetName().GetString() );
			}
		}
		
	}catch( const deException &e ){
		if( node ){
			delete node;
		}
		e.PrintError();
		throw;
	}
}

void deSkinModule::pParsePropertyNodeImage( const decXmlElementTag &root,
const deSkin &skin, deSkinPropertyNodeImage &image ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		if( tagName == "path" ){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				image.SetPath( cdata->GetData() );
			}
			
		}else if( tagName == "repeat" ){
			decPoint repeat( 1, 1 );
			
			if( pFindAttribute( *tag, "x" ) ){
				repeat.x = pGetAttributeInt( *tag, "x" );
			}
			if( pFindAttribute( *tag, "y" ) ){
				repeat.y = pGetAttributeInt( *tag, "y" );
			}
			
			image.SetRepeat( repeat );
			
		}else if( ! pParsePropertyNodeCommon( *tag, skin, image ) ){
			LogWarnFormat( "image(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyNodeShape( const decXmlElementTag &root,
const deSkin &skin, deSkinPropertyNodeShape &shape ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		if( tagName == "type" ){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				LogWarnFormat( "shape(%i:%i): Unknown type %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(),
					cdata->GetData().GetString() );
				continue;
			}
			
			if( strcmp( cdata->GetData(), "rectangle" ) == 0 ){
				shape.SetShapeType( deSkinPropertyNodeShape::estRectangle );
				
			}else if( strcmp( cdata->GetData(), "ellipse" ) == 0 ){
				shape.SetShapeType( deSkinPropertyNodeShape::estEllipse );
				
			}else{
				LogWarnFormat( "shape(%i:%i): Unknown type %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(),
					cdata->GetData().GetString() );
			}
			
		}else if( tagName == "fillColor" ){
			decColor color;
			
			if( pFindAttribute( *tag, "r" ) ){
				color.r = pGetAttributeFloat( *tag, "r" );
			}
			if( pFindAttribute( *tag, "g" ) ){
				color.g = pGetAttributeFloat( *tag, "g" );
			}
			if( pFindAttribute( *tag, "b" ) ){
				color.b = pGetAttributeFloat( *tag, "b" );
			}
			if( pFindAttribute( *tag, "a" ) ){
				color.a = pGetAttributeFloat( *tag, "a" );
			}
			color.Normalize();
			
			shape.SetFillColor( color );
			
		}else if( tagName == "lineColor" ){
			decColor color;
			
			if( pFindAttribute( *tag, "r" ) ){
				color.r = pGetAttributeFloat( *tag, "r" );
			}
			if( pFindAttribute( *tag, "g" ) ){
				color.g = pGetAttributeFloat( *tag, "g" );
			}
			if( pFindAttribute( *tag, "b" ) ){
				color.b = pGetAttributeFloat( *tag, "b" );
			}
			if( pFindAttribute( *tag, "a" ) ){
				color.a = pGetAttributeFloat( *tag, "a" );
			}
			color.Normalize();
			
			shape.SetLineColor( color );
			
		}else if( tagName == "thickness" ){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				shape.SetThickness( strtof( cdata->GetData(), NULL ) );
			}
		
		}else if( tagName == "shapeMapped" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				return;
			}
			
			const int index = skin.IndexOfMappedNamed( cdata->GetData() );
			const char * const name = pGetAttributeString( *tag, "name" );
			
			if( strcmp( name, "fillColorR" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmFillColorRed, index );
				
			}else if( strcmp( name, "fillColorG" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmFillColorGreen, index );
				
			}else if( strcmp( name, "fillColorB" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmFillColorBlue, index );
				
			}else if( strcmp( name, "fillColorA" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmFillColorAlpha, index );
				
			}else if( strcmp( name, "lineColorR" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmLineColorRed, index );
				
			}else if( strcmp( name, "lineColorG" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmLineColorGreen, index );
				
			}else if( strcmp( name, "lineColorB" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmLineColorBlue, index );
				
			}else if( strcmp( name, "lineColorA" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmLineColorAlpha, index );
				
			}else if( strcmp( name, "thickness" ) == 0 ){
				shape.SetShapeMappedFor( deSkinPropertyNodeShape::esmThickness, index );
			
			}else{
				LogWarnFormat( "shape(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString() );
			}
			
		}else if( ! pParsePropertyNodeCommon( *tag, skin, shape ) ){
			LogWarnFormat( "shape(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyNodeText( const decXmlElementTag &root,
const deSkin &skin, deSkinPropertyNodeText &text ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		if( tagName == "path" ){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				text.SetPath( cdata->GetData() );
			}
			
		}else if( tagName == "fontSize" ){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				text.SetFontSize( strtof( cdata->GetData(), NULL ) );
			}
			
		}else if( tagName == "text" ){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				text.SetText( cdata->GetData() );
			}
			
		}else if( tagName == "color" ){
			decColor color;
			
			if( pFindAttribute( *tag, "r" ) ){
				color.r = pGetAttributeFloat( *tag, "r" );
			}
			if( pFindAttribute( *tag, "g" ) ){
				color.g = pGetAttributeFloat( *tag, "g" );
			}
			if( pFindAttribute( *tag, "b" ) ){
				color.b = pGetAttributeFloat( *tag, "b" );
			}
			if( pFindAttribute( *tag, "a" ) ){
				color.a = pGetAttributeFloat( *tag, "a" );
			}
			color.Normalize();
			
			text.SetColor( color );
		
		}else if( tagName == "textMapped" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				return;
			}
			
			const int index = skin.IndexOfMappedNamed( cdata->GetData() );
			const char * const name = pGetAttributeString( *tag, "name" );
			
			if( strcmp( name, "fontSize" ) == 0 ){
				text.SetTextMappedFor( deSkinPropertyNodeText::etmFontSize, index );
				
			}else if( strcmp( name, "colorR" ) == 0 ){
				text.SetTextMappedFor( deSkinPropertyNodeText::etmColorRed, index );
				
			}else if( strcmp( name, "colorG" ) == 0 ){
				text.SetTextMappedFor( deSkinPropertyNodeText::etmColorGreen, index );
				
			}else if( strcmp( name, "colorB" ) == 0 ){
				text.SetTextMappedFor( deSkinPropertyNodeText::etmColorBlue, index );
				
			}else{
				LogWarnFormat( "text(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString() );
			}
			
		}else if( ! pParsePropertyNodeCommon( *tag, skin, text ) ){
			LogWarnFormat( "text(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

decColor deSkinModule::pParseColor( const decXmlElementTag &root ){
	decXmlElementTag *tag;
	decColor color;
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		tag = pGetTagAt( root, i );
		if( tag ){
			if( strcmp( tag->GetName(), "red" ) == 0 ){
				color.r = strtof( tag->GetFirstData()->GetData(), NULL );
				
			}else if( strcmp( tag->GetName(), "green" ) == 0 ){
				color.g = strtof( tag->GetFirstData()->GetData(), NULL );
				
			}else if( strcmp( tag->GetName(), "blue" ) == 0 ){
				color.b = strtof( tag->GetFirstData()->GetData(), NULL );
				
			}else if( strcmp( tag->GetName(), "alpha" ) == 0 ){
				color.a = strtof( tag->GetFirstData()->GetData(), NULL );
				
			}else if( strcmp( tag->GetName(), "all" ) == 0 ){
				color.r = strtof( tag->GetFirstData()->GetData(), NULL );
				color.g = color.r;
				color.b = color.r;
				color.a = color.r;
				
			}else{
				LogWarnFormat( "%s(%i:%i): Unknown Tag %s, ignoring",
					root.GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), tag->GetName().GetString() );
			}
		}
	}
	return color;
}

void deSkinModule::pReadVector2( const decXmlElementTag &tag, decVector2 &vector ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "x" );
	if( value ){
		vector.x = strtof( value->GetValue(), NULL );
	}
	
	value = tag.FindAttribute( "y" );
	if( value ){
		vector.y = strtof( value->GetValue(), NULL );
	}
}



void deSkinModule::pWriteSkin( decXmlWriter &writer, const deSkin &skin ){
	int i;
	
	writer.WriteOpeningTag( "skin" );
	
	const int mappedCount = skin.GetMappedCount();
	for( i=0; i<mappedCount; i++ ){
		pWriteMapped( writer, *skin.GetMappedAt( i ) );
	}
	
	const int textureCount = skin.GetTextureCount();
	for( i=0; i<textureCount; i++ ){
		pWriteTexture( writer, skin, *skin.GetTextureAt( i ) );
	}
	
	writer.WriteClosingTag( "skin", true );
}

void deSkinModule::pWriteMapped( decXmlWriter &writer, const deSkinMapped &mapped ) {
	writer.WriteOpeningTagStart( "mapped" );
	writer.WriteAttributeString( "name", mapped.GetName() );
	writer.WriteOpeningTagEnd();
	
	if( mapped.GetCurve().GetPointCount() > 0 ){
		const int pointCount = mapped.GetCurve().GetPointCount();
		int j;
		
		writer.WriteOpeningTag( "curve" );
		
		switch( mapped.GetCurve().GetInterpolationMode() ){
		case decCurveBezier::eimConstant:
			writer.WriteDataTagString( "interpolation", "constant" );
			break;
			
		case decCurveBezier::eimLinear:
			writer.WriteDataTagString( "interpolation", "linear" );
			break;
			
		case decCurveBezier::eimBezier:
			break;
		}
		
		for( j=0; j<pointCount; j++ ){
			const decCurveBezierPoint &point = mapped.GetCurve().GetPointAt( j );
			writer.WriteOpeningTag( "point" );
			
			writer.WriteOpeningTagStart( "coordinates" );
			writer.WriteAttributeFloat( "x", point.GetPoint().x );
			writer.WriteAttributeFloat( "y", point.GetPoint().y );
			writer.WriteOpeningTagEnd( true );
			
			writer.WriteOpeningTagStart( "handle1" );
			writer.WriteAttributeFloat( "x", point.GetHandle1().x );
			writer.WriteAttributeFloat( "y", point.GetHandle1().y );
			writer.WriteOpeningTagEnd( true );
			
			writer.WriteOpeningTagStart( "handle2" );
			writer.WriteAttributeFloat( "x", point.GetHandle2().x );
			writer.WriteAttributeFloat( "y", point.GetHandle2().y );
			writer.WriteOpeningTagEnd( true );
			
			writer.WriteClosingTag( "point" );
		}
		
		writer.WriteClosingTag( "curve" );
	}
	
	switch( mapped.GetInputType() ){
	case deSkinMapped::eitTime:
		break;
		
	case deSkinMapped::eitBonePositionX:
		writer.WriteDataTagString( "inputType", "bonePositionX" );
		break;
		
	case deSkinMapped::eitBonePositionY:
		writer.WriteDataTagString( "inputType", "bonePositionY" );
		break;
		
	case deSkinMapped::eitBonePositionZ:
		writer.WriteDataTagString( "inputType", "bonePositionZ" );
		break;
		
	case deSkinMapped::eitBoneRotationX:
		writer.WriteDataTagString( "inputType", "boneRotationX" );
		break;
		
	case deSkinMapped::eitBoneRotationY:
		writer.WriteDataTagString( "inputType", "boneRotationY" );
		break;
		
	case deSkinMapped::eitBoneRotationZ:
		writer.WriteDataTagString( "inputType", "boneRotationZ" );
		break;
		
	case deSkinMapped::eitBoneScaleX:
		writer.WriteDataTagString( "inputType", "boneScaleX" );
		break;
		
	case deSkinMapped::eitBoneScaleY:
		writer.WriteDataTagString( "inputType", "boneScaleY" );
		break;
		
	case deSkinMapped::eitBoneScaleZ:
		writer.WriteDataTagString( "inputType", "boneScaleZ" );
		break;
		
	case deSkinMapped::eitRenderable:
		writer.WriteDataTagString( "inputType", "renderable" );
		break;
	}
	
	if( fabsf( mapped.GetInputLower() ) >= FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "inputLower", mapped.GetInputLower() );
	}
	if( fabsf( mapped.GetInputUpper() - 1.0f ) >= FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "inputUpper", mapped.GetInputUpper() );
	}
	
	if( mapped.GetInputClamped() ){
		writer.WriteDataTagBool( "inputClamped", mapped.GetInputClamped() );
	}
	
	if( fabsf( mapped.GetOutputLower() ) >= FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "outputLower", mapped.GetOutputLower() );
	}
	if( fabsf( mapped.GetOutputUpper() - 1.0f ) >= FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "outputUpper", mapped.GetOutputUpper() );
	}
	
	if( ! mapped.GetBone().IsEmpty() ){
		writer.WriteDataTagString( "bone", mapped.GetBone() );
	}
	
	if( ! mapped.GetRenderable().IsEmpty() ){
		writer.WriteDataTagString( "renderable", mapped.GetRenderable() );
	}
	
	switch( mapped.GetRenderableComponent() ){
	case deSkinMapped::ercRed:
		break;
		
	case deSkinMapped::ercGreen:
		writer.WriteDataTagString( "renderableComponent", "green" );
		break;
		
	case deSkinMapped::ercBlue:
		writer.WriteDataTagString( "renderableComponent", "blue" );
		break;
		
	case deSkinMapped::ercAlpha:
		writer.WriteDataTagString( "renderableComponent", "alpha" );
		break;
	}
	
	writer.WriteClosingTag( "mapped", true );
}

void deSkinModule::pWriteTexture( decXmlWriter &writer, const deSkin &skin, const deSkinTexture &texture ){
	int p, propertyCount = texture.GetPropertyCount();
	
	writer.WriteOpeningTagStart( "texture" );
	writer.WriteAttributeString( "name", texture.GetName() );
	writer.WriteOpeningTagEnd();
	
	for( p=0; p<propertyCount; p++ ){
		pWriteProperty( writer, skin, *texture.GetPropertyAt( p ) );
	}
	
	writer.WriteClosingTag( "texture", true );
}

void deSkinModule::pWriteProperty( decXmlWriter &writer, const deSkin &skin, deSkinProperty &property ){
	desmWriteProperty writeProperty( writer, skin, property.GetType() );
	
	property.Visit( writeProperty );
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class desmModuleInternal : public deInternalModule{
public:
	desmModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DESkin");
		SetDescription("Handles skins in the XML Drag[en]gine skin format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtSkin);
		SetDirectoryName("deskin");
		GetPatternList().Add(".deskin");
		SetDefaultExtension(".deskin");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(DESkinCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *desmRegisterInternalModule(deModuleSystem *system){
	return new desmModuleInternal(system);
}
#endif
