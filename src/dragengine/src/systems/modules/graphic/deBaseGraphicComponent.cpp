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
