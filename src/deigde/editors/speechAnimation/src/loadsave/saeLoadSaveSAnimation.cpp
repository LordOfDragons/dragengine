/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeLoadSaveSAnimation.h"
#include "../sanimation/saeSAnimation.h"
#include "../sanimation/phoneme/saePhoneme.h"
#include "../sanimation/dictionary/saeWord.h"
#include "../gui/saeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class saeLoadSaveSAnimation
////////////////////////////////

// Constructor, destructor
////////////////////////////

saeLoadSaveSAnimation::saeLoadSaveSAnimation( saeLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
	pLSSys = lssys;
	
	pName = "Speech Animation";
	pPattern = ".desanim";
}



// Loading and saving
///////////////////////

void saeLoadSaveSAnimation::LoadSAnimation( saeSAnimation &sanimation, decBaseFileReader &reader ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "speechAnimation" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadSAnimation( *root, sanimation );
}

void saeLoadSaveSAnimation::SaveSAnimation( const saeSAnimation &sanimation, decBaseFileWriter &writer ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteSAnimation( xmlWriter, sanimation );
}



// Private Functions
//////////////////////

void saeLoadSaveSAnimation::pWriteSAnimation( decXmlWriter &writer, const saeSAnimation &sanimation ){
	const saePhonemeList &phonemeList = sanimation.GetPhonemeList();
	const saeWordList &wordList = sanimation.GetWordList();
	int count, i;
	
	writer.WriteOpeningTag( "speechAnimation", false, true );
	
	pWriteDisplay( writer, sanimation );
	
	writer.WriteDataTagString( "rig", sanimation.GetRigPath().GetString() );
	writer.WriteDataTagString( "animation", sanimation.GetAnimationPath().GetString() );
	writer.WriteDataTagString( "neutralMoveName", sanimation.GetNeutralMoveName().GetString() );
	
	count = phonemeList.GetCount();
	for( i=0; i<count; i++ ){
		pWritePhoneme( writer, *phonemeList.GetAt( i ) );
	}
	
	count = wordList.GetCount();
	for( i=0; i<count; i++ ){
		pWriteWord( writer, *wordList.GetAt( i ) );
	}
	
	writer.WriteClosingTag( "speechAnimation", true );
}

void saeLoadSaveSAnimation::pWriteDisplay( decXmlWriter &writer, const saeSAnimation &sanimation ){
	writer.WriteOpeningTag( "display" );
	
	writer.WriteDataTagString( "model", sanimation.GetDisplayModelPath().GetString() );
	writer.WriteDataTagString( "skin", sanimation.GetDisplaySkinPath().GetString() );
	writer.WriteDataTagString( "rig", sanimation.GetDisplayRigPath().GetString() );
	
	writer.WriteClosingTag( "display" );
}

void saeLoadSaveSAnimation::pWritePhoneme( decXmlWriter &writer, const saePhoneme &phoneme ){
	writer.WriteOpeningTagStart( "phoneme" );
	writer.WriteAttributeInt( "ipa", phoneme.GetIPA() );
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString( "sampleText", phoneme.GetSampleText().GetString() );
	writer.WriteDataTagString( "moveName", phoneme.GetMoveName().GetString() );
	writer.WriteDataTagFloat( "length", phoneme.GetLength() );
	
	writer.WriteClosingTag( "phoneme" );
}

void saeLoadSaveSAnimation::pWriteWord( decXmlWriter &writer, const saeWord &word ){
	writer.WriteOpeningTagStart( "word" );
	writer.WriteAttributeString( "name", word.GetName().GetString() );
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString( decUnicodeString::NewFromUTF8( "phonetics" ), word.GetPhonetics() );
	
	writer.WriteClosingTag( "word" );
}



void saeLoadSaveSAnimation::pReadSAnimation( const decXmlElementTag &root, saeSAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "display" ) == 0 ){
				pReadDisplay( *tag, sanimation );
				
			}else if( strcmp( tag->GetName(), "rig" ) == 0 ){
				sanimation.SetRigPath( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "animation" ) == 0 ){
				sanimation.SetAnimationPath( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "neutralMoveName" ) == 0 ){
				sanimation.SetNeutralMoveName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "phoneme" ) == 0 ){
				pReadPhoneme( *tag, sanimation );
				
			}else if( strcmp( tag->GetName(), "word" ) == 0 ){
				pReadWord( *tag, sanimation );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
	}
}

void saeLoadSaveSAnimation::pReadDisplay( const decXmlElementTag &root, saeSAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "model" ) == 0 ){
				sanimation.SetDisplayModelPath( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "skin" ) == 0 ){
				sanimation.SetDisplaySkinPath( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "rig" ) == 0 ){
				sanimation.SetDisplayRigPath( GetCDataString( *tag ) );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
	}
}

void saeLoadSaveSAnimation::pReadPhoneme( const decXmlElementTag &root, saeSAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	saePhoneme *phoneme = NULL;
	decString text;
	int e;
	
	try{
		phoneme = new saePhoneme;
		if( ! phoneme ) DETHROW( deeOutOfMemory );
		
		/*if( ! HasAttribute( root, "name" ) ){
			LogErrorGenericProblemValue( root, "name", "Attribute missing" );
		}*/
		phoneme->SetIPA( GetAttributeInt( root, "ipa" ) );
		
		if( sanimation.GetPhonemeList().HasIPA( phoneme->GetIPA() ) ){
			text.Format( "%i", phoneme->GetIPA() );
			LogErrorGenericProblemValue( root, text.GetString(), "Duplicate Phoneme" );
		}
		
		for( e=0; e<elementCount; e++ ){
			tag = root.GetElementIfTag( e );
			
			if( tag ){
				if( strcmp( tag->GetName(), "sampleText" ) == 0 ){
					phoneme->SetSampleText( GetCDataString( *tag ) );
					
				}else if( strcmp( tag->GetName(), "moveName" ) == 0 ){
					phoneme->SetMoveName( GetCDataString( *tag ) );
					
				}else if( strcmp( tag->GetName(), "length" ) == 0 ){
					phoneme->SetLength( GetCDataFloat( *tag ) );
					
				}else{
					LogWarnUnknownTag( root, *tag );
				}
			}
		}
		
		sanimation.AddPhoneme( phoneme );
		
	}catch( const deException & ){
		if( phoneme ){
			phoneme->FreeReference();
		}
		
		throw;
	}
}

void saeLoadSaveSAnimation::pReadWord( const decXmlElementTag &root, saeSAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	saeWord *word = NULL;
	int e;
	
	try{
		word = new saeWord;
		if( ! word ) DETHROW( deeOutOfMemory );
		
		/*if( ! HasAttribute( root, "name" ) ){
			LogErrorGenericProblemValue( root, "name", "Attribute missing" );
		}*/
		word->SetName( GetAttributeString( root, "name" ) );
		
		if( sanimation.GetWordList().HasNamed( word->GetName().GetString() ) ){
			LogErrorGenericProblemValue( root, word->GetName().GetString(), "Duplicate Word" );
		}
		
		for( e=0; e<elementCount; e++ ){
			tag = root.GetElementIfTag( e );
			
			if( tag ){
				if( strcmp( tag->GetName(), "phonetics" ) == 0 ){
					word->SetPhonetics( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
					
				}else{
					LogWarnUnknownTag( root, *tag );
				}
			}
		}
		
		sanimation.AddWord( word );
		
	}catch( const deException & ){
		if( word ){
			word->FreeReference();
		}
		
		throw;
	}
}
