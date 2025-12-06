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

#include "dealConfigXML.h"
#include "dealConfigWindow.h"
#include "dealConfiguration.h"

#include "../logger/deLogger.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/xmlparser/decXmlWriter.h"
#include "../common/xmlparser/decXmlDocument.h"
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
	decXmlDocument::Ref xmldoc(decXmlDocument::Ref::NewWith());
	
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
