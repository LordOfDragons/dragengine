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

#include "igdeLoadParticleEmitter.h"
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
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Definitions
////////////////

struct sParameter{
	deParticleEmitterType::eParameters parameter;
	const char *name;
};

static const sParameter vParameters[ deParticleEmitterType::epBeamEnd + 1 ] = {
	{ deParticleEmitterType::epTimeToLive, "timeToLive" },
	{ deParticleEmitterType::epInterval, "interval" },
	{ deParticleEmitterType::epParticleCount, "particleCount" },
	{ deParticleEmitterType::epCastAngleX, "castAngleX" },
	{ deParticleEmitterType::epCastAngleY, "castAngleY" },
	{ deParticleEmitterType::epSize, "size" },
	{ deParticleEmitterType::epRed, "red" },
	{ deParticleEmitterType::epGreen, "green" },
	{ deParticleEmitterType::epBlue, "blue" },
	{ deParticleEmitterType::epTransparency, "transparency" },
	{ deParticleEmitterType::epEmissivity, "emissivity" },
	{ deParticleEmitterType::epMass, "mass" },
	{ deParticleEmitterType::epRotation, "rotation" },
	{ deParticleEmitterType::epLinearVelocity, "linearVelocity" },
	{ deParticleEmitterType::epAngularVelocity, "angularVelocity" },
	{ deParticleEmitterType::epBrownMotion, "brownMotion" },
	{ deParticleEmitterType::epDamping, "damping" },
	{ deParticleEmitterType::epDrag, "drag" },
	{ deParticleEmitterType::epGravityX, "gravityX" },
	{ deParticleEmitterType::epGravityY, "gravityY" },
	{ deParticleEmitterType::epGravityZ, "gravityZ" },
	{ deParticleEmitterType::epLocalGravity, "localGravity" },
	{ deParticleEmitterType::epForceFieldDirect, "forceFieldDirect" },
	{ deParticleEmitterType::epForceFieldSurface, "forceFieldSurface" },
	{ deParticleEmitterType::epForceFieldMass, "forceFieldMass" },
	{ deParticleEmitterType::epForceFieldSpeed, "forceFieldSpeed" },
	{ deParticleEmitterType::epElasticity, "elasticity" },
	{ deParticleEmitterType::epRoughness, "roughness" },
	{ deParticleEmitterType::epEmitDirection, "emitDirection" },
	{ deParticleEmitterType::epBeamStart, "beamStart" },
	{ deParticleEmitterType::epBeamEnd, "beamEnd" }
};

static bool ParameterForName( const char *name, deParticleEmitterType::eParameters &type ){
	int i;
	for( i=0; i<=deParticleEmitterType::epBeamEnd; i++ ){
		if( strcmp( vParameters[ i ].name, name ) == 0 ){
			type = vParameters[ i ].parameter;
			return true;
		}
	}
	return false;
}



struct sController{
	deParticleEmitterType::eEmitControllers controller;
	const char *name;
};

static const sController vControllers[ deParticleEmitterType::eecAngularVelocity + 1 ] = {
	{ deParticleEmitterType::eecLifetime, "lifetime" },
	{ deParticleEmitterType::eecMass, "mass" },
	{ deParticleEmitterType::eecLinearVelocity, "linearVelocity" },
	{ deParticleEmitterType::eecAngularVelocity, "angularVelocity" }
};

static bool ControllerForName( const char *name, deParticleEmitterType::eEmitControllers &controller ){
	int i;
	for( i=0; i<=deParticleEmitterType::eecAngularVelocity; i++ ){
		if( strcmp( vControllers[ i ].name, name ) == 0 ){
			controller = vControllers[ i ].controller;
			return true;
		}
	}
	return false;
}



// Class igdeLoadParticleEmitter
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadParticleEmitter::igdeLoadParticleEmitter( igdeEnvironment &environment,
deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ),
pEnvironment( environment ),
pName( "Drag[en]gine Particle Emitter" ),
pPattern( "*.departem" ),
pDefaultExtension( ".departem" ){
}

