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

#include "debpColliderAttachment.h"
#include "debpCollider.h"

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/sensor/deLumimeter.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpColliderAttachment
/////////////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderAttachment::debpColliderAttachment( deColliderAttachment *attachment ) :
pAttachment( attachment ),
pTrackBone( -1 ),
pBoneMappings( nullptr ),
pBoneMappingCount( 0 ),
pVpsMappings( nullptr ),
pVpsMappingCount( 0 ),
pDirtyMappings( true ),
pDirtyLocalMatrix( true ),
pHasLocalMatrix( false ){
}

debpColliderAttachment::~debpColliderAttachment(){
	pCleanUp();
}



// Management
///////////////

void debpColliderAttachment::SetBoneMappingCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( count == pBoneMappingCount ){
		return;
	}
	
	int i, *newArray = nullptr;
	if( count > 0 ){
		newArray = new int[ count ];
		for( i=0; i<count; i++ ){
			newArray[ i ] = -1;
		}
	}
	
	if( pBoneMappings ){
		delete [] pBoneMappings;
	}
	pBoneMappings = newArray;
	pBoneMappingCount = count;
}

void debpColliderAttachment::SetVpsMappingCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( count == pVpsMappingCount ){
		return;
	}
	
	int i, *newArray = nullptr;
	if( count > 0 ){
		newArray = new int[ count ];
		for( i=0; i<count; i++ ){
			newArray[ i ] = -1;
		}
	}
	
	if( pVpsMappings ){
		delete [] pVpsMappings;
	}
	pVpsMappings = newArray;
	pVpsMappingCount = count;
}

void debpColliderAttachment::SetTrackBone( int boneIndex ){
	DEASSERT_TRUE( boneIndex >= -1 )
	
	pTrackBone = boneIndex;
}

int debpColliderAttachment::GetBoneMappingAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pBoneMappingCount )
	
	return pBoneMappings[ index ];
}

void debpColliderAttachment::SetBoneMappingAt( int index, int boneIndex ){
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pBoneMappingCount )
	DEASSERT_TRUE( boneIndex >= -1 )
	
	pBoneMappings[ index ] = boneIndex;
}

int debpColliderAttachment::GetVpsMappingAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pVpsMappingCount )
	
	return pVpsMappings[ index ];
}

void debpColliderAttachment::SetVpsMappingAt( int index, int boneIndex ){
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pVpsMappingCount )
	DEASSERT_TRUE( boneIndex >= -1 )
	
	pVpsMappings[ index ] = boneIndex;
}

void debpColliderAttachment::SetDirtyMappings( bool dirtyMappings ){
	pDirtyMappings = dirtyMappings;
}

void debpColliderAttachment::SetAccumRelMoveMatrix( const decDMatrix &matrix ){
	pAccumRelMoveMatrix = matrix;
}



void debpColliderAttachment::AttachmentChanged(){
	pDirtyMappings = true;
	pDirtyLocalMatrix = true;
}



void debpColliderAttachment::Reposition( const decDMatrix &matrix, const decVector &velocity,
bool changeNotify ){
	pPrepareLocalMatrix();
	
	if( pHasLocalMatrix ){
		const decDMatrix attmat( pLocalMatrix.QuickMultiply( matrix ) );
		const decDVector scale( attmat.GetScale() );
		const decDMatrix nor( attmat.Normalized() );
		pRepositionResource( nor.GetPosition(), nor.ToQuaternion(), scale, velocity, changeNotify );
		
	}else{
		const decDVector scale( matrix.GetScale() );
		const decDMatrix nor( matrix.Normalized() );
		pRepositionResource( nor.GetPosition(), nor.ToQuaternion(), scale, velocity, changeNotify );
	}
}

void debpColliderAttachment::Reposition( const decDVector &position,
const decQuaternion &orientation, const decDVector &scale, const decVector &velocity,
bool changeNotify ){
	pPrepareLocalMatrix();
	
	if( pHasLocalMatrix ){
		const decDMatrix attmat( pLocalMatrix.QuickMultiply(
			decDMatrix::CreateWorld( position, orientation, scale ) ) );
		const decDVector amscale( attmat.GetScale() );
		const decDMatrix nor( attmat.Normalized() );
		pRepositionResource( nor.GetPosition(), nor.ToQuaternion(), amscale, velocity, changeNotify );
		
	}else{
		pRepositionResource( position, orientation, scale, velocity, changeNotify );
	}
}

