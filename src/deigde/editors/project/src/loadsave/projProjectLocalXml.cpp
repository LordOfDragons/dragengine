/* 
 * Drag[en]gine IGDE Project Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
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
	decXmlDocumentReference xmlDoc;
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
