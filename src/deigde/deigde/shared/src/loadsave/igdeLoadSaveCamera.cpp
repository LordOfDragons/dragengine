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

#include "igdeLoadSaveCamera.h"
#include "../gui/igdeCamera.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Class igdeLoadSaveCamera
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSaveCamera::igdeLoadSaveCamera(igdeEnvironment &environment,
	deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pEnvironment(environment),
pName("Drag[en]gine Camera"),
pPattern("*.decamera"),
pDefaultExtension(".decamera"){
}

igdeLoadSaveCamera::~igdeLoadSaveCamera(){
}



// Management
///////////////

void igdeLoadSaveCamera::SetName(const char *name){
	pName = name;
}

void igdeLoadSaveCamera::SetPattern(const char *pattern){
	pPattern = pattern;
}

void igdeLoadSaveCamera::SetDefaultExtension(const char *extension){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadSaveCamera::Load(const decString &path, igdeCamera &camera, decBaseFileReader &reader){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(root)
	DEASSERT_TRUE(root->GetName() == "camera")
	
	pReadCamera(*root, camera);
}

void igdeLoadSaveCamera::Save(const igdeCamera &camera, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	xmlWriter.WriteOpeningTag("camera", false, true);
	
	pWriteCamera(xmlWriter, camera);
	
	xmlWriter.WriteClosingTag("camera", true);
}



// Private Functions
//////////////////////

void igdeLoadSaveCamera::pReadCamera(const decXmlElementTag &root, igdeCamera &camera){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(! tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "fov"){
			camera.SetFov(GetCDataFloat(*tag));
			
		}else if(tagName == "fovRatio"){
			camera.SetFovRatio(GetCDataFloat(*tag));
			
		}else if(tagName == "imageDistance"){
			camera.SetImageDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "viewDistance"){
			camera.SetViewDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "enableHDRR"){
			camera.SetEnableHDRR(GetCDataBool(*tag));
			
		}else if(tagName == "exposure"){
			camera.SetExposure(GetCDataFloat(*tag));
			
		}else if(tagName == "lowestIntensity"){
			camera.SetLowestIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "highestIntensity"){
			camera.SetHighestIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "adaptionTime"){
			camera.SetAdaptionTime(GetCDataFloat(*tag));
			
		}else if(tagName == "enableGI"){
			camera.SetEnableGI(GetCDataBool(*tag));
			
		}else if(tagName == "whiteIntensity"){
			camera.SetWhiteIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "bloomIntensity"){
			camera.SetBloomIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "bloomStrength"){
			camera.SetBloomStrength(GetCDataFloat(*tag));
			
		}else if(tagName == "bloomBlend"){
			camera.SetBloomBlend(GetCDataFloat(*tag));
			
		}else if(tagName == "bloomSize"){
			camera.SetBloomSize(GetCDataFloat(*tag));
			
		}else if(tagName == "toneMapCurve"){
			decCurveBezier curve;
			ReadCurveBezier(*tag, curve);
			camera.SetToneMapCurve(curve);
		}
	}
}



void igdeLoadSaveCamera::pWriteCamera(decXmlWriter &writer, const igdeCamera &camera){
	writer.WriteDataTagFloat("fov", camera.GetFov());
	writer.WriteDataTagFloat("fovRatio", camera.GetFovRatio());
	writer.WriteDataTagFloat("imageDistance", camera.GetImageDistance());
	writer.WriteDataTagFloat("viewDistance", camera.GetViewDistance());
	
	writer.WriteDataTagBool("enableHDRR", camera.GetEnableHDRR());
	writer.WriteDataTagFloat("exposure", camera.GetExposure());
	writer.WriteDataTagFloat("lowestIntensity", camera.GetLowestIntensity());
	writer.WriteDataTagFloat("highestIntensity", camera.GetHighestIntensity());
	writer.WriteDataTagFloat("adaptionTime", camera.GetAdaptionTime());
	
	writer.WriteDataTagBool("enableGI", camera.GetEnableGI());
	
	writer.WriteDataTagFloat("whiteIntensity", camera.GetWhiteIntensity());
	writer.WriteDataTagFloat("bloomIntensity", camera.GetBloomIntensity());
	writer.WriteDataTagFloat("bloomStrength", camera.GetBloomStrength());
	writer.WriteDataTagFloat("bloomBlend", camera.GetBloomBlend());
	writer.WriteDataTagFloat("bloomSize", camera.GetBloomSize());
	
	if(camera.GetToneMapCurve().GetPointCount() > 0){
		WriteCurveBezier(writer, "toneMapCurve", camera.GetToneMapCurve());
	}
}
