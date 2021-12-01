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

#include "dealEngineModuleXML.h"
#include "../../logger/deLogger.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/xmlparser/decXmlWriter.h"
#include "../../common/xmlparser/decXmlDocument.h"
#include "../../common/xmlparser/decXmlDocumentReference.h"
#include "../../common/xmlparser/decXmlCharacterData.h"
#include "../../common/xmlparser/decXmlElementTag.h"
#include "../../common/xmlparser/decXmlAttValue.h"
#include "../../common/xmlparser/decXmlVisitor.h"
#include "../../common/xmlparser/decXmlParser.h"
#include "../../common/exceptions.h"



// Class dealEngineModuleXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngineModuleXML::dealEngineModuleXML( deLogger *logger, const char *loggerSource ) : dealBaseXML( logger, loggerSource ){
}

dealEngineModuleXML::~dealEngineModuleXML(){
}



// Management
///////////////

void dealEngineModuleXML::ReadFromFile( decBaseFileReader &reader, dealEngineModule &module ){
	decXmlDocumentReference xmldoc;
	xmldoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	
	parser.ParseXml( &reader, xmldoc );
	
	xmldoc->StripComments();
	xmldoc->CleanCharData();
	
	decXmlElementTag * const root = xmldoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "module" ) != 0 ) DETHROW( deeInvalidParam );
	
	pReadModule( *root, module );
}



// Private Functions
//////////////////////

void dealEngineModuleXML::pReadModule( const decXmlElementTag &root, dealEngineModule &module ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "name" ) == 0 ){
				module.SetName( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "description" ) == 0 ){
				module.SetDescription( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "author" ) == 0 ){
				module.SetAuthor( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "version" ) == 0 ){
				module.SetVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "type" ) == 0 ){
				module.SetType( pModuleTypeFromString( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "pattern" ) == 0 ){
				module.SetPattern( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "homepage" ) == 0 ){
				// no interest in this tag
				
			}else if( strcmp( tag->GetName(), "library" ) == 0 ){
				pReadModuleLibrary( *tag, module );
				
			}else if( strcmp( tag->GetName(), "data" ) == 0 ){
				// deprecated
				
			}else if( strcmp( tag->GetName(), "fallback" ) == 0 ){
				module.SetIsFallback( true );
				
			}else{
// 				pErrorUnknownTag( root, *tag );
			}
		}
	}
}

void dealEngineModuleXML::pReadModuleLibrary( const decXmlElementTag &root, dealEngineModule &module ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "file" ) == 0 ){
				module.SetLibFileName( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "size" ) == 0 ){
				module.SetLibFileSizeShould( pGetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "sha1" ) == 0 ){
				module.SetLibFileHashShould( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "entrypoint" ) == 0 ){
				module.SetLibFileEntryPoint( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "preloadLibrary" ) == 0 ){
				// no need to store this information. list of libraries the module needs to be preloaded.
				// this is very much internal information.
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}

dealEngineModule::eModuleTypes dealEngineModuleXML::pModuleTypeFromString( const char *typeString ) const{
	if( strcmp( typeString, "Graphic" ) == 0 ){
		return dealEngineModule::emtGraphic;
		
	}else if( strcmp( typeString, "Audio" ) == 0 ){
		return dealEngineModule::emtAudio;
		
	}else if( strcmp( typeString, "Input" ) == 0 ){
		return dealEngineModule::emtInput;
		
	}else if( strcmp( typeString, "Network" ) == 0 ){
		return dealEngineModule::emtNetwork;
		
	}else if( strcmp( typeString, "Physics" ) == 0 ){
		return dealEngineModule::emtPhysics;
		
	}else if( strcmp( typeString, "Image" ) == 0 ){
		return dealEngineModule::emtImage;
		
	}else if( strcmp( typeString, "Video" ) == 0 ){
		return dealEngineModule::emtVideo;
		
	}else if( strcmp( typeString, "Script" ) == 0 ){
		return dealEngineModule::emtScript;
		
	}else if( strcmp( typeString, "Model" ) == 0 ){
		return dealEngineModule::emtModel;
		
	}else if( strcmp( typeString, "Rig" ) == 0 ){
		return dealEngineModule::emtRig;
		
	}else if( strcmp( typeString, "Skin" ) == 0 ){
		return dealEngineModule::emtSkin;
		
	}else if( strcmp( typeString, "Animation" ) == 0 ){
		return dealEngineModule::emtAnimation;
		
	}else if( strcmp( typeString, "Font" ) == 0 ){
		return dealEngineModule::emtFont;
		
	}else if( strcmp( typeString, "CrashRecovery" ) == 0 ){
		return dealEngineModule::emtCrashRecovery;
		
	}else if( strcmp( typeString, "LanguagePack" ) == 0 ){
		return dealEngineModule::emtLanguagePack;
		
	}else if( strcmp( typeString, "Animator" ) == 0 ){
		return dealEngineModule::emtAnimator;
		
	}else if( strcmp( typeString, "Sound" ) == 0 ){
		return dealEngineModule::emtSound;
		
	}else if( strcmp( typeString, "Video" ) == 0 ){
		return dealEngineModule::emtVideo;
		
	}else if( strcmp( typeString, "AI" ) == 0 ){
		return dealEngineModule::emtAI;
		
	}else if( strcmp( typeString, "OcclusionMesh" ) == 0 ){
		return dealEngineModule::emtOcclusionMesh;
		
	}else if( strcmp( typeString, "Synthesizer" ) == 0 ){
		return dealEngineModule::emtSynthesizer;
		
	}else{
		return dealEngineModule::emtUnknown;
	}
}
