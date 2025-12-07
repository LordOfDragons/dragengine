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

#include "sePropertyNode.h"
#include "sePropertyNodeGroup.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class sePropertyNode
/////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNode::sePropertyNode(eNodeTypes nodeType, deEngine &engine, int mappedCount) :
pEngine(engine),

pParent(nullptr),
pMaskParent(nullptr),

pNodeType(nodeType),

pSize(256, 256, 1),
pRotation(0.0f),
pShearing(0.0f),

pBrightness(0.0f),
pContrast(1.0f),
pGamma(1.0f),
pColorize(1.0f, 1.0f, 1.0f),

pTransparency(1.0f),
pMask(nullptr),
pCombineMode(deSkinPropertyNode::ecmBlend),

pMapped(new seMapped::Ref[mappedCount]),
pMappedCount(mappedCount),

pSelected(false),
pActive(false){
}

sePropertyNode::sePropertyNode(const sePropertyNode &node) :
pEngine(node.pEngine),

pParent(nullptr),
pMaskParent(nullptr),

pNodeType(node.pNodeType),

pPosition(node.pPosition),
pSize(node.pSize),
pRotation(node.pRotation),
pShearing(node.pShearing),

pBrightness(node.pBrightness),
pContrast(node.pContrast),
pGamma(node.pGamma),
pColorize(node.pColorize),

pTransparency(node.pTransparency),
pMask(nullptr),
pCombineMode(node.pCombineMode),

pMapped(new seMapped::Ref[node.pMappedCount]),
pMappedCount(node.pMappedCount),

pSelected(false),
pActive(false)
{
	if(node.pMask){
		pMask = node.pMask->Copy();
		pMask->SetMaskParent(this);
	}
	
	int i;
	for(i=0; i<node.pMappedCount; i++){
		pMapped[i] = node.pMapped[i];
	}
}

sePropertyNode::~sePropertyNode(){
	if(pMask){
		pMask->SetMaskParent(nullptr);
	}
	
	if(pMapped){
		delete [] pMapped;
	}
}



// Management
///////////////

void sePropertyNode::SetParent(sePropertyNodeGroup *parent){
	if(parent == pParent){
		return;
	}
	
	pParent = parent;
	UpdateResources();
}

void sePropertyNode::SetMaskParent(sePropertyNode *maskParent){
	if(maskParent == pMaskParent){
		return;
	}
	
	pMaskParent = maskParent;
	UpdateResources();
}

seProperty *sePropertyNode::GetProperty() const{
	if(pParent){
		return pParent->GetProperty();
		
	}else if(pMaskParent){
		return pMaskParent->GetProperty();
		
	}else{
		return NULL;
	}
}



void sePropertyNode::SetPosition(const decPoint3 &position){
	if(position == pPosition){
		return;
	}
	
	pPosition = position;
	NotifyChanged();
}

void sePropertyNode::SetSize(const decPoint3 &size){
	if(size == pSize){
		return;
	}
	
	pSize = size;
	NotifyChanged();
}