void debpColliderAttachment::Transform( const decDMatrix &matrix, const decVector &velocity,
bool changeNotify ){
	deResource * const resource = pAttachment->GetResource();
	
	switch( resource->GetResourceManager()->GetResourceType() ){
	case deResourceManager::ertCollider:{
		deCollider &collider = *( ( deCollider* )resource );
		const decDVector position( matrix * collider.GetPosition() );
		const decQuaternion orientation( collider.GetOrientation() * matrix.Normalized().ToQuaternion() );
		const decDVector scale( matrix.GetScale().Multiply( collider.GetScale() ) );
		debpCollider &bpCollider = *( ( debpCollider* )collider.GetPeerPhysics() );
		
		if( changeNotify ){
			if( collider.GetPosition().IsEqualTo( position )
			&& collider.GetOrientation().IsEqualTo( orientation )
			&& ( pAttachment->GetNoScaling() || collider.GetScale().IsEqualTo( scale ) ) ){
				break;
			}
		}
		
		if( pAttachment->GetNoScaling() ){
			collider.SetGeometry( position, orientation );
			
		}else{
			collider.SetGeometry( position, orientation, scale );
		}
		
		if( changeNotify ){
			collider.GetPeerScripting()->ColliderChanged( &collider );
			bpCollider.ClearRequiresUpdate();
			
		}else{
			// notification is prevented. register collider for finish detection.
			// at this time the notification can be resolved
			bpCollider.RegisterColDetFinish();
		}
		}break;
		
	case deResourceManager::ertBillboard:{
		deBillboard &billboard = *( ( deBillboard* )resource );
		billboard.SetPosition( matrix * billboard.GetPosition() );
		if( ! pAttachment->GetNoScaling() ){
			const decDVector msca( matrix.GetScale() );
			const decVector2 bsca( ( float )msca.x, ( float )msca.y );
			billboard.SetSize( billboard.GetSize().Multiply( bsca ) );
			billboard.SetOffset( billboard.GetOffset().Multiply( bsca ) );
		}
		}break;
		
	case deResourceManager::ertCamera:{
		deCamera &camera = *( ( deCamera* )resource );
		camera.SetPosition( matrix * camera.GetPosition() );
		camera.SetOrientation( camera.GetOrientation() * matrix.Normalized().ToQuaternion() );
		}break;
		
	case deResourceManager::ertComponent:{
		deComponent &component = *( ( deComponent* )resource );
		component.SetPosition( matrix * component.GetPosition() );
		component.SetOrientation( component.GetOrientation() * matrix.Normalized().ToQuaternion() );
		if( ! pAttachment->GetNoScaling() ){
			component.SetScaling( component.GetScaling().Multiply( matrix.GetScale() ) );
		}
		}break;
		
	case deResourceManager::ertDebugDrawer:{
		deDebugDrawer &debugDrawer = *( ( deDebugDrawer* )resource );
		debugDrawer.SetPosition( matrix * debugDrawer.GetPosition() );
		debugDrawer.SetOrientation( debugDrawer.GetOrientation() * matrix.Normalized().ToQuaternion() );
		if( ! pAttachment->GetNoScaling() ){
			debugDrawer.SetScale( debugDrawer.GetScale().Multiply( matrix.GetScale() ) );
		}
		}break;
		
	case deResourceManager::ertEnvMapProbe:{
		deEnvMapProbe &envMapProbe = *( ( deEnvMapProbe* )resource );
		envMapProbe.SetPosition( matrix * envMapProbe.GetPosition() );
		envMapProbe.SetOrientation( envMapProbe.GetOrientation() * matrix.Normalized().ToQuaternion() );
		}break;
		
	case deResourceManager::ertForceField:{
		deForceField &forceField = *( ( deForceField* )resource );
		forceField.SetPosition( matrix * forceField.GetPosition() );
		forceField.SetOrientation( forceField.GetOrientation() * matrix.Normalized().ToQuaternion() );
		}break;
		
	case deResourceManager::ertLight:{
		deLight &light = *( ( deLight* )resource );
		light.SetPosition( matrix * light.GetPosition() );
		light.SetOrientation( light.GetOrientation() * matrix.Normalized().ToQuaternion() );
		}break;
		
	case deResourceManager::ertLumimeter:{
		deLumimeter &lumimeter = *( ( deLumimeter* )resource );
		lumimeter.SetPosition( matrix * lumimeter.GetPosition() );
		//lumimeter.SetOrientation( lumimeter.GetOrientation() * matrix.Normalized().ToQuaternion() ); // TODO missing
		}break;
		
	case deResourceManager::ertMicrophone:{
		deMicrophone &microphone = *( ( deMicrophone* )resource );
		microphone.SetPosition( matrix * microphone.GetPosition() );
		microphone.SetOrientation( microphone.GetOrientation() * matrix.Normalized().ToQuaternion() );
		microphone.SetVelocity( velocity );
		}break;
		
	case deResourceManager::ertNavigationSpace:{
		deNavigationSpace &navigationSpace = *( ( deNavigationSpace* )resource );
		navigationSpace.SetPosition( matrix * navigationSpace.GetPosition() );
		navigationSpace.SetOrientation( navigationSpace.GetOrientation() * matrix.Normalized().ToQuaternion() );
		// TODO navigation block has scaling but not navigation space. add it?
		}break;
		
	case deResourceManager::ertNavigationBlocker:{
		deNavigationBlocker &navigationBlocker = *( ( deNavigationBlocker* )resource );
		navigationBlocker.SetPosition( matrix * navigationBlocker.GetPosition() );
		navigationBlocker.SetOrientation( navigationBlocker.GetOrientation() * matrix.Normalized().ToQuaternion() );
		if( ! pAttachment->GetNoScaling() ){
			navigationBlocker.SetScaling( navigationBlocker.GetScaling().Multiply( matrix.GetScale() ) );
		}
		}break;
		
	case deResourceManager::ertParticleEmitterInstance:{
		deParticleEmitterInstance &instance = *( ( deParticleEmitterInstance* )resource );
		instance.SetPosition( matrix * instance.GetPosition() );
		instance.SetOrientation( instance.GetOrientation() * matrix.Normalized().ToQuaternion() );
		}break;
		
	case deResourceManager::ertPropField:{
		dePropField &propField = *( ( dePropField* )resource );
		propField.SetPosition( matrix * propField.GetPosition() );
		//propField.SetOrientation( propfield.GetOrientation() * matrix.Normalized().ToQuaternion() ); // TODO missing
		}break;
		
	case deResourceManager::ertSpeaker:{
		deSpeaker &speaker = *( ( deSpeaker* )resource );
		speaker.SetPosition( matrix * speaker.GetPosition() );
		speaker.SetOrientation( speaker.GetOrientation() * matrix.Normalized().ToQuaternion() );
		speaker.SetVelocity( velocity );
		}break;
		
	case deResourceManager::ertTouchSensor:{
		deTouchSensor &touchSensor = *( ( deTouchSensor* )resource );
		touchSensor.SetPosition( matrix * touchSensor.GetPosition() );
		touchSensor.SetOrientation( touchSensor.GetOrientation() * matrix.Normalized().ToQuaternion() );
		// TODO touch sensor has no size. add it?
		}break;
		
	case deResourceManager::ertSoundLevelMeter:{
		deSoundLevelMeter &soundLevelMeter = *( ( deSoundLevelMeter* )resource );
		soundLevelMeter.SetPosition( matrix * soundLevelMeter.GetPosition() );
		soundLevelMeter.SetOrientation( soundLevelMeter.GetOrientation() * matrix.Normalized().ToQuaternion() );
		}break;
		
	default:
		break; // not supported
	}
}



