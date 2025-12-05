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

#include "igdeLoadSky.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyLayerBody.h>
#include <dragengine/resources/sky/deSkyLink.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class igdeLoadSky
//////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSky::igdeLoadSky( igdeEnvironment &environment, deLogger *logger,
	const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ),
pEnvironment( environment ),
pName( "Drag[en]gine Sky" ),
pPattern( "*.desky" ),
pDefaultExtension( ".desky" ){
}

igdeLoadSky::~igdeLoadSky(){
}



// Management
///////////////

void igdeLoadSky::SetName( const char *name ){
	pName = name;
}

void igdeLoadSky::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void igdeLoadSky::SetDefaultExtension( const char *extension ){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadSky::Load( const decString &pathSky, deSky &sky, decBaseFileReader &reader ){
	decPath basePath;
	basePath.SetFromUnix( pathSky.GetString() );
	if( basePath.GetComponentCount() > 1 ){
		basePath.RemoveLastComponent();
		
	}else{
		basePath.SetFromUnix( "/" );
	}
	
	decXmlDocument::Ref xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "sky" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadSky( *root, basePath.GetPathUnix(), sky );
}



// Private Functions
//////////////////////

void igdeLoadSky::pReadSky( const decXmlElementTag &root, const char *basePath, deSky &sky ){
	int controllerCount = 0;
	int linkCount = 0;
	int layerCount = 0;
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "controller" ){
			controllerCount++;
			
		}else if( tagName == "link" ){
			linkCount++;
			
		}else if( tagName == "layer" ){
			layerCount++;
		}
	}
	
	sky.SetControllerCount( controllerCount );
	sky.SetLinkCount( linkCount );
	sky.SetLayerCount( layerCount );
	
	int controllerIndex = 0;
	int linkIndex = 0;
	int layerIndex = 0;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "bgColor" ){
			decColor color( 1.0f, 1.0f, 1.0f );
			ReadColor( *tag, color );
			sky.SetBgColor( color );
			
		}else if( tagName == "controller" ){
			pReadController( *tag, sky.GetControllerAt( controllerIndex++ ) );
			
		}else if( tagName == "link" ){
			pReadLink( *tag, sky.GetLinkAt( linkIndex++ ) );
			
		}else if( tagName == "layer" ){
			pReadLayer( *tag, sky, sky.GetLayerAt( layerIndex++ ), basePath );
		}
	}
}

void igdeLoadSky::pReadController( const decXmlElementTag &root, deSkyController &controller ){
	int i;
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "name" ){
			controller.SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "clamp"){
			controller.SetClamp( GetCDataBool( *tag ) );
			
		}else if( tagName == "frozen"){
			controller.SetFrozen( GetCDataBool( *tag ) );
			
		}else if( tagName == "limits"){
			controller.SetValueRange( GetAttributeFloat( *tag, "min" ), GetAttributeFloat( *tag, "max" ) );
		}
	}
}

void igdeLoadSky::pReadLink( const decXmlElementTag &root, deSkyLink &link ){
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "name" ){
			// used only in editor: GetCDataString( *tag )
			
		}else if( tagName == "controller"){
			link.SetController( GetCDataInt( *tag ) );
			
		}else if( tagName == "repeat"){
			link.SetRepeat( GetCDataInt( *tag ) );
			
		}else if( tagName == "curve"){
			ReadCurveBezier( *tag, link.GetCurve() );
		}
	}
}

void igdeLoadSky::pReadLayer( const decXmlElementTag &root, deSky &sky,
deSkyLayer &layer, const char *basePath ){
	int i, bodyCount = 0;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "body"){
			bodyCount++;
		}
	}
	
	layer.SetBodyCount( bodyCount );
	
	int bodyIndex = 0;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "name"){
			// used only in editor: GetCDataString( *tag );
			
		}else if( tagName == "offset"){
			decVector offset;
			ReadVector( *tag, offset );
			layer.SetOffset( offset );
			
		}else if( tagName == "orientation"){
			decVector orientation;
			ReadVector( *tag, orientation );
			layer.SetOrientation( orientation * DEG2RAD );
			
		}else if( tagName == "color"){
			decColor color( 1.0f, 1.0f, 1.0f );
			ReadColor( *tag, color );
			layer.SetColor( color );
			
		}else if( tagName == "intensity"){
			layer.SetIntensity( GetCDataFloat( *tag ) );
			
		}else if( tagName == "transparency"){
			layer.SetTransparency( GetCDataFloat( *tag ) );
			
		}else if( tagName == "mulBySkyLight"){
			layer.SetMultiplyBySkyLight( GetCDataBool( *tag ) );
			
		}else if( tagName == "mulBySkyColor"){
			layer.SetMultiplyBySkyColor( GetCDataBool( *tag ) );
			
		}else if( tagName == "skin"){
			const decString path( GetCDataString( *tag ) );
			
			if( ! path.IsEmpty() ){
				deSkin *skin = NULL;
				
				try{
					skin = sky.GetEngine()->GetSkinManager()->LoadSkin( path, basePath );
					layer.SetSkin( skin );
					skin->FreeReference();
					
				}catch( const deException & ){
					if( skin ){
						skin->FreeReference();
					}
					LogWarnGenericProblemTag( *tag, tag->GetName(),
						"Failed loading resource file" );
				}
			}
			
		}else if( tagName == "lightOrientation"){
			decVector orientation;
			ReadVector( *tag, orientation );
			layer.SetLightOrientation( decQuaternion::CreateFromEuler( orientation * DEG2RAD ) );
			
		}else if( tagName == "lightColor"){
			decColor color( 1.0f, 1.0f, 1.0f );
			ReadColor( *tag, color );
			layer.SetLightColor( color );
			
		}else if( tagName == "lightIntensity"){
			layer.SetLightIntensity( GetCDataFloat( *tag ) );
			
		}else if( tagName == "ambientIntensity"){
			layer.SetAmbientIntensity( GetCDataFloat( *tag ) );
			
		}else if( tagName == "body"){
			pReadBody( *tag, sky, layer.GetBodyAt( bodyIndex++ ), basePath );
			
		}else if( tagName == "target"){
			pReadTarget( *tag, layer );
		}
	}
}

