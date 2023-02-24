/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seLoadSaveSkin.h"
#include "../skin/seSkin.h"
#include "../skin/texture/seTexture.h"
#include "../skin/property/seProperty.h"
#include "../skin/property/node/sePropertyNodeImage.h"
#include "../skin/property/node/sePropertyNodeGroup.h"
#include "../skin/property/node/sePropertyNodeShape.h"
#include "../skin/property/node/sePropertyNodeText.h"
#include "../skin/seSkinBuilder.h"
#include "../gui/seWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinBuilder.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitorIdentify.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class seLoadSaveSkin
/////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSkin::seLoadSaveSkin( deBaseSkinModule *module ) :
pModule( module )
{
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	for( i=0; i<patternCount; i++ ){
		if( i > 0 ){
			pPattern.AppendCharacter( ',' );
		}
		pPattern.AppendCharacter( '*' );
		pPattern.Append( patternList.GetAt( i ) );
	}
}

seLoadSaveSkin::~seLoadSaveSkin(){
}



// Management
///////////////

void seLoadSaveSkin::SetName( const char *name ){
	pName = name;
}

void seLoadSaveSkin::SetPattern( const char *pattern ){
	pPattern = pattern;
}



void seLoadSaveSkin::LoadSkin( seSkin *skin, decBaseFileReader *file,
const igdeTexturePropertyList &knownPropertyList ){
	if( ! skin || ! file ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine *engine = pModule->GetGameEngine();
	deSkinPropertyVisitorIdentify identifyProperty;
	sePropertyNodeGroup *nodeGroup = NULL;
	seProperty *property = NULL;
	seTexture *texture = NULL;
	deSkin *engSkin = NULL;
	int p, propertyCount;
	int t, textureCount;
	
	deSkinBuilder builder;
	
	try{
		engSkin = engine->GetSkinManager()->CreateSkin( "", builder );
		pModule->LoadSkin( *file, *engSkin );
		
		textureCount = engSkin->GetTextureCount();
		
		for( t=0; t<textureCount; t++ ){
			const deSkinTexture &engTexture = *engSkin->GetTextureAt( t );
			
			texture = new seTexture( engine );
			texture->SetName( engTexture.GetName() );
			
			propertyCount = engTexture.GetPropertyCount();
			
			for( p=0; p<propertyCount; p++ ){
				deSkinProperty &engProperty = *engTexture.GetPropertyAt( p );
				
				property = new seProperty( engine );
				property->SetName( engProperty.GetType().GetString() );
				property->InitDefaults( knownPropertyList );
				
				property->SetRenderableName( engProperty.GetRenderable().GetString() );
				
				engProperty.Visit( identifyProperty );
				
				switch( identifyProperty.GetType() ){
				case deSkinPropertyVisitorIdentify::eptValue:
					property->SetValueType( seProperty::evtValue );
					property->SetValue( identifyProperty.CastToValue().GetValue() );
					break;
					
				case deSkinPropertyVisitorIdentify::eptColor:
					property->SetValueType( seProperty::evtColor );
					property->SetColor( identifyProperty.CastToColor().GetColor() );
					break;
					
				case deSkinPropertyVisitorIdentify::eptImage:
					property->SetValueType( seProperty::evtImage );
					property->SetImagePath( identifyProperty.CastToImage().GetPath() );
					break;
					
				case deSkinPropertyVisitorIdentify::eptVideo:
					property->SetValueType( seProperty::evtVideo );
					property->SetVideoPath( identifyProperty.CastToVideo().GetPath().GetString() );
					property->SetVideoSharedTime( identifyProperty.CastToVideo().GetSharedTime() );
					break;
					
				case deSkinPropertyVisitorIdentify::eptMapped:
					property->SetValueType( seProperty::evtMapped );
					property->SetMappedComponent( 0, identifyProperty.CastToMapped().GetRed() );
					property->SetMappedComponent( 1, identifyProperty.CastToMapped().GetGreen() );
					property->SetMappedComponent( 2, identifyProperty.CastToMapped().GetBlue() );
					property->SetMappedComponent( 3, identifyProperty.CastToMapped().GetAlpha() );
					break;
					
				case deSkinPropertyVisitorIdentify::eptConstructed:{
					const deSkinPropertyConstructed &constructed = identifyProperty.CastToConstructed();
					property->SetValueType( seProperty::evtConstructed );
					property->SetNodeColor( constructed.GetColor() );
					property->SetNodeTileX( constructed.GetTileX() );
					property->SetNodeTileY( constructed.GetTileY() );
					property->SetNodeBitCount( constructed.GetBitCount() );
					nodeGroup = LoadPropertyNodeGroup( constructed.GetContent() );
					property->SetNodeGroup( nodeGroup );
					nodeGroup->FreeReference();
					nodeGroup = NULL;
					}break;
					
				default:
					property->FreeReference();
					property = NULL;
				}
				
				if( property ){
					texture->AddProperty( property );
					property->FreeReference();
					property = NULL;
				}
			}
			
			skin->AddTexture( texture );
			texture->FreeReference();
			texture = NULL;
		}
		
		engSkin->FreeReference();
		
	}catch( const deException & ){
		if( nodeGroup ){
			nodeGroup->FreeReference();
		}
		if( property ){
			property->FreeReference();
		}
		if( texture ){
			texture->FreeReference();
		}
		if( engSkin ){
			engSkin->FreeReference();
		}
		throw;
	}
}

sePropertyNodeGroup *seLoadSaveSkin::LoadPropertyNodeGroup( const deSkinPropertyNodeGroup &engNodeGroup ){
	deEngine &engine = *pModule->GetGameEngine();
	deSkinPropertyNodeVisitorIdentify identifyNode;
	sePropertyNodeGroup *nodeGroup = NULL;
	sePropertyNode *childNode = NULL;
	const int count = engNodeGroup.GetNodeCount();
	int i;
	
	try{
		nodeGroup = new sePropertyNodeGroup( engine );
		LoadPropertyNodeCommon( *nodeGroup, engNodeGroup );
		
		for( i=0; i<count; i++ ){
			childNode = LoadPropertyNode( *engNodeGroup.GetNodeAt( i ) );
			nodeGroup->AddNode( childNode );
			childNode->FreeReference();
			childNode = NULL;
		}
		
	}catch( const deException & ){
		if( childNode ){
			childNode->FreeReference();
		}
		if( nodeGroup ){
			nodeGroup->FreeReference();
		}
		throw;
	}
	
	return nodeGroup;
}

sePropertyNode *seLoadSaveSkin::LoadPropertyNode( deSkinPropertyNode &engNode ){
	deEngine &engine = *pModule->GetGameEngine();
	deSkinPropertyNodeVisitorIdentify identifyNode;
	sePropertyNode *childNode = NULL;
	
	try{
		engNode.Visit( identifyNode );
		
		switch( identifyNode.GetType() ){
		case deSkinPropertyNodeVisitorIdentify::entImage:{
			const deSkinPropertyNodeImage &engNodeImage = identifyNode.CastToImage();
			sePropertyNodeImage * const childImage = new sePropertyNodeImage( engine );
			childNode = childImage;
			LoadPropertyNodeCommon( *childNode, engNode );
			childImage->SetPath( engNodeImage.GetPath() );
			childImage->SetRepeat( engNodeImage.GetRepeat() );
			}break;
			
		case deSkinPropertyNodeVisitorIdentify::entShape:{
			const deSkinPropertyNodeShape &engNodeShape = identifyNode.CastToShape();
			sePropertyNodeShape * const childShape = new sePropertyNodeShape( engine );
			childNode = childShape;
			LoadPropertyNodeCommon( *childNode, engNode );
			childShape->SetShapeType( engNodeShape.GetShapeType() );
			childShape->SetFillColor( engNodeShape.GetFillColor() );
			childShape->SetLineColor( engNodeShape.GetLineColor() );
			childShape->SetThickness( engNodeShape.GetThickness() );
			}break;
			
		case deSkinPropertyNodeVisitorIdentify::entText:{
			const deSkinPropertyNodeText &engNodeText = identifyNode.CastToText();
			sePropertyNodeText * const childText = new sePropertyNodeText( engine );
			childNode = childText;
			LoadPropertyNodeCommon( *childNode, engNode );
			childText->SetPath( engNodeText.GetPath() );
			childText->SetFontSize( engNodeText.GetFontSize() );
			childText->SetText( engNodeText.GetText() );
			childText->SetColor( engNodeText.GetColor() );
			}break;
			
		case deSkinPropertyNodeVisitorIdentify::entGroup:
			childNode = LoadPropertyNodeGroup( identifyNode.CastToGroup() );
			break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
	}catch( const deException & ){
		if( childNode ){
			childNode->FreeReference();
		}
		throw;
	}
	
	return childNode;
}

void seLoadSaveSkin::LoadPropertyNodeCommon( sePropertyNode &node, const deSkinPropertyNode &engNode ){
	node.SetPosition( engNode.GetPosition() );
	node.SetSize( engNode.GetSize() );
	node.SetRotation( engNode.GetRotation() / DEG2RAD );
	node.SetShearing( engNode.GetShear() / DEG2RAD );
	
	node.SetBrightness( engNode.GetBrightness() );
	node.SetContrast( engNode.GetContrast() );
	node.SetGamma( engNode.GetGamma() );
	node.SetColorize( engNode.GetColorize() );
	
	node.SetTransparency( engNode.GetTransparency() );
	node.SetCombineMode( engNode.GetCombineMode() );
	
	if( engNode.GetMask() ){
		sePropertyNode *mask = NULL;
		
		try{
			mask = LoadPropertyNode( *engNode.GetMask() );
			node.SetMask( mask );
			mask->FreeReference();
			
		}catch( const deException & ){
			if( mask ){
				mask->FreeReference();
			}
			throw;
		}
	}
}

void seLoadSaveSkin::SaveSkin( seSkin *skin, decBaseFileWriter *file ){
	if( ! skin || ! file ){
		DETHROW( deeInvalidParam );
	}
	
	// the skin to save does not contain one single skin but actually a bunch of
	// skins assigned to components for quicker editing. for saving we need an
	// entire skin with all texture combined. for this we build a temporary skin
	// containing all textures. since the textures hold the data in their skins
	// already no loading is done and this process should be quick.
	seSkinBuilder builder( *skin );
	deSkin *temporarySkin = NULL;
	
	try{
		temporarySkin = skin->GetEngine()->GetSkinManager()->CreateSkin( "", builder );
		pModule->SaveSkin( *file, *temporarySkin );
		temporarySkin->FreeReference();
		
	}catch( const deException & ){
		if( temporarySkin ){
			temporarySkin->FreeReference();
		}
		throw;
	}
}