igdeLoadParticleEmitter::~igdeLoadParticleEmitter(){
}



// Management
///////////////

void igdeLoadParticleEmitter::SetName( const char *name ){
	pName = name;
}

void igdeLoadParticleEmitter::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void igdeLoadParticleEmitter::SetDefaultExtension( const char *extension ){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadParticleEmitter::Load( const decString &path, deParticleEmitter &particleEmitter,
decBaseFileReader &reader ){
	decPath basePath;
	basePath.SetFromUnix( path.GetString() );
	if( basePath.GetComponentCount() > 1 ){
		basePath.RemoveLastComponent();
		
	}else{
		basePath.SetFromUnix( "/" );
	}
	
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "particleEmitter" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadEmitter( *root, basePath.GetPathUnix(), particleEmitter );
}



// Loading and saving
///////////////////////

void igdeLoadParticleEmitter::pReadEmitter( const decXmlElementTag &root, const char *basePath,
deParticleEmitter &particleEmitter ){
	const int count = root.GetElementCount();
	int i;
	
	int typeCount = 0;
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( tag && tag->GetName() == "type" ){
			typeCount++;
		}
	}
	particleEmitter.SetTypeCount( typeCount );
	
	int nextTypeIndex = 0;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "controller" ) == 0 ){
			pReadController( *tag, particleEmitter );
			
		}else if( strcmp( tag->GetName(), "burstLifetime" ) == 0 ){
			particleEmitter.SetBurstLifetime( GetCDataFloat( *tag ) );
			
		}else if( strcmp( tag->GetName(), "emitBurst" ) == 0 ){
			particleEmitter.SetEmitBurst( GetCDataBool( *tag ) );
			
		}else if( strcmp( tag->GetName(), "type" ) == 0 ){
			pReadType( *tag, basePath, particleEmitter, particleEmitter.GetTypeAt( nextTypeIndex++ ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeLoadParticleEmitter::pReadController( const decXmlElementTag &root,
deParticleEmitter &particleEmitter ){
	deParticleEmitterController * const controller = new deParticleEmitterController;
	const int count = root.GetElementCount();
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "name" ) == 0 ){
				controller->SetName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "clamp" ) == 0 ){
				controller->SetClamp( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "frozen" ) == 0 ){
				controller->SetFrozen( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "lower" ) == 0 ){ // deprecated
				controller->SetRange( GetCDataFloat( *tag ), controller->GetUpper() );
				
			}else if( strcmp( tag->GetName(), "upper" ) == 0 ){ // deprecated
				controller->SetRange( controller->GetLower(), GetCDataFloat( *tag ) );
				
			}else if( strcmp( tag->GetName(), "limits" ) == 0 ){
				controller->SetRange( GetAttributeFloat( *tag, "lower" ), GetAttributeFloat( *tag, "upper" ) );
				
			}else if( strcmp( tag->GetName(), "linkToTime" ) == 0 ){
				// editor only parameter
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
		particleEmitter.AddController( controller );
		
	}catch( const deException & ){
		delete controller;
		throw;
	}
}

