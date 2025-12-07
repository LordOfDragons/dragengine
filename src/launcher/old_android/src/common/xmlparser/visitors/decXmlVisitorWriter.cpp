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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlVisitorWriter.h"
#include "../decXmlAttValue.h"
#include "../decXmlCharacterData.h"
#include "../decXmlCharReference.h"
#include "../decXmlCDSect.h"
#include "../decXmlComment.h"
#include "../decXmlDocument.h"
#include "../decXmlEntityReference.h"
#include "../decXmlElementTag.h"
#include "../decXmlNamespace.h"
#include "../decXmlPI.h"
#include "../decXmlWriter.h"
#include "../../exceptions.h"



// Class decXmlVisitorWriter
//////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlVisitorWriter::decXmlVisitorWriter() :
pCompact(false),
pWriter(NULL){
}

decXmlVisitorWriter::~decXmlVisitorWriter(){
}



// Management
///////////////

void decXmlVisitorWriter::SetCompact(bool compact){
	pCompact = compact;
}

void decXmlVisitorWriter::WriteDocument(decBaseFileWriter *file, decXmlDocument &document){
	if(! file || ! document.GetRoot()){
		DETHROW(deeInvalidParam);
	}
	if(pWriter){
		DETHROW(deeInvalidAction);
	}
	
	try{
		pWriter = new decXmlWriter(file);
		pWriter->WriteXMLDeclaration();
		
		if(! document.GetDocType().IsEmpty() && ! document.GetSystemLiteral()){
			pWriter->WriteDocTypeDeclaration(document.GetDocType(), document.GetSystemLiteral());
		}
		
		document.GetRoot()->Visit(*this);
		
		delete pWriter;
		pWriter = NULL;
		
	}catch(const deException &){
		if(pWriter){
			delete pWriter;
			pWriter = NULL;
		}
		throw;
	}
}



// Visiting
/////////////

void decXmlVisitorWriter::VisitComment(decXmlComment &comment){
	pWriter->WriteComment(comment.GetComment(), ! pCompact, ! pCompact);
}

void decXmlVisitorWriter::VisitPI(decXmlPI &pi){
	// ignore
}

void decXmlVisitorWriter::VisitElementTag(decXmlElementTag &tag){
	const int elementCount = tag.GetElementCount();
	int i;
	
	// check if the tag has children
	bool firstChildIsTag = false;
	bool lastChildCData = true;
	bool emptyTag = true;
	
	for(i=0; i<elementCount; i++){
		const decXmlElement &child = *tag.GetElementAt(i);
		
		if(child.CanCastToAttValue()){
			continue;
		}
		
		if(child.CanCastToElementTag()){
			if(emptyTag){
				firstChildIsTag = true;
			}
			lastChildCData = false;
			
		}else if(child.CanCastToCharacterData() || child.CanCastToCDSect()){
			lastChildCData = true;
		}
		
		emptyTag = false;
	}
	
	// write opening tag
	pWriter->WriteOpeningTagStart(tag.GetName(), ! pCompact);
	
	for(i=0; i<elementCount; i++){
		decXmlElement &child = *tag.GetElementAt(i);
		
		if(child.CanCastToAttValue()){
			VisitAttValue(*child.CastToAttValue());
		}
	}
	
	pWriter->WriteOpeningTagEnd(emptyTag, (emptyTag || firstChildIsTag) && ! pCompact);
	
	// write tag content if present
	if(! emptyTag){
		for(i=0; i<elementCount; i++){
			decXmlElement &child = *tag.GetElementAt(i);
			if(! child.CanCastToAttValue()){
				child.Visit(*this);
			}
		}
		
		// write end tag
		pWriter->WriteClosingTag(tag.GetName(), ! lastChildCData && ! pCompact, ! pCompact);
	}
}

void decXmlVisitorWriter::VisitCharacterData(decXmlCharacterData &data){
	pWriter->WriteTextString(data.GetData());
}

void decXmlVisitorWriter::VisitEntityReference(decXmlEntityReference &ref){
	decString text;
	text.Format("&%s;", ref.GetName().GetString());
	pWriter->WriteTextString(text);
}

void decXmlVisitorWriter::VisitCharReference(decXmlCharReference &ref){
	// not created by parser
}

void decXmlVisitorWriter::VisitCDSect(decXmlCDSect &cdsect){
	pWriter->WriteTextString(cdsect.GetData());
}

void decXmlVisitorWriter::VisitAttValue(decXmlAttValue &attribute){
	pWriter->WriteAttributeString(attribute.GetName(), attribute.GetValue());
}

void decXmlVisitorWriter::VisitNamespace(decXmlNamespace &ns){
	// not used
}
