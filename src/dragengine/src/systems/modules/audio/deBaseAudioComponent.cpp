/* 
 * Drag[en]gine Game Engine
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

// includes
#include "deBaseAudioComponent.h"



// Class deBaseAudioComponent
///////////////////////////////

deBaseAudioComponent::deBaseAudioComponent(){
}

deBaseAudioComponent::~deBaseAudioComponent(){
}



// Management
///////////////



// Notifications
//////////////////

void deBaseAudioComponent::PositionChanged(){
}

void deBaseAudioComponent::ScalingChanged(){
}

void deBaseAudioComponent::OrientationChanged(){
}

void deBaseAudioComponent::ModelChanged(){
}

void deBaseAudioComponent::RigChanged(){
}

void deBaseAudioComponent::SkinChanged(){
}

void deBaseAudioComponent::ModelAndSkinChanged(){
}

void deBaseAudioComponent::AudioModelChanged(){
}

void deBaseAudioComponent::VisibilityChanged(){
}

void deBaseAudioComponent::ExtendsChanged(){
}

void deBaseAudioComponent::LayerMaskChanged(){
}

void deBaseAudioComponent::MeshDirty(){
}

void deBaseAudioComponent::ParametersChanged(){
}

void deBaseAudioComponent::TextureChanged( int index, deComponentTexture &texture ){
}

void deBaseAudioComponent::DynamicSkinChanged(){
}



void deBaseAudioComponent::DecalAdded( deDecal *decal ){
}

void deBaseAudioComponent::DecalRemoved( deDecal *decal ){
}

void deBaseAudioComponent::AllDecalsRemoved(){
}