// private functions
//////////////////////

void debpColliderAttachment::pCleanUp(){
	if( pBoneMappings ){
		delete [] pBoneMappings;
	}
	if( pVpsMappings ){
		delete [] pVpsMappings;
	}
}



void debpColliderAttachment::pPrepareLocalMatrix(){
	if( ! pDirtyLocalMatrix ){
		return;
	}
	
	const decQuaternion &localOrientation = pAttachment->GetOrientation();
	const decVector &localPosition = pAttachment->GetPosition();
	const decVector &localScaling = pAttachment->GetScaling();
	
	if( localPosition.IsEqualTo( decVector() ) && localOrientation.IsEqualTo( decQuaternion() )
	&& localScaling.IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) ) ){
		pLocalMatrix.SetIdentity();
		pHasLocalMatrix = false;
		
	}else{
		pLocalMatrix.SetWorld( localPosition, localOrientation, localScaling );
		pHasLocalMatrix = true;
	}
	
	pDirtyLocalMatrix = false;
}

void debpColliderAttachment::pRepositionResource( const decDVector &position,
const decQuaternion &orientation, const decDVector &scale, const decVector &velocity,
bool changeNotify ){
	deResource * const resource = pAttachment->GetResource();
	
	switch( resource->GetResourceManager()->GetResourceType() ){
	case deResourceManager::ertCollider:{
		deCollider &collider = *( ( deCollider* )resource );
		debpCollider &bpCollider = *( ( debpCollider* )collider.GetPeerPhysics() );
		
		if( changeNotify ){
			if( collider.GetPosition().IsEqualTo( position )
			&& collider.GetOrientation().IsEqualTo( orientation )
			&& ( pAttachment->GetNoScaling() || collider.GetScale().IsEqualTo( scale ) ) ){
				break;
			}
		}
		
		if( ! pAttachment->GetNoScaling() ){
			collider.SetGeometry( position, orientation );
			
		}else{
			collider.SetGeometry( position, orientation, scale );
		}
		
		if( changeNotify ){
			collider.GetPeerScripting()->ColliderChanged( &collider );
			bpCollider.ClearRequiresUpdate();
			
		}else{
			// notification is prevented. register collider for finish detection.
			// at this time the notification can be resolved
			bpCollider.RegisterColDetFinish();
		}
		}break;
		
	case deResourceManager::ertBillboard:{
		deBillboard &billboard = *( ( deBillboard* )resource );
		billboard.SetPosition( position );
		if( ! pAttachment->GetNoScaling() ){
// 			const decVector2 oldsca( billboard.GetSize() );
// 			const decVector2 newsca( ( float )scale.x, ( float )scale.y );
// 			const decVector2 resca( fabsf( oldsca.x ) != 0.001f ? newsca.x / oldsca.x : 1.0f,
// 				fabsf( oldsca.y ) != 0.001f ? newsca.y / oldsca.y : 1.0f );
			billboard.SetSize( decVector2( ( float )scale.x, ( float )scale.y ) );
// 			billboard.SetOffset( billboard.GetOffset().Multiply( resca ) );
		}
		}break;
		
	case deResourceManager::ertCamera:{
		deCamera &camera = *( ( deCamera* )resource );
		camera.SetPosition( position );
		camera.SetOrientation( orientation );
		}break;
		
	case deResourceManager::ertComponent:{
		deComponent &component = *( ( deComponent* )resource );
		component.SetPosition( position );
		component.SetOrientation( orientation );
		if( ! pAttachment->GetNoScaling() ){
			component.SetScaling( scale );
		}
		}break;
		
	case deResourceManager::ertDebugDrawer:{
		deDebugDrawer &debugDrawer = *( ( deDebugDrawer* )resource );
		debugDrawer.SetPosition( position );
		debugDrawer.SetOrientation( orientation );
		if( ! pAttachment->GetNoScaling() ){
			debugDrawer.SetScale( scale );
		}
		}break;
		
	case deResourceManager::ertEnvMapProbe:{
		deEnvMapProbe &envMapProbe = *( ( deEnvMapProbe* )resource );
		envMapProbe.SetPosition( position );
		envMapProbe.SetOrientation( orientation );
		}break;
		
	case deResourceManager::ertForceField:{
		deForceField &forceField = *( ( deForceField* )resource );
		forceField.SetPosition( position );
		forceField.SetOrientation( orientation );
		}break;
		
	case deResourceManager::ertLight:{
		deLight &light = *( ( deLight* )resource );
		light.SetPosition( position );
		light.SetOrientation( orientation );
		}break;
		
	case deResourceManager::ertLumimeter:{
		deLumimeter &lumimeter = *( ( deLumimeter* )resource );
		lumimeter.SetPosition( position );
		//lumimeter.SetOrientation( orientation ); // TODO missing
		}break;
		
	case deResourceManager::ertMicrophone:{
		deMicrophone &microphone = *( ( deMicrophone* )resource );
		microphone.SetPosition( position );
		microphone.SetOrientation( orientation );
		microphone.SetVelocity( velocity );
		}break;
		
	case deResourceManager::ertNavigationSpace:{
		deNavigationSpace &navigationSpace = *( ( deNavigationSpace* )resource );
		navigationSpace.SetPosition( position );
		navigationSpace.SetOrientation( orientation );
		// TODO navigation blocker has scaling but not navigation space. add it?
		}break;
		
	case deResourceManager::ertNavigationBlocker:{
		deNavigationBlocker &navigationBlocker = *( ( deNavigationBlocker* )resource );
		navigationBlocker.SetPosition( position );
		navigationBlocker.SetOrientation( orientation );
		if( ! pAttachment->GetNoScaling() ){
			navigationBlocker.SetScaling( scale );
		}
		}break;
		
	case deResourceManager::ertParticleEmitterInstance:{
		deParticleEmitterInstance &particleEmitterInstance = *( ( deParticleEmitterInstance* )resource );
		particleEmitterInstance.SetPosition( position );
		particleEmitterInstance.SetOrientation( orientation );
		}break;
		
	case deResourceManager::ertPropField:{
		dePropField &propField = *( ( dePropField* )resource );
		propField.SetPosition( position );
		//propField.SetOrientation( orientation ); // TODO missing
		}break;
		
	case deResourceManager::ertSpeaker:{
		deSpeaker &speaker = *( ( deSpeaker* )resource );
		speaker.SetPosition( position );
		speaker.SetOrientation( orientation );
		speaker.SetVelocity( velocity );
		}break;
		
	case deResourceManager::ertTouchSensor:{
		deTouchSensor &touchSensor = *( ( deTouchSensor* )resource );
		touchSensor.SetPosition( position );
		touchSensor.SetOrientation( orientation );
		// TODO touch sensor has no scaling. add it?
		}break;
		
	case deResourceManager::ertSoundLevelMeter:{
		deSoundLevelMeter &soundLevelMeter = *( ( deSoundLevelMeter* )resource );
		soundLevelMeter.SetPosition( position );
		soundLevelMeter.SetOrientation( orientation );
		}break;
		
	default:
		break; // not supported
	}
}