void igdeLoadSky::pReadTarget( const decXmlElementTag &root, deSkyLayer &layer ){
	const decString &type = GetAttributeString( root, "type" );
	deSkyControllerTarget *target = NULL;
	
	if( type == "offsetX" ){
		target = &layer.GetTarget( deSkyLayer::etOffsetX );
		
	}else if( type == "offsetY" ){
		target = &layer.GetTarget( deSkyLayer::etOffsetY );
		
	}else if( type == "offsetZ" ){
		target = &layer.GetTarget( deSkyLayer::etOffsetZ );
		
	}else if( type == "orieintationX" ){
		target = &layer.GetTarget( deSkyLayer::etOrientationX );
		
	}else if( type == "orientationY" ){
		target = &layer.GetTarget( deSkyLayer::etOrientationY );
		
	}else if( type == "orientationZ" ){
		target = &layer.GetTarget( deSkyLayer::etOrientationZ );
		
	}else if( type == "rotationX" ){
		target = &layer.GetTarget( deSkyLayer::etRotationX );
		
	}else if( type == "rotationY" ){
		target = &layer.GetTarget( deSkyLayer::etRotationY );
		
	}else if( type == "rotationZ" ){
		target = &layer.GetTarget( deSkyLayer::etRotationZ );
		
	}else if( type == "colorR" ){
		target = &layer.GetTarget( deSkyLayer::etColorR );
		
	}else if( type == "colorG" ){
		target = &layer.GetTarget( deSkyLayer::etColorG );
		
	}else if( type == "colorB" ){
		target = &layer.GetTarget( deSkyLayer::etColorB );
		
	}else if( type == "intensity" ){
		target = &layer.GetTarget( deSkyLayer::etIntensity );
		
	}else if( type == "transparency" ){
		target = &layer.GetTarget( deSkyLayer::etTransparency );
		
	}else if( type == "lightColorR" ){
		target = &layer.GetTarget( deSkyLayer::etLightColorR );
		
	}else if( type == "lightColorG" ){
		target = &layer.GetTarget( deSkyLayer::etLightColorG );
		
	}else if( type == "lightColorB" ){
		target = &layer.GetTarget( deSkyLayer::etLightColorB );
		
	}else if( type == "lightIntensity" ){
		target = &layer.GetTarget( deSkyLayer::etLightIntensity );
		
	}else if( type == "ambientIntensity" ){
		target = &layer.GetTarget( deSkyLayer::etAmbientIntensity );
		
	}else{
		return;
	}
	
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "link"){
			const int link = GetCDataInt( *tag );
			if( ! target->HasLink( link ) ){
				target->AddLink( link );
			}
		}
	}
}

void igdeLoadSky::pReadBody( const decXmlElementTag &root, deSky &sky,
deSkyLayerBody &body, const char *basePath ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "orientation"){
			decVector orientation;
			ReadVector( *tag, orientation );
			body.SetOrientation( decQuaternion::CreateFromEuler( orientation ) );
			
		}else if( tagName == "size"){
			decVector2 size( 5.0f, 5.0f );
			ReadVector2( *tag, size );
			body.SetSize( size * DEG2RAD );
			
		}else if( tagName == "color"){
			decColor color( 1.0f, 1.0f, 1.0f );
			ReadColor( *tag, color );
			body.SetColor( color );
			
		}else if( tagName == "skin"){
			const decString path( GetCDataString( *tag ) );
			
			if( ! path.IsEmpty() ){
				deSkin *skin = NULL;
				
				try{
					skin = sky.GetEngine()->GetSkinManager()->LoadSkin( path, basePath );
					body.SetSkin( skin );
					skin->FreeReference();
					
				}catch( const deException & ){
					if( skin ){
						skin->FreeReference();
					}
					LogWarnGenericProblemTag( *tag, tag->GetName(),
						"Failed loading resource file" );
				}
			}
		}
	}
}