void igdeLoadParticleEmitter::pReadType( const decXmlElementTag &root, const char *basePath,
deParticleEmitter &particleEmitter, deParticleEmitterType &type ){
	const int count = root.GetElementCount();
	const char *identifier;
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "name" ) == 0 ){
			// used in editor only
			
		}else if( strcmp( tag->GetName(), "skin" ) == 0 ){
			const decString path( GetCDataString( *tag ) );
			if( path.IsEmpty() ){
				continue;
			}
			
			type.SetSkin(deSkin::Ref::New(
				 particleEmitter.GetEngine()->GetSkinManager()->LoadSkin( path, basePath ) ));
			
		}else if( strcmp( tag->GetName(), "model" ) == 0 ){
			const decString path( GetCDataString( *tag ) );
			if( path.IsEmpty() ){
				continue;
			}
			
			type.SetModel(deModel::Ref::New(
				 particleEmitter.GetEngine()->GetModelManager()->LoadModel( path, basePath ) ));
			
		}else if( strcmp( tag->GetName(), "modelSkin" ) == 0 ){
			const decString path( GetCDataString( *tag ) );
			if( path.IsEmpty() ){
				continue;
			}
			
			type.SetModelSkin(deSkin::Ref::New(
				 particleEmitter.GetEngine()->GetSkinManager()->LoadSkin( path, basePath ) ));
			
		}else if( strcmp( tag->GetName(), "castFrom" ) == 0 ){
			identifier = GetCDataString( *tag );
			
			if( strcmp( identifier, "vertex" ) == 0 ){
				type.SetCastFrom( deParticleEmitterType::ecfVertex );
				
			}else if( strcmp( identifier, "face" ) == 0 ){
				type.SetCastFrom( deParticleEmitterType::ecfFace );
				
			}else if( strcmp( identifier, "volume" ) == 0 ){
				type.SetCastFrom( deParticleEmitterType::ecfVolume );
				
			}else{
				LogWarnUnknownValue( *tag, identifier );
			}
			
		}else if( strcmp( tag->GetName(), "simulationType" ) == 0 ){
			identifier = GetCDataString( *tag );
			
			if( strcmp( identifier, "particle" ) == 0 ){
				type.SetSimulationType( deParticleEmitterType::estParticle );
				
			}else if( strcmp( identifier, "ribbon" ) == 0 ){
				type.SetSimulationType( deParticleEmitterType::estRibbon );
				
			}else if( strcmp( identifier, "beam" ) == 0 ){
				type.SetSimulationType( deParticleEmitterType::estBeam );
				
			}else{
				LogWarnUnknownValue( *tag, identifier );
			}
			
		}else if( strcmp( tag->GetName(), "intervalAsDistance" ) == 0 ){
			type.SetIntervalAsDistance( GetCDataBool( *tag ) );
			
		}else if( strcmp( tag->GetName(), "trailEmitter" ) == 0 ){
			const decString path( GetCDataString( *tag ) );
			if( path.IsEmpty() ){
				continue;
			}
			
			const decPath trailPath( decPath::AbsolutePathUnix( path, basePath ) );
			
			decBaseFileReader::Ref trailReader(decBaseFileReader::Ref::New(
				 particleEmitter.GetEngine()->GetVirtualFileSystem()->OpenFileForReading( trailPath ) ));
			
			deParticleEmitter::Ref trailEmitter(deParticleEmitter::Ref::New(
				 particleEmitter.GetEngine()->GetParticleEmitterManager()->CreateParticleEmitter() ));
			Load( trailPath.GetPathUnix(), trailEmitter, trailReader );
			
			type.SetTrailEmitter( trailEmitter );
			
		}else if( strcmp( tag->GetName(), "trailController" ) == 0 ){
			identifier = GetAttributeString( *tag, "id" );
			deParticleEmitterType::eEmitControllers controller;
			if( ! ControllerForName( identifier, controller ) ){
				LogErrorUnknownValue( *tag, identifier );
				continue;
			}
			if( type.GetTrailEmitter() ){
				type.SetTrailController( controller, type.GetTrailEmitter()->
					IndexOfControllerNamed( GetCDataString( *tag ) ) );
			}
			
		}else if( strcmp( tag->GetName(), "physicsSize" ) == 0 ){
			type.SetPhysicsSize( GetCDataFloat( *tag ) );
			
		}else if( strcmp( tag->GetName(), "collisionResponse" ) == 0 ){
			identifier = GetCDataString( *tag );
			
			if( strcmp( identifier, "destroy" ) == 0 ){
				type.SetCollisionResponse( deParticleEmitterType::ecrDestroy );
				
			}else if( strcmp( identifier, "physical" ) == 0 ){
				type.SetCollisionResponse( deParticleEmitterType::ecrPhysical );
				
			}else if( strcmp( identifier, "custom" ) == 0 ){
				type.SetCollisionResponse( deParticleEmitterType::ecrCustom );
				
			}else{
				LogWarnUnknownValue( *tag, identifier );
			}
			
		}else if( strcmp( tag->GetName(), "collisionEmitter" ) == 0 ){
			const decString path( GetCDataString( *tag ) );
			if( path.IsEmpty() ){
				continue;
			}
			
			const decPath colPath( decPath::AbsolutePathUnix( path, basePath ) );
			
			decBaseFileReader::Ref colReader(decBaseFileReader::Ref::New(
				 particleEmitter.GetEngine()->GetVirtualFileSystem()->OpenFileForReading( colPath ) ));
			
			deParticleEmitter::Ref colEmitter(deParticleEmitter::Ref::New(
				 particleEmitter.GetEngine()->GetParticleEmitterManager()->CreateParticleEmitter() ));
			Load( colPath.GetPathUnix(), colEmitter, colReader );
			
			type.SetCollisionEmitter( colEmitter );
			
		}else if( strcmp( tag->GetName(), "emitMinImpulse" ) == 0 ){
			type.SetEmitMinImpulse( GetCDataFloat( *tag ) );
			
		}else if( strcmp( tag->GetName(), "emitController" ) == 0 ){
			identifier = GetAttributeString( *tag, "id" );
			deParticleEmitterType::eEmitControllers controller;
			if( ! ControllerForName( identifier, controller ) ){
				LogErrorUnknownValue( *tag, identifier );
				continue;
			}
			if( type.GetCollisionEmitter() ){
				type.SetEmitController( controller, type.GetCollisionEmitter()->
					IndexOfControllerNamed( GetCDataString( *tag ) ) );
			}
			
		}else if( strcmp( tag->GetName(), "parameter" ) == 0 ){
			pReadParameter( *tag, particleEmitter, type );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeLoadParticleEmitter::pReadParameter( const decXmlElementTag &root, deParticleEmitter &,
deParticleEmitterType &type ){
	const char *id = GetAttributeString( root, "id" );
	const int count = root.GetElementCount();
	const decXmlElementTag *tag;
	decCurveBezier curve;
	int i;
	
	deParticleEmitterType::eParameters parameterType = deParticleEmitterType::epTimeToLive;
	if( ! ParameterForName( id, parameterType ) ){
		LogErrorUnknownValue( root, id );
	}
	deParticleEmitterParameter &parameter = type.GetParameter( parameterType );
	
	for( i=0; i<count; i++ ){
		tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "value" ) == 0 ){
			parameter.SetValue( GetCDataFloat( *tag ) );
			
		}else if( strcmp( tag->GetName(), "spread" ) == 0 ){
			parameter.SetSpread( GetCDataFloat( *tag ) );
			
		}else if( strcmp( tag->GetName(), "controllerValue" ) == 0 ){
			parameter.SetControllerValue( GetCDataInt( *tag ) );
			
		}else if( strcmp( tag->GetName(), "controllerSpread" ) == 0 ){
			parameter.SetControllerSpread( GetCDataInt( *tag ) );
			
		}else if( strcmp( tag->GetName(), "curveValue" ) == 0 ){
			curve.RemoveAllPoints();
			ReadCurveBezier( *tag, curve );
			parameter.GetCurveValue() = curve;
			
		}else if( strcmp( tag->GetName(), "curveSpread" ) == 0 ){
			curve.RemoveAllPoints();
			ReadCurveBezier( *tag, curve );
			parameter.GetCurveSpread() = curve;
			
		}else if( strcmp( tag->GetName(), "curveProgress" ) == 0 ){
			curve.RemoveAllPoints();
			ReadCurveBezier( *tag, curve );
			parameter.GetCurveProgress() = curve;
			
		}else if( strcmp( tag->GetName(), "curveBeam" ) == 0 ){
			curve.RemoveAllPoints();
			ReadCurveBezier( *tag, curve );
			parameter.GetCurveBeam() = curve;
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
