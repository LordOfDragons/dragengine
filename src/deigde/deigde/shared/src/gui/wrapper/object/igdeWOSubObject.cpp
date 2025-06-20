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

#include "igdeWOSubObject.h"
#include "../igdeWObject.h"
#include "../../../codec/igdeCodecPropertyString.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../triggersystem/igdeTriggerTargetList.h"
#include "../../../triggersystem/igdeTriggerExpression.h"
#include "../../../triggersystem/igdeTriggerExpressionParser.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/rig/deRig.h>



// Class igdeWOSubObject
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSubObject::igdeWOSubObject( igdeWObject &wrapper, const decString &prefix ) :
pWrapper( wrapper ),
pPrefix( prefix ),
pHasBoxExtends( false ){
}

igdeWOSubObject::~igdeWOSubObject(){
}



// Management
///////////////

void igdeWOSubObject::SetBoxExtends( const decVector &boxMinExtends, const decVector &boxMaxExtends ){
	if( pHasBoxExtends && pBoxMinExtends.IsEqualTo( boxMinExtends )
	&& pBoxMaxExtends.IsEqualTo( boxMaxExtends ) ){
		return;
	}
	
	pBoxMinExtends = boxMinExtends;
	pBoxMaxExtends = boxMaxExtends;
	pHasBoxExtends = true;
	pWrapper.SubObjectExtendsDirty();
}

void igdeWOSubObject::ClearBoxExtends(){
	if( ! pHasBoxExtends ){
		return;
	}
	
	pBoxMinExtends.SetZero();
	pBoxMaxExtends.SetZero();
	pHasBoxExtends = false;
	pWrapper.SubObjectExtendsDirty();
}

void igdeWOSubObject::ReattachToColliders(){
	DetachFromCollider();
	AttachToCollider();
}

void igdeWOSubObject::InitTriggers(){
}

void igdeWOSubObject::UpdateTriggers(){
}

void igdeWOSubObject::UpdateVisibility(){
}

void igdeWOSubObject::UpdateLayerMasks(){
}

void igdeWOSubObject::UpdateCollisionFilter(){
}

void igdeWOSubObject::UpdateGeometry(){
}

void igdeWOSubObject::UpdateColliderResponseType(){
}

void igdeWOSubObject::ColliderUserPointerChanged(){
}

void igdeWOSubObject::Update(float){
}

void igdeWOSubObject::ResetPhysics(){
}

void igdeWOSubObject::ResetComponentTextures(){
}

void igdeWOSubObject::CameraChanged(){
}

void igdeWOSubObject::OutlineSkinChanged(){
}

bool igdeWOSubObject::IsContentVisible(){
	return false;
}



// Protected Functions
////////////////////////

igdeEnvironment &igdeWOSubObject::GetEnvironment() const{
	return pWrapper.GetEnvironment();
}

igdeGameDefinition &igdeWOSubObject::GetGameDefinition() const{
	return *GetEnvironment().GetGameProject()->GetGameDefinition();
}

deLogger &igdeWOSubObject::GetLogger() const{
	return *GetEnvironment().GetLogger();
}

deEngine &igdeWOSubObject::GetEngine() const{
	return *GetEnvironment().GetEngineController()->GetEngine();
}

bool igdeWOSubObject::GetPropertyValue( const decString &name, decString &value ) const{
	if( name.IsEmpty() ){
		return false;
	}
	
	const decString propertyName( pPrefix + name );
	
	const decString *propertyValue;
	if( pWrapper.GetProperties().GetAt( propertyName, &propertyValue ) ){
		value = *propertyValue;
		return true;
	}
	
	if( pWrapper.GetGDClass() && pWrapper.GetGDClass()->GetDefaultPropertyValue( propertyName, value ) ){
		return true;
	}
	
	return false;
}

decString igdeWOSubObject::GetStringProperty( const decString &name, const decString &defaultValue ) const{
	decString value;
	return GetPropertyValue( name, value ) ? value : defaultValue;
}

decVector igdeWOSubObject::GetVectorProperty( const decString &name, const decVector &defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		try{
			igdeCodecPropertyString codec;
			decVector vector;
			codec.DecodeVector( value, vector );
			return vector;
			
		}catch( const deException & ){
			pWrapper.GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
				"Invalid vector property '%s' value '%s'", name.GetString(), value.GetString() );
		}
	}
	return defaultValue;
}

