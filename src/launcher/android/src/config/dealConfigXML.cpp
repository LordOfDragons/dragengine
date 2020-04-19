/* 
 * Drag[en]gine Android Launcher
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

#include "dealConfigXML.h"
#include "dealConfigWindow.h"
#include "dealConfiguration.h"

#include "../logger/deLogger.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/xmlparser/decXmlWriter.h"
#include "../common/xmlparser/decXmlDocument.h"
#include "../common/xmlparser/decXmlDocumentReference.h"
#include "../common/xmlparser/decXmlCharacterData.h"
#include "../common/xmlparser/decXmlElementTag.h"
#include "../common/xmlparser/decXmlAttValue.h"
#include "../common/xmlparser/decXmlVisitor.h"
#include "../common/xmlparser/decXmlParser.h"
#include "../common/exceptions.h"



// Class dealConfigXML
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealConfigXML::dealConfigXML( deLogger *logger, const char *loggerSource ) :
dealBaseXML( logger, loggerSource ){
}

dealConfigXML::~dealConfigXML(){
}



// Management
///////////////

void dealConfigXML::ReadFromFile( decBaseFileReader &reader, dealConfiguration &config ){
	decXmlDocumentReference xmldoc;
	xmldoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	
	parser.ParseXml( &reader, xmldoc );
	
	xmldoc->StripComments();
	xmldoc->CleanCharData();
	
	decXmlElementTag * const root = xmldoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "delaunchergui" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void dealConfigXML::WriteToFile( decBaseFileWriter &writer, const dealConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void dealConfigXML::pWriteConfig( decXmlWriter &writer, const dealConfiguration &config ){
	writer.WriteOpeningTag( "delaunchergui", false, true );
	
	pWriteWindow( writer, config.GetWindowMain(), "windowMain" );
	
	writer.WriteClosingTag( "delaunchergui", true );
}

void dealConfigXML::pWriteWindow( decXmlWriter &writer, const dealConfigWindow &window, const char *tagName ){
	writer.WriteOpeningTag( tagName, false, true );
	
	writer.WriteDataTagInt( "x", window.GetX() );
	writer.WriteDataTagInt( "y", window.GetY() );
	writer.WriteDataTagInt( "width", window.GetWidth() );
	writer.WriteDataTagInt( "height", window.GetHeight() );
	
	writer.WriteClosingTag( tagName, true );
}



void dealConfigXML::pReadConfig( const decXmlElementTag &root, dealConfiguration &config ){
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

void dealConfigXML::pReadWindow( const decXmlElementTag &root, dealConfigWindow &window ){
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
