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
#include <inttypes.h>

#include "fbxSkinModule.h"
#include "../shared/fbxConnection.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/fbxScene.h"
#include "../shared/managed/fbxModel.h"
#include "../shared/managed/fbxMaterial.h"
#include "../shared/managed/fbxTexture.h"
#include "../shared/property/fbxPropertyArrayDouble.h"
#include "../shared/property/fbxPropertyArrayFloat.h"
#include "../shared/property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXSkinCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *FBXSkinCreateModule( deLoadableModule *loadableModule ){
	try{
		return new fbxSkinModule( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class fbxSkinModule
////////////////////////

// Constructor, destructor
////////////////////////////

fbxSkinModule::fbxSkinModule( deLoadableModule &loadableModule ) :
deBaseSkinModule( loadableModule ){
}

fbxSkinModule::~fbxSkinModule(){
}



// Loading and Saving
///////////////////////

void fbxSkinModule::LoadSkin( decBaseFileReader &reader, deSkin &skin ){
	try{
		fbxScene scene( reader );
		scene.Prepare( *this );
		
		pLoadSkin( skin, scene );
		
	}catch( const deException & ){
		LogErrorFormat( "Failed reading file '%s' at file position %d",
			reader.GetFilename(), reader.GetPosition() );
		throw;
	}
}

void fbxSkinModule::SaveSkin(decBaseFileWriter&, const deSkin & ){
	// nothing yet
	DETHROW_INFO( deeInvalidAction, "Saving not supported yet" );
}



// Private Functions
//////////////////////

void fbxSkinModule::pLoadSkin( deSkin &skin, fbxScene &scene ){
	fbxNode &nodeGeometry = *scene.FirstNodeNamed( "Geometry" );
	const fbxModel::Ref model( fbxModel::Ref::New( new fbxModel( scene, nodeGeometry ) ) );
	
	// find connections involving model node
	const int64_t idModel = model->GetModelID();
	decPointerList cons;
	scene.FindConnections( idModel, cons );
	
	// add material nodes connected to model node
	const int conCount = cons.GetCount();
	int i;
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )cons.GetAt( i ) );
		if( connection.GetTarget() != idModel ){
			continue;
		}
		
		fbxNode &node = *scene.NodeWithID( connection.OtherID( idModel ) );
		if( node.GetName() == "Material" ){
			pLoadMaterial( skin, scene, fbxMaterial::Ref::New( new fbxMaterial( scene, node ) ) );
		}
	}
}

