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

#include "gdeLoadSaveGameDefinition.h"
#include "../gamedef/gdeGameDefinition.h"
#include "../gamedef/category/gdeCategory.h"
#include "../gamedef/category/gdeCategoryList.h"
#include "../gamedef/filepattern/gdeFilePattern.h"
#include "../gamedef/filepattern/gdeFilePatternList.h"
#include "../gamedef/objectClass/gdeObjectClass.h"
#include "../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../gamedef/objectClass/camera/gdeOCCamera.h"
#include "../gamedef/objectClass/component/gdeOCComponent.h"
#include "../gamedef/objectClass/component/gdeOCComponentTexture.h"
#include "../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../gamedef/objectClass/light/gdeOCLight.h"
#include "../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"
#include "../gamedef/objectClass/forceField/gdeOCForceField.h"
#include "../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../gamedef/objectClass/world/gdeOCWorld.h"
#include "../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../gamedef/property/gdeProperty.h"
#include "../gamedef/skin/gdeSkin.h"
#include "../gamedef/sky/gdeSky.h"
#include "../gamedef/sky/gdeSkyController.h"
#include "../gui/gdeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/light/deLight.h>



// Class gdeLoadSaveGameDefinition
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeLoadSaveGameDefinition::gdeLoadSaveGameDefinition(
gdeLoadSaveSystem &loadSaveSystem, deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ),
pLoadSaveSystem( loadSaveSystem ),
pName( "Game definition" ),
pPattern( ".degd" ){
}



// Loading and saving
///////////////////////

void gdeLoadSaveGameDefinition::LoadGameDefinition( gdeGameDefinition &gameDefinition,
decBaseFileReader &reader ){
	decXmlDocument::Ref xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root ){
		DETHROW( deeInvalidParam );
	}
	
	if( strcmp( root->GetName(), "gameDefinition" ) != 0 ){
		if( strcmp( root->GetName(), "gameDef" ) != 0 ){ // deprecated version
			DETHROW( deeInvalidParam );
		}
	}
	
	pReadGameDefinition( *root, gameDefinition );
}

void gdeLoadSaveGameDefinition::SaveGameDefinition( const gdeGameDefinition &gameDefinition,
decBaseFileWriter &writer ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteGameDefinition( xmlWriter, gameDefinition );
}



// Private Functions
//////////////////////