void sePropertyNode::SetRotation(float rotation){
	rotation = decMath::normalize(rotation, -180.0f, 180.0f);
	
	if(fabsf(rotation - pRotation) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRotation = rotation;
	NotifyChanged();
}

void sePropertyNode::SetShearing(float shearing){
	shearing = decMath::clamp(shearing, -89.9f, 89.9f);
	
	if(fabsf(shearing - pShearing) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pShearing = shearing;
	NotifyChanged();
}

decTexMatrix2 sePropertyNode::CreateCanvasTransformMatrix() const{
	const decVector2 offset((float)abs(pSize.x) * 0.5f, (float)abs(pSize.y) * 0.5f);
	return decTexMatrix2::CreateTranslation(-offset)
		* decTexMatrix2::CreateShear( tanf( pShearing * DEG2RAD ), 0.0f )
		* decTexMatrix2::CreateRotation( pRotation * DEG2RAD )
		* decTexMatrix2::CreateTranslation( offset );
}

decTexMatrix2 sePropertyNode::CreateParentTransformMatrix() const{
	const decVector2 offset((float)abs(pSize.x) * 0.5f, (float)abs(pSize.y) * 0.5f);
	return decTexMatrix2::CreateScale(pSize.x < 0 ? -1.0f : 1.0f, pSize.y < 0 ? -1.0f : 1.0f)
		* decTexMatrix2::CreateTranslation( -offset )
		* decTexMatrix2::CreateShear( tanf( pShearing * DEG2RAD ), 0.0f )
		* decTexMatrix2::CreateRotation( pRotation * DEG2RAD )
		* decTexMatrix2::CreateTranslation( offset + decVector2( ( float )pPosition.x, ( float )pPosition.y ) );
}

decTexMatrix2 sePropertyNode::CreateScreenTransformMatrix() const{
	if(pParent){
		return CreateParentTransformMatrix() * pParent->CreateScreenTransformMatrix();
		
	}else if(pMaskParent){
		return CreateParentTransformMatrix() * pMaskParent->CreateScreenTransformMatrix();
		
	}else{
		return CreateParentTransformMatrix();
	}
}

void sePropertyNode::SetFromMatrix(const decTexMatrix2 &matrix, const decPoint3 &referenceSize,
float referenceRotation){
	if(decVector2(matrix.GetAxisX()).Length() < 0.001f){
		return; // we can not set the geometry using this matrix
	}
	
	decVector2 scaling(matrix.GetScaling());
	float rotation = matrix.GetRotation() * RAD2DEG;
	
	bool flip = (referenceSize.x < 0);
	if(decMath::normalize(rotation - referenceRotation, -180.0f, 180.0f) > 90.0f){
		flip = !flip;
	}
	
	if(flip){
		// the result has to be flipped to be correct if the node size x before the
		// scaling is negative and if the rotation is opposite to the node rotation
		// before the scaling. both flipping can apply in which case they cancel
		// each other. shearing is not affected by flipping only scaling and rotation
		scaling.Negate();
		rotation += 180.0f;
	}
	
	pSize.x = (int)(scaling.x * fabsf((float)referenceSize.x) + 0.5f);
	pSize.y = (int)(scaling.y * fabsf((float)referenceSize.y) + 0.5f);
	pSize.z = referenceSize.z;
	
	pRotation = decMath::normalize(rotation, -180.0f, 180.0f);
	
	try{
		pShearing = decMath::clamp(atanf(matrix.GetShearing()) * RAD2DEG, -89.9f, 89.9f);
		
	}catch(const deeDivisionByZero &){
		pShearing = 0.0f; // y-axis is 0-length or x/y is nearly colinear
	}
	
	const decPoint position((matrix.GetPosition() - CreateCanvasTransformMatrix().GetPosition()).Round());
	pPosition.x = position.x;
	pPosition.y = position.y;
	
	NotifyChanged();
}



void sePropertyNode::SetBrightness(float brightness){
	if(fabsf(brightness - pBrightness) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBrightness = brightness;
	NotifyChanged();
}

void sePropertyNode::SetContrast(float contrast){
	if(fabsf(contrast - pContrast) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pContrast = contrast;
	NotifyChanged();
}

void sePropertyNode::SetGamma(float gamma){
	if(fabsf(gamma - pGamma) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pGamma = gamma;
	NotifyChanged();
}

void sePropertyNode::SetColorize(const decColor &color){
	if(color.IsEqualTo(pColorize)){
		return;
	}
	
	pColorize = color;
	NotifyChanged();
}

decColorMatrix sePropertyNode::CreateCanvasColorTransformMatrix() const{
	return decColorMatrix::CreateScaling(decColor(pColorize, 1.0f))
		* decColorMatrix::CreateContrast( pContrast )
		* decColorMatrix::CreateBrightness( pBrightness );
}



void sePropertyNode::SetTransparency(float transparency){
	transparency = decMath::clamp(transparency, 0.0f, 1.0f);
	if(fabsf(transparency - pTransparency) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTransparency = transparency;
	NotifyChanged();
}

void sePropertyNode::SetMask(sePropertyNode *mask){
	if(mask == pMask){
		return;
	}
	
	if(pMask){
		pMask->SetMaskParent(NULL);
	}
	
	pMask = mask;
	
	if(mask){
		mask->AddReference();
		mask->SetMaskParent(this);
	}
	
	NotifyChanged();
}

void sePropertyNode::SetCombineMode(deSkinPropertyNode::eCombineModes mode){
	if(mode == pCombineMode){
		return;
	}
	
	pCombineMode = mode;
	
	NotifyChanged();
}

const seMapped::Ref &sePropertyNode::GetMappedFor(int type) const{
	DEASSERT_TRUE(type >= 0)
	DEASSERT_TRUE(type < pMappedCount)
	
	return pMapped[type];
}

void sePropertyNode::SetMappedFor(int type, seMapped *mapped){
	DEASSERT_TRUE(type >= 0)
	DEASSERT_TRUE(type < pMappedCount)
	
	if(pMapped[type] == mapped){
		return;
	}
	
	pMapped[type] = mapped;
	
	NotifyChanged();
}



void sePropertyNode::SetActive(bool active){
	pActive = active;
}

void sePropertyNode::SetSelected(bool selected){
	pSelected = selected;
}



void sePropertyNode::NotifyStructreChanged(){
	seProperty * const property = GetProperty();
	if(!property){
		return;
	}
	
	seTexture * const texture = property->GetTexture();
	if(!texture){
		return;
	}
	
	texture->InvalidateEngineSkin();
	
	seSkin * const skin = texture->GetSkin();
	if(skin){
		skin->NotifyPropertyNodeStructureChanged(texture, property);
	}
}

void sePropertyNode::NotifyChanged(){
	seProperty * const property = GetProperty();
	if(!property){
		return;
	}
	
	seTexture * const texture = property->GetTexture();
	if(!texture){
		return;
	}
	
	texture->InvalidateEngineSkin();
	
	seSkin * const skin = texture->GetSkin();
	if(skin){
		skin->NotifyPropertyNodeChanged(texture, property, this);
	}
}

void sePropertyNode::UpdateResources(){
	if(pMask){
		pMask->UpdateResources();
	}
}
