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

#include "igdePropertyStringEncodeShapeList.h"
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



// Class igdePropertyStringEncodeShapeList
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdePropertyStringEncodeShapeList::igdePropertyStringEncodeShapeList(decString *string){
	if(!string){
		DETHROW(deeInvalidParam);
	}
	
	pString = string;
	pRequiresSeparator = false;
}

igdePropertyStringEncodeShapeList::~igdePropertyStringEncodeShapeList(){
}



// Management
///////////////

void igdePropertyStringEncodeShapeList::Reset(){
	pRequiresSeparator = false;
}



// Visiting
/////////////

void igdePropertyStringEncodeShapeList::VisitShape(decShape &shape){
	DETHROW(deeInvalidAction);
}

void igdePropertyStringEncodeShapeList::VisitShapeSphere(decShapeSphere &sphere){
	const decVector &position = sphere.GetPosition();
	const float radius = sphere.GetRadius();
	
	pAddSeparator();
	pString->Append("sphere");
	pString->AppendFormat(":position,%g,%g,%g", position.x, position.y, position.z);
	pString->AppendFormat(":radius,%g", radius);
}

void igdePropertyStringEncodeShapeList::VisitShapeBox(decShapeBox &box){
	const decVector &position = box.GetPosition();
	const decVector rotation = decMatrix::CreateFromQuaternion(box.GetOrientation()).GetEulerAngles() / DEG2RAD;
	const decVector &extends = box.GetHalfExtends();
	
	pAddSeparator();
	pString->Append("box");
	pString->AppendFormat(":position,%g,%g,%g", position.x, position.y, position.z);
	if(!rotation.IsEqualTo(decVector())){
		pString->AppendFormat(":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z);
	}
	pString->AppendFormat(":extends,%g,%g,%g", extends.x, extends.y, extends.z);
}

void igdePropertyStringEncodeShapeList::VisitShapeCylinder(decShapeCylinder &cylinder){
	const decVector &position = cylinder.GetPosition();
	const decVector rotation = decMatrix::CreateFromQuaternion(cylinder.GetOrientation()).GetEulerAngles() / DEG2RAD;
	const float height = cylinder.GetHalfHeight();
	const float topRadius = cylinder.GetTopRadius();
	const float bottomRadius = cylinder.GetBottomRadius();
	const decVector2 &topAxisScaling = cylinder.GetTopAxisScaling();
	const decVector2 &bottomAxisScaling = cylinder.GetBottomAxisScaling();
	
	pAddSeparator();
	pString->Append("cylinder");
	pString->AppendFormat(":position,%g,%g,%g", position.x, position.y, position.z);
	if(!rotation.IsEqualTo(decVector())){
		pString->AppendFormat(":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z);
	}
	pString->AppendFormat(":height,%g:radius,%g,%g", height, topRadius, bottomRadius);
	if(!topAxisScaling.IsEqualTo(decVector2(1.0f, 1.0f))){
		pString->AppendFormat(":topAxisScaling,%g,%g", topAxisScaling.x, topAxisScaling.y);
	}
	if(!bottomAxisScaling.IsEqualTo(decVector2(1.0f, 1.0f))){
		pString->AppendFormat(":bottomAxisScaling,%g,%g", bottomAxisScaling.x, bottomAxisScaling.y);
	}
}

void igdePropertyStringEncodeShapeList::VisitShapeCapsule(decShapeCapsule &capsule){
	const decVector &position = capsule.GetPosition();
	const decVector rotation = decMatrix::CreateFromQuaternion(capsule.GetOrientation()).GetEulerAngles() / DEG2RAD;
	const float height = capsule.GetHalfHeight();
	const float topRadius = capsule.GetTopRadius();
	const float bottomRadius = capsule.GetBottomRadius();
	const decVector2 &topAxisScaling = capsule.GetTopAxisScaling();
	const decVector2 &bottomAxisScaling = capsule.GetBottomAxisScaling();
	
	pAddSeparator();
	pString->Append("capsule");
	pString->AppendFormat(":position,%g,%g,%g", position.x, position.y, position.z);
	if(!rotation.IsEqualTo(decVector())){
		pString->AppendFormat(":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z);
	}
	pString->AppendFormat(":height,%g:radius,%g,%g", height, topRadius, bottomRadius);
	if(!topAxisScaling.IsEqualTo(decVector2(1.0f, 1.0f))){
		pString->AppendFormat(":topAxisScaling,%g,%g", topAxisScaling.x, topAxisScaling.y);
	}
	if(!bottomAxisScaling.IsEqualTo(decVector2(1.0f, 1.0f))){
		pString->AppendFormat(":bottomAxisScaling,%g,%g", bottomAxisScaling.x, bottomAxisScaling.y);
	}
}

void igdePropertyStringEncodeShapeList::VisitShapeHull(decShapeHull &hull){
	const decVector &position = hull.GetPosition();
	const decVector rotation(decMatrix::CreateFromQuaternion(hull.GetOrientation()).GetEulerAngles() / DEG2RAD);
	const int pointCount = hull.GetPointCount();
	
	pAddSeparator();
	pString->Append("hull");
	pString->AppendFormat(":position,%g,%g,%g", position.x, position.y, position.z);
	if(!rotation.IsEqualTo(decVector())){
		pString->AppendFormat(":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z);
	}
	if(pointCount > 0){
		int i;
		pString->Append(":points");
		for(i=0; i<pointCount; i++){
			const decVector &point = hull.GetPointAt(i);
			pString->AppendFormat(",%g,%g,%g", point.x, point.y, point.z);
		}
	}
}



// Private Functions
//////////////////////

void igdePropertyStringEncodeShapeList::pAddSeparator(){
	if(pRequiresSeparator){
		pString->AppendCharacter(' ');
	}
	pRequiresSeparator = true;
}
