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

#include "dealEngineModule.h"

#include "../../common/exceptions.h"



// Class dealEngineModule
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngineModule::dealEngineModule(){
	pType = emtUnknown;
	pIsFallback = false;
	pStatus = emsNotTested;
	pErrorCode = 0;
	
	pLibFileSizeShould = 0;
	pLibFileSizeIs = 0;
}

dealEngineModule::~dealEngineModule(){
}



// Management
///////////////

void dealEngineModule::SetType( eModuleTypes type ){
	pType = type;
}

void dealEngineModule::SetName( const char *name ){
	pName = name;
}

void dealEngineModule::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void dealEngineModule::SetAuthor( const decUnicodeString &author ){
	pAuthor = author;
}

void dealEngineModule::SetVersion( const char *version ){
	pVersion = version;
}

void dealEngineModule::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void dealEngineModule::SetIsFallback( bool isFallback ){
	pIsFallback = isFallback;
}

void dealEngineModule::SetStatus( int status ){
	pStatus = status;
}

void dealEngineModule::SetErrorCode( int errorCode ){
	pErrorCode = errorCode;
}



void dealEngineModule::SetLibFileName( const char *name ){
	pLibFileName = name;
}

void dealEngineModule::SetLibFileSizeShould( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	pLibFileSizeShould = size;
}

void dealEngineModule::SetLibFileSizeIs( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	pLibFileSizeIs = size;
}

void dealEngineModule::SetLibFileHashShould( const char *hash ){
	pLibFileHashShould = hash;
}

void dealEngineModule::SetLibFileHashIs( const char *hash ){
	pLibFileHashIs = hash;
}

void dealEngineModule::SetLibFileEntryPoint( const char *name ){
	pLibFileEntryPoint = name;
}



// Helpers
////////////

int dealEngineModule::CompareVersion( const char *version1, const char *version2 ){
	if( ! version1 || ! version2 ){
		DETHROW( deeInvalidParam );
	}
	
	const int len1 = strlen( version1 );
	const int len2 = strlen( version2 );
	int last1, last2;
	int pos1 = 0;
	int pos2 = 0;
	
	while( pos1 < len1 && pos2 < len2 ){
		// find next version parts
		last1 = pos1;
		while( pos1 < len1 && version1[ pos1 ] != '.' ){
			pos1++;
		}
		
		last2 = pos2;
		while( pos2 < len2 && version2[ pos2 ] != '.' ){
			pos2++;
		}
		
		// check if they differ. a missing version part is assumed to be 0
		const long part1 = strtol( version1 + last1, NULL, 10 );
		const long part2 = strtol( version2 + last2, NULL, 10 );
		
		if( part1 < part2 ){
			return -1;
		}
		if( part1 > part2 ){
			return 1;
		}
		
		// skip the dots
		pos1++;
		pos2++;
	}
	
	return 0;
}

bool dealEngineModule::IsSingleType( eModuleTypes type ){
	switch( type ){
	case emtGraphic:
	case emtAudio:
	case emtInput:
	case emtNetwork:
	case emtPhysics:
	case emtScript:
	case emtAnimator:
	case emtAI:
	case emtCrashRecovery:
	case emtSynthesizer:
		return true;
		
	default:
		return false;
	}
}

dealEngineModule::eModuleTypes dealEngineModule::GetTypeFromString( const char *typeString ){
	if( strcmp( typeString, "Graphic" ) == 0 ){
		return emtGraphic;
		
	}else if( strcmp( typeString, "Audio" ) == 0 ){
		return emtAudio;
		
	}else if( strcmp( typeString, "Input" ) == 0 ){
		return emtInput;
		
	}else if( strcmp( typeString, "Network" ) == 0 ){
		return emtNetwork;
		
	}else if( strcmp( typeString, "Physics" ) == 0 ){
		return emtPhysics;
		
	}else if( strcmp( typeString, "Image" ) == 0 ){
		return emtImage;
		
	}else if( strcmp( typeString, "Video" ) == 0 ){
		return emtVideo;
		
	}else if( strcmp( typeString, "Script" ) == 0 ){
		return emtScript;
		
	}else if( strcmp( typeString, "Model" ) == 0 ){
		return emtModel;
		
	}else if( strcmp( typeString, "Rig" ) == 0 ){
		return emtRig;
		
	}else if( strcmp( typeString, "Skin" ) == 0 ){
		return emtSkin;
		
	}else if( strcmp( typeString, "Animation" ) == 0 ){
		return emtAnimation;
		
	}else if( strcmp( typeString, "Font" ) == 0 ){
		return emtFont;
		
	}else if( strcmp( typeString, "CrashRecovery" ) == 0 ){
		return emtCrashRecovery;
		
	}else if( strcmp( typeString, "LanguagePack" ) == 0 ){
		return emtLanguagePack;
		
	}else if( strcmp( typeString, "Animator" ) == 0 ){
		return emtAnimator;
		
	}else if( strcmp( typeString, "Sound" ) == 0 ){
		return emtSound;
		
	}else if( strcmp( typeString, "AI" ) == 0 ){
		return emtAI;
		
	}else if( strcmp( typeString, "OcclusionMesh" ) == 0 ){
		return emtOcclusionMesh;
		
	}else if( strcmp( typeString, "Synthesizer" ) == 0 ){
		return emtSynthesizer;
		
	}else{
		return emtUnknown;
	}
}

const char *dealEngineModule::GetStringForType( eModuleTypes type ){
	switch( type ){
	case emtGraphic:
		return "Graphic";
		
	case emtAudio:
		return "Audio";
		
	case emtInput:
		return "Input";
		
	case emtNetwork:
		return "Network";
		
	case emtPhysics:
		return "Physics";
		
	case emtImage:
		return "Image";
		
	case emtVideo:
		return "Video";
		
	case emtScript:
		return "Script";
		
	case emtModel:
		return "Model";
		
	case emtRig:
		return "Rig";
		
	case emtSkin:
		return "Skin";
		
	case emtAnimation:
		return "Animation";
		
	case emtFont:
		return "Font";
		
	case emtCrashRecovery:
		return "CrashRecovery";
		
	case emtLanguagePack:
		return "LanguagePack";
		
	case emtAnimator:
		return "Animator";
		
	case emtSound:
		return "Sound";
		
	case emtAI:
		return "AI";
		
	case emtOcclusionMesh:
		return "OcclusionMesh";
		
	case emtSynthesizer:
		return "Synthesizer";
		
	default:
		return "?";
	}
}
