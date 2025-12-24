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

#include "seLoadSaveSkin.h"
#include "../skin/seSkin.h"
#include "../skin/mapped/seMapped.h"
#include "../skin/texture/seTexture.h"
#include "../skin/property/seProperty.h"
#include "../skin/property/node/sePropertyNodeImage.h"
#include "../skin/property/node/sePropertyNodeGroup.h"
#include "../skin/property/node/sePropertyNodeShape.h"
#include "../skin/property/node/sePropertyNodeText.h"
#include "../skin/seSkinBuilder.h"
#include "../gui/seWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinBuilder.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitorIdentify.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class seLoadSaveSkin
/////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSkin::seLoadSaveSkin(deBaseSkinModule *module) :
pModule(module)
{
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	for(i=0; i<patternCount; i++){
		if(i > 0){
			pPattern.AppendCharacter(',');
		}
		pPattern.AppendCharacter('*');
		pPattern.Append(patternList.GetAt(i));
	}
}

seLoadSaveSkin::~seLoadSaveSkin(){
}



// Management
///////////////

void seLoadSaveSkin::SetName(const char *name){
	pName = name;
}

void seLoadSaveSkin::SetPattern(const char *pattern){
	pPattern = pattern;
}



void seLoadSaveSkin::LoadSkin(seSkin *skin, decBaseFileReader *file,
const igdeTextureProperty::List &knownPropertyList){
	if(!skin || !file){
		DETHROW(deeInvalidParam);
	}
	
	deEngine *engine = pModule->GetGameEngine();
	int p, propertyCount;
	int t, textureCount;
	int i;
	
	deSkinBuilder builder;
	
	const deSkin::Ref engSkin(engine->GetSkinManager()->CreateSkin("", builder));
	pModule->LoadSkin(*file, *engSkin);
	
	// mapped
	const int mappedCount = engSkin->GetMappedCount();
	for(i=0; i<mappedCount; i++){
		const deSkinMapped &engMapped = *engSkin->GetMappedAt(i);
		
		const seMapped::Ref mapped(seMapped::Ref::New(engMapped.GetName()));
		
		mapped->SetCurve(engMapped.GetCurve());
		mapped->SetInputType(engMapped.GetInputType());
		mapped->SetInputLower(engMapped.GetInputLower());
		mapped->SetInputUpper(engMapped.GetInputUpper());
		mapped->SetInputClamped(engMapped.GetInputClamped());
		mapped->SetOutputLower(engMapped.GetOutputLower());
		mapped->SetOutputUpper(engMapped.GetOutputUpper());
		mapped->SetBone(engMapped.GetBone());
		mapped->SetRenderable(engMapped.GetRenderable());
		mapped->SetRenderableComponent(engMapped.GetRenderableComponent());
		
		skin->AddMapped(mapped);
	}
	
	const seMapped::List &listMapped = skin->GetMapped();
	
	// textures
	textureCount = engSkin->GetTextureCount();
	
	for(t=0; t<textureCount; t++){
		const deSkinTexture &engTexture = *engSkin->GetTextureAt(t);
		
		const seTexture::Ref texture(seTexture::Ref::New(engine));
		texture->SetName(engTexture.GetName());
		
		propertyCount = engTexture.GetPropertyCount();
		
		for(p=0; p<propertyCount; p++){
			deSkinProperty &engProperty = *engTexture.GetPropertyAt(p);
			
			seProperty::Ref property(seProperty::Ref::New(engine));
			property->SetName(engProperty.GetType());
			property->InitDefaults(knownPropertyList);
			
			property->SetRenderableName(engProperty.GetRenderable());
			property->SetBoneName(engProperty.GetBone());
			
			deSkinPropertyVisitorIdentify identifyProperty;
			engProperty.Visit(identifyProperty);
			
			switch(identifyProperty.GetType()){
			case deSkinPropertyVisitorIdentify::eptValue:
				property->SetValueType(seProperty::evtValue);
				property->SetValue(identifyProperty.CastToValue().GetValue());
				break;
				
			case deSkinPropertyVisitorIdentify::eptColor:
				property->SetValueType(seProperty::evtColor);
				property->SetColor(identifyProperty.CastToColor().GetColor());
				break;
				
			case deSkinPropertyVisitorIdentify::eptImage:
				property->SetValueType(seProperty::evtImage);
				property->SetImagePath(identifyProperty.CastToImage().GetPath());
				break;
				
			case deSkinPropertyVisitorIdentify::eptVideo:
				property->SetValueType(seProperty::evtVideo);
				property->SetVideoPath(identifyProperty.CastToVideo().GetPath().GetString());
				property->SetVideoSharedTime(identifyProperty.CastToVideo().GetSharedTime());
				break;
				
			case deSkinPropertyVisitorIdentify::eptMapped:{
				const deSkinPropertyMapped &mapped = identifyProperty.CastToMapped();
				property->SetValueType(seProperty::evtMapped);
				if(mapped.GetRed() != -1){
					property->SetMappedComponent(0, listMapped.GetAt(mapped.GetRed()));
				}
				if(mapped.GetGreen() != -1){
					property->SetMappedComponent(1, listMapped.GetAt(mapped.GetGreen()));
				}
				if(mapped.GetBlue() != -1){
					property->SetMappedComponent(2, listMapped.GetAt(mapped.GetBlue()));
				}
				if(mapped.GetAlpha() != -1){
					property->SetMappedComponent(3, listMapped.GetAt(mapped.GetAlpha()));
				}
				}break;
				
			case deSkinPropertyVisitorIdentify::eptConstructed:{
				const deSkinPropertyConstructed &constructed = identifyProperty.CastToConstructed();
				property->SetValueType(seProperty::evtConstructed);
				property->SetNodeColor(constructed.GetColor());
				property->SetNodeTileX(constructed.GetTileX());
				property->SetNodeTileY(constructed.GetTileY());
				property->SetNodeBitCount(constructed.GetBitCount());
				property->SetNodeGroup(LoadPropertyNodeGroup(*skin, constructed.GetContent()));
				}break;
				
			default:
				property = nullptr;
			}
			
			if(property){
				texture->AddProperty(property);
			}
		}
		
		skin->AddTexture(texture);
	}
}

