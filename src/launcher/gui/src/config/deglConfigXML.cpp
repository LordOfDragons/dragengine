/* 
 * Drag[en]gine GUI Launcher
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

#include "deglConfigXML.h"
#include "deglConfigWindow.h"
#include "deglConfiguration.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class deglConfigXML
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglConfigXML::deglConfigXML( deLogger *logger, const char *loggerSource ) : deglBaseXML( logger, loggerSource ){
}

deglConfigXML::~deglConfigXML(){
}



// Management
///////////////

void deglConfigXML::ReadFromFile( decBaseFileReader &reader, deglConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "delaunchergui" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void deglConfigXML::WriteToFile( decBaseFileWriter &writer, const deglConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void deglConfigXML::pWriteConfig( decXmlWriter &writer, const deglConfiguration &config ){
	writer.WriteOpeningTag( "delaunchergui", false, true );
	
	pWriteWindow( writer, config.GetWindowMain(), "windowMain" );
	
	writer.WriteClosingTag( "delaunchergui", true );
}

void deglConfigXML::pWriteWindow( decXmlWriter &writer, const deglConfigWindow &window, const char *tagName ){
	writer.WriteOpeningTag( tagName, false, true );
	
	// NOTE windows has a tendency to somehow produce -32000 as coordinates. if this is
	//      saved the user is no more able to get the window back visible. prevent this
	//      from happening.
#ifdef OS_W32
	writer.WriteDataTagInt( "x", decMath::max( window.GetX(), 0 ) );
	writer.WriteDataTagInt( "y", decMath::max( window.GetY(), 10 ) );
#else
	writer.WriteDataTagInt( "x", window.GetX() );
	writer.WriteDataTagInt( "y", window.GetY() );
#endif
	
	writer.WriteDataTagInt( "width", window.GetWidth() );
	writer.WriteDataTagInt( "height", window.GetHeight() );
	
	writer.WriteClosingTag( tagName, true );
}



void deglConfigXML::pReadConfig( const decXmlElementTag &root, deglConfiguration &config ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "windowMain" ) == 0 ){
				pReadWindow( *tag, config.GetWindowMain() );
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}

void deglConfigXML::pReadWindow( const decXmlElementTag &root, deglConfigWindow &window ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "x" ) == 0 ){
				window.SetX( pGetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "y" ) == 0 ){
				window.SetY( pGetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "width" ) == 0 ){
				window.SetWidth( pGetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "height" ) == 0 ){
				window.SetHeight( pGetCDataInt( *tag ) );
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}
