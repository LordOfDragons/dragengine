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

#include "dealEngineModuleXML.h"
#include "../../logger/deLogger.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/xmlparser/decXmlWriter.h"
#include "../../common/xmlparser/decXmlDocument.h"
#include "../../common/xmlparser/decXmlCharacterData.h"
#include "../../common/xmlparser/decXmlElementTag.h"
#include "../../common/xmlparser/decXmlAttValue.h"
#include "../../common/xmlparser/decXmlVisitor.h"
#include "../../common/xmlparser/decXmlParser.h"
#include "../../common/exceptions.h"



// Class dealEngineModuleXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngineModuleXML::dealEngineModuleXML(deLogger *logger, const char *loggerSource) : dealBaseXML(logger, loggerSource){
}

dealEngineModuleXML::~dealEngineModuleXML(){
}



// Management
///////////////

void dealEngineModuleXML::ReadFromFile(decBaseFileReader &reader, dealEngineModule &module){
	decXmlDocument::Ref xmldoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser parser(GetLogger());
	
	parser.ParseXml(&reader, xmldoc);
	
	xmldoc->StripComments();
	xmldoc->CleanCharData();
	
	decXmlElementTag * const root = xmldoc->GetRoot();
	if(! root || strcmp(root->GetName(), "module") != 0) DETHROW(deeInvalidParam);
	
	pReadModule(*root, module);
}



// Private Functions
//////////////////////

void dealEngineModuleXML::pReadModule(const decXmlElementTag &root, dealEngineModule &module){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				module.SetName(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "description") == 0){
				module.SetDescription(decUnicodeString::NewFromUTF8(pGetCDataString(*tag)));
				
			}else if(strcmp(tag->GetName(), "author") == 0){
				module.SetAuthor(decUnicodeString::NewFromUTF8(pGetCDataString(*tag)));
				
			}else if(strcmp(tag->GetName(), "version") == 0){
				module.SetVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "type") == 0){
				module.SetType(pModuleTypeFromString(pGetCDataString(*tag)));
				
			}else if(strcmp(tag->GetName(), "pattern") == 0){
				module.SetPattern(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "homepage") == 0){
				// no interest in this tag
				
			}else if(strcmp(tag->GetName(), "library") == 0){
				pReadModuleLibrary(*tag, module);
				
			}else if(strcmp(tag->GetName(), "data") == 0){
				// deprecated
				
			}else if(strcmp(tag->GetName(), "fallback") == 0){
				module.SetIsFallback(true);
				
			}else{
// 				pErrorUnknownTag( root, *tag );
			}
		}
	}
}

void dealEngineModuleXML::pReadModuleLibrary(const decXmlElementTag &root, dealEngineModule &module){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "file") == 0){
				module.SetLibFileName(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "size") == 0){
				module.SetLibFileSizeShould(pGetCDataInt(*tag));
				
			}else if(strcmp(tag->GetName(), "sha1") == 0){
				module.SetLibFileHashShould(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "entrypoint") == 0){
				module.SetLibFileEntryPoint(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "preloadLibrary") == 0){
				// no need to store this information. list of libraries the module needs to be preloaded.
				// this is very much internal information.
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

dealEngineModule::eModuleTypes dealEngineModuleXML::pModuleTypeFromString(const char *typeString) const{
	if(strcmp(typeString, "Graphic") == 0){
		return dealEngineModule::emtGraphic;
		
	}else if(strcmp(typeString, "Audio") == 0){
		return dealEngineModule::emtAudio;
		
	}else if(strcmp(typeString, "Input") == 0){
		return dealEngineModule::emtInput;
		
	}else if(strcmp(typeString, "Network") == 0){
		return dealEngineModule::emtNetwork;
		
	}else if(strcmp(typeString, "Physics") == 0){
		return dealEngineModule::emtPhysics;
		
	}else if(strcmp(typeString, "Image") == 0){
		return dealEngineModule::emtImage;
		
	}else if(strcmp(typeString, "Video") == 0){
		return dealEngineModule::emtVideo;
		
	}else if(strcmp(typeString, "Script") == 0){
		return dealEngineModule::emtScript;
		
	}else if(strcmp(typeString, "Model") == 0){
		return dealEngineModule::emtModel;
		
	}else if(strcmp(typeString, "Rig") == 0){
		return dealEngineModule::emtRig;
		
	}else if(strcmp(typeString, "Skin") == 0){
		return dealEngineModule::emtSkin;
		
	}else if(strcmp(typeString, "Animation") == 0){
		return dealEngineModule::emtAnimation;
		
	}else if(strcmp(typeString, "Font") == 0){
		return dealEngineModule::emtFont;
		
	}else if(strcmp(typeString, "CrashRecovery") == 0){
		return dealEngineModule::emtCrashRecovery;
		
	}else if(strcmp(typeString, "LanguagePack") == 0){
		return dealEngineModule::emtLanguagePack;
		
	}else if(strcmp(typeString, "Animator") == 0){
		return dealEngineModule::emtAnimator;
		
	}else if(strcmp(typeString, "Sound") == 0){
		return dealEngineModule::emtSound;
		
	}else if(strcmp(typeString, "Video") == 0){
		return dealEngineModule::emtVideo;
		
	}else if(strcmp(typeString, "AI") == 0){
		return dealEngineModule::emtAI;
		
	}else if(strcmp(typeString, "OcclusionMesh") == 0){
		return dealEngineModule::emtOcclusionMesh;
		
	}else if(strcmp(typeString, "Synthesizer") == 0){
		return dealEngineModule::emtSynthesizer;
		
	}else{
		return dealEngineModule::emtUnknown;
	}
}
