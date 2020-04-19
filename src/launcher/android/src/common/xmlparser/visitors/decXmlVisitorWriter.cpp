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
pCompact( false ),
pWriter( NULL ){
}

decXmlVisitorWriter::~decXmlVisitorWriter(){
}



// Management
///////////////

void decXmlVisitorWriter::SetCompact( bool compact ){
	pCompact = compact;
}

void decXmlVisitorWriter::WriteDocument( decBaseFileWriter *file, decXmlDocument &document ){
	if( ! file || ! document.GetRoot() ){
		DETHROW( deeInvalidParam );
	}
	if( pWriter ){
		DETHROW( deeInvalidAction );
	}
	
	try{
		pWriter = new decXmlWriter( file );
		pWriter->WriteXMLDeclaration();
		
		if( ! document.GetDocType().IsEmpty() && ! document.GetSystemLiteral() ){
			pWriter->WriteDocTypeDeclaration( document.GetDocType(), document.GetSystemLiteral() );
		}
		
		document.GetRoot()->Visit( *this );
		
		delete pWriter;
		pWriter = NULL;
		
	}catch( const deException & ){
		if( pWriter ){
			delete pWriter;
			pWriter = NULL;
		}
		throw;
	}
}



// Visiting
/////////////

void decXmlVisitorWriter::VisitComment( decXmlComment &comment ){
	pWriter->WriteComment( comment.GetComment(), ! pCompact, ! pCompact );
}

void decXmlVisitorWriter::VisitPI( decXmlPI &pi ){
	// ignore
}

void decXmlVisitorWriter::VisitElementTag( decXmlElementTag &tag ){
	const int elementCount = tag.GetElementCount();
	int i;
	
	// check if the tag has children
	bool firstChildIsTag = false;
	bool lastChildCData = true;
	bool emptyTag = true;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElement &child = *tag.GetElementAt( i );
		
		if( child.CanCastToAttValue() ){
			continue;
		}
		
		if( child.CanCastToElementTag() ){
			if( emptyTag ){
				firstChildIsTag = true;
			}
			lastChildCData = false;
			
		}else if( child.CanCastToCharacterData() || child.CanCastToCDSect() ){
			lastChildCData = true;
		}
		
		emptyTag = false;
	}
	
	// write opening tag
	pWriter->WriteOpeningTagStart( tag.GetName(), ! pCompact );
	
	for( i=0; i<elementCount; i++ ){
		decXmlElement &child = *tag.GetElementAt( i );
		
		if( child.CanCastToAttValue() ){
			VisitAttValue( *child.CastToAttValue() );
		}
	}
	
	pWriter->WriteOpeningTagEnd( emptyTag, ( emptyTag || firstChildIsTag ) && ! pCompact );
	
	// write tag content if present
	if( ! emptyTag ){
		for( i=0; i<elementCount; i++ ){
			decXmlElement &child = *tag.GetElementAt( i );
			if( ! child.CanCastToAttValue() ){
				child.Visit( *this );
			}
		}
		
		// write end tag
		pWriter->WriteClosingTag( tag.GetName(), ! lastChildCData && ! pCompact, ! pCompact );
	}
}

void decXmlVisitorWriter::VisitCharacterData( decXmlCharacterData &data ){
	pWriter->WriteTextString( data.GetData() );
}

void decXmlVisitorWriter::VisitEntityReference( decXmlEntityReference &ref ){
	decString text;
	text.Format( "&%s;", ref.GetName().GetString() );
	pWriter->WriteTextString( text );
}

void decXmlVisitorWriter::VisitCharReference( decXmlCharReference &ref ){
	// not created by parser
}

void decXmlVisitorWriter::VisitCDSect( decXmlCDSect &cdsect ){
	pWriter->WriteTextString( cdsect.GetData() );
}

void decXmlVisitorWriter::VisitAttValue( decXmlAttValue &attribute ){
	pWriter->WriteAttributeString( attribute.GetName(), attribute.GetValue() );
}

void decXmlVisitorWriter::VisitNamespace( decXmlNamespace &ns ){
	// not used
}
