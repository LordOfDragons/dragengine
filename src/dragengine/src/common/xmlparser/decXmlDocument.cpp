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

#include "decXmlDocument.h"
#include "decXmlVisitor.h"
#include "visitors/decXmlVisitorCleanCharData.h"
#include "visitors/decXmlVisitorStripComments.h"
#include "../exceptions.h"



// Class decXmlDocument
/////////////////////////

// Constructor, destructor
////////////////////////////

decXmlDocument::decXmlDocument() :
pStandalone(false){
}

decXmlDocument::~decXmlDocument(){
}



// Management
///////////////

void decXmlDocument::SetEncoding(const char *encoding){
	pEncoding = encoding;
}

void decXmlDocument::SetDocType(const char *docType){
	pDocType = docType;
}

void decXmlDocument::SetSystemLiteral(const char *sysLit){
	pSysLit = sysLit;
}

void decXmlDocument::SetPublicLiteral(const char *pubLit){
	pPubLit = pubLit;
}

void decXmlDocument::SetStandalone(bool standalone){
	pStandalone = standalone;
}

decXmlElementTag *decXmlDocument::GetRoot() const{
	const int count = GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		decXmlElement * const element = GetElementAt(i);
		if(element->CanCastToElementTag()){
			return element->CastToElementTag();
		}
	}
	
	return NULL;
}



void decXmlDocument::Visit(decXmlVisitor &visitor){
	visitor.VisitDocument(*this);
}



void decXmlDocument::CleanCharData(){
	decXmlVisitorCleanCharData visitor;
	Visit(visitor);
}
void decXmlDocument::StripComments(){
	decXmlVisitorStripComments visitor;
	Visit(visitor);
}



bool decXmlDocument::CanCastToDocument() const{
	return true;
}

decXmlDocument *decXmlDocument::CastToDocument(){
	return this;
}
