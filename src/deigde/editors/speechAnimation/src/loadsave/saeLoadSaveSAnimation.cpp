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

#include "saeLoadSaveSAnimation.h"
#include "../sanimation/saeSAnimation.h"
#include "../sanimation/phoneme/saePhoneme.h"
#include "../sanimation/dictionary/saeWord.h"
#include "../gui/saeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class saeLoadSaveSAnimation
////////////////////////////////

// Constructor, destructor
////////////////////////////

saeLoadSaveSAnimation::saeLoadSaveSAnimation(saeLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource){
	pLSSys = lssys;
	
	pName = "Speech Animation";
	pPattern = ".desanim";
}



// Loading and saving
///////////////////////

void saeLoadSaveSAnimation::LoadSAnimation(saeSAnimation &sanimation, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "speechAnimation") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadSAnimation(*root, sanimation);
}

void saeLoadSaveSAnimation::SaveSAnimation(const saeSAnimation &sanimation, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteSAnimation(xmlWriter, sanimation);
}



// Private Functions
//////////////////////

void saeLoadSaveSAnimation::pWriteSAnimation(decXmlWriter &writer, const saeSAnimation &sanimation){
	const saePhonemeList &phonemeList = sanimation.GetPhonemeList();
	const saeWordList &wordList = sanimation.GetWordList();
	int count, i;
	
	writer.WriteOpeningTag("speechAnimation", false, true);
	
	pWriteDisplay(writer, sanimation);
	
	writer.WriteDataTagString("rig", sanimation.GetRigPath());
	writer.WriteDataTagString("animation", sanimation.GetAnimationPath());
	
	if(!sanimation.GetNeutralMoveName().IsEmpty()){
		writer.WriteDataTagString("neutralMoveName", sanimation.GetNeutralMoveName());
	}
	
	const decStringSet &neutralVertexPositionSets = sanimation.GetNeutralVertexPositionSets();
	count = neutralVertexPositionSets.GetCount();
	for(i=0; i<count; i++){
		writer.WriteDataTagString("neutralVertexPositionSet", neutralVertexPositionSets.GetAt(i));
	}
	
	count = phonemeList.GetCount();
	for(i=0; i<count; i++){
		pWritePhoneme(writer, *phonemeList.GetAt(i));
	}
	
	count = wordList.GetCount();
	for(i=0; i<count; i++){
		pWriteWord(writer, *wordList.GetAt(i));
	}
	
	writer.WriteClosingTag("speechAnimation", true);
}

void saeLoadSaveSAnimation::pWriteDisplay(decXmlWriter &writer, const saeSAnimation &sanimation){
	writer.WriteOpeningTag("display");
	
	writer.WriteDataTagString("model", sanimation.GetDisplayModelPath());
	writer.WriteDataTagString("skin", sanimation.GetDisplaySkinPath());
	writer.WriteDataTagString("rig", sanimation.GetDisplayRigPath());
	
	writer.WriteClosingTag("display");
}

void saeLoadSaveSAnimation::pWritePhoneme(decXmlWriter &writer, const saePhoneme &phoneme){
	writer.WriteOpeningTagStart("phoneme");
	writer.WriteAttributeInt("ipa", phoneme.GetIPA());
	writer.WriteOpeningTagEnd();
	
	if(!phoneme.GetSampleText().IsEmpty()){
		writer.WriteDataTagString("sampleText", phoneme.GetSampleText());
	}
	if(!phoneme.GetMoveName().IsEmpty()){
		writer.WriteDataTagString("moveName", phoneme.GetMoveName());
	}
	if(!phoneme.GetVertexPositionSet().IsEmpty()){
		writer.WriteDataTagString("vertexPositionSet", phoneme.GetVertexPositionSet());
	}
	writer.WriteDataTagFloat("length", phoneme.GetLength());
	
	writer.WriteClosingTag("phoneme");
}

void saeLoadSaveSAnimation::pWriteWord(decXmlWriter &writer, const saeWord &word){
	writer.WriteOpeningTagStart("word");
	writer.WriteAttributeString("name", word.GetName().GetString());
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString(decUnicodeString::NewFromUTF8("phonetics"), word.GetPhonetics());
	
	writer.WriteClosingTag("word");
}



void saeLoadSaveSAnimation::pReadSAnimation(const decXmlElementTag &root, saeSAnimation &sanimation){
	const int elementCount = root.GetElementCount();
	decStringSet neutralVertexPositionSets;
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "display") == 0){
				pReadDisplay(*tag, sanimation);
				
			}else if(strcmp(tag->GetName(), "rig") == 0){
				sanimation.SetRigPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "animation") == 0){
				sanimation.SetAnimationPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "neutralMoveName") == 0){
				sanimation.SetNeutralMoveName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "neutralVertexPositionSet") == 0){
				neutralVertexPositionSets.Add(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "phoneme") == 0){
				pReadPhoneme(*tag, sanimation);
				
			}else if(strcmp(tag->GetName(), "word") == 0){
				pReadWord(*tag, sanimation);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
	
	sanimation.SetNeutralVertexPositionSets(neutralVertexPositionSets);
}

void saeLoadSaveSAnimation::pReadDisplay(const decXmlElementTag &root, saeSAnimation &sanimation){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "model") == 0){
				sanimation.SetDisplayModelPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "skin") == 0){
				sanimation.SetDisplaySkinPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "rig") == 0){
				sanimation.SetDisplayRigPath(GetCDataString(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void saeLoadSaveSAnimation::pReadPhoneme(const decXmlElementTag &root, saeSAnimation &sanimation){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	saePhoneme::Ref phoneme = NULL;
	decString text;
	int e;
	
	try{
		phoneme.TakeOver(new saePhoneme);
		if(!phoneme) DETHROW(deeOutOfMemory);
		
		/*if( ! HasAttribute( root, "name" ) ){
			LogErrorGenericProblemValue(root, "name", "Attribute missing");
		}*/
		phoneme->SetIPA(GetAttributeInt(root, "ipa"));
		
		if(sanimation.GetPhonemeList().HasIPA(phoneme->GetIPA())){
			text.Format("%i", phoneme->GetIPA());
			LogErrorGenericProblemValue(root, text.GetString(), "Duplicate Phoneme");
		}
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(tag->GetName() == "sampleText"){
					phoneme->SetSampleText(GetCDataString(*tag));
					
				}else if(tag->GetName() == "moveName"){
					phoneme->SetMoveName(GetCDataString(*tag));
					
				}else if(tag->GetName() == "vertexPositionSet"){
					phoneme->SetVertexPositionSet(GetCDataString(*tag));
					
				}else if(tag->GetName() == "length"){
					phoneme->SetLength(GetCDataFloat(*tag));
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		sanimation.AddPhoneme(phoneme);
		
	}catch(const deException &){
		throw;
	}
}

void saeLoadSaveSAnimation::pReadWord(const decXmlElementTag &root, saeSAnimation &sanimation){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	saeWord::Ref word = NULL;
	int e;
	
	try{
		word.TakeOver(new saeWord);
		if(!word) DETHROW(deeOutOfMemory);
		
		/*if( ! HasAttribute( root, "name" ) ){
			LogErrorGenericProblemValue(root, "name", "Attribute missing");
		}*/
		word->SetName(GetAttributeString(root, "name"));
		
		if(sanimation.GetWordList().HasNamed(word->GetName().GetString())){
			LogErrorGenericProblemValue(root, word->GetName().GetString(), "Duplicate Word");
		}
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "phonetics") == 0){
					word->SetPhonetics(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		sanimation.AddWord(word);
		
	}catch(const deException &){
		throw;
	}
}
