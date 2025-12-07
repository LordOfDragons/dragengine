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

#include "gdeOCForceField.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCForceField
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCForceField::gdeOCForceField() :
pRadius(1.0f),
pExponent(1.0f),
pFieldType(deForceField::eftRadial),
pApplicationType(deForceField::eatDirect),
pForce(1.0f),
pFluctuationDirection(0.0f),
pFluctuationForce(0.0f),
pEnabled(true){
}

gdeOCForceField::gdeOCForceField(const gdeOCForceField &field) :
pPosition(field.pPosition),
pRotation(field.pRotation),
pBoneName(field.pBoneName),
pInfluenceArea(field.pInfluenceArea),
pRadius(field.pRadius),
pExponent(field.pExponent),
pFieldType(field.pFieldType),
pApplicationType(field.pApplicationType),
pDirection(field.pDirection),
pForce(field.pForce),
pFluctuationDirection(field.pFluctuationDirection),
pFluctuationForce(field.pFluctuationForce),
pShape(field.pShape),
pEnabled(field.pEnabled)
{
	int i;
	for(i=0; i<=epAttachRotation; i++){
		pPropertyNames[i] = field.pPropertyNames[i];
	}
	for(i=etEnabled; i<=etEnabled; i++){
		pTriggerNames[i] = field.pTriggerNames[i];
	}
}

gdeOCForceField::~gdeOCForceField(){
}



// Management
///////////////

void gdeOCForceField::SetPosition(const decVector &position){
	pPosition = position;
}

void gdeOCForceField::SetRotation(const decVector &orientation){
	pRotation = orientation;
}

void gdeOCForceField::SetBoneName(const char *boneName){
	pBoneName = boneName;
}

void gdeOCForceField::SetInfluenceArea(const decShapeList &area){
	pInfluenceArea = area;
}

void gdeOCForceField::SetRadius(float radius){
	pRadius = decMath::max(radius, 0.0f);
}

void gdeOCForceField::SetExponent(float exponent){
	pExponent = decMath::max(exponent, 0.0f);
}

void gdeOCForceField::SetFieldType(deForceField::eFieldTypes type){
	pFieldType = type;
}

void gdeOCForceField::SetApplicationType(deForceField::eApplicationTypes type){
	pApplicationType = type;
}

void gdeOCForceField::SetDirection(const decVector &direction){
	pDirection = direction;
}

void gdeOCForceField::SetForce(float force){
	pForce = force;
}

void gdeOCForceField::SetFluctuationDirection(float fluctuation){
	pFluctuationDirection = fluctuation;
}

void gdeOCForceField::SetFluctuationForce(float fluctuation){
	pFluctuationForce = fluctuation;
}

void gdeOCForceField::SetShape(const decShapeList &shape){
	pShape = shape;
}

void gdeOCForceField::SetEnabled(bool enabled){
	pEnabled = enabled;
}



bool gdeOCForceField::IsPropertySet(eProperties property) const{
	return !pPropertyNames[property].IsEmpty();
}

const decString &gdeOCForceField::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void gdeOCForceField::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}

bool gdeOCForceField::HasPropertyWithName(const char *name) const{
	int i;
	for(i=0; i<=epAttachRotation; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	return false;
}



bool gdeOCForceField::IsTriggerSet(eTriggers trigger) const{
	return !pTriggerNames[trigger].IsEmpty();
}

const decString &gdeOCForceField::GetTriggerName(eTriggers trigger) const{
	return pTriggerNames[trigger];
}

void gdeOCForceField::SetTriggerName(eTriggers trigger, const char *name){
	pTriggerNames[trigger] = name;
}