void fbxSkinModule::pLoadMaterial( deSkin &skin, fbxScene &scene, const fbxMaterial &material ){
	deSkinTexture *texture = nullptr;
	
	try{
		texture = new deSkinTexture( material.GetName() );
		
		// find properties
		const fbxNode &nodeMaterial = material.GetNodeMaterial();
		const decColor white( 1.0f, 1.0f, 1.0f );
		const decColor black( 0.0f, 0.0f, 0.0f );
		
		const decColor fbxColor( ( nodeMaterial.GetPropertyColor( "DiffuseColor", black )
			* nodeMaterial.GetPropertyFloat( "DiffuseFactor", 1.0f ) ).Normalized() );
		
		const decColor fbxSpecular( ( nodeMaterial.GetPropertyColor( "SpecularColor", black )
			* nodeMaterial.GetPropertyFloat( "SpecularFactor", 1.0f ) ).Normalized() );
		
		const float fbxShininess = decMath::clamp(
			nodeMaterial.GetPropertyFloat( "Shininess", 0.0f ), 0.0f, 64.0f );
		// "ShininessExponent" ?
		
		const decColor fbxEmissiveColor( nodeMaterial.GetPropertyColor( "EmissiveColor", black ).Normalized() );
		const float fbxEmissiveFactor = decMath::max( nodeMaterial.GetPropertyFloat( "EmissiveFactor", 0.0f ), 0.0f );
		
// 		const decColor fbxTransparentColor( nodeMaterial.GetPropertyColor( "TransparentColor", black ).Normalized() );
		const float fbxTransparentFactor = decMath::clamp(
				nodeMaterial.GetPropertyFloat( "TransparentFactor", 0.0f ), 0.0f, 1.0f );
		
		// "ReflectionColor", "ReflectionFactor"
		
		// convert to PBR as good as possible
		// 
		// shininess: spec = pow(max(dot(normal, halfwayDir), 0.0), shininess)
		//            with dot(n,hd) = cos(angle)
		//            and roughness = 0 -> 90 angle
		//            and shininess = 0 -> 64
		//            mapping shininess 64 to roughness 0 and shininess 0 to roughness 1
		//            this mapping is not linear. good values would be:
		//            shininess(0) = roughness(1)
		//            shininess(8) = roughness(0.2)
		//            shininess(64) = roughness(0)
		//            using pow(1-shininess/64,12)
		decColor color, reflectivity, emissivity;
		float roughness, emissivityIntensity, solidity;
		
		reflectivity = fbxSpecular;
		color.r = fbxColor.r * ( 1.0f - reflectivity.r );
		color.g = fbxColor.g * ( 1.0f - reflectivity.g );
		color.b = fbxColor.b * ( 1.0f - reflectivity.b );
		
		roughness = powf( 1.0f - ( fbxShininess / 64.0f ), 12.0f );
		
		emissivityIntensity = 0.0f;
		if( fbxEmissiveFactor > 1e-4f ){
			emissivityIntensity = fbxEmissiveFactor;
			emissivity = fbxEmissiveColor;
		}
		
		solidity = 1.0f - fbxTransparentFactor;
		//fbxTransparentColor; // this would need to be mixed with "color"
		
		// find textures
		const int64_t idMaterial = material.GetMaterialID();
		decPointerList texConns;
		scene.FindConnections( idMaterial, texConns );
		
		fbxTexture::Ref fbxTexDiffuseColor, fbxTexSpecularColor, fbxTexShininess;
		fbxTexture::Ref fbxTexEmissiveColor, fbxTexTransparencyFactor;
		fbxTexture::Ref fbxTexBump;
		
		const int texConnCount = texConns.GetCount();
		int i;
		
		for( i=0; i<texConnCount; i++ ){
			const fbxConnection &connection = *( ( fbxConnection* )texConns.GetAt( i ) );
			if( connection.GetTarget() != idMaterial ){
				continue;
			}
			
			fbxNode &node = *scene.NodeWithID( connection.OtherID( idMaterial ) );
			if( node.GetName() != "Texture" ){
				continue;
			}
			
// 			node.DebugPrintStructure( *this, "Texture:", true );
			
			const decString &mpname = connection.GetProperty();
// 			LogInfoFormat( "texture map '%s'", mpname.GetString() );
			
			if( mpname == "DiffuseColor" ){
				fbxTexDiffuseColor.TakeOver( new fbxTexture( scene, node ) );
				
			}else if( mpname == "SpecularColor" ){
				fbxTexSpecularColor.TakeOver( new fbxTexture( scene, node ) );
				
			}else if( mpname == "Shininess" ){
				fbxTexShininess.TakeOver( new fbxTexture( scene, node ) );
				
			}else if( mpname == "EmissiveColor" ){
				fbxTexEmissiveColor.TakeOver( new fbxTexture( scene, node ) );
				
			}else if( mpname == "TransparencyFactor" ){
				fbxTexTransparencyFactor.TakeOver( new fbxTexture( scene, node ) );
				
			}else if( mpname == "Bump" || mpname == "NormalMap" ){
				fbxTexBump.TakeOver( new fbxTexture( scene, node ) );
			}
		}
		
		// add texture properties
		if( fbxTexDiffuseColor ){
			pAddPropertyImage( *texture, "color", fbxTexDiffuseColor );
			
		}else{
			pAddPropertyColor( *texture, "color", color );
		}
		
		if( fbxTexSpecularColor ){
			pAddPropertyImage( *texture, "reflectivity", fbxTexSpecularColor );
			
		}else{
			pAddPropertyColor( *texture, "reflectivity", reflectivity );
		}
		
		if( fbxTexShininess && false ){
			pAddPropertyImage( *texture, "roughness", fbxTexShininess );
			
		}else{
			pAddPropertyValue( *texture, "roughness", roughness );
		}
		
		if( fbxTexEmissiveColor ){
			pAddPropertyImage( *texture, "emissivity", fbxTexEmissiveColor );
			
		}else{
			if( ! emissivity.IsEqualTo( black ) ){
				pAddPropertyColor( *texture, "emissivity", emissivity );
			}
		}
		
		if( emissivityIntensity > 1e-4f || fbxTexEmissiveColor ){
			pAddPropertyValue( *texture, "emissivity.intensity", emissivityIntensity );
			pAddPropertyValue( *texture, "emissivity.camera.adapted", 1.0f );
		}
		
		if( fbxTexTransparencyFactor ){
			pAddPropertyImage( *texture, "solidity", fbxTexTransparencyFactor );
			
		}else if( solidity < 1.0f - 1e-4f ){
			pAddPropertyValue( *texture, "solidity", solidity );
		}
		
		if( fbxTexBump ){
			pAddPropertyImage( *texture, "normal", fbxTexBump );
		}
		
		skin.AddTexture( texture );
		
	}catch( const deException & ){
		if( texture ){
			delete texture;
		}
		throw;
	}
}