void gdeLoadSaveGameDefinition::pReadGameDefinition( const decXmlElementTag &root, gdeGameDefinition &gameDefinition ){
	const int elementCount = root.GetElementCount();
	decStringList autoFindPathObjectClasses;
	decStringList autoFindPathSkins;
	decStringList autoFindPathSkies;
	decStringList baseGameDefIDs;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "identifier" ){
			gameDefinition.SetID( GetCDataString( *tag ) );
			
		}else if( tagName == "description" ){
			gameDefinition.SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "basePath" ){
			gameDefinition.SetBasePath( GetCDataString( *tag ) );
			
		}else if( tagName == "vfsPath" ){
			gameDefinition.SetVFSPath( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "baseGameDefinition" ) == 0 ){
			baseGameDefIDs.Add( GetCDataString( *tag ) );
			
		}else if( tagName == "scriptModule" ){
			gameDefinition.SetScriptModule( GetCDataString( *tag ) );
			
		}else if( tagName == "defaultClass" ){
			gameDefinition.SetDefaultObjectClass( GetCDataString( *tag ) );
			
		}else if( tagName == "defaultSkin" ){
			gameDefinition.SetDefaultSkin( GetCDataString( *tag ) );
			
		}else if( tagName == "defaultSky" ){
			gameDefinition.SetDefaultSky( GetCDataString( *tag ) );
			
		}else if( tagName == "class" ){
			pReadObjectClass( *tag, gameDefinition );
			
		}else if( tagName == "skin" ){
			pReadSkin( *tag, gameDefinition );
			
		}else if( tagName == "sky" ){
			pReadSky( *tag, gameDefinition );
			
		}else if( tagName == "particleEmitter" ){
			pReadParticleEmitter( *tag, gameDefinition );
			
		}else if( tagName == "categories" ){
			pReadCategories( *tag, gameDefinition );
			
		}else if( tagName == "property" ){
			pReadProperty( *tag, gameDefinition.GetWorldProperties() );
			
		}else if( tagName == "decalProperty" ){
			pReadProperty( *tag, gameDefinition.GetDecalProperties() );
			
		}else if( tagName == "findPathClasses" ){
			const char * const cdata = GetCDataString( *tag );
			if( cdata && ! autoFindPathObjectClasses.Has( cdata ) ){
				autoFindPathObjectClasses.Add( cdata );
			}
			
		}else if( tagName == "findPathSkins" ){
			const char * const cdata = GetCDataString( *tag );
			if( cdata && ! autoFindPathSkins.Has( cdata ) ){
				autoFindPathSkins.Add( cdata );
			}
			
		}else if( tagName == "findPathSkies" ){
			const char * const cdata = GetCDataString( *tag );
			if( cdata && ! autoFindPathSkies.Has( cdata ) ){
				autoFindPathSkies.Add( cdata );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	gameDefinition.SetBaseGameDefinitionIDList( baseGameDefIDs );
	gameDefinition.UpdateBaseGameDefinitions( pLoadSaveSystem );
	
	gameDefinition.SetAutoFindPathObjectClasses( autoFindPathObjectClasses );
	gameDefinition.SetAutoFindPathSkins( autoFindPathSkins );
	gameDefinition.SetAutoFindPathSkies( autoFindPathSkies );
	
	gameDefinition.UpdateUsedTagsObjectClass();
	gameDefinition.UpdateDefinedUsedIDs();
}

void gdeLoadSaveGameDefinition::pReadProperty( const decXmlElementTag &root, gdePropertyList &propertyList ){
	const gdeProperty::Ref objRef(gdeProperty::Ref::NewWith(GetAttributeString( root, "name" )));
	gdeProperty &property = ( gdeProperty& )( deObject& )objRef;
	
	pReadProperty( root, property );
	
	propertyList.Add( &property );
}

void gdeLoadSaveGameDefinition::pReadProperty( const decXmlElementTag &root, gdeProperty &property ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "description" ){
			property.SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "type" ){
			const decString type( GetCDataString( *tag ) );
			
			if( type == "string" ){
				property.SetType( gdeProperty::eptString );
				
			}else if( type == "integer" ){
				property.SetType( gdeProperty::eptInteger );
				
			}else if( type == "point2" ){
				property.SetType( gdeProperty::eptPoint2 );
				
			}else if( type == "point3" ){
				property.SetType( gdeProperty::eptPoint3 );
				
			}else if( type == "float" ){
				property.SetType( gdeProperty::eptFloat );
				
			}else if( type == "vector2" ){
				property.SetType( gdeProperty::eptVector2 );
				
			}else if( type == "vector3" ){
				property.SetType( gdeProperty::eptVector3 );
				
			}else if( type == "color" ){
				property.SetType( gdeProperty::eptColor );
				
			}else if( type == "boolean" ){
				property.SetType( gdeProperty::eptBoolean );
				
			}else if( type == "range" ){
				property.SetType( gdeProperty::eptRange );
				
			}else if( type == "selection" ){
				property.SetType( gdeProperty::eptSelect );
				
			}else if( type == "list" ){
				property.SetType( gdeProperty::eptList );
				
			}else if( type == "path" ){
				property.SetType( gdeProperty::eptPath );
				
			}else if( type == "triggerExpression" ){
				property.SetType( gdeProperty::eptTriggerExpression );
				
			}else if( type == "triggerTarget" ){
				property.SetType( gdeProperty::eptTriggerTarget );
				
			}else if( type == "shape" ){
				property.SetType( gdeProperty::eptShape );
				
			}else if( type == "shapeList" ){
				property.SetType( gdeProperty::eptShapeList );
				
			}else if( type == "identifier" ){
				property.SetType( gdeProperty::eptIdentifier );
				
			}else{
				property.SetType( gdeProperty::eptString );
				LogWarnUnknownValue( *tag, type );
			}
			
		}else if( tagName == "range" ){
			property.SetMinimumValue( GetAttributeFloat( *tag, "min" ) );
			property.SetMaximumValue( GetAttributeFloat( *tag, "max" ) );
			
		}else if( tagName == "default" ){
			property.SetDefaultValue( ReadMultilineString( *tag ) );
			
		}else if( tagName == "option" ){
			property.GetOptions().Add( ReadMultilineString( *tag ) );
			
		}else if( tagName == "pathPatternType" ){
			const decString type( GetCDataString( *tag ) );
			
			if( type == "all" ){
				property.SetPathPatternType( gdeProperty::epptAll );
				
			}else if( type == "model" ){
				property.SetPathPatternType( gdeProperty::epptModel );
				
			}else if( type == "skin" ){
				property.SetPathPatternType( gdeProperty::epptSkin );
				
			}else if( type == "rig" ){
				property.SetPathPatternType( gdeProperty::epptRig );
				
			}else if( type == "animation" ){
				property.SetPathPatternType( gdeProperty::epptAnimation );
				
			}else if( type == "animator" ){
				property.SetPathPatternType( gdeProperty::epptAnimator );
				
			}else if( type == "image" ){
				property.SetPathPatternType( gdeProperty::epptImage );
				
			}else if( type == "occlusionMesh" ){
				property.SetPathPatternType( gdeProperty::epptOcclusionMesh );
				
			}else if( type == "navigationSpace" ){
				property.SetPathPatternType( gdeProperty::epptNavigationSpace );
				
			}else if( type == "particleEmitter" ){
				property.SetPathPatternType( gdeProperty::epptParticleEmitter );
				
			}else if( type == "sound" ){
				property.SetPathPatternType( gdeProperty::epptSound );
				
			}else if( type == "synthesizer" ){
				property.SetPathPatternType( gdeProperty::epptSynthesizer );
				
			}else if( type == "video" ){
				property.SetPathPatternType( gdeProperty::epptVideo );
				
			}else if( type == "font" ){
				property.SetPathPatternType( gdeProperty::epptFont );
				
			}else if( type == "sky" ){
				property.SetPathPatternType( gdeProperty::epptSky );
				
			}else if( type == "camera" ){
				property.SetPathPatternType( gdeProperty::epptCamera );
				
			}else if(type == "world"){
				property.SetPathPatternType(gdeProperty::epptWorld);
				
			}else if( type == "custom" ){
				property.SetPathPatternType( gdeProperty::epptCustom );
				
			}else{
				property.SetPathPatternType( gdeProperty::epptAll );
				LogWarnUnknownValue( *tag, type );
			}
			
		}else if( tagName == "customPathPattern" ){
			pReadCustomFilePatternList( *tag, property.GetCustomPathPattern() );
			
		}else if( tagName == "idGroup" ){
			property.SetIdentifierGroup( GetCDataString( *tag ) );
				
		}else if( tagName == "idUsage" ){
			property.SetIdentifierUsage( GetCDataBool( *tag ) );
		
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void gdeLoadSaveGameDefinition::pReadObjectClass( const decXmlElementTag &root, gdeGameDefinition &gameDefinition ){
	const int elementCount = root.GetElementCount();
	decStringSet partialHideTags;
	decStringSet hideTags;
	int i;
	
	const gdeObjectClass::Ref objectClass(gdeObjectClass::Ref::NewWith());
	objectClass->SetName( GetAttributeString( root, "name" ) );
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "description" ){
			objectClass->SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "property" ){
			pReadProperty( *tag, objectClass->GetProperties() );
			
		}else if( tagName == "propertyValue" ){
			objectClass->GetPropertyValues().SetAt( GetAttributeString( *tag, "name" ),
				ReadMultilineString( *tag ) );
			
		}else if( tagName == "textureProperty" ){
			pReadProperty( *tag, objectClass->GetTextureProperties() );
			
		}else if( tagName == "scaleMode" ){
			const decString scaleMode( GetCDataString( *tag ) );
			
			if( scaleMode == "fixed" ){
				objectClass->SetScaleMode( gdeObjectClass::esmFixed );
				
			}else if( scaleMode == "uniform" ){
				objectClass->SetScaleMode( gdeObjectClass::esmUniform );
				
			}else if( scaleMode == "free" ){
				objectClass->SetScaleMode( gdeObjectClass::esmFree );
				
			}else{
				LogWarnUnknownValue( *tag, scaleMode );
			}
			
		}else if( tagName == "component" ){
			pReadObjectClassComponent( *tag, objectClass );
			
		}else if( tagName == "billboard" ){
			pReadObjectClassBillboard( *tag, objectClass );
			
		}else if( tagName == "camera" ){
			pReadObjectClassCamera( *tag, objectClass );
			
		}else if( tagName == "light" ){
			pReadObjectClassLight( *tag, objectClass );
			
		}else if( tagName == "snapPoint" ){
			pReadObjectClassSnapPoint( *tag, objectClass );
			
		}else if( tagName == "particleEmitter" ){
			pReadObjectClassParticleEmitter( *tag, objectClass );
			
		}else if( tagName == "forceField" ){
			pReadObjectClassForceField( *tag, objectClass );
			
		}else if( tagName == "environmentMapProbe" ){
			pReadObjectClassEnvMapProbe( *tag, objectClass );
			
		}else if( tagName == "speaker" ){
			pReadObjectClassSpeaker( *tag, objectClass );
			
		}else if( tagName == "navigationSpace" ){
			pReadObjectClassNavigationSpace( *tag, objectClass );
			
		}else if( tagName == "navigationBlocker" ){
			pReadObjectClassNavigationBlocker( *tag, objectClass );
			
		}else if(tagName == "world"){
			pReadObjectClassWorld(*tag, objectClass);
			
		}else if( tagName == "texture" ){
			pReadObjectClassTexture( *tag, objectClass );
			
		}else if( tagName == "category" ){
			objectClass->SetCategory( GetCDataString( *tag ) );
			
		}else if( tagName == "ghost" ){
			objectClass->SetIsGhost( GetCDataBool( *tag ) );
			
		}else if( tagName == "canInstantiate"
		/* backwards compatibility */ || tagName == "canInstanciate" ){
			objectClass->SetCanInstantiate( GetCDataBool( *tag ) );
			
		}else if( tagName == "attachableBehavior" ){
			objectClass->SetIsAttachableBehavior(GetCDataBool(*tag));
			
		}else if( tagName == "replaceSubObjects" ){
			const decStringList keys( decString( GetCDataString( *tag ) ).Split( ',' ) );
			int j, inherit = igdeGDClass::FilterSubObjectsAll;
			const int keyCount = keys.GetCount();
			
			for( j=0; j<keyCount; j++ ){
				const decString &key = keys.GetAt( j );
				
				if( key == "billboards" ){
					inherit &= ~igdeGDClass::efsoBillboards;
					
				}else if( key == "components" ){
					inherit &= ~igdeGDClass::efsoComponents;
					
				}else if( key == "lights" ){
					inherit &= ~igdeGDClass::efsoLights;
					
				}else if( key == "snapPoints" ){
					inherit &= ~igdeGDClass::efsoSnapPoints;
					
				}else if( key == "particleEmitters" ){
					inherit &= ~igdeGDClass::efsoParticleEmitters;
					
				}else if( key == "forceFields" ){
					inherit &= ~igdeGDClass::efsoForceFields;
					
				}else if( key == "envMapProbes" ){
					inherit &= ~igdeGDClass::efsoEnvMapProbes;
					
				}else if( key == "speakers" ){
					inherit &= ~igdeGDClass::efsoSpeakers;
					
				}else if( key == "navigationSpaces" ){
					inherit &= ~igdeGDClass::efsoNavigationSpaces;
					
				}else if( key == "navigationBlockers" ){
					inherit &= ~igdeGDClass::efsoNavigationBlockers;
					
				}else if( key == "worlds" ){
					inherit &= ~igdeGDClass::efsoWorlds;
				}
			}
			
			objectClass->SetInheritSubObjects( inherit );
			
		}else if( tagName == "inherit" ){
			pReadObjectClassInherit( *tag, objectClass );
			
		}else if( tagName == "defaultInheritPropertyPrefix" ){
			objectClass->SetDefaultInheritPropertyPrefix( GetCDataString( *tag ) );
			
		}else if( tagName == "hideTag" ){
			hideTags.Add( GetCDataString( *tag ) );
			
		}else if( tagName == "partialHideTag" ){
			partialHideTags.Add( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass->SetHideTags( hideTags );
	objectClass->SetPartialHideTags( partialHideTags );
	gameDefinition.AddObjectClass(objectClass);
}

void gdeLoadSaveGameDefinition::pReadObjectClassInherit( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCInherit::Ref inherit(gdeOCInherit::Ref::NewWith(GetAttributeString(root, "name")));
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		if( tagName == "propertyPrefix" ){
			inherit->SetPropertyPrefix( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetInherits().Add(inherit);
}

void gdeLoadSaveGameDefinition::pReadObjectClassBillboard( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCBillboard::Ref billboard(gdeOCBillboard::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "skin" ){
			billboard->SetSkinPath( GetCDataString( *tag ) );
			
		}else if( tagName == "axis" ){
			decVector axis;
			ReadVector( *tag, axis );
			billboard->SetAxis( axis );
			
		}else if( tagName == "size" ){
			decVector2 size;
			ReadVector2( *tag, size );
			billboard->SetSize( size );
			
		}else if( tagName == "offset" ){
			decVector2 offset;
			ReadVector2( *tag, offset );
			billboard->SetOffset( offset );
			
		}else if( tagName == "locked" ){
			billboard->SetLocked( GetCDataBool( *tag ) );
			
		}else if( tagName == "spherical" ){
			billboard->SetSpherical( GetCDataBool( *tag ) );
			
		}else if( tagName == "sizeFixedToScreen" ){
			billboard->SetSizeFixedToScreen( GetCDataBool( *tag ) );
			
		}else if( tagName == "noScaling" ){
			billboard->SetDoNotScale( GetCDataBool( *tag ) );
			
		}else if( tagName == "partialHide" ){
			billboard->SetPartialHide( GetCDataBool( *tag ) );
			
		}else if( tagName == "renderEnvMap" ){
			billboard->SetRenderEnvMap( GetCDataBool( *tag ) );
			
		}else if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			billboard->SetPosition( position );
			
		}else if( tagName == "bone" ){
			billboard->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "link" ){
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "skin" ){
				billboard->SetPropertyName( gdeOCBillboard::epSkin,
					GetAttributeString( *tag, "property" ) );
				
			}else if( value == "axis" ){
				billboard->SetPropertyName( gdeOCBillboard::epAxis,
					GetAttributeString( *tag, "property" ) );
				
			}else if( value == "offset" ){
				billboard->SetPropertyName( gdeOCBillboard::epOffset,
					GetAttributeString( *tag, "property" ) );
				
			}else if( value == "locked" ){
				billboard->SetPropertyName( gdeOCBillboard::epLocked,
					GetAttributeString( *tag, "property" ) );
				
			}else if( value == "spherical" ){
				billboard->SetPropertyName( gdeOCBillboard::epSpherical,
					GetAttributeString( *tag, "property" ) );
				
			}else if( value == "renderEnvMap" ){
				billboard->SetPropertyName( gdeOCBillboard::epRenderEnvMap,
					GetAttributeString( *tag, "property" ) );
				
			}else if( value == "attachPosition" ){
				billboard->SetPropertyName( gdeOCBillboard::epAttachPosition,
					GetAttributeString( *tag, "property" ) );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag  );
		}
	}
	
	objectClass.GetBillboards().Add(billboard);
}

void gdeLoadSaveGameDefinition::pReadObjectClassComponent( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCComponent::Ref component(gdeOCComponent::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "model" ){
			component->SetModelPath( GetCDataString( *tag ) );
			
		}else if( tagName == "skin" ){
			component->SetSkinPath( GetCDataString( *tag ) );
			
		}else if( tagName == "rig" ){
			component->SetRigPath( GetCDataString( *tag ) );
			
		}else if( tagName == "animator" ){
			component->SetAnimatorPath( GetCDataString( *tag ) );
			
		}else if( tagName == "animation" ){
			component->SetAnimationPath( GetCDataString( *tag ) );
			
		}else if( tagName == "move" ){
			component->SetMove( GetCDataString( *tag ) );
			
		}else if( tagName == "occlusionMesh" ){
			component->SetOcclusionMeshPath( GetCDataString( *tag ) );
			
		}else if( tagName == "audioModel" ){
			component->SetAudioModelPath( GetCDataString( *tag ) );
			
		}else if( tagName == "playbackController" ){
			component->SetPlaybackController( GetCDataString( *tag ) );
			
		}else if( tagName == "noScaling" ){
			component->SetDoNotScale( GetCDataBool( *tag ) );
			
		}else if( tagName == "static" ){
			component->SetStatic( GetCDataBool( *tag ) );
			
		}else if( tagName == "partialHide" ){
			component->SetPartialHide( GetCDataBool( *tag ) );
			
		}else if( tagName == "attachTarget" ){
			component->SetAttachTarget( GetCDataBool( *tag ) );
			
		}else if( tagName == "colliderResponseType" ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "static" ){
				component->SetColliderResponseType( deCollider::ertStatic );
				
			}else if( value == "kinematic" ){
				component->SetColliderResponseType( deCollider::ertKinematic );
				
			}else if( value == "dynamic" ){
				component->SetColliderResponseType( deCollider::ertDynamic );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "renderEnvMap" ){
			component->SetRenderEnvMap( GetCDataBool( *tag ) );
			
		}else if( tagName == "affectsAudio" ){
			component->SetAffectsAudio( GetCDataBool( *tag ) );
			
		}else if( tagName == "lightShadowIgnore" ){
			component->SetLightShadowIgnore( GetCDataBool( *tag ) );
			
		}else if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			component->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			component->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			component->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "texture" ){
			pReadObjectClassComponentTexture( *tag, objectClass, component );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "model" ){
				component->SetPropertyName( gdeOCComponent::epModel, property );
				
			}else if( value == "skin" ){
				component->SetPropertyName( gdeOCComponent::epSkin, property );
				
			}else if( value == "rig" ){
				component->SetPropertyName( gdeOCComponent::epRig, property );
				
			}else if( value == "animator" ){
				component->SetPropertyName( gdeOCComponent::epAnimator, property );
				
			}else if( value == "playbackController" ){
				component->SetPropertyName( gdeOCComponent::epPlaybackController, property );
				
			}else if( value == "occlusionMesh" ){
				component->SetPropertyName( gdeOCComponent::epOcclusionMesh, property );
				
			}else if( value == "audioModel" ){
				component->SetPropertyName( gdeOCComponent::epAudioModel, property );
				
			}else if( value == "renderEnvMap" ){
				component->SetPropertyName( gdeOCComponent::epRenderEnvMap, property );
				
			}else if( value == "affectsAudio" ){
				component->SetPropertyName( gdeOCComponent::epAffectsAudio, property );
				
			}else if( value == "lightShadowIgnore" ){
				component->SetPropertyName( gdeOCComponent::epLightShadowIgnore, property );
				
			}else if( value == "attachPosition" ){
				component->SetPropertyName( gdeOCComponent::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				component->SetPropertyName( gdeOCComponent::epAttachRotation, property );
				
			}else if( value == "animation" ){
				component->SetPropertyName( gdeOCComponent::epAnimation, property );
				
			}else if( value == "move" ){
				component->SetPropertyName( gdeOCComponent::epMove, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetComponents().Add(component);
}

void gdeLoadSaveGameDefinition::pReadObjectClassComponentTexture(
const decXmlElementTag &root, gdeObjectClass&, gdeOCComponent &component ){
	const char * const name = GetAttributeString( root, "name" );
	if( component.GetTextures().HasNamed( name ) ){
		LogErrorGenericProblemValue( root, name, "A texture with this name exists already." );
	}
	
	const gdeOCComponentTexture::Ref objRef(gdeOCComponentTexture::Ref::NewWith( name ));
	const int elementCount = root.GetElementCount();
	int i;
	gdeOCComponentTexture &texture = ( gdeOCComponentTexture& )( deObject& )objRef;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "skin" ){
			texture.SetPathSkin( GetCDataString( *tag ) );
			
		}else if( tagName == "offset" ){
			decVector2 offset;
			ReadVector2( *tag, offset );
			texture.SetOffset( offset );
			
		}else if( tagName == "scale" ){
			decVector2 scale;
			ReadVector2( *tag, scale );
			texture.SetScale( scale );
			
		}else if( tagName == "rotate" ){
			texture.SetRotation( GetCDataFloat( *tag ) * DEG2RAD );
			
		}else if( tagName == "tint" ){
			decColor color;
			ReadColor( *tag, color );
			texture.SetColorTint( color );
			
		}else if( tagName == "property" ){
			texture.GetProperties().SetAt( GetAttributeString( *tag, "name" ),
				ReadMultilineString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	component.GetTextures().Add( &texture );
}

void gdeLoadSaveGameDefinition::pReadObjectClassLight( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCLight::Ref light(gdeOCLight::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			light->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			light->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			light->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "color" ){
			decColor color;
			ReadColor( *tag, color );
			light->SetColor( color );
			
		}else if( tagName == "type" ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "point" ){
				light->SetType( deLight::eltPoint );
				
			}else if( value == "spot" ){
				light->SetType( deLight::eltSpot );
				
			}else if( value == "projector" ){
				light->SetType( deLight::eltProjector );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "intensity" ){
			light->SetIntensity( GetCDataFloat( *tag ) );
			
		}else if( tagName == "range" ){
			light->SetRange( GetCDataFloat( *tag ) );
			
		}else if( tagName == "ambientRatio" ){
			light->SetAmbientRatio( GetCDataFloat( *tag ) );
			
		}else if( tagName == "halfIntensityDistance" ){
			light->SetHalfIntensityDistance( GetCDataFloat( *tag ) );
			
		}else if( tagName == "spotAngle" ){
			light->SetSpotAngle( GetCDataFloat( *tag ) );
			
		}else if( tagName == "spotRatio" ){
			light->SetSpotRatio( GetCDataFloat( *tag ) );
			
		}else if( tagName == "spotSmoothness" ){
			light->SetSpotSmoothness( GetCDataFloat( *tag ) );
			
		}else if( tagName == "spotExponent" ){
			light->SetSpotExponent( GetCDataFloat( *tag ) );
			
		}else if( tagName == "shadowOrigin" ){
			// deprecated
			
		}else if( tagName == "shadowGap" ){
			// deprecated
			
		}else if( tagName == "activated" ){
			light->SetActivated( GetCDataBool( *tag ) );
			
		}else if( tagName == "castShadows" ){
			light->SetCastShadows( GetCDataBool( *tag ) );
			
		}else if( tagName == "lightSkin" ){
			light->SetLightSkinPath( GetCDataString( *tag ) );
			
		}else if( tagName == "hintLightImportance" ){
			light->SetHintLightImportance( GetCDataInt( *tag ) );
			
		}else if( tagName == "hintShadowImportance" ){
			light->SetHintShadowImportance( GetCDataInt( *tag ) );
			
		}else if( tagName == "hintMovement" ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "static" ){
				light->SetHintMovement( deLight::emhStationary );
				
			}else if( value == "jittering" ){
				light->SetHintMovement( deLight::emhJittering );
				
			}else if( value == "dynamic" ){
				light->SetHintMovement( deLight::emhDynamic );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "hintParameter" ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "static" ){
				light->SetHintParameter( deLight::ephStatic );
				
			}else if( value == "activation" ){
				light->SetHintParameter( deLight::ephActivation );
				
			}else if( value == "flicker" ){
				light->SetHintParameter( deLight::ephFlicker );
				
			}else if( value == "dynamic" ){
				light->SetHintParameter( deLight::ephDynamic );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "type" ){
				light->SetPropertyName( gdeOCLight::epType, property );
				
			}else if( value == "color" ){
				light->SetPropertyName( gdeOCLight::epColor, property );
				
			}else if( value == "intensity" ){
				light->SetPropertyName( gdeOCLight::epIntensity, property );
				
			}else if( value == "range" ){
				light->SetPropertyName( gdeOCLight::epRange, property );
				
			}else if( value == "ambientRatio" ){
				light->SetPropertyName( gdeOCLight::epAmbientRatio, property );
				
			}else if( value == "halfIntensityDistance" ){
				light->SetPropertyName( gdeOCLight::epHalfIntDist, property );
				
			}else if( value == "spotAngle" ){
				light->SetPropertyName( gdeOCLight::epSpotAngle, property );
				
			}else if( value == "spotRatio" ){
				light->SetPropertyName( gdeOCLight::epSpotRatio, property );
				
			}else if( value == "spotSmoothness" ){
				light->SetPropertyName( gdeOCLight::epSpotSmoothness, property );
				
			}else if( value == "spotExponent" ){
				light->SetPropertyName( gdeOCLight::epSpotExponent, property );
				
			}else if( value == "lightSkin" ){
				light->SetPropertyName( gdeOCLight::epLightSkin, property );
				
			}else if( value == "activated" ){
				light->SetPropertyName( gdeOCLight::epActivated, property );
				
			}else if( value == "castShadows" ){
				light->SetPropertyName( gdeOCLight::epCastShadows, property );
				
			}else if( value == "hintLightImportance" ){
				light->SetPropertyName( gdeOCLight::epHintLightImportance, property );
				
			}else if( value == "hintShadowImportance" ){
				light->SetPropertyName( gdeOCLight::epHintShadowImportance, property );
				
			}else if( value == "attachPosition" ){
				light->SetPropertyName( gdeOCLight::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				light->SetPropertyName( gdeOCLight::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "trigger" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "activated" ){
				light->SetTriggerName( gdeOCLight::etActivated, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetLights().Add(light);
}

void gdeLoadSaveGameDefinition::pReadObjectClassSnapPoint( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCSnapPoint::Ref snapPoint(gdeOCSnapPoint::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "name" ){
			snapPoint->SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			snapPoint->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector rotation;
			ReadVector( *tag, rotation );
			snapPoint->SetRotation( rotation );
			
		}else if( tagName == "snapDistance" ){
			snapPoint->SetSnapDistance( GetCDataFloat( *tag ) );
			
		}else if( tagName == "snapToRotation" ){
			snapPoint->SetSnapToRotation( GetCDataBool( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetSnapPoints().Add(snapPoint);
}

void gdeLoadSaveGameDefinition::pReadObjectClassParticleEmitter( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCParticleEmitter::Ref emitter(gdeOCParticleEmitter::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "path" ){
			emitter->SetPath( GetCDataString( *tag ) );
			
		}else if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			emitter->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			emitter->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			emitter->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "casting" ){
			emitter->SetCasting( GetCDataBool( *tag ) );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "path" ){
				emitter->SetPropertyName( gdeOCParticleEmitter::epPath, property );
				
			}else if( value == "casting" ){
				emitter->SetPropertyName( gdeOCParticleEmitter::epCasting, property );
				
			}else if( value == "attachPosition" ){
				emitter->SetPropertyName( gdeOCParticleEmitter::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				emitter->SetPropertyName( gdeOCParticleEmitter::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "trigger" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "casting" ){
				emitter->SetTriggerName( gdeOCParticleEmitter::etCasting, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetParticleEmitters().Add(emitter);
}

void gdeLoadSaveGameDefinition::pReadObjectClassForceField(
const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCForceField::Ref field(gdeOCForceField::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	igdeCodecPropertyString codec;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			field->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			field->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			field->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "influenceArea" ){
			decShapeList list;
			codec.DecodeShapeList( GetCDataString( *tag ), list );
			field->SetInfluenceArea( list );
			
		}else if( tagName == "radius" ){
			field->SetRadius( GetCDataFloat( *tag ) );
			
		}else if( tagName == "exponent" ){
			field->SetExponent( GetCDataFloat( *tag ) );
			
		}else if( tagName == "fieldType" ){
			const decString cdata( GetCDataString( *tag ) );
			
			if( cdata == "radial" ){
				field->SetFieldType( deForceField::eftRadial );
				
			}else if( cdata == "linear" ){
				field->SetFieldType( deForceField::eftLinear );
				
			}else if( cdata == "vortex" ){
				field->SetFieldType( deForceField::eftVortex );
				
			}else{
				LogWarnUnknownValue( *tag, cdata );
			}
			
		}else if( tagName == "applicationType" ){
			const decString cdata( GetCDataString( *tag ) );
			
			if( cdata == "direct" ){
				field->SetApplicationType( deForceField::eatDirect );
				
			}else if( cdata == "surface" ){
				field->SetApplicationType( deForceField::eatSurface );
				
			}else if( cdata == "mass" ){
				field->SetApplicationType( deForceField::eatMass );
				
			}else if( cdata == "speed" ){
				field->SetApplicationType( deForceField::eatSpeed );
				
			}else{
				LogWarnUnknownValue( *tag, cdata );
			}
			
		}else if( tagName == "direction" ){
			decVector direction;
			ReadVector( *tag, direction );
			field->SetDirection( direction );
			
		}else if( tagName == "force" ){
			field->SetForce( GetCDataFloat( *tag ) );
			
		}else if( tagName == "fluctuationDirection" ){
			field->SetFluctuationDirection( GetCDataFloat( *tag ) );
			
		}else if( tagName == "fluctuationForce" ){
			field->SetFluctuationForce( GetCDataFloat( *tag ) );
			
		}else if( tagName == "shape" ){
			decShapeList list;
			codec.DecodeShapeList( GetCDataString( *tag ), list );
			field->SetShape( list );
			
		}else if( tagName == "enabled" ){
			field->SetEnabled( GetCDataBool( *tag ) );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "influenceArea" ){
				field->SetPropertyName( gdeOCForceField::epInfluenceArea, property );
				
			}else if( value == "radius" ){
				field->SetPropertyName( gdeOCForceField::epRadius, property );
				
			}else if( value == "exponent" ){
				field->SetPropertyName( gdeOCForceField::epExponent, property );
				
			}else if( value == "fieldType" ){
				field->SetPropertyName( gdeOCForceField::epFieldType, property );
				
			}else if( value == "applicationType" ){
				field->SetPropertyName( gdeOCForceField::epApplicationType, property );
				
			}else if( value == "direction" ){
				field->SetPropertyName( gdeOCForceField::epDirection, property );
				
			}else if( value == "force" ){
				field->SetPropertyName( gdeOCForceField::epForce, property );
				
			}else if( value == "fluctuationDirection" ){
				field->SetPropertyName( gdeOCForceField::epFluctuationDirection, property );
				
			}else if( value == "fluctuationForce" ){
				field->SetPropertyName( gdeOCForceField::epFluctuationForce, property );
				
			}else if( value == "shape" ){
				field->SetPropertyName( gdeOCForceField::epShape, property );
				
			}else if( value == "enabled" ){
				field->SetPropertyName( gdeOCForceField::epEnabled, property );
				
			}else if( value == "attachPosition" ){
				field->SetPropertyName( gdeOCForceField::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				field->SetPropertyName( gdeOCForceField::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "trigger" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "enabled" ){
				field->SetTriggerName( gdeOCForceField::etEnabled, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetForceFields().Add(field);
}

void gdeLoadSaveGameDefinition::pReadObjectClassEnvMapProbe( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCEnvMapProbe::Ref envMapProbe(gdeOCEnvMapProbe::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	igdeCodecPropertyString codec;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			envMapProbe->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			envMapProbe->SetRotation( orientation );
			
		}else if( tagName == "scaling" ){
			decVector scaling;
			ReadVector( *tag, scaling );
			envMapProbe->SetScaling( scaling );
			
		}else if( tagName == "influenceArea" ){
			envMapProbe->GetShapeListInfluence().RemoveAll();
			codec.DecodeShapeList( GetCDataString( *tag ), envMapProbe->GetShapeListInfluence() );
			
		}else if( tagName == "reflectionShape" ){
			decShapeList shapeList;
			codec.DecodeShapeList( GetCDataString( *tag ), shapeList );
			
			if( shapeList.GetCount() == 0 ){
				envMapProbe->SetShapeReflection( NULL );
				
			}else{
				envMapProbe->SetShapeReflection( shapeList.GetAt( 0 )->Copy() );
			}
			
		}else if( tagName == "reflectionMask" ){
			envMapProbe->GetShapeListReflectionMask().RemoveAll();
			codec.DecodeShapeList( GetCDataString( *tag ), envMapProbe->GetShapeListReflectionMask() );
			
		}else if( tagName == "influenceBorderSize" ){
			envMapProbe->SetInfluenceBorderSize( GetCDataFloat( *tag ) );
			
		}else if( tagName == "influencePriority" ){
			envMapProbe->SetInfluencePriority( GetCDataInt( *tag ) );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "influenceArea" ){
				envMapProbe->SetPropertyName( gdeOCEnvMapProbe::epInfluenceArea, property );
				
			}else if( value == "influenceBorderSize" ){
				envMapProbe->SetPropertyName( gdeOCEnvMapProbe::epInfluenceBorderSize, property );
				
			}else if( value == "influencePriority" ){
				envMapProbe->SetPropertyName( gdeOCEnvMapProbe::epInfluencePriority, property );
				
			}else if( value == "reflectionShape" ){
				envMapProbe->SetPropertyName( gdeOCEnvMapProbe::epReflectionShape, property );
				
			}else if( value == "reflectionMask" ){
				envMapProbe->SetPropertyName( gdeOCEnvMapProbe::epReflectionMask, property );
				
			}else if( value == "attachPosition" ){
				envMapProbe->SetPropertyName( gdeOCComponent::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				envMapProbe->SetPropertyName( gdeOCComponent::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetEnvMapProbes().Add(envMapProbe);
}

void gdeLoadSaveGameDefinition::pReadObjectClassSpeaker( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCSpeaker::Ref speaker(gdeOCSpeaker::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "sound" ){
			speaker->SetPathSound( GetCDataString( *tag ) );
			
		}else if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			speaker->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			speaker->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			speaker->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "looping" ){
			speaker->SetLooping( GetCDataBool( *tag ) );
			
		}else if( tagName == "playing" ){
			speaker->SetPlaying( GetCDataBool( *tag ) );
			
		}else if( tagName == "volume" ){
			speaker->SetVolume( GetCDataFloat( *tag ) );
			
		}else if( tagName == "range" ){
			speaker->SetRange( GetCDataFloat( *tag ) );
			
		}else if( tagName == "rollOff" ){
			speaker->SetRollOff( GetCDataFloat( *tag ) );
			
		}else if( tagName == "distanceOffset" ){
			speaker->SetDistanceOffset( GetCDataFloat( *tag ) );
			
		}else if( tagName == "playSpeed" ){
			speaker->SetPlaySpeed( GetCDataFloat( *tag ) );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "sound" ){
				speaker->SetPropertyName( gdeOCSpeaker::epSound, property );
				
			}else if( value == "looping" ){
				speaker->SetPropertyName( gdeOCSpeaker::epLooping, property );
				
			}else if( value == "playing" ){
				speaker->SetPropertyName( gdeOCSpeaker::epPlaying, property );
				
			}else if( value == "volume" ){
				speaker->SetPropertyName( gdeOCSpeaker::epVolume, property );
				
			}else if( value == "range" ){
				speaker->SetPropertyName( gdeOCSpeaker::epRange, property );
				
			}else if( value == "rollOff" ){
				speaker->SetPropertyName( gdeOCSpeaker::epRollOff, property );
				
			}else if( value == "distanceOffset" ){
				speaker->SetPropertyName( gdeOCSpeaker::epDistanceOffset, property );
				
			}else if( value == "playSpeed" ){
				speaker->SetPropertyName( gdeOCSpeaker::epPlaySpeed, property );
				
			}else if( value == "attachPosition" ){
				speaker->SetPropertyName( gdeOCSpeaker::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				speaker->SetPropertyName( gdeOCSpeaker::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "trigger" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "playing" ){
				speaker->SetTriggerName( gdeOCSpeaker::etPlaying, property );
				
			}else if( value == "muted" ){
				speaker->SetTriggerName( gdeOCSpeaker::etMuted, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetSpeakers().Add(speaker);
}

void gdeLoadSaveGameDefinition::pReadObjectClassNavigationSpace( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCNavigationSpace::Ref navspace(gdeOCNavigationSpace::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	igdeCodecPropertyString codec;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "path" ){
			navspace->SetPath( GetCDataString( *tag ) );
			
		}else if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			navspace->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			navspace->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			navspace->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "layer" ){
			navspace->SetLayer( GetCDataInt( *tag ) );
			
		}else if( tagName == "type" ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "grid" ){
				navspace->SetType( deNavigationSpace::estGrid );
				
			}else if( value == "mesh" ){
				navspace->SetType( deNavigationSpace::estMesh );
				
			}else if( value == "volume" ){
				navspace->SetType( deNavigationSpace::estVolume );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "blockingPriority" ){
			navspace->SetBlockingPriority( GetCDataInt( *tag ) );
			
		}else if( tagName == "blockerShape" ){
			navspace->GetBlockerShapeList().RemoveAll();
			codec.DecodeShapeList( GetCDataString( *tag ), navspace->GetBlockerShapeList() );
			
		}else if( tagName == "snapDistance" ){
			navspace->SetSnapDistance( GetCDataFloat( *tag ) );
			
		}else if( tagName == "snapAngle" ){
			navspace->SetSnapAngle( GetCDataFloat( *tag ) );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "path" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epPath, property );
				
			}else if( value == "blockerShape" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epBlockerShape, property );
				
			}else if( value == "layer" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epLayer, property );
				
			}else if( value == "blockingPriority" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epBlockingPriority, property );
				
			}else if( value == "snapDistance" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epSnapDistance, property );
				
			}else if( value == "snapAngle" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epSnapAngle, property );
				
			}else if( value == "attachPosition" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				navspace->SetPropertyName( gdeOCNavigationSpace::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetNavigationSpaces().Add(navspace);
}

void gdeLoadSaveGameDefinition::pReadObjectClassNavigationBlocker( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCNavigationBlocker::Ref navblocker(gdeOCNavigationBlocker::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	igdeCodecPropertyString codec;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			navblocker->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			navblocker->SetRotation( orientation );
			
		}else if( tagName == "bone" ){
			navblocker->SetBoneName( GetCDataString( *tag ) );
			
		}else if( tagName == "enabled" ){
			navblocker->SetEnabled( GetCDataBool( *tag ) );
			
		}else if( tagName == "layer" ){
			navblocker->SetLayer( GetCDataInt( *tag ) );
			
		}else if( tagName == "type" ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "grid" ){
				navblocker->SetType( deNavigationSpace::estGrid );
				
			}else if( value == "mesh" ){
				navblocker->SetType( deNavigationSpace::estMesh );
				
			}else if( value == "volume" ){
				navblocker->SetType( deNavigationSpace::estVolume );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( tagName == "blockingPriority" ){
			navblocker->SetBlockingPriority( GetCDataInt( *tag ) );
			
		}else if( tagName == "shape" ){
			navblocker->GetShapeList().RemoveAll();
			codec.DecodeShapeList( GetCDataString( *tag ), navblocker->GetShapeList() );
			
		}else if( tagName == "link" ){
			const char * const property = GetAttributeString( *tag, "property" );
			const decString value( GetAttributeString( *tag, "target" ) );
			
			if( value == "enabled" ){
				navblocker->SetPropertyName( gdeOCNavigationBlocker::epEnabled, property );
				
			}else if( value == "shape" ){
				navblocker->SetPropertyName( gdeOCNavigationBlocker::epShape, property );
				
			}else if( value == "layer" ){
				navblocker->SetPropertyName( gdeOCNavigationBlocker::epLayer, property );
				
			}else if( value == "blockingPriority" ){
				navblocker->SetPropertyName( gdeOCNavigationBlocker::epBlockingPriority, property );
				
			}else if( value == "attachPosition" ){
				navblocker->SetPropertyName( gdeOCNavigationBlocker::epAttachPosition, property );
				
			}else if( value == "attachRotation" ){
				navblocker->SetPropertyName( gdeOCNavigationBlocker::epAttachRotation, property );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetNavigationBlockers().Add(navblocker);
}

void gdeLoadSaveGameDefinition::pReadObjectClassWorld(const decXmlElementTag &root, gdeObjectClass &objectClass){
	const gdeOCWorld::Ref world(gdeOCWorld::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "path"){
			world->SetPath(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			world->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			world->SetRotation(orientation);
			
		}else if(tagName == "link"){
			const decString &property = GetAttributeString(*tag, "property");
			const decString &value = GetAttributeString(*tag, "target");
			
			if(value == "path"){
				world->SetPropertyName(gdeOCWorld::epPath, property);
				
			}else if(value == "position"){
				world->SetPropertyName(gdeOCWorld::epPosition, property);
				
			}else if(value == "rotation"){
				world->SetPropertyName(gdeOCWorld::epRotation, property);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	objectClass.GetWorlds().Add(world);
}

void gdeLoadSaveGameDefinition::pReadObjectClassTexture( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const char * const name = GetAttributeString( root, "name" );
	if( objectClass.GetTextures().HasNamed( name ) ){
		LogErrorGenericProblemValue( root, name, "A texture with this name exists already." );
	}
	
	const gdeOCComponentTexture::Ref objRef(gdeOCComponentTexture::Ref::NewWith( name ));
	const int elementCount = root.GetElementCount();
	int i;
	gdeOCComponentTexture &texture = ( gdeOCComponentTexture& )( deObject& )objRef;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "skin" ){
			texture.SetPathSkin( GetCDataString( *tag ) );
			
		}else if( tagName == "offset" ){
			decVector2 offset;
			ReadVector2( *tag, offset );
			texture.SetOffset( offset );
			
		}else if( tagName == "scale" ){
			decVector2 scale( 1.0f, 1.0f );
			ReadVector2( *tag, scale );
			texture.SetScale( scale );
			
		}else if( tagName == "rotate" ){
			texture.SetRotation( GetCDataFloat( *tag ) * DEG2RAD );
			
		}else if( tagName == "tint" ){
			decColor color( 1.0f, 1.0f, 1.0f );
			ReadColor( *tag, color );
			texture.SetColorTint( color );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetTextures().Add( &texture );
}

void gdeLoadSaveGameDefinition::pReadCustomFilePatternList(
const decXmlElementTag &root, gdeFilePatternList &list ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "add" ){
			gdeFilePattern *pattern = NULL;
			
			try{
				pattern = new gdeFilePattern;
				pattern->SetName( GetAttributeString( *tag, "name" ) );
				pattern->SetPattern( GetAttributeString( *tag, "pattern" ) );
				pattern->SetDefaultExtension( GetAttributeString( *tag, "default" ) );
				
				list.Add( pattern );
				pattern->FreeReference();
				
			}catch( const deException & ){
				if( pattern ){
					pattern->FreeReference();
				}
				throw;
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void gdeLoadSaveGameDefinition::pReadObjectClassCamera( const decXmlElementTag &root, gdeObjectClass &objectClass ){
	const gdeOCCamera::Ref camera(gdeOCCamera::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "position" ){
			decVector position;
			ReadVector( *tag, position );
			camera->SetPosition( position );
			
		}else if( tagName == "orientation" ){
			decVector orientation;
			ReadVector( *tag, orientation );
			camera->SetRotation( orientation );
			
		}else if( tagName == "fov" ){
			camera->SetFov( GetCDataFloat( *tag ) );
			
		}else if( tagName == "fovRatio" ){
			camera->SetFovRatio( GetCDataFloat( *tag ) );
			
		}else if( tagName == "imageDistance" ){
			camera->SetImageDistance( GetCDataFloat( *tag ) );
			
		}else if( tagName == "viewDistance" ){
			camera->SetViewDistance( GetCDataFloat( *tag ) );
			
		}else if(tagName == "propName"){
			camera->SetPropName(GetCDataString(*tag));
			
		}else if(tagName == "propPosition"){
			camera->SetPropPosition(GetCDataString(*tag));
			
		}else if(tagName == "propRotation"){
			camera->SetPropRotation(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	objectClass.GetCameras().Add(camera);
}

void gdeLoadSaveGameDefinition::pReadParticleEmitter( const decXmlElementTag &root, gdeGameDefinition &gameDefinition ){
	const gdeParticleEmitter::Ref emitter(gdeParticleEmitter::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "path" ){
			emitter->SetPath( GetCDataString( *tag ) );
			
		}else if( tagName == "name" ){
			emitter->SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "description" ){
			emitter->SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "category" ){
			emitter->SetCategory( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	if( emitter->GetPath().IsEmpty() ){
		LogErrorMissingTag( root, "path" );
	}
	if( emitter->GetName().IsEmpty() ){
		LogErrorMissingTag( root, "name" );
	}
	
	gameDefinition.AddParticleEmitter(emitter);
}

void gdeLoadSaveGameDefinition::pReadSkin( const decXmlElementTag &root, gdeGameDefinition &gameDefinition ){
	const gdeSkin::Ref skin(gdeSkin::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "path" ){
			skin->SetPath( GetCDataString( *tag ) );
			
		}else if( tagName == "name" ){
			skin->SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "description" ){
			skin->SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "category" ){
			skin->SetCategory( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	if( skin->GetPath().IsEmpty() ){
		LogErrorMissingTag( root, "path" );
	}
	if( skin->GetName().IsEmpty() ){
		LogErrorMissingTag( root, "name" );
	}
	
	gameDefinition.AddSkin(skin);
}

void gdeLoadSaveGameDefinition::pReadSky( const decXmlElementTag &root, gdeGameDefinition &gameDefinition ){
	const gdeSky::Ref sky(gdeSky::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "path" ){
			sky->SetPath( GetCDataString( *tag ) );
			
		}else if( tagName == "name" ){
			sky->SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "description" ){
			sky->SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "category" ){
			sky->SetCategory( GetCDataString( *tag ) );
			
		}else if( tagName == "controller" ){
			gdeSkyController *controller = NULL;
			try{
				controller = new gdeSkyController(
					GetAttributeString( *tag, "name" ), GetCDataFloat( *tag ) );
				sky->AddController( controller );
				controller->FreeReference();
				
			}catch( const deException & ){
				if( controller ){
					controller->FreeReference();
				}
				throw;
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	if( sky->GetPath().IsEmpty() ){
		LogErrorMissingTag( root, "path" );
	}
	if( sky->GetName().IsEmpty() ){
		LogErrorMissingTag( root, "name" );
	}
	
	gameDefinition.AddSky(sky);
}

void gdeLoadSaveGameDefinition::pReadCategories( const decXmlElementTag &root, gdeGameDefinition &gameDefinition ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "classes" ){
			pReadCategories( *tag, &gameDefinition.GetCategoriesObjectClass(), NULL );
			
		}else if( tagName == "skins" ){
			pReadCategories( *tag, &gameDefinition.GetCategoriesSkin(), NULL );
			
		}else if( tagName == "skies" ){
			pReadCategories( *tag, &gameDefinition.GetCategoriesSky(), NULL );
			
		}else if( tagName == "particleEmitters" ){
			pReadCategories( *tag, &gameDefinition.GetCategoriesParticleEmitter(), NULL );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void gdeLoadSaveGameDefinition::pReadCategories( const decXmlElementTag &root,
gdeCategoryList *list, gdeCategory *parent ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "category" ){
			pReadCategory( *tag, list, parent );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void gdeLoadSaveGameDefinition::pReadCategory( const decXmlElementTag &root,
gdeCategoryList *list, gdeCategory *parent ){
	decString categoryName;
	int i;
	
	// first we have to look for the important name tag which is required to construct and add a new category.
	const int elementCount = root.GetElementCount();
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		
		if( tag && decString( tag->GetName() ) == "name" ){
			categoryName = GetCDataString( *tag );
			
			if( list ){
				if( list->HasNamed( categoryName ) ){
					LogErrorGenericProblemValue( *tag, categoryName, "Category with this name exists already" );
				}
				
			}else{
				if( parent->GetCategories().HasNamed( categoryName ) ){
					LogErrorGenericProblemValue( *tag, categoryName, "Category with this name exists already" );
				}
			}
		}
	}
	
	if( categoryName.IsEmpty() ){
		LogErrorMissingTag( root, "name" );
	}
	
	const gdeCategory::Ref objRef(gdeCategory::Ref::NewWith(categoryName));
	gdeCategory &category = ( gdeCategory& )( deObject& )objRef;
	
	if( list ){
		list->Add( &category );
		
	}else{
		parent->AddCategory( &category );
	}
	
	// now we read all the other tags
	decStringSet autoCategorizePattern;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "name" ){
			// we did this already so we can skip it safely.
			// TODO move this into an attribute
			
		}else if( tagName == "description" ){
			category.SetDescription( ReadMultilineString( *tag ) );
			
		}else if( tagName == "autoCategorize" ){
			autoCategorizePattern.Add( GetCDataString( *tag ) );
			
		}else if( tagName == "hidden" ){
			category.SetHidden( GetCDataBool( *tag ) );
			
		}else if( tagName == "category" ){
			pReadCategory( *tag, NULL, &category );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	category.SetAutoCategorizePattern( autoCategorizePattern );
}



void gdeLoadSaveGameDefinition::pWriteGameDefinition( decXmlWriter &writer,
const gdeGameDefinition &gameDefinition ){
	writer.WriteOpeningTag( "gameDefinition" );
	
	writer.WriteDataTagString( "identifier", gameDefinition.GetID() );
	WriteMultilineString( writer, "description", gameDefinition.GetDescription() );
	writer.WriteDataTagString( "basePath", gameDefinition.GetBasePath() );
	writer.WriteDataTagString( "vfsPath", gameDefinition.GetVFSPath() );
	writer.WriteDataTagString( "scriptModule", gameDefinition.GetScriptModule() );
	
	// base game definition identifiers
	const decStringList &baseGameDefIDs = gameDefinition.GetBaseGameDefinitionIDList();
	const int baseGameDefIDCount = baseGameDefIDs.GetCount();
	int i;
	
	for( i=0; i<baseGameDefIDCount; i++ ){
		writer.WriteDataTagString( "baseGameDefinition", baseGameDefIDs.GetAt( i ) );
	}
	
	// categories
	writer.WriteOpeningTag( "categories" );
	pWriteCategories( writer, gameDefinition.GetCategoriesObjectClass(), "classes" );
	pWriteCategories( writer, gameDefinition.GetCategoriesSkin(), "skins" );
	pWriteCategories( writer, gameDefinition.GetCategoriesSky(), "skies" );
	pWriteCategories( writer, gameDefinition.GetCategoriesParticleEmitter(), "particleEmitters" );
	writer.WriteClosingTag( "categories" );
	
	// properties
	pWriteProperties( writer, gameDefinition.GetWorldProperties(), "property" );
	pWriteProperties( writer, gameDefinition.GetDecalProperties(), "decalProperty" );
	
	// auto find path
	const decStringList &autoFindPathObjectClasses = gameDefinition.GetAutoFindPathObjectClasses();
	int autoFindPathCount = autoFindPathObjectClasses.GetCount();
	for( i=0; i<autoFindPathCount; i++ ){
		writer.WriteDataTagString( "findPathClasses", autoFindPathObjectClasses.GetAt( i ) );
	}
	
	const decStringList &autoFindPathSkins = gameDefinition.GetAutoFindPathSkins();
	autoFindPathCount = autoFindPathSkins.GetCount();
	for( i=0; i<autoFindPathCount; i++ ){
		writer.WriteDataTagString( "findPathSkins", autoFindPathSkins.GetAt( i ) );
	}
	
	const decStringList &autoFindPathSkies = gameDefinition.GetAutoFindPathSkies();
	autoFindPathCount = autoFindPathSkies.GetCount();
	for( i=0; i<autoFindPathCount; i++ ){
		writer.WriteDataTagString( "findPathSkies", autoFindPathSkies.GetAt( i ) );
	}
	
	// default
	if( ! gameDefinition.GetDefaultObjectClass().IsEmpty() ){
		writer.WriteDataTagString( "defaultClass", gameDefinition.GetDefaultObjectClass() );
	}
	if( ! gameDefinition.GetDefaultSkin().IsEmpty() ){
		writer.WriteDataTagString( "defaultSkin", gameDefinition.GetDefaultSkin() );
	}
	if( ! gameDefinition.GetDefaultSky().IsEmpty() ){
		writer.WriteDataTagString( "defaultSky", gameDefinition.GetDefaultSky() );
	}
	
	// skies
	const gdeSkyList &skies = gameDefinition.GetSkies();
	const int skyCount = skies.GetCount();
	for( i=0; i<skyCount; i++ ){
		pWriteSky( writer, gameDefinition, *skies.GetAt( i ) );
	}
	
	// skins
	const gdeSkinList &skins = gameDefinition.GetSkins();
	const int skinCount = skins.GetCount();
	for( i=0; i<skinCount; i++ ){
		pWriteSkin( writer, gameDefinition, *skins.GetAt( i ) );
	}
	
	// particle emitters
	const gdeParticleEmitterList &particleEmitters = gameDefinition.GetParticleEmitters();
	const int particleEmitterCount = particleEmitters.GetCount();
	for( i=0; i<particleEmitterCount; i++ ){
		pWriteParticleEmitter( writer, gameDefinition, *particleEmitters.GetAt( i ) );
	}
	
	// object classes
	const gdeObjectClassList &objectClasses = gameDefinition.GetObjectClasses();
	const int objectClassCount = objectClasses.GetCount();
	for( i=0; i<objectClassCount; i++ ){
		pWriteObjectClass( writer, gameDefinition, *objectClasses.GetAt( i ) );
	}
	
	writer.WriteClosingTag( "gameDefinition" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClass( decXmlWriter &writer,
const gdeGameDefinition&, const gdeObjectClass &objectClass ){
	writer.WriteOpeningTagStart( "class" );
	writer.WriteAttributeString( "name", objectClass.GetName() );
	writer.WriteOpeningTagEnd();
	
	WriteMultilineString( writer, "description", objectClass.GetDescription() );
	
	switch( objectClass.GetScaleMode() ){
	case gdeObjectClass::esmFixed:
		writer.WriteDataTagString( "scaleMode", "fixed" );
		break;
		
	case gdeObjectClass::esmUniform:
		break; // default, would be "uniform"
		
	case gdeObjectClass::esmFree:
		writer.WriteDataTagString( "scaleMode", "free" );
		break;
	}
	
	if( objectClass.GetIsGhost() ){
		writer.WriteDataTagBool( "ghost", objectClass.GetIsGhost() );
	}
	if( ! objectClass.GetCanInstantiate() ){
		writer.WriteDataTagBool( "canInstantiate", objectClass.GetCanInstantiate() );
	}
	if(objectClass.GetIsAttachableBehavior()){
		writer.WriteDataTagBool("attachableBehavior", objectClass.GetIsAttachableBehavior());
	}
	if( objectClass.GetInheritSubObjects() != igdeGDClass::FilterSubObjectsAll ){
		decString keys;
		
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoBillboards ) == 0 ){
			keys.Append( "billboards," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoComponents ) == 0 ){
			keys.Append( "components," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoLights ) == 0 ){
			keys.Append( "lights," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoSnapPoints ) == 0 ){
			keys.Append( "snapPoints," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoParticleEmitters ) == 0 ){
			keys.Append( "particleEmitters," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoForceFields ) == 0 ){
			keys.Append( "forceFields," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoEnvMapProbes ) == 0 ){
			keys.Append( "envMapProbes," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoSpeakers ) == 0 ){
			keys.Append( "speakers," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoNavigationSpaces ) == 0 ){
			keys.Append( "navigationSpaces," );
		}
		if( ( objectClass.GetInheritSubObjects() & igdeGDClass::efsoNavigationBlockers ) == 0 ){
			keys.Append( "navigationBlockers," );
		}
		if((objectClass.GetInheritSubObjects() & igdeGDClass::efsoWorlds) == 0){
			keys.Append("worlds,");
		}
		if( ! keys.IsEmpty() ){
			keys.SetAt( -1, 0 );
		}
		writer.WriteDataTagString( "replaceSubObjects", keys );
	}
	
	if( ! objectClass.GetDefaultInheritPropertyPrefix().IsEmpty() ){
		writer.WriteDataTagString( "defaultInheritPropertyPrefix", objectClass.GetDefaultInheritPropertyPrefix() );
	}
	
	// inherit
	const gdeOCInheritList &inherits = objectClass.GetInherits();
	const int inheritCount = inherits.GetCount();
	int i;
	for( i=0; i<inheritCount; i++ ){
		pWriteObjectClassInherit( writer, *inherits.GetAt( i ) );
	}
	
	// components
	const gdeOCComponentList &components = objectClass.GetComponents();
	const int componentCount = components.GetCount();
	for( i=0; i<componentCount; i++ ){
		pWriteObjectClassComponent( writer, *components.GetAt( i ) );
	}
	
	// billboards
	const gdeOCBillboardList &billboards = objectClass.GetBillboards();
	const int billboardCount = billboards.GetCount();
	for( i=0; i<billboardCount; i++ ){
		pWriteObjectClassBillboard( writer, *billboards.GetAt( i ) );
	}
	
	// cameras
	const gdeOCCameraList &cameras = objectClass.GetCameras();
	const int cameraCount = cameras.GetCount();
	for( i=0; i<cameraCount; i++ ){
		pWriteObjectClassCamera( writer, *cameras.GetAt( i ) );
	}
	
	// lights
	const gdeOCLightList &lights = objectClass.GetLights();
	const int lightCount = lights.GetCount();
	for( i=0; i<lightCount; i++ ){
		pWriteObjectClassLight( writer, *lights.GetAt( i ) );
	}
	
	// snap point
	const gdeOCSnapPointList &snapPoints = objectClass.GetSnapPoints();
	const int snapPointCount = snapPoints.GetCount();
	for( i=0; i<snapPointCount; i++ ){
		pWriteObjectClassSnapPoint( writer, *snapPoints.GetAt( i ) );
	}
	
	// particle emitter
	const gdeOCParticleEmitterList &particleEmitters = objectClass.GetParticleEmitters();
	const int particleEmitterCount = particleEmitters.GetCount();
	for( i=0; i<particleEmitterCount; i++ ){
		pWriteObjectClassParticleEmitter( writer, *particleEmitters.GetAt( i ) );
	}
	
	// force fields
	const gdeOCForceFieldList &forceFields = objectClass.GetForceFields();
	const int forceFieldCount = forceFields.GetCount();
	for( i=0; i<forceFieldCount; i++ ){
		pWriteObjectClassForceField( writer, *forceFields.GetAt( i ) );
	}
	
	// environment map probes
	const gdeOCEnvMapProbeList &envMapProbes = objectClass.GetEnvMapProbes();
	const int envMapProbeCount = envMapProbes.GetCount();
	for( i=0; i<envMapProbeCount; i++ ){
		pWriteObjectClassEnvMapProbe( writer, *envMapProbes.GetAt( i ) );
	}
	
	// speakers
	const gdeOCSpeakerList &speakers = objectClass.GetSpeakers();
	const int speakerCount = speakers.GetCount();
	for( i=0; i<speakerCount; i++ ){
		pWriteObjectClassSpeaker( writer, *speakers.GetAt( i ) );
	}
	
	// navigation spaces
	const gdeOCNavigationSpaceList &navSpaces = objectClass.GetNavigationSpaces();
	const int navSpaceCOunt = navSpaces.GetCount();
	for( i=0; i<navSpaceCOunt; i++ ){
		pWriteObjectClassNavSpace( writer, *navSpaces.GetAt( i ) );
	}
	
	// navigation blockers
	const gdeOCNavigationBlockerList &navBlockers = objectClass.GetNavigationBlockers();
	const int navBlockerCount = navBlockers.GetCount();
	for( i=0; i<navBlockerCount; i++ ){
		pWriteObjectClassNavBlocker( writer, *navBlockers.GetAt( i ) );
	}
	
	// worlds
	const gdeOCWorldList &worlds = objectClass.GetWorlds();
	const int worldCount = worlds.GetCount();
	for(i=0; i<worldCount; i++){
		pWriteObjectClassWorld(writer, *worlds.GetAt(i));
	}
	
	// textures
	const gdeOCComponentTextureList &textures = objectClass.GetTextures();
	const int textureCount = textures.GetCount();
	for( i=0; i<textureCount; i++ ){
		pWriteObjectClassTexture( writer, *textures.GetAt( i ) );
	}
	
	pWriteProperties( writer, objectClass.GetProperties(), "property" );
	pWriteProperties( writer, objectClass.GetPropertyValues(), "propertyValue" );
	pWriteProperties( writer, objectClass.GetTextureProperties(), "textureProperty" );
	
	// category and tags
	pWriteTags( writer, objectClass.GetHideTags(), "hideTag" );
	pWriteTags( writer, objectClass.GetPartialHideTags(), "partialHideTag" );
	
	writer.WriteDataTagString( "category", objectClass.GetCategory() );
	
	writer.WriteClosingTag( "class" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassInherit( decXmlWriter &writer, const gdeOCInherit &inherit ){
	writer.WriteOpeningTagStart( "inherit" );
	writer.WriteAttributeString( "name", inherit.GetName() );
	writer.WriteOpeningTagEnd();
	
	if( ! inherit.GetPropertyPrefix().IsEmpty() ){
		writer.WriteDataTagString( "propertyPrefix", inherit.GetPropertyPrefix() );
	}
	
	writer.WriteClosingTag( "inherit" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassComponent(
decXmlWriter &writer, const gdeOCComponent &component ){
	writer.WriteOpeningTag( "component" );
	
	writer.WriteDataTagString( "model", component.GetModelPath() );
	if( ! component.GetSkinPath().IsEmpty() ){
		writer.WriteDataTagString( "skin", component.GetSkinPath() );
	}
	if( ! component.GetRigPath().IsEmpty() ){
		writer.WriteDataTagString( "rig", component.GetRigPath() );
	}
	if( ! component.GetAnimatorPath().IsEmpty() ){
		writer.WriteDataTagString( "animator", component.GetAnimatorPath() );
	}
	if( ! component.GetAnimationPath().IsEmpty() ){
		writer.WriteDataTagString( "animation", component.GetAnimationPath() );
	}
	if( ! component.GetMove().IsEmpty() ){
		writer.WriteDataTagString( "move", component.GetMove() );
	}
	if( ! component.GetOcclusionMeshPath().IsEmpty() ){
		writer.WriteDataTagString( "occlusionMesh", component.GetOcclusionMeshPath() );
	}
	if( ! component.GetAudioModelPath().IsEmpty() ){
		writer.WriteDataTagString( "audioModel", component.GetAudioModelPath() );
	}
	if( ! component.GetPlaybackController().IsEmpty() ){
		writer.WriteDataTagString( "playbackController", component.GetPlaybackController() );
	}
	if( component.GetDoNotScale() ){
		writer.WriteDataTagBool( "noScaling", component.GetDoNotScale() );
	}
	if( component.GetStatic() ){
		writer.WriteDataTagBool( "static", component.GetStatic() );
	}
	if( component.GetPartialHide() ){
		writer.WriteDataTagBool( "partialHide", component.GetPartialHide() );
	}
	if( ! component.GetAttachTarget() ){
		writer.WriteDataTagBool( "attachTarget", component.GetAttachTarget() );
	}
	
	switch( component.GetColliderResponseType() ){
	case deCollider::ertStatic:
		break; // default: "static"
		
	case deCollider::ertKinematic:
		writer.WriteDataTagString( "colliderResponseType", "kinematic" );
		break;
		
	case deCollider::ertDynamic:
		writer.WriteDataTagString( "colliderResponseType", "dynamic" );
		break;
	}
	
	if( ! component.GetRenderEnvMap() ){
		writer.WriteDataTagBool( "renderEnvMap", component.GetRenderEnvMap() );
	}
	if( ! component.GetAffectsAudio() ){
		writer.WriteDataTagBool( "affectsAudio", component.GetAffectsAudio() );
	}
	if( component.GetLightShadowIgnore() ){
		writer.WriteDataTagBool( "lightShadowIgnore", component.GetLightShadowIgnore() );
	}
	
	const decVector &position = component.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = component.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! component.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", component.GetBoneName() );
	}
	
	// textures
	const gdeOCComponentTextureList &textures = component.GetTextures();
	const int textureCount = textures.GetCount();
	int i;
	for( i=0; i<textureCount; i++ ){
		pWriteObjectClassComponentTexture( writer, *textures.GetAt( i ) );
	}
	
	// links
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epModel ),
		"link", "model" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epSkin ),
		"link", "skin" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epRig ),
		"link", "rig" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epAnimator ),
		"link", "animator" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epPlaybackController ),
		"link", "playbackController" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epOcclusionMesh ),
		"link", "occlusionMesh" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epAudioModel ),
		"link", "audioModel" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epRenderEnvMap ),
		"link", "renderEnvMap" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epAffectsAudio ),
		"link", "affectsAudio" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epLightShadowIgnore ),
		"link", "lightShadowIgnore" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epAttachPosition ),
		"link", "attachPosition" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epAttachRotation ),
		"link", "attachRotation" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epAnimation ),
		"link", "animation" );
	pWriteLink( writer, component.GetPropertyName( gdeOCComponent::epMove ),
		"link", "move" );
	
	writer.WriteClosingTag( "component" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassComponentTexture(
decXmlWriter &writer, const gdeOCComponentTexture &texture ){
	writer.WriteOpeningTagStart( "texture" );
	writer.WriteAttributeString( "name", texture.GetName() );
	writer.WriteOpeningTagEnd();
	
	if( ! texture.GetPathSkin().IsEmpty() ){
		writer.WriteDataTagString( "skin", texture.GetPathSkin() );
	}
	
	const decVector2 &offset = texture.GetOffset();
	if( ! offset.IsEqualTo( decVector2() ) ){
		writer.WriteOpeningTagStart( "offset" );
		writer.WriteAttributeFloat( "x", offset.x );
		writer.WriteAttributeFloat( "y", offset.y );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector2 &scale = texture.GetScale();
	if( ! decVector2( 1.0f, 1.0f ).IsEqualTo( scale ) ){
		writer.WriteOpeningTagStart( "scale" );
		writer.WriteAttributeFloat( "x", scale.x );
		writer.WriteAttributeFloat( "y", scale.y );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( fabsf( texture.GetRotation() ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "rotate", texture.GetRotation() / DEG2RAD );
	}
	
	const decColor &tint = texture.GetColorTint();
	if( ! decColor( 1.0f, 1.0f, 1.0f ).IsEqualTo( tint ) ){
		writer.WriteOpeningTagStart( "tint" );
		writer.WriteAttributeFloat( "r", tint.r );
		writer.WriteAttributeFloat( "g", tint.g );
		writer.WriteAttributeFloat( "b", tint.b );
		writer.WriteOpeningTagEnd( true );
	}
	
	pWriteProperties( writer, texture.GetProperties(), "property" );
	
	writer.WriteClosingTag( "texture" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassBillboard(
decXmlWriter &writer, const gdeOCBillboard &billboard ){
	writer.WriteOpeningTag( "billboard" );
	
	writer.WriteDataTagString( "skin", billboard.GetSkinPath() );
	
	const decVector &axis = billboard.GetAxis();
	if( ! decVector( 0.0f, 1.0f, 0.0f ).IsEqualTo( axis ) ){
		writer.WriteOpeningTagStart( "axis" );
		writer.WriteAttributeFloat( "x", axis.x );
		writer.WriteAttributeFloat( "y", axis.y );
		writer.WriteAttributeFloat( "z", axis.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector2 &size = billboard.GetSize();
	writer.WriteOpeningTagStart( "size" );
	writer.WriteAttributeFloat( "x", size.x );
	writer.WriteAttributeFloat( "y", size.y );
	writer.WriteOpeningTagEnd( true );
	
	const decVector2 &offset = billboard.GetOffset();
	if( ! offset.IsEqualTo( decVector2() ) ){
		writer.WriteOpeningTagStart( "offset" );
		writer.WriteAttributeFloat( "x", offset.x );
		writer.WriteAttributeFloat( "y", offset.y );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! billboard.GetLocked() ){
		writer.WriteDataTagBool( "locked", billboard.GetLocked() );
	}
	if( ! billboard.GetSpherical() ){
		writer.WriteDataTagBool( "spherical", billboard.GetSpherical() );
	}
	if( billboard.GetSizeFixedToScreen() ){
		writer.WriteDataTagBool( "sizeFixedToScreen", billboard.GetSizeFixedToScreen() );
	}
	if( billboard.GetDoNotScale() ){
		writer.WriteDataTagBool( "noScaling", billboard.GetDoNotScale() );
	}
	if( billboard.GetPartialHide() ){
		writer.WriteDataTagBool( "partialHide", billboard.GetPartialHide() );
	}
	
	const decVector &position = billboard.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! billboard.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", billboard.GetBoneName() );
	}
	
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epSkin ), "link", "skin" );
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epAxis ), "link", "axis" );
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epOffset ), "link", "offset" );
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epLocked ), "link", "locked" );
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epSpherical ), "link", "spherical" );
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epRenderEnvMap ), "link", "renderEnvMap" );
	pWriteLink( writer, billboard.GetPropertyName( gdeOCBillboard::epAttachPosition ), "link", "attachPosition" );
	
	writer.WriteClosingTag( "billboard" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassCamera(
decXmlWriter &writer, const gdeOCCamera &camera ){
	writer.WriteOpeningTag( "camera" );
	
	const decVector &position = camera.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = camera.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( fabsf( camera.GetFov() - 90.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "fov", camera.GetFov() );
	}
	if( fabsf( camera.GetFovRatio() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "fovRatio", camera.GetFovRatio() );
	}
	if( fabsf( camera.GetImageDistance() - 0.01f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "imageDistance", camera.GetImageDistance() );
	}
	if( fabsf( camera.GetViewDistance() - 500.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "viewDistance", camera.GetViewDistance() );
	}
	if( ! camera.GetPropName().IsEmpty() ){
		writer.WriteDataTagString( "propName", camera.GetPropName() );
	}
	if(!camera.GetPropPosition().IsEmpty()){
		writer.WriteDataTagString("propPosition", camera.GetPropPosition());
	}
	if(!camera.GetPropRotation().IsEmpty()){
		writer.WriteDataTagString("propRotation", camera.GetPropRotation());
	}
	
	writer.WriteClosingTag( "camera" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassLight(
decXmlWriter &writer, const gdeOCLight &light ){
	writer.WriteOpeningTag( "light" );
	
	const decVector &position = light.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = light.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! light.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", light.GetBoneName() );
	}
	
	const decColor &color = light.GetColor();
	writer.WriteOpeningTagStart( "color" );
	writer.WriteAttributeFloat( "r", color.r );
	writer.WriteAttributeFloat( "g", color.g );
	writer.WriteAttributeFloat( "b", color.b );
	writer.WriteOpeningTagEnd( true );
	
	switch( light.GetType() ){
	case deLight::eltPoint:
		break; // default: "point"
		
	case deLight::eltSpot:
		writer.WriteDataTagString( "type", "spot" );
		break;
		
	case deLight::eltProjector:
		writer.WriteDataTagString( "type", "projector" );
		break;
	}
	
	writer.WriteDataTagFloat( "intensity", light.GetIntensity() );
	writer.WriteDataTagFloat( "range", light.GetRange() );
	if( fabsf( light.GetAmbientRatio() ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "ambientRatio", light.GetAmbientRatio() );
	}
	if( fabsf( light.GetHalfIntensityDistance() - 0.1f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "halfIntensityDistance", light.GetHalfIntensityDistance() );
	}
	
	switch( light.GetType() ){
	case deLight::eltPoint:
		break;
		
	case deLight::eltSpot:
	case deLight::eltProjector:
		writer.WriteDataTagFloat( "spotAngle", light.GetSpotAngle() );
		if( fabsf( light.GetSpotRatio() - 1.0f ) > FLOAT_SAFE_EPSILON ){
			writer.WriteDataTagFloat( "spotRatio", light.GetSpotRatio() );
		}
		if( fabsf( light.GetSpotSmoothness() - 1.0f ) > FLOAT_SAFE_EPSILON ){
			writer.WriteDataTagFloat( "spotSmoothness", light.GetSpotSmoothness() );
		}
		if( fabsf( light.GetSpotExponent() - 1.0f ) > FLOAT_SAFE_EPSILON ){
			writer.WriteDataTagFloat( "spotExponent", light.GetSpotExponent() );
		}
		break;
	}
	
	if( ! light.GetActivated() ){
		writer.WriteDataTagBool( "activated", light.GetActivated() );
	}
	if( ! light.GetCastShadows() ){
		writer.WriteDataTagBool( "castShadows", light.GetCastShadows() );
	}
	if( ! light.GetLightSkinPath().IsEmpty() ){
		writer.WriteDataTagString( "lightSkin", light.GetLightSkinPath() );
	}
	
	if( light.GetHintLightImportance() != 100 ){
		writer.WriteDataTagInt( "hintLightImportance", light.GetHintLightImportance() );
	}
	if( light.GetHintShadowImportance() != 100 ){
		writer.WriteDataTagInt( "hintShadowImportance", light.GetHintShadowImportance() );
	}
	
	switch( light.GetHintMovement() ){
	case deLight::emhStationary:
		writer.WriteDataTagString( "hintMovement", "static" );
		break;
		
	case deLight::emhJittering:
		writer.WriteDataTagString( "hintMovement", "jittering" );
		break;
		
	case deLight::emhDynamic:
		break; // default: "dynamic"
	}
	
	switch( light.GetHintParameter() ){
	case deLight::ephStatic:
		writer.WriteDataTagString( "hintParameter", "static" );
		break;
		
	case deLight::ephActivation:
		writer.WriteDataTagString( "hintParameter", "activation" );
		break;
		
	case deLight::ephFlicker:
		writer.WriteDataTagString( "hintParameter", "flicker" );
		break;
		
	case deLight::ephDynamic:
		break; // default: "dynamic"
	}
	
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epType ), "link", "type" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epColor ), "link", "color" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epIntensity ), "link", "intensity" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epRange ), "link", "range" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epAmbientRatio ), "link", "ambientRatio" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epHalfIntDist ), "link", "halfIntensityDistance" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epSpotAngle ), "link", "spotAngle" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epSpotRatio ), "link", "spotRatio" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epSpotSmoothness ), "link", "spotSmoothness" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epSpotExponent ), "link", "spotExponent" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epLightSkin ), "link", "lightSkin" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epActivated ), "link", "activated" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epCastShadows ), "link", "castShadows" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epHintLightImportance ), "link", "hintLightImportance" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epHintShadowImportance ), "link", "hintShadowImportance" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epAttachPosition ), "link", "attachPosition" );
	pWriteLink( writer, light.GetPropertyName( gdeOCLight::epAttachRotation ),"link", "attachRotation" );
	
	pWriteLink( writer, light.GetTriggerName( gdeOCLight::etActivated ), "trigger", "activated" );
	
	writer.WriteClosingTag( "light" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassSnapPoint(
decXmlWriter &writer, const gdeOCSnapPoint &snapPoint ){
	writer.WriteOpeningTag( "snapPoint" );
	
	writer.WriteDataTagString( "name", snapPoint.GetName() );
	
	const decVector &position = snapPoint.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = snapPoint.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	writer.WriteDataTagFloat( "snapDistance", snapPoint.GetSnapDistance() );
	
	if( ! snapPoint.GetSnapToRotation() ){
		writer.WriteDataTagBool( "snapToRotation", snapPoint.GetSnapToRotation() );
	}
	
	writer.WriteClosingTag( "snapPoint" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassParticleEmitter(
decXmlWriter &writer, const gdeOCParticleEmitter &particleEmitter ){
	writer.WriteOpeningTag( "particleEmitter" );
	
	if( ! particleEmitter.GetPath().IsEmpty() ){
		writer.WriteDataTagString( "path", particleEmitter.GetPath() );
	}
	
	const decVector &position = particleEmitter.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = particleEmitter.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! particleEmitter.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", particleEmitter.GetBoneName() );
	}
	
	if( ! particleEmitter.GetCasting() ){
		writer.WriteDataTagBool( "casting", particleEmitter.GetCasting() );
	}
	
	pWriteLink( writer, particleEmitter.GetPropertyName( gdeOCParticleEmitter::epPath ), "link", "path" );
	pWriteLink( writer, particleEmitter.GetPropertyName( gdeOCParticleEmitter::epCasting ), "link", "casting" );
	pWriteLink( writer, particleEmitter.GetPropertyName( gdeOCParticleEmitter::epAttachPosition ), "link", "attachPosition" );
	pWriteLink( writer, particleEmitter.GetPropertyName( gdeOCParticleEmitter::epAttachRotation ),  "link", "attachRotation" );
	
	pWriteLink( writer, particleEmitter.GetTriggerName( gdeOCParticleEmitter::etCasting ), "trigger", "casting" );
	
	writer.WriteClosingTag( "particleEmitter" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassForceField(
decXmlWriter &writer, const gdeOCForceField &forceField ){
	igdeCodecPropertyString codec;
	
	writer.WriteOpeningTag( "forceField" );
	
	const decVector &position = forceField.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = forceField.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! forceField.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", forceField.GetBoneName() );
	}
	
	if( forceField.GetInfluenceArea().GetCount() > 0 ){
		decString value;
		codec.EncodeShapeList( forceField.GetInfluenceArea(), value );
		writer.WriteDataTagString( "influenceArea", value );
	}
	
	if( fabsf( forceField.GetRadius() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "radius", forceField.GetRadius() );
	}
	if( fabsf( forceField.GetExponent() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "exponent", forceField.GetExponent() );
	}
	
	switch( forceField.GetFieldType() ){
	case deForceField::eftRadial:
		//writer.WriteDataTagString( "fieldType", "radial" );
		break;
		
	case deForceField::eftLinear:
		writer.WriteDataTagString( "fieldType", "linear" );
		break;
		
	case deForceField::eftVortex:
		writer.WriteDataTagString( "fieldType", "vortex" );
		break;
	}
	
	switch( forceField.GetApplicationType() ){
	case deForceField::eatDirect:
		//writer.WriteDataTagString( "applicationType", "direct" );
		break;
		
	case deForceField::eatSurface:
		writer.WriteDataTagString( "applicationType", "surface" );
		break;
		
	case deForceField::eatMass:
		writer.WriteDataTagString( "applicationType", "mass" );
		break;
		
	case deForceField::eatSpeed:
		writer.WriteDataTagString( "applicationType", "speed" );
		break;
	}
	
	const decVector &direction = forceField.GetDirection();
	if( ! direction.IsZero() ){
		writer.WriteOpeningTagStart( "direction" );
		writer.WriteAttributeFloat( "x", direction.x );
		writer.WriteAttributeFloat( "y", direction.y );
		writer.WriteAttributeFloat( "z", direction.z );
		writer.WriteOpeningTagEnd( true );
		
	}
	
	if( fabsf( forceField.GetForce() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "force", forceField.GetForce() );
	}
	if( fabsf( forceField.GetFluctuationDirection() ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "fluctuationDirection", forceField.GetFluctuationDirection() );
	}
	if( fabsf( forceField.GetFluctuationForce() ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "fluctuationForce", forceField.GetFluctuationForce() );
	}
	
	if( forceField.GetShape().GetCount() > 0 ){
		decString value;
		codec.EncodeShapeList( forceField.GetShape(), value );
		writer.WriteDataTagString( "shape", value );
	}
	
	if( ! forceField.GetEnabled() ){
		writer.WriteDataTagBool( "enabled", forceField.GetEnabled() );
	}
	
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epInfluenceArea ), "link", "influenceArea" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epRadius ), "link", "radius" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epExponent ), "link", "exponent" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epFieldType ),  "link", "fieldType" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epApplicationType ),  "link", "applicationType" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epDirection ),  "link", "direction" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epForce ),  "link", "force" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epFluctuationDirection ),  "link", "fluctuationDirection" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epFluctuationForce ),  "link", "fluctuationForce" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epShape ),  "link", "shape" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epEnabled ),  "link", "enabled" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epAttachPosition ),  "link", "attachPosition" );
	pWriteLink( writer, forceField.GetPropertyName( gdeOCForceField::epAttachRotation ),  "link", "attachRotation" );
	
	pWriteLink( writer, forceField.GetTriggerName( gdeOCForceField::etEnabled ), "trigger", "enabled" );
	
	writer.WriteClosingTag( "forceField" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassEnvMapProbe(
decXmlWriter &writer, const gdeOCEnvMapProbe &envMapProbe ){
	igdeCodecPropertyString codec;
	
	writer.WriteOpeningTag( "environmentMapProbe" );
	
	const decVector &position = envMapProbe.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = envMapProbe.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &scaling = envMapProbe.GetScaling();
	if( ! scaling.IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) ) ){
		writer.WriteOpeningTagStart( "scaling" );
		writer.WriteAttributeFloat( "x", scaling.x );
		writer.WriteAttributeFloat( "y", scaling.y );
		writer.WriteAttributeFloat( "z", scaling.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( envMapProbe.GetShapeListInfluence().GetCount() > 0 ){
		decString value;
		codec.EncodeShapeList( envMapProbe.GetShapeListInfluence(), value );
		writer.WriteDataTagString( "influenceArea", value );
	}
	
	if( envMapProbe.GetShapeReflection() ){
		decString value;
		decShapeList shapeList;
		shapeList.Add( envMapProbe.GetShapeReflection()->Copy() );
		codec.EncodeShapeList( shapeList, value );
		writer.WriteDataTagString( "reflectionShape", value );
	}
	
	if( envMapProbe.GetShapeListReflectionMask().GetCount() > 0 ){
		decString value;
		codec.EncodeShapeList( envMapProbe.GetShapeListReflectionMask(), value );
		writer.WriteDataTagString( "reflectionMask", value );
	}
	
	if( fabsf( envMapProbe.GetInfluenceBorderSize() - 0.1f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "influenceBorderSize", envMapProbe.GetInfluenceBorderSize() );
	}
	if( envMapProbe.GetInfluencePriority() != 0 ){
		writer.WriteDataTagInt( "influencePriority", envMapProbe.GetInfluencePriority() );
	}
	
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epInfluenceArea ),
		"link", "influenceArea" );
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epInfluenceBorderSize ),
		"link", "influenceBorderSize" );
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epInfluencePriority ),
		"link", "influencePriority" );
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epReflectionShape ),
		"link", "reflectionShape" );
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epReflectionMask ),
		"link", "reflectionMask" );
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epAttachPosition ),
		"link", "attachPosition" );
	pWriteLink( writer, envMapProbe.GetPropertyName( gdeOCEnvMapProbe::epAttachRotation ),
		"link", "attachRotation" );
	
	writer.WriteClosingTag( "environmentMapProbe" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassSpeaker(
decXmlWriter &writer, const gdeOCSpeaker &speaker ){
	writer.WriteOpeningTag( "speaker" );
	
	if( ! speaker.GetPathSound().IsEmpty() ){
		writer.WriteDataTagString( "sound", speaker.GetPathSound() );
	}
	
	const decVector &position = speaker.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = speaker.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! speaker.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", speaker.GetBoneName() );
	}
	
	if( ! speaker.GetLooping() ){
		writer.WriteDataTagBool( "looping", speaker.GetLooping() );
	}
	if( ! speaker.GetPlaying() ){
		writer.WriteDataTagBool( "playing", speaker.GetPlaying() );
	}
	
	if( fabsf( speaker.GetVolume() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "volume", speaker.GetVolume() );
	}
	if( fabsf( speaker.GetRange() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "range", speaker.GetRange() );
	}
	if( fabsf( speaker.GetRollOff() - 0.1f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "rollOff", speaker.GetRollOff() );
	}
	if( fabsf( speaker.GetDistanceOffset() - 0.1f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "distanceOffset", speaker.GetDistanceOffset() );
	}
	if( fabsf( speaker.GetPlaySpeed() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "playSpeed", speaker.GetPlaySpeed() );
	}
	
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epSound ),
		"link", "sound" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epLooping ),
		"link", "looping" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epPlaying ),
		"link", "playing" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epVolume ),
		"link", "volume" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epRange ),
		"link", "range" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epRollOff ),
		"link", "rollOff" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epDistanceOffset ),
		"link", "distanceOffset" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epPlaySpeed ),
		"link", "playSpeed" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epAttachPosition ),
		"link", "attachPosition" );
	pWriteLink( writer, speaker.GetPropertyName( gdeOCSpeaker::epAttachRotation ),
		"link", "attachRotation" );
	
	pWriteLink( writer, speaker.GetTriggerName( gdeOCSpeaker::etPlaying ),
		"trigger", "playing" );
	pWriteLink( writer, speaker.GetTriggerName( gdeOCSpeaker::etMuted ),
		"trigger", "muted" );
	
	writer.WriteClosingTag( "speaker" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassNavSpace(
decXmlWriter &writer, const gdeOCNavigationSpace &navspace ){
	igdeCodecPropertyString codec;
	
	writer.WriteOpeningTag( "navigationSpace" );
	
	if( ! navspace.GetPath().IsEmpty() ){
		writer.WriteDataTagString( "path", navspace.GetPath() );
	}
	
	const decVector &position = navspace.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = navspace.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! navspace.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", navspace.GetBoneName() );
	}
	
	if( navspace.GetLayer() != 0 ){
		writer.WriteDataTagInt( "layer", navspace.GetLayer() );
	}
	
	switch( navspace.GetType() ){
	case deNavigationSpace::estGrid:
		writer.WriteDataTagString( "type", "grid" );
		break;
		
	case deNavigationSpace::estMesh:
		break; // default: "mesh"
		
	case deNavigationSpace::estVolume:
		writer.WriteDataTagString( "type", "volume" );
		break;
	}
	
	if( navspace.GetBlockingPriority() != 0 ){
		writer.WriteDataTagInt( "blockingPriority", navspace.GetBlockingPriority() );
	}
	if( navspace.GetBlockerShapeList().GetCount() > 0 ){
		decString value;
		codec.EncodeShapeList( navspace.GetBlockerShapeList(), value );
		writer.WriteDataTagString( "blockerShape", value );
	}
	if( fabsf( navspace.GetSnapDistance() - 0.001f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "snapDistance", navspace.GetSnapDistance() );
	}
	if( fabsf( navspace.GetSnapAngle() - 180.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "snapAngle", navspace.GetSnapAngle() );
	}
	
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epPath ), "link", "path" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epBlockerShape ), "link", "blockerShape" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epLayer ), "link", "layer" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epBlockingPriority ), "link", "blockingPriority" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epSnapDistance ), "link", "snapDistance" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epSnapAngle ), "link", "snapAngle" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epAttachPosition ), "link", "attachPosition" );
	pWriteLink( writer, navspace.GetPropertyName( gdeOCNavigationSpace::epAttachRotation ),"link", "attachRotation" );
	
	writer.WriteClosingTag( "navigationSpace" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassNavBlocker(
decXmlWriter &writer, const gdeOCNavigationBlocker &navblocker ){
	igdeCodecPropertyString codec;
	
	writer.WriteOpeningTag( "navigationBlocker" );
	
	const decVector &position = navblocker.GetPosition();
	if( ! position.IsZero() ){
		writer.WriteOpeningTagStart( "position" );
		writer.WriteAttributeFloat( "x", position.x );
		writer.WriteAttributeFloat( "y", position.y );
		writer.WriteAttributeFloat( "z", position.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	const decVector &orientation = navblocker.GetRotation();
	if( ! orientation.IsZero() ){
		writer.WriteOpeningTagStart( "orientation" );
		writer.WriteAttributeFloat( "x", orientation.x );
		writer.WriteAttributeFloat( "y", orientation.y );
		writer.WriteAttributeFloat( "z", orientation.z );
		writer.WriteOpeningTagEnd( true );
	}
	
	if( ! navblocker.GetBoneName().IsEmpty() ){
		writer.WriteDataTagString( "bone", navblocker.GetBoneName() );
	}
	
	if( ! navblocker.GetEnabled() ){
		writer.WriteDataTagBool( "enabled", navblocker.GetEnabled() );
	}
	if( navblocker.GetLayer() != 0 ){
		writer.WriteDataTagInt( "layer", navblocker.GetLayer() );
	}
	
	switch( navblocker.GetType() ){
	case deNavigationSpace::estGrid:
		writer.WriteDataTagString( "type", "grid" );
		break;
		
	case deNavigationSpace::estMesh:
		break; // default: "mesh"
		
	case deNavigationSpace::estVolume:
		writer.WriteDataTagString( "type", "volume" );
		break;
	}
	
	if( navblocker.GetBlockingPriority() != 0 ){
		writer.WriteDataTagInt( "blockingPriority", navblocker.GetBlockingPriority() );
	}
	if( navblocker.GetShapeList().GetCount() > 0 ){
		decString value;
		codec.EncodeShapeList( navblocker.GetShapeList(), value );
		writer.WriteDataTagString( "shape", value );
	}
	
	pWriteLink( writer, navblocker.GetPropertyName( gdeOCNavigationBlocker::epEnabled ), "link", "enabled" );
	pWriteLink( writer, navblocker.GetPropertyName( gdeOCNavigationBlocker::epShape ), "link", "shape" );
	pWriteLink( writer, navblocker.GetPropertyName( gdeOCNavigationBlocker::epLayer ), "link", "layer" );
	pWriteLink( writer, navblocker.GetPropertyName( gdeOCNavigationBlocker::epBlockingPriority ), "link", "blockingPriority" );
	pWriteLink( writer, navblocker.GetPropertyName( gdeOCNavigationBlocker::epAttachPosition ), "link", "attachPosition" );
	pWriteLink( writer, navblocker.GetPropertyName( gdeOCNavigationBlocker::epAttachRotation ),"link", "attachRotation" );
	
	writer.WriteClosingTag( "navigationBlocker" );
}

void gdeLoadSaveGameDefinition::pWriteObjectClassWorld(decXmlWriter &writer, const gdeOCWorld &world){
	writer.WriteOpeningTag("world");
	
	if(!world.GetPath().IsEmpty() ){
		writer.WriteDataTagString("path", world.GetPath());
	}
	if(!world.GetPosition().IsZero()){
		WriteVector(writer, "position", world.GetPosition());
	}
	if(!world.GetRotation().IsZero()){
		WriteVector(writer, "orientation", world.GetRotation());
	}
	
	pWriteLink(writer, world.GetPropertyName(gdeOCWorld::epPath), "link", "path");
	pWriteLink(writer, world.GetPropertyName(gdeOCWorld::epPosition), "link", "position");
	pWriteLink(writer, world.GetPropertyName(gdeOCWorld::epRotation), "link", "rotation");
	
	writer.WriteClosingTag("world");
}

void gdeLoadSaveGameDefinition::pWriteObjectClassTexture( decXmlWriter &writer, const gdeOCComponentTexture &texture ){
	writer.WriteOpeningTagStart( "texture" );
	writer.WriteAttributeString( "name", texture.GetName() );
	writer.WriteOpeningTagEnd();
	
	if( ! texture.GetPathSkin().IsEmpty() ){
		writer.WriteDataTagString( "skin", texture.GetPathSkin() );
	}
	if( ! texture.GetOffset().IsEqualTo( decVector2() ) ){
		WriteVector2( writer, "offset", texture.GetOffset() );
	}
	if( ! decVector2( 1.0f, 1.0f ).IsEqualTo( texture.GetScale() ) ){
		WriteVector2( writer, "scale", texture.GetScale() );
	}
	if( fabsf( texture.GetRotation() ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "rotate", texture.GetRotation() / DEG2RAD );
	}
	if( ! decColor( 1.0f, 1.0f, 1.0f ).IsEqualTo( texture.GetColorTint() ) ){
		WriteColor( writer, "tint", texture.GetColorTint() );
	}
	
	writer.WriteClosingTag( "texture" );
}

void gdeLoadSaveGameDefinition::pWriteSkin( decXmlWriter &writer,
const gdeGameDefinition&, const gdeSkin &skin ){
	writer.WriteOpeningTag( "skin" );
	
	writer.WriteDataTagString( "path", skin.GetPath() );
	writer.WriteDataTagString( "name", skin.GetName() );
	if( ! skin.GetDescription().IsEmpty() ){
		WriteMultilineString( writer, "description", skin.GetDescription() );
	}
	writer.WriteDataTagString( "category", skin.GetCategory() );
	
	writer.WriteClosingTag( "skin" );
}

void gdeLoadSaveGameDefinition::pWriteSky( decXmlWriter &writer,
const gdeGameDefinition&, const gdeSky &sky ){
	writer.WriteOpeningTag( "sky" );
	
	writer.WriteDataTagString( "path", sky.GetPath() );
	writer.WriteDataTagString( "name", sky.GetName() );
	if( ! sky.GetDescription().IsEmpty() ){
		WriteMultilineString( writer, "description", sky.GetDescription() );
	}
	
	const gdeSkyControllerList &controllers = sky.GetControllers();
	const int controllerCount = controllers.GetCount();
	int i;
	for( i=0; i<controllerCount; i++ ){
		const gdeSkyController &controller = *controllers.GetAt( i );
		writer.WriteOpeningTagStart( "controller" );
		writer.WriteAttributeString( "name", controller.GetName() );
		writer.WriteOpeningTagEnd( false, false );
		writer.WriteTextFloat( controller.GetValue() );
		writer.WriteClosingTag( "controller", false );
	}
	
	writer.WriteDataTagString( "category", sky.GetCategory() );
	
	writer.WriteClosingTag( "sky" );
}

void gdeLoadSaveGameDefinition::pWriteParticleEmitter( decXmlWriter &writer,
const gdeGameDefinition&, const gdeParticleEmitter &particleEmitter ){
	writer.WriteOpeningTag( "particleEmitter" );
	
	writer.WriteDataTagString( "path", particleEmitter.GetPath() );
	writer.WriteDataTagString( "name", particleEmitter.GetName() );
	if( ! particleEmitter.GetDescription().IsEmpty() ){
		WriteMultilineString( writer, "description", particleEmitter.GetDescription() );
	}
	writer.WriteDataTagString( "category", particleEmitter.GetCategory() );
	
	writer.WriteClosingTag( "particleEmitter" );
}

void gdeLoadSaveGameDefinition::pWriteLink( decXmlWriter &writer,
const decString &link, const char *tagName, const char *target ){
	if( link.IsEmpty() ){
		return;
	}
	
	writer.WriteOpeningTagStart( tagName );
	writer.WriteAttributeString( "target", target );
	writer.WriteAttributeString( "property", link );
	writer.WriteOpeningTagEnd( true );
}

void gdeLoadSaveGameDefinition::pWriteTags( decXmlWriter &writer,
const decStringSet &tags, const char *tagName ){
	const int count = tags.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		writer.WriteDataTagString( tagName, tags.GetAt( i ) );
	}
}

void gdeLoadSaveGameDefinition::pWriteCategories( decXmlWriter &writer,
const gdeCategoryList &categories, const char *name ){
	const int categoryCount = categories.GetCount();
	if( categoryCount == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( name );
	
	int i;
	for( i=0; i<categoryCount; i++ ){
		pWriteCategory( writer, *categories.GetAt( i ) );
	}
	
	writer.WriteClosingTag( name );
}

void gdeLoadSaveGameDefinition::pWriteCategory( decXmlWriter &writer, const gdeCategory &category ){
	writer.WriteOpeningTag( "category" );
	
	writer.WriteDataTagString( "name", category.GetName() ); // mvoe to attribute?
	if( ! category.GetDescription().IsEmpty() ){
		WriteMultilineString( writer, "description", category.GetDescription() );
	}
	
	const decStringSet &autoCategorizePattern = category.GetAutoCategorizePattern();
	const int patternCount = autoCategorizePattern.GetCount();
	int i;
	for( i=0; i<patternCount; i++ ){
		writer.WriteDataTagString( "autoCategorize", autoCategorizePattern.GetAt( i ) );
	}
	
	if( category.GetHidden() ){
		writer.WriteDataTagBool( "hidden", category.GetHidden() );
	}
	
	const gdeCategoryList &categories = category.GetCategories();
	const int categoryCount = categories.GetCount();
	for( i=0; i<categoryCount; i++ ){
		pWriteCategory( writer, *categories.GetAt( i ) );
	}
	
	writer.WriteClosingTag( "category" );
}

void gdeLoadSaveGameDefinition::pWriteProperties( decXmlWriter &writer,
const decStringDictionary &properties, const char *tagName ){
	const decStringList keys( properties.GetKeys() );
	const int count = keys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		writer.WriteOpeningTagStart( tagName );
		writer.WriteAttributeString( "name", keys.GetAt( i ) );
		writer.WriteOpeningTagEnd( false, false );
		WriteTextMultilineString( writer, properties.GetAt( keys.GetAt( i ) ) );
		writer.WriteClosingTag( tagName, false );
	}
}

void gdeLoadSaveGameDefinition::pWriteProperties( decXmlWriter &writer,
const gdePropertyList &properties, const char *tagName ){
	const int count = properties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pWriteProperty( writer, *properties.GetAt( i ), tagName );
	}
}

void gdeLoadSaveGameDefinition::pWriteProperty( decXmlWriter &writer,
const gdeProperty &property, const char *tagName ){
	writer.WriteOpeningTagStart( tagName );
	writer.WriteAttributeString( "name", property.GetName() );
	writer.WriteOpeningTagEnd();
	
	WriteMultilineString( writer, "description", property.GetDescription() );
	
	switch( property.GetType() ){
	case gdeProperty::eptString:
		writer.WriteDataTagString( "type", "string" );
		break;
		
	case gdeProperty::eptInteger:
		writer.WriteDataTagString( "type", "integer" );
		break;
		
	case gdeProperty::eptPoint2:
		writer.WriteDataTagString( "type", "point2" );
		break;
		
	case gdeProperty::eptPoint3:
		writer.WriteDataTagString( "type", "point3" );
		break;
		
	case gdeProperty::eptFloat:
		writer.WriteDataTagString( "type", "float" );
		break;
		
	case gdeProperty::eptVector2:
		writer.WriteDataTagString( "type", "vector2" );
		break;
		
	case gdeProperty::eptVector3:
		writer.WriteDataTagString( "type", "vector3" );
		break;
		
	case gdeProperty::eptColor:
		writer.WriteDataTagString( "type", "color" );
		break;
		
	case gdeProperty::eptBoolean:
		writer.WriteDataTagString( "type", "boolean" );
		break;
		
	case gdeProperty::eptRange:
		writer.WriteDataTagString( "type", "range" );
		
		writer.WriteOpeningTagStart( "range" );
		writer.WriteAttributeFloat( "min", property.GetMinimumValue() );
		writer.WriteAttributeFloat( "max", property.GetMaximumValue() );
		writer.WriteOpeningTagEnd( true );
		break;
		
	case gdeProperty::eptSelect:{
		const decStringList &options = property.GetOptions();
		const int optionCount = options.GetCount();
		int i;
		
		writer.WriteDataTagString( "type", "selection" );
		
		for( i=0; i<optionCount; i++ ){
			WriteMultilineString( writer, "option", options.GetAt( i ) );
		}
		}break;
		
	case gdeProperty::eptList:
		writer.WriteDataTagString( "type", "list" );
		break;
		
	case gdeProperty::eptPath:
		writer.WriteDataTagString( "type", "path" );
		
		switch( property.GetPathPatternType() ){
		case gdeProperty::epptAll:
			writer.WriteDataTagString( "pathPatternType", "all" );
			break;
			
		case gdeProperty::epptModel:
			writer.WriteDataTagString( "pathPatternType", "model" );
			break;
			
		case gdeProperty::epptSkin:
			writer.WriteDataTagString( "pathPatternType", "skin" );
			break;
			
		case gdeProperty::epptRig:
			writer.WriteDataTagString( "pathPatternType", "rig" );
			break;
			
		case gdeProperty::epptAnimation:
			writer.WriteDataTagString( "pathPatternType", "animation" );
			break;
			
		case gdeProperty::epptAnimator:
			writer.WriteDataTagString( "pathPatternType", "animator" );
			break;
			
		case gdeProperty::epptImage:
			writer.WriteDataTagString( "pathPatternType", "image" );
			break;
			
		case gdeProperty::epptOcclusionMesh:
			writer.WriteDataTagString( "pathPatternType", "occlusionMesh" );
			break;
			
		case gdeProperty::epptNavigationSpace:
			writer.WriteDataTagString( "pathPatternType", "navigationSpace" );
			break;
			
		case gdeProperty::epptParticleEmitter:
			writer.WriteDataTagString( "pathPatternType", "particleEmitter" );
			break;
			
		case gdeProperty::epptSound:
			writer.WriteDataTagString( "pathPatternType", "sound" );
			break;
			
		case gdeProperty::epptSynthesizer:
			writer.WriteDataTagString( "pathPatternType", "synthesizer" );
			break;
			
		case gdeProperty::epptVideo:
			writer.WriteDataTagString( "pathPatternType", "video" );
			break;
			
		case gdeProperty::epptFont:
			writer.WriteDataTagString( "pathPatternType", "font" );
			break;
			
		case gdeProperty::epptSky:
			writer.WriteDataTagString( "pathPatternType", "sky" );
			break;
			
		case gdeProperty::epptCamera:
			writer.WriteDataTagString( "pathPatternType", "camera" );
			break;
			
		case gdeProperty::epptWorld:
			writer.WriteDataTagString("pathPatternType", "world");
			break;
			
		case gdeProperty::epptCustom:{
			const gdeFilePatternList &filePatterns = property.GetCustomPathPattern();
			const int filePatternCount = filePatterns.GetCount();
			int i;
			
			writer.WriteDataTagString( "pathPatternType", "custom" );
			
			writer.WriteOpeningTag( "customPathPattern" );
			for( i=0; i<filePatternCount; i++ ){
				const gdeFilePattern &filePattern = *filePatterns.GetAt( i );
				
				writer.WriteOpeningTagStart( "add" );
				writer.WriteAttributeString( "name", filePattern.GetName() );
				writer.WriteAttributeString( "pattern", filePattern.GetPattern() );
				writer.WriteAttributeString( "default", filePattern.GetDefaultExtension() );
				writer.WriteOpeningTagEnd( true );
			}
			writer.WriteClosingTag( "customPathPattern" );
			
			}break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
		break;
		
	case gdeProperty::eptTriggerExpression:
		writer.WriteDataTagString( "type", "triggerExpression" );
		break;
		
	case gdeProperty::eptTriggerTarget:
		writer.WriteDataTagString( "type", "triggerTarget" );
		break;
		
	case gdeProperty::eptShape:
		writer.WriteDataTagString( "type", "shape" );
		break;
		
	case gdeProperty::eptShapeList:
		writer.WriteDataTagString( "type", "shapeList" );
		break;
		
	case gdeProperty::eptIdentifier:
		writer.WriteDataTagString( "type", "identifier" );
		writer.WriteDataTagString( "idGroup", property.GetIdentifierGroup() );
		writer.WriteDataTagBool( "idUsage", property.GetIdentifierUsage() );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	if( ! property.GetDefaultValue().IsEmpty() ){
		WriteMultilineString( writer, "default", property.GetDefaultValue() );
	}
	
	writer.WriteClosingTag( tagName );
}
