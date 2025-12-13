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

#include "aeLoadSaveAttachmentConfig.h"
#include "../animator/aeAnimator.h"
#include "../animator/attachment/aeAttachment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>



// Class aeLoadSaveAttachmentConfig
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeLoadSaveAttachmentConfig::aeLoadSaveAttachmentConfig(deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pName("Attachment Configuration"),
pPattern(".deac"){
}



// Loading and saving
///////////////////////

void aeLoadSaveAttachmentConfig::LoadAttachmentConfig(aeAnimator &animator, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "attachmentConfig") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadConfiguration(*root, animator);
}

void aeLoadSaveAttachmentConfig::SaveAttachmentConfig(const aeAnimator &animator, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfiguration(xmlWriter, animator);
}



// Private Functions
//////////////////////

void aeLoadSaveAttachmentConfig::pWriteConfiguration(decXmlWriter &writer, const aeAnimator &animator){
	writer.WriteOpeningTag("attachmentConfig", false, true);
	
	const int attachmentCount = animator.GetAttachmentCount();
	int i;
	
	for(i=0; i<attachmentCount; i++){
		pWriteAttachment(writer, *animator.GetAttachmentAt(i));
	}
	
	writer.WriteClosingTag("attachmentConfig", true);
}

void aeLoadSaveAttachmentConfig::pWriteAttachment(decXmlWriter &writer, const aeAttachment &attachment){
	const igdeWObject &wpobject = attachment.GetObjectWrapper();
	const decDVector &position = wpobject.GetPosition();
	const decVector orientation(wpobject.GetOrientation().GetEulerAngles() * RAD2DEG);
	const decVector &scaling = wpobject.GetScaling();
	
	writer.WriteOpeningTag("attachment");
	
	writer.WriteDataTagString("name", attachment.GetName());
	
	switch(attachment.GetAttachType()){
	case aeAttachment::eatNone:
		writer.WriteDataTagString("attachType", "none");
		break;
		
	case aeAttachment::eatBone:
		writer.WriteDataTagString("attachType", "bone");
		break;
		
	case aeAttachment::eatRig:
		writer.WriteDataTagString("attachType", "rig");
		break;
	};
	
	writer.WriteDataTagString("attachBone", attachment.GetBoneName());
	
	if(wpobject.GetGDClass()){
		writer.WriteDataTagString("gdclass", wpobject.GetGDClass()->GetName());
	}
	
	if(!position.IsEqualTo(decDVector())){
		writer.WriteOpeningTagStart("position");
		writer.WriteAttributeDouble("x", position.x);
		writer.WriteAttributeDouble("y", position.y);
		writer.WriteAttributeDouble("z", position.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!orientation.IsEqualTo(decVector())){
		writer.WriteOpeningTagStart("orientation");
		writer.WriteAttributeFloat("x", orientation.x);
		writer.WriteAttributeFloat("y", orientation.y);
		writer.WriteAttributeFloat("z", orientation.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!scaling.IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("scaling");
		writer.WriteAttributeFloat("x", scaling.x);
		writer.WriteAttributeFloat("y", scaling.y);
		writer.WriteAttributeFloat("z", scaling.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!wpobject.GetVisible()){
		writer.WriteDataTagBool("visible", false);
	}
	
	if(wpobject.GetDynamicCollider()){
		writer.WriteDataTagBool("dynamicCollider", true);
	}
	
	writer.WriteClosingTag("attachment");
}



void aeLoadSaveAttachmentConfig::pReadConfiguration(const decXmlElementTag &root, aeAnimator &animator){
	const int elementCount = root.GetElementCount();
	int i;
	
	animator.RemoveAllAttachments();
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		
		if(tag){
			if(strcmp(tag->GetName(), "attachment") == 0){
				pReadAttachment(*tag, animator);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void aeLoadSaveAttachmentConfig::pReadAttachment(const decXmlElementTag &root, aeAnimator &animator){
	const aeAttachment::Ref attachment(aeAttachment::Ref::New(animator.GetEnvironment()));
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				attachment->SetName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "attachType") == 0){
				const decString typeName(GetCDataString(*tag));
				
				if(typeName == "none"){
					attachment->SetAttachType(aeAttachment::eatNone);
					
				}else if(typeName == "bone"){
					attachment->SetAttachType(aeAttachment::eatBone);
					
				}else if(typeName == "rig"){
					attachment->SetAttachType(aeAttachment::eatRig);
					
				}else{
					LogWarnUnknownValue(*tag, typeName);
				}
				
			}else if(strcmp(tag->GetName(), "attachBone") == 0){
				attachment->SetBoneName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "gdclass") == 0){
				attachment->GetObjectWrapper()->SetGDClassName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "position") == 0){
				attachment->GetObjectWrapper()->SetPosition(decDVector(GetAttributeDouble(*tag, "x"),
					GetAttributeDouble(*tag, "y"), GetAttributeDouble(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "orientation") == 0){
				attachment->GetObjectWrapper()->SetOrientation(decQuaternion::CreateFromEuler(
					GetAttributeFloat(*tag, "x") * DEG2RAD, GetAttributeFloat(*tag, "y") * DEG2RAD,
					GetAttributeFloat(*tag, "z") * DEG2RAD));
				
			}else if(strcmp(tag->GetName(), "scaling") == 0){
				attachment->GetObjectWrapper()->SetScaling(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "visible") == 0){
				attachment->GetObjectWrapper()->SetVisible(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "dynamicCollider") == 0){
				attachment->GetObjectWrapper()->SetDynamicCollider(GetCDataBool(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
	
	animator.AddAttachment(attachment);
}