void fbxSkinModule::pAddPropertyColor( deSkinTexture &texture, const char *name, const decColor &color ){
	deSkinPropertyColor *property = nullptr;
	
	try{
		property = new deSkinPropertyColor( name );
		property->SetColor( color );
		texture.AddProperty( property );
		
	}catch( const deException & ){
		if( property ){
			delete property;
		}
		throw;
	}
}

void fbxSkinModule::pAddPropertyValue( deSkinTexture &texture, const char *name, float value ){
	deSkinPropertyValue *property = nullptr;
	
	try{
		property = new deSkinPropertyValue( name );
		property->SetValue( value );
		texture.AddProperty( property );
		
	}catch( const deException & ){
		if( property ){
			delete property;
		}
		throw;
	}
}

void fbxSkinModule::pAddPropertyImage( deSkinTexture &texture, const char *name, const fbxTexture &fbxtex ){
	deSkinPropertyImage *property = nullptr;
	
	try{
		property = new deSkinPropertyImage( name );
		
		// relative path can be with '/' or '\\' depending on the system the FBX file
		// has been created on. we do not know which delimiter is used so we search
		// for both and use the first one found ensuring the path is in unix style
		// before merged with the
		const decString &relativeFilename = fbxtex.GetRelativeFilename();
		
		if( relativeFilename.Find( '/' ) != -1 ){
			property->SetPath( relativeFilename );
			
		}else if( relativeFilename.Find( '\\' ) != -1 ){
			property->SetPath( relativeFilename.GetReplaced( '\\', '/' ) );
			
		}else{
			property->SetPath( relativeFilename );
		}
		
		texture.AddProperty( property );
		
	}catch( const deException & ){
		if( property ){
			delete property;
		}
		throw;
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class fbxSkinModuleInternal : public deInternalModule{
public:
	fbxSkinModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("FBXSkin");
		SetDescription("Handles skins in the binary FBX format.");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtSkin);
		SetDirectoryName("fbxskin");
		GetPatternList().Add(".fbx");
		SetDefaultExtension(".fbx");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(FBXSkinCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *fbxSkinRegisterInternalModule(deModuleSystem *system){
	return new fbxSkinModuleInternal(system);
}
#endif
