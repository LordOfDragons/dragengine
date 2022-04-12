/* 
 * Drag[en]gine Skin Module
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
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>


// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DESkinCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
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
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
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

bool deSkinModule::IsCompressed( decBaseFileReader& ){
	return false;
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
	
	// load
	try{
		for( i=0; i<root.GetElementCount(); i++ ){
			tag = pGetTagAt( root, i );
			
			if( tag ){
				if( strcmp( tag->GetName(), "texture" ) == 0 ){
					texture = pParseTexture( *tag, basePath, skin );
					
					if( skin.IndexOfTextureNamed( texture->GetName() ) != -1 ){
						LogErrorFormat( "texture(%i:%i): Duplicate texture!",
							tag->GetLineNumber(), tag->GetPositionNumber() );
						DETHROW( deeInvalidParam );
					}
					
					skin.AddTexture( texture );
					texture = NULL;
					
				}else{
					LogWarnFormat( "skin(%i:%i): Unknown Tag %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(),
						tag->GetName().GetString() );
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
				if( strcmp( tag->GetName(), "value" ) == 0 ){
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
					
					texture->AddProperty( propertyValue );
					propertyValue = NULL;
					
				}else if( strcmp( tag->GetName(), "color" ) == 0 ){
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
					
					texture->AddProperty( propertyColor );
					propertyColor = NULL;
					
				}else if( strcmp( tag->GetName(), "image" ) == 0 ){
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
					
					texture->AddProperty( propertyImage );
					propertyImage = NULL;
					
				}else if( strcmp( tag->GetName(), "video" ) == 0 ){
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
					
					if( pFindAttribute( *tag, "sharedTime" ) ){
						propertyVideo->SetSharedTime( pGetAttributeBool( *tag, "sharedTime" ) );
					}
					
					texture->AddProperty( propertyVideo );
					propertyVideo = NULL;
					
				}else if( strcmp( tag->GetName(), "mapped" ) == 0 ){
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
					
					pParsePropertyMapped( *tag, *propertyMapped );
					
					texture->AddProperty( propertyMapped );
					propertyMapped = NULL;
					
				}else if( strcmp( tag->GetName(), "constructed" ) == 0 ){
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
					
					pParsePropertyConstructed( *tag, *propertyConstructed );
					
					texture->AddProperty( propertyConstructed );
					propertyConstructed = NULL;
					
				}else if( strcmp( tag->GetName(), "fullAlpha" ) == 0 ){
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

void deSkinModule::pParsePropertyMapped( const decXmlElementTag &root, deSkinPropertyMapped &property ){
	int i;
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "red" ){
			pParsePropertyMappedComponent( *tag, property.GetRed() );
			
		}else if( tag->GetName() == "green" ){
			pParsePropertyMappedComponent( *tag, property.GetGreen() );
			
		}else if( tag->GetName() == "blue" ){
			pParsePropertyMappedComponent( *tag, property.GetBlue() );
			
		}else if( tag->GetName() == "alpha" ){
			pParsePropertyMappedComponent( *tag, property.GetAlpha() );
			
		}else{
			LogWarnFormat( "mapped(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyMappedComponent( const decXmlElementTag &root,
deSkinPropertyMapped::cComponent &component ){
	int i;
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "curve" ){
			pParsePropertyMappedCurve( *tag, component.GetCurve() );
			
		}else if( tag->GetName() == "inputType" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				continue;
			}
			
			const decString &type = cdata->GetData();
			
			if( type == "time" ){
				component.SetInputType( deSkinPropertyMapped::eitTime );
				
			}else if( type == "bonePositionX" ){
				component.SetInputType( deSkinPropertyMapped::eitBonePositionX );
				
			}else if( type == "bonePositionY" ){
				component.SetInputType( deSkinPropertyMapped::eitBonePositionY );
				
			}else if( type == "bonePositionZ" ){
				component.SetInputType( deSkinPropertyMapped::eitBonePositionZ );
				
			}else if( type == "boneRotationX" ){
				component.SetInputType( deSkinPropertyMapped::eitBoneRotationX );
				
			}else if( type == "boneRotationY" ){
				component.SetInputType( deSkinPropertyMapped::eitBoneRotationY );
				
			}else if( type == "boneRotationZ" ){
				component.SetInputType( deSkinPropertyMapped::eitBoneRotationZ );
				
			}else if( type == "boneScaleX" ){
				component.SetInputType( deSkinPropertyMapped::eitBoneScaleX );
				
			}else if( type == "boneScaleY" ){
				component.SetInputType( deSkinPropertyMapped::eitBoneScaleY );
				
			}else if( type == "boneScaleZ" ){
				component.SetInputType( deSkinPropertyMapped::eitBoneScaleZ );
			}
			
		}else if( tag->GetName() == "inputLower" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				component.SetInputLower( cdata->GetData().ToFloat() );
			}
			
		}else if( tag->GetName() == "inputUpper" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				component.SetInputUpper( cdata->GetData().ToFloat() );
			}
			
		}else if( tag->GetName() == "inputClamped" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				component.SetInputClamped( cdata->GetData() == "true"
					|| cdata->GetData() == "yes" || cdata->GetData() == "1" );
			}
			
		}else if( tag->GetName() == "bone" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				component.SetBone( cdata->GetData() );
			}
			
		}else{
			LogWarnFormat( "constructed(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyMappedCurve( const decXmlElementTag &root, decCurveBezier &curve ){
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
			pParsePropertyMappedCurvePoint( *tag, curve );
			
		}else{
			LogWarnFormat( "curve(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyMappedCurvePoint( const decXmlElementTag &root, decCurveBezier &curve ){
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
			LogWarnFormat( "point(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
	
	curve.AddPoint( decCurveBezierPoint( point, handle1, handle2 ) );
}

void deSkinModule::pParsePropertyConstructed( const decXmlElementTag& root,
deSkinPropertyConstructed& property ){
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
				node = pParsePropertyNode( *tag );
				
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

deSkinPropertyNode *deSkinModule::pParsePropertyNode( const decXmlElementTag &tag ){
	deSkinPropertyNode *node = NULL;
	
	try{
		const decString tagName( tag.GetName() );
		if( tagName == "group" ){
			node = new deSkinPropertyNodeGroup;
			pParsePropertyNodeGroup( tag, *( ( deSkinPropertyNodeGroup* )node ) );
			
		}else if( tagName == "image" ){
			node = new deSkinPropertyNodeImage;
			pParsePropertyNodeImage( tag, *( ( deSkinPropertyNodeImage* )node ) );
			
		}else if( tagName == "shape" ){
			node = new deSkinPropertyNodeShape;
			pParsePropertyNodeShape( tag, *( ( deSkinPropertyNodeShape* )node ) );
			
		}else if( tagName == "text" ){
			node = new deSkinPropertyNodeText;
			pParsePropertyNodeText( tag, *( ( deSkinPropertyNodeText* )node ) );
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

bool deSkinModule::pParsePropertyNodeCommon( const decXmlElementTag &tag, deSkinPropertyNode &node ){
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
				
				const decString tagName( tagMask->GetName() );
				if( tagName == "group" ){
					nodeMask = new deSkinPropertyNodeGroup;
					pParsePropertyNodeGroup( *tagMask, *( ( deSkinPropertyNodeGroup* )nodeMask ) );
					
				}else if( tagName == "image" ){
					nodeMask = new deSkinPropertyNodeImage;
					pParsePropertyNodeImage( *tagMask, *( ( deSkinPropertyNodeImage* )nodeMask ) );
					
				}else if( tagName == "shape" ){
					nodeMask = new deSkinPropertyNodeShape;
					pParsePropertyNodeShape( *tagMask, *( ( deSkinPropertyNodeShape* )nodeMask ) );
					
				}else if( tagName == "text" ){
					nodeMask = new deSkinPropertyNodeText;
					pParsePropertyNodeText( *tagMask, *( ( deSkinPropertyNodeText* )nodeMask ) );
					
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
		
	}else{
		return false;
	}
}

void deSkinModule::pParsePropertyNodeGroup( const decXmlElementTag &root, deSkinPropertyNodeGroup &group ){
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
			
			node = pParsePropertyNode( *tag );
			if( node ){
				group.AddNode( node );
				node = NULL;
				
			}else if( ! pParsePropertyNodeCommon( *tag, group ) ){
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

void deSkinModule::pParsePropertyNodeImage( const decXmlElementTag &root, deSkinPropertyNodeImage &image ){
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
			
		}else if( ! pParsePropertyNodeCommon( *tag, image ) ){
			LogWarnFormat( "image(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyNodeShape( const decXmlElementTag &root, deSkinPropertyNodeShape &shape ){
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
			
		}else if( ! pParsePropertyNodeCommon( *tag, shape ) ){
			LogWarnFormat( "shape(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString() );
		}
	}
}

void deSkinModule::pParsePropertyNodeText( const decXmlElementTag &root, deSkinPropertyNodeText &text ){
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
			
		}else if( ! pParsePropertyNodeCommon( *tag, text ) ){
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
	int t, textureCount = skin.GetTextureCount();
	
	writer.WriteOpeningTag( "skin" );
	
	for( t=0; t<textureCount; t++ ){
		pWriteTexture( writer, skin, *skin.GetTextureAt( t ) );
	}
	
	writer.WriteClosingTag( "skin", true );
}

void deSkinModule::pWriteTexture( decXmlWriter &writer, const deSkin &skin, const deSkinTexture &texture ){
	int p, propertyCount = texture.GetPropertyCount();
	
	writer.WriteOpeningTagStart( "texture" );
	writer.WriteAttributeString( "name", texture.GetName() );
	writer.WriteOpeningTagEnd();
	
	for( p=0; p<propertyCount; p++ ){
		pWriteProperty( writer, *texture.GetPropertyAt( p ) );
	}
	
	writer.WriteClosingTag( "texture", true );
}

void deSkinModule::pWriteProperty( decXmlWriter &writer, deSkinProperty &property ){
	desmWriteProperty writeProperty( writer, property.GetType().GetString() );
	
	property.Visit( writeProperty );
}
