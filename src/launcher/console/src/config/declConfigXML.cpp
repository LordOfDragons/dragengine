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

#include "declConfigXML.h"
#include "declConfiguration.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class declConfigXML
////////////////////////

// Constructors and Destructors
/////////////////////////////////

declConfigXML::declConfigXML( deLogger *logger, const char *loggerSource ) : delBaseXML( logger, loggerSource ){
}

declConfigXML::~declConfigXML(){
}



// Management
///////////////

void declConfigXML::ReadFromFile( decBaseFileReader &reader, declConfiguration &config ){
	decXmlDocument::Ref xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "delauncherconsole" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void declConfigXML::WriteToFile( decBaseFileWriter &writer, const declConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void declConfigXML::pWriteConfig( decXmlWriter &writer, const declConfiguration &config ){
	writer.WriteOpeningTag( "delauncherconsole", false, true );
	
	writer.WriteClosingTag( "delauncherconsole", true );
}



void declConfigXML::pReadConfig( const decXmlElementTag &root, declConfiguration &config ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "DUMMY" ) == 0 ){
				
			}else{
				ErrorUnknownTag( root, *tag );
			}
		}
	}
}