sePropertyNodeGroup::Ref seLoadSaveSkin::LoadPropertyNodeGroup(seSkin &skin,
const deSkinPropertyNodeGroup &engNodeGroup){
	deEngine &engine = *pModule->GetGameEngine();
	deSkinPropertyNodeVisitorIdentify identifyNode;
	const int count = engNodeGroup.GetNodeCount();
	
	const sePropertyNodeGroup::Ref nodeGroup(sePropertyNodeGroup::Ref::New(engine));
	LoadPropertyNodeCommon(skin, nodeGroup, engNodeGroup);
	
	int i;
	for(i=0; i<count; i++){
		nodeGroup->AddNode(LoadPropertyNode(skin, *engNodeGroup.GetNodeAt(i)));
	}
	
	return nodeGroup;
}

sePropertyNode::Ref seLoadSaveSkin::LoadPropertyNode(seSkin &skin, deSkinPropertyNode &engNode){
	deEngine &engine = *pModule->GetGameEngine();
	deSkinPropertyNodeVisitorIdentify identifyNode;
	int i;
	
	engNode.Visit(identifyNode);
	
	switch(identifyNode.GetType()){
	case deSkinPropertyNodeVisitorIdentify::entImage:{
		const deSkinPropertyNodeImage &engNodeImage = identifyNode.CastToImage();
		const sePropertyNodeImage::Ref childImage(sePropertyNodeImage::Ref::New(engine));
		LoadPropertyNodeCommon(skin, childImage, engNode);
		childImage->SetPath(engNodeImage.GetPath());
		childImage->SetRepeat(engNodeImage.GetRepeat());
		return childImage;
		}
		
	case deSkinPropertyNodeVisitorIdentify::entShape:{
		const deSkinPropertyNodeShape &engNodeShape = identifyNode.CastToShape();
		const sePropertyNodeShape::Ref childShape(sePropertyNodeShape::Ref::New(engine));
		LoadPropertyNodeCommon(skin, childShape, engNode);
		childShape->SetShapeType(engNodeShape.GetShapeType());
		childShape->SetFillColor(engNodeShape.GetFillColor());
		childShape->SetLineColor(engNodeShape.GetLineColor());
		childShape->SetThickness(engNodeShape.GetThickness());
		
		const struct sMappedShape{
			sePropertyNodeShape::eShapeMapped to;
			deSkinPropertyNodeShape::eShapeMapped from;
		} mappingsShape[9] = {
			{sePropertyNodeShape::esmFillColorRed, deSkinPropertyNodeShape::esmFillColorRed},
			{sePropertyNodeShape::esmFillColorGreen, deSkinPropertyNodeShape::esmFillColorGreen},
			{sePropertyNodeShape::esmFillColorBlue, deSkinPropertyNodeShape::esmFillColorBlue},
			{sePropertyNodeShape::esmFillColorAlpha, deSkinPropertyNodeShape::esmFillColorAlpha},
			{sePropertyNodeShape::esmLineColorRed, deSkinPropertyNodeShape::esmLineColorRed},
			{sePropertyNodeShape::esmLineColorGreen, deSkinPropertyNodeShape::esmLineColorGreen},
			{sePropertyNodeShape::esmLineColorBlue, deSkinPropertyNodeShape::esmLineColorBlue},
			{sePropertyNodeShape::esmLineColorAlpha, deSkinPropertyNodeShape::esmLineColorAlpha},
			{sePropertyNodeShape::esmThickness, deSkinPropertyNodeShape::esmThickness}
		};
		
		for(i=0; i<9; i++){
			const int index = engNodeShape.GetShapeMappedFor(mappingsShape[i].from);
			if(index != -1){
				childShape->SetMappedFor(mappingsShape[i].to, skin.GetMapped().GetAt(index));
			}
		}
		return childShape;
		}
		
	case deSkinPropertyNodeVisitorIdentify::entText:{
		const deSkinPropertyNodeText &engNodeText = identifyNode.CastToText();
		const sePropertyNodeText::Ref childText(sePropertyNodeText::Ref::New(engine));
		LoadPropertyNodeCommon(skin, childText, engNode);
		childText->SetPath(engNodeText.GetPath());
		childText->SetTextSize(engNodeText.GetFontSize());
		childText->SetText(engNodeText.GetText());
		childText->SetColor(engNodeText.GetColor());
		
		const struct sMappedText{
			sePropertyNodeText::eTextMapped to;
			deSkinPropertyNodeText::eTextMapped from;
		} mappingsText[4] = {
			{sePropertyNodeText::etmFontSize, deSkinPropertyNodeText::etmFontSize},
			{sePropertyNodeText::etmColorRed, deSkinPropertyNodeText::etmColorRed},
			{sePropertyNodeText::etmColorGreen, deSkinPropertyNodeText::etmColorGreen},
			{sePropertyNodeText::etmColorBlue, deSkinPropertyNodeText::etmColorBlue}
		};
		
		for(i=0; i<4; i++){
			const int index = engNodeText.GetTextMappedFor(mappingsText[i].from);
			if(index != -1){
				childText->SetMappedFor(mappingsText[i].to, skin.GetMapped().GetAt(index));
			}
		}
		return childText;
		}
		
	case deSkinPropertyNodeVisitorIdentify::entGroup:
		return LoadPropertyNodeGroup(skin, identifyNode.CastToGroup());
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void seLoadSaveSkin::LoadPropertyNodeCommon(seSkin &skin, sePropertyNode &node,
const deSkinPropertyNode &engNode){
	node.SetPosition(engNode.GetPosition());
	node.SetSize(engNode.GetSize());
	node.SetRotation(engNode.GetRotation() * RAD2DEG);
	node.SetShearing(engNode.GetShear() * RAD2DEG);
	
	node.SetBrightness(engNode.GetBrightness());
	node.SetContrast(engNode.GetContrast());
	node.SetGamma(engNode.GetGamma());
	node.SetColorize(engNode.GetColorize());
	
	node.SetTransparency(engNode.GetTransparency());
	node.SetCombineMode(engNode.GetCombineMode());
	
	if(engNode.GetMask()){
		node.SetMask(LoadPropertyNode(skin, *engNode.GetMask()));
	}
	
	const struct sMapped{
		sePropertyNode::eMapped to;
		deSkinPropertyNode::eMapped from;
	} mappings[15] = {
		{sePropertyNode::emPositionX, deSkinPropertyNode::emPositionX},
		{sePropertyNode::emPositionY, deSkinPropertyNode::emPositionY},
		{sePropertyNode::emPositionZ, deSkinPropertyNode::emPositionZ},
		{sePropertyNode::emSizeX, deSkinPropertyNode::emSizeX},
		{sePropertyNode::emSizeY, deSkinPropertyNode::emSizeY},
		{sePropertyNode::emSizeZ, deSkinPropertyNode::emSizeZ},
		{sePropertyNode::emRotation, deSkinPropertyNode::emRotation},
		{sePropertyNode::emShear, deSkinPropertyNode::emShear},
		{sePropertyNode::emBrightness, deSkinPropertyNode::emBrightness},
		{sePropertyNode::emContrast, deSkinPropertyNode::emContrast},
		{sePropertyNode::emGamma, deSkinPropertyNode::emGamma},
		{sePropertyNode::emColorizeRed, deSkinPropertyNode::emColorizeRed},
		{sePropertyNode::emColorizeGreen, deSkinPropertyNode::emColorizeGreen},
		{sePropertyNode::emColorizeBlue, deSkinPropertyNode::emColorizeBlue},
		{sePropertyNode::emTransparency, deSkinPropertyNode::emTransparency}
	};
	
	int i;
	for(i=0; i<15; i++){
		const int index = engNode.GetMappedFor(mappings[i].from);
		if(index != -1){
			node.SetMappedFor(mappings[i].to, skin.GetMapped().GetAt(index));
		}
	}
}

void seLoadSaveSkin::SaveSkin(seSkin *skin, decBaseFileWriter *file){
	if(!skin || !file){
		DETHROW(deeInvalidParam);
	}
	
	// the skin to save does not contain one single skin but actually a bunch of
	// skins assigned to components for quicker editing. for saving we need an
	// entire skin with all texture combined. for this we build a temporary skin
	// containing all textures. since the textures hold the data in their skins
	// already no loading is done and this process should be quick.
	seSkinBuilder builder(*skin);
	pModule->SaveSkin(*file, skin->GetEngine()->GetSkinManager()->CreateSkin("", builder));
}
