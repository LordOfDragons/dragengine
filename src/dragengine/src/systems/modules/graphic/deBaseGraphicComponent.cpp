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

#include "deBaseGraphicComponent.h"



// Class deBaseGraphicComponent
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deBaseGraphicComponent::deBaseGraphicComponent(){
}

deBaseGraphicComponent::~deBaseGraphicComponent(){
}



// Notifications
//////////////////

void deBaseGraphicComponent::PositionChanged(){
}

void deBaseGraphicComponent::OrientationChanged(){
}

void deBaseGraphicComponent::ScalingChanged(){
}

void deBaseGraphicComponent::ModelChanged(){
}

void deBaseGraphicComponent::ModelAndSkinChanged(){
}

void deBaseGraphicComponent::SkinChanged(){
}

void deBaseGraphicComponent::RigChanged(){
}

void deBaseGraphicComponent::OcclusionMeshChanged(){
}

void deBaseGraphicComponent::ParametersChanged(){
}

void deBaseGraphicComponent::VisibilityChanged(){
}

void deBaseGraphicComponent::ExtendsChanged(){
}

void deBaseGraphicComponent::MeshDirty(){
}



void deBaseGraphicComponent::TextureChanged( int index, deComponentTexture &texture ){
}

int deBaseGraphicComponent::IndexOfTextureClosestTo( const decVector &vector, float radius ){
	return -1;
}

void deBaseGraphicComponent::DynamicSkinChanged(){
}

void deBaseGraphicComponent::AnimatorChanged(){
}

void deBaseGraphicComponent::LayerMaskChanged(){
}

void deBaseGraphicComponent::DecalAdded( deDecal *decal ){
}

void deBaseGraphicComponent::DecalRemoved( deDecal *decal ){
}

void deBaseGraphicComponent::AllDecalsRemoved(){
}
