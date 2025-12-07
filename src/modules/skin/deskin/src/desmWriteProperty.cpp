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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "desmWriteProperty.h"
#include "desmWritePropertyNode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>



// Class desmWriteProperty
////////////////////////////

// Constructor, destructor
////////////////////////////

desmWriteProperty::desmWriteProperty(decXmlWriter &writer, const deSkin &skin, const char *propertyType) :
pWriter(writer),
pSkin(skin),
pPropertyType(propertyType)
{
	DEASSERT_NOTNULL(propertyType)
}

desmWriteProperty::~desmWriteProperty(){
}



// Visiting
/////////////

void desmWriteProperty::VisitProperty(deSkinProperty &property){
}

void desmWriteProperty::VisitValue(deSkinPropertyValue &property){
	pWriter.WriteOpeningTagStart("value");
	pWriter.WriteAttributeString("property", pPropertyType);
	if(! property.GetRenderable().IsEmpty()){
		pWriter.WriteAttributeString("renderable", property.GetRenderable());
	}
	if(! property.GetBone().IsEmpty()){
		pWriter.WriteAttributeString("bone", property.GetBone());
	}
	pWriter.WriteOpeningTagEnd(false, false);
	
	pWriter.WriteTextFloat(property.GetValue());
	
	pWriter.WriteClosingTag("value", false);
}

void desmWriteProperty::VisitColor(deSkinPropertyColor &property){
	const decColor &color = property.GetColor();
	
	pWriter.WriteOpeningTagStart("color");
	pWriter.WriteAttributeString("property", pPropertyType);
	if(fabsf(color.r) > FLOAT_SAFE_EPSILON){
		pWriter.WriteAttributeFloat("r", color.r);
	}
	if(fabsf(color.g) > FLOAT_SAFE_EPSILON){
		pWriter.WriteAttributeFloat("g", color.g);
	}
	if(fabsf(color.b) > FLOAT_SAFE_EPSILON){
		pWriter.WriteAttributeFloat("b", color.b);
	}
	if(fabsf(1.0f - color.a) > FLOAT_SAFE_EPSILON){
		pWriter.WriteAttributeFloat("a", color.a);
	}
	if(! property.GetRenderable().IsEmpty()){
		pWriter.WriteAttributeString("renderable", property.GetRenderable());
	}
	if(! property.GetBone().IsEmpty()){
		pWriter.WriteAttributeString("bone", property.GetBone());
	}
	pWriter.WriteOpeningTagEnd(true);
}

void desmWriteProperty::VisitImage(deSkinPropertyImage &property){
	pWriter.WriteOpeningTagStart("image");
	pWriter.WriteAttributeString("property", pPropertyType);
	if(! property.GetRenderable().IsEmpty()){
		pWriter.WriteAttributeString("renderable", property.GetRenderable());
	}
	if(! property.GetBone().IsEmpty()){
		pWriter.WriteAttributeString("bone", property.GetBone());
	}
	pWriter.WriteOpeningTagEnd(false, false);
	
	pWriter.WriteTextString(property.GetPath());
	
	pWriter.WriteClosingTag("image", false);
}

void desmWriteProperty::VisitVideo(deSkinPropertyVideo &property){
	pWriter.WriteOpeningTagStart("video");
	pWriter.WriteAttributeString("property", pPropertyType);
	if(! property.GetRenderable().IsEmpty()){
		pWriter.WriteAttributeString("renderable", property.GetRenderable());
	}
	if(! property.GetBone().IsEmpty()){
		pWriter.WriteAttributeString("bone", property.GetBone());
	}
	if(! property.GetSharedTime()){
		pWriter.WriteAttributeBool("sharedTime", property.GetSharedTime());
	}
	pWriter.WriteOpeningTagEnd(false, false);
	
	pWriter.WriteTextString(property.GetPath().GetString());
	
	pWriter.WriteClosingTag("video", false);
}

void desmWriteProperty::VisitConstructed(deSkinPropertyConstructed &property){
	pWriter.WriteOpeningTagStart("constructed");
	pWriter.WriteAttributeString("property", pPropertyType);
	if(! property.GetRenderable().IsEmpty()){
		pWriter.WriteAttributeString("renderable", property.GetRenderable());
	}
	if(! property.GetBone().IsEmpty()){
		pWriter.WriteAttributeString("bone", property.GetBone());
	}
	pWriter.WriteOpeningTagEnd();
	
	const deSkinPropertyNodeGroup &content = property.GetContent();
	const decPoint3 &size = content.GetSize();
	pWriter.WriteOpeningTagStart("size");
	pWriter.WriteAttributeInt("x", size.x);
	pWriter.WriteAttributeInt("y", size.y);
	if(size.z != 1){
		pWriter.WriteAttributeInt("z", size.z);
	}
	pWriter.WriteOpeningTagEnd(true);
	
	const decColor &color = property.GetColor();
	if(! color.IsEqualTo(decColor())){
		pWriter.WriteOpeningTagStart("color");
		if(fabsf(color.r) > FLOAT_SAFE_EPSILON){
			pWriter.WriteAttributeFloat("r", color.r);
		}
		if(fabsf(color.g) > FLOAT_SAFE_EPSILON){
			pWriter.WriteAttributeFloat("g", color.g);
		}
		if(fabsf(color.b) > FLOAT_SAFE_EPSILON){
			pWriter.WriteAttributeFloat("b", color.b);
		}
		if(fabsf(1.0f - color.a) > FLOAT_SAFE_EPSILON){
			pWriter.WriteAttributeFloat("a", color.a);
		}
		pWriter.WriteOpeningTagEnd(true);
	}
	
	if(property.GetTileX()){
		pWriter.WriteDataTagBool("tileX", property.GetTileX());
	}
	if(property.GetTileY()){
		pWriter.WriteDataTagBool("tileY", property.GetTileY());
	}
	if(property.GetBitCount() != 8){
		pWriter.WriteDataTagInt("bitCount", property.GetBitCount());
	}
	
	desmWritePropertyNode writer(pWriter, pSkin);
	const int count = content.GetNodeCount();
	int i;
	
	for(i=0; i<count; i++){
		content.GetNodeAt(i)->Visit(writer);
	}
	
	pWriter.WriteClosingTag("constructed");
}

void desmWriteProperty::VisitMapped(deSkinPropertyMapped &property){
	pWriter.WriteOpeningTagStart("mapped");
	pWriter.WriteAttributeString("property", pPropertyType);
	if(! property.GetRenderable().IsEmpty()){
		pWriter.WriteAttributeString("renderable", property.GetRenderable());
	}
	if(! property.GetBone().IsEmpty()){
		pWriter.WriteAttributeString("bone", property.GetBone());
	}
	pWriter.WriteOpeningTagEnd();
	
	if(property.GetRed() != -1){
		pWriter.WriteDataTagString("mappedRed", pSkin.GetMappedAt(property.GetRed())->GetName());
	}
	if(property.GetGreen() != -1){
		pWriter.WriteDataTagString("mappedGreen", pSkin.GetMappedAt(property.GetGreen())->GetName());
	}
	if(property.GetBlue() != -1){
		pWriter.WriteDataTagString("mappedBlue", pSkin.GetMappedAt(property.GetBlue())->GetName());
	}
	if(property.GetAlpha() != -1){
		pWriter.WriteDataTagString("mappedAlpha", pSkin.GetMappedAt(property.GetAlpha())->GetName());
	}
	
	pWriter.WriteClosingTag("mapped");
}
