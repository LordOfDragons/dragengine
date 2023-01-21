/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceSpeechAnimationXML.h"
#include "ceSpeechAnimation.h"
#include "ceSAPhoneme.h"
#include "ceSAWord.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>



// Class ceSpeechAnimationXML
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceSpeechAnimationXML::ceSpeechAnimationXML( deLogger *logger, const char *loggerSource ) : igdeBaseXML( logger, loggerSource ){
}



// Loading and saving
///////////////////////

void ceSpeechAnimationXML::ReadFromFile( const decString &pathSAnimation, decBaseFileReader &reader, ceSpeechAnimation &sanimation ){
	decPath basePath;
	basePath.SetFromUnix( pathSAnimation.GetString() );
	if( basePath.GetComponentCount() > 1 ){
		basePath.RemoveLastComponent();
		
	}else{
		basePath.SetFromUnix( "/" );
	}
	
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "speechAnimation" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadSAnimation( *root, basePath.GetPathUnix(), sanimation );
}



// Private Functions
//////////////////////

void ceSpeechAnimationXML::pReadSAnimation( const decXmlElementTag &root, const char *basePath, ceSpeechAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	deAnimation *animation = NULL;
	deRig *rig = NULL;
	int i;
	
	for( i=0; i <elementCount; i++ ){
		tag = root.GetElementIfTag( i );
		
		if( tag ){
			if( strcmp( tag->GetName(), "display" ) == 0 ){
				
			}else if( strcmp( tag->GetName(), "rig" ) == 0 ){
				if( strlen( GetCDataString( *tag ) ) > 0 ){
					rig = NULL;
					
					try{
						rig = sanimation.GetEngine()->GetRigManager()->LoadRig( GetCDataString( *tag ), basePath );
						sanimation.GetEngineAnimator()->SetRig( rig );
						rig->FreeReference();
						
					}catch( const deException & ){
						if( rig ){
							rig->FreeReference();
						}
						LogWarnGenericProblemTag( root, tag->GetName(), "Failed loading resource file" );
					}
				}
				
			}else if( strcmp( tag->GetName(), "animation" ) == 0 ){
				if( strlen( GetCDataString( *tag ) ) > 0 ){
					animation = NULL;
					
					try{
						animation = sanimation.GetEngine()->GetAnimationManager()->LoadAnimation( GetCDataString( *tag ), basePath );
						sanimation.GetEngineAnimator()->SetAnimation( animation );
						animation->FreeReference();
						
					}catch( const deException & ){
						if( animation ){
							animation->FreeReference();
						}
						LogWarnGenericProblemTag( root, tag->GetName(), "Failed loading resource file" );
					}
				}
				
			}else if( strcmp( tag->GetName(), "neutralMoveName" ) == 0 ){
				sanimation.SetNeutralMoveName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "phoneme" ) == 0 ){
				pReadPhoneme( *tag, sanimation );
				
			}else if( strcmp( tag->GetName(), "word" ) == 0 ){
				pReadWord( *tag, sanimation );
			}
		}
	}
	
	sanimation.CreateAnimator();
}

void ceSpeechAnimationXML::pReadPhoneme( const decXmlElementTag &root, ceSpeechAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	ceSAPhoneme *phoneme = NULL;
	decString text;
	int e;
	
	try{
		phoneme = new ceSAPhoneme( GetAttributeInt( root, "ipa" ) );
		
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
				}
			}
		}
		
		sanimation.GetPhonemeList().Add( phoneme );
		
	}catch( const deException & ){
		if( phoneme ){
			phoneme->FreeReference();
		}
		
		throw;
	}
}

void ceSpeechAnimationXML::pReadWord( const decXmlElementTag &root, ceSpeechAnimation &sanimation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	ceSAWord *word = NULL;
	int e;
	
	try{
		word = new ceSAWord( GetAttributeString( root, "name" ) );
		
		if( sanimation.GetWordList().HasNamed( word->GetName().GetString() ) ){
			LogErrorGenericProblemValue( root, word->GetName().GetString(), "Duplicate Word" );
		}
		
		for( e=0; e<elementCount; e++ ){
			tag = root.GetElementIfTag( e );
			
			if( tag ){
				if( strcmp( tag->GetName(), "phonetics" ) == 0 ){
					word->SetPhonetics( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
				}
			}
		}
		
		sanimation.GetWordList().Add( word );
		
	}catch( const deException & ){
		if( word ){
			word->FreeReference();
		}
		
		throw;
	}
}