decVector2 igdeWOSubObject::GetVector2Property( const decString &name, const decVector2 &defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		try{
			igdeCodecPropertyString codec;
			decVector2 vector;
			codec.DecodeVector2( value, vector );
			return vector;
			
		}catch( const deException & ){
			pWrapper.GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
				"Invalid vector2 property '%s' value '%s'", name.GetString(), value.GetString() );
		}
	}
	return defaultValue;
}

decQuaternion igdeWOSubObject::GetRotationProperty( const decString &name, const decQuaternion &defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		try{
			igdeCodecPropertyString codec;
			decVector vector;
			codec.DecodeVector( value, vector );
			return decQuaternion::CreateFromEuler( vector * DEG2RAD );
			
		}catch( const deException & ){
			pWrapper.GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
				"Invalid vector property '%s' value '%s'", name.GetString(), value.GetString() );
		}
	}
	return defaultValue;
}

decColor igdeWOSubObject::GetColor3Property( const decString &name, const decColor &defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		try{
			igdeCodecPropertyString codec;
			decColor color;
			codec.DecodeColor3( value, color );
			return color;
			
		}catch( const deException & ){
			pWrapper.GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
				"Invalid color3 property '%s' value '%s'", name.GetString(), value.GetString() );
		}
	}
	return defaultValue;
}

decColor igdeWOSubObject::GetColor4Property( const decString &name, const decColor &defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		try{
			igdeCodecPropertyString codec;
			decColor color;
			codec.DecodeColor4( value, color );
			return color;
			
		}catch( const deException & ){
			pWrapper.GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
				"Invalid color4 property '%s' value '%s'", name.GetString(), value.GetString() );
		}
	}
	return defaultValue;
}

bool igdeWOSubObject::GetBoolProperty( const decString &name, bool defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		return value.ToInt() != 0;
	}
	return defaultValue;
}

float igdeWOSubObject::GetFloatProperty( const decString &name, float defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		return value.ToFloat();
	}
	return defaultValue;
}

int igdeWOSubObject::GetIntProperty( const decString &name, int defaultValue ) const{
	decString value;
	if( GetPropertyValue( name, value ) ){
		return value.ToInt();
	}
	return defaultValue;
}



void igdeWOSubObject::pInitTrigger( igdeTriggerExpressionReference &trigger, const decString &propertyName ){
	if( trigger ){
		trigger->UnlinkTriggerTargets();
		trigger = NULL;
	}
	
	decString value;
	if( pWrapper.GetTriggerTable() && GetPropertyValue( propertyName, value ) && ! value.IsEmpty() ){
		try{
			const igdeTriggerExpressionParser parser;
			trigger.TakeOver( parser.StringToExpression( value ) );
			
		}catch( const deException & ){
			pWrapper.GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
				"Invalid trigger expression '%s'", value.GetString() );
		}
		
		if( trigger ){
			trigger->LinkTriggerTargets( *pWrapper.GetTriggerTable(), pWrapper.GetTriggerListener() );
		}
	}
}

void igdeWOSubObject::pClearTrigger( igdeTriggerExpressionReference &trigger ){
	if( trigger ){
		trigger->UnlinkTriggerTargets();
		trigger = NULL;
	}
}



deColliderComponent *igdeWOSubObject::GetAttachableColliderComponent() const{
	// TODO iterate over all created sub-object components finding the first usable collider component
	return pWrapper.GetColliderComponent();
}

decDMatrix igdeWOSubObject::GetBoneMatrix( const decString &name ) const{
	if( ! pWrapper.GetComponent() ){
		return decDMatrix();
	}
	const deComponent &component = *pWrapper.GetComponent();
	
	if( ! component.GetRig() ){
		return decDMatrix();
	}
	
	const int index = component.GetRig()->IndexOfBoneNamed( name );
	if( index == -1 ){
		return decDMatrix();
	}
	
	return decDMatrix( component.GetBoneAt( index ).GetMatrix() );
}

decLayerMask igdeWOSubObject::LayerMaskFromInt( int mask ) const{
	decLayerMask layerMask;
	int i;
	for( i=0; i<32; i++ ){
		if( mask & ( 1 << i ) ){
			layerMask.SetBit( i );
		}
	}
	return layerMask;
}
