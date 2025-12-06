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

#include "projProjectLocalXml.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Class projProjectLocalXml
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projProjectLocalXml::projProjectLocalXml( deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
}

projProjectLocalXml::~projProjectLocalXml(){
}



// Management
///////////////

void projProjectLocalXml::ReadFromFile( decBaseFileReader &reader, projProject &project ){
	decXmlDocument::Ref xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "projectLocal" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadProjectLocal( *root, project );
}

void projProjectLocalXml::WriteToFile( decBaseFileWriter &writer, const projProject &project ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteProjectLocal( xmlWriter, project );
}



// Private Functions
//////////////////////

void projProjectLocalXml::pWriteProjectLocal( decXmlWriter &writer,
const projProject &project ){
	writer.WriteOpeningTag( "projectLocal" );
	
	if( project.GetActiveProfile() ){
		writer.WriteDataTagString( "activeProfile", project.GetActiveProfile()->GetName() );
	}
	
	if( ! project.GetActiveLaunchProfile().IsEmpty() ){
		writer.WriteDataTagString( "launchProfile", project.GetActiveLaunchProfile() );
	}
	
	writer.WriteClosingTag( "projectLocal" );
}



void projProjectLocalXml::pReadProjectLocal( const decXmlElementTag &root,
projProject &project ){
	const int count = root.GetElementCount();
	decString activeProfile;
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "activeProfile" ){
			activeProfile = GetCDataString( *tag );
			
		}else if( tagName == "launchProfile" ){
			project.SetActiveLaunchProfile( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	// select active profile
	const projProfileList &profiles = project.GetProfiles();
	
	if( ! activeProfile.IsEmpty() ){
		project.SetActiveProfile( profiles.GetNamed( activeProfile ) );
	}
	
	if( ! project.GetActiveProfile() && profiles.GetCount() > 0 ){
		project.SetActiveProfile( profiles.GetAt( 0 ) );
	}
}
