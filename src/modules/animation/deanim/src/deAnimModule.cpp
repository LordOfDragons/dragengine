/* 
 * Drag[en]gine Animation Module
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
#include "deAnimModule.h"
#include "dragengine/resources/animation/deAnimation.h"
#include "dragengine/resources/animation/deAnimationBone.h"
#include "dragengine/resources/animation/deAnimationKeyframe.h"
#include "dragengine/resources/animation/deAnimationKeyframeList.h"
#include "dragengine/resources/animation/deAnimationMove.h"
#include "dragengine/common/math/decMath.h"
#include "dragengine/common/file/decBaseFileReader.h"
#include "dragengine/common/file/decBaseFileWriter.h"
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define FLAG_HAS_VAR_POS		0x1
#define FLAG_HAS_VAR_ROT		0x2
#define FLAG_HAS_VAR_SCALE		0x4
#define FLAG_HAS_FEW_KEYFRAMES	0x8
#define FLAG_IGNORE_BONE		0x10
#define FLAG_FORMAT_FLOAT		0x20



// Export definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEAnimCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry function
///////////////////

deBaseModule *DEAnimCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deAnimModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Structures
///////////////

struct sWeight{
	int bone;
	float weight;
};



// Class deAnimModule
///////////////////////

// Constructor, destructor
////////////////////////////

deAnimModule::deAnimModule( deLoadableModule &loadableModule ) :
deBaseAnimationModule( loadableModule ){
}

deAnimModule::~deAnimModule(){
}



// Management
///////////////

void deAnimModule::LoadAnimation( decBaseFileReader &file, deAnimation &anim ){
	const char * const headerSig = "Drag[en]gine Animation  ";
	deAnimationKeyframeList *newList = NULL;
	deAnimationKeyframe *newKeyframe = NULL;
	deAnimationBone *newBone = NULL;
	deAnimationMove *newMove = NULL;
	float playtimeSeconds;
	decString readStr;
	int i, j, k, version;
	char checkSig[ 24 ];
	int playtimeFrames;
	int keyframeCount;
	float timeFactor;
	int boneCount;
	int moveCount;
	int flagsFile;
	int flagsBone;
	bool hasVarPos;
	bool hasVarRot;
	bool hasVarScale;
	bool formatFloat;
	bool fewKeyframes;
	bool ignoreBone;
	decVector vector;
	int fps;
	
	// check header
	file.Read( &checkSig, 24 );
	if( strncmp( checkSig, headerSig, 24 ) != 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	version = file.ReadByte();
	if( version != 1 ){
		DETHROW( deeInvalidFormat );
	}
	
	flagsFile = file.ReadByte();
	
	const bool deprOldFormat = ( flagsFile & 0xf0 ) == 0;
	
	// read the rest
	try{
		// bones
		boneCount = file.ReadByte();
		if( boneCount < 1 ){
			DETHROW( deeInvalidFormat );
		}
		
		for( i=0; i<boneCount; i++ ){
			newBone = new deAnimationBone;
			if( ! newBone ){
				DETHROW( deeOutOfMemory );
			}
			
			readStr = file.ReadString8();
			newBone->SetName( readStr );
			
			anim.AddBone( newBone );
			newBone = NULL;
		}
		
		// moves
		moveCount = file.ReadUShort();
		if( moveCount < 1 ){
			DETHROW( deeInvalidFormat );
		}
		
		for( i=0; i<moveCount; i++ ){
			newMove = new deAnimationMove;
			if( ! newMove ){
				DETHROW( deeOutOfMemory );
			}
			
			readStr = file.ReadString8();
			newMove->SetName( readStr );
			
			if( deprOldFormat ){
				fps = file.ReadByte();
				timeFactor = 1.0f / ( float )fps;
				
				playtimeFrames = file.ReadUShort();
				
				newMove->SetPlaytime( timeFactor * playtimeFrames );
				
			}else{
				playtimeSeconds = file.ReadFloat();
				playtimeFrames = file.ReadUShort();
				
				if( playtimeFrames > 0 ){
					timeFactor = playtimeSeconds / ( float )playtimeFrames;
					fps = ( int )( 1.0f / timeFactor );
					
				}else{
					timeFactor = 0.0f;
					fps = 50;
				}
				
				newMove->SetPlaytime( playtimeSeconds );
			}
			
			// bones
			for( j=0; j<boneCount; j++ ){
				newList = new deAnimationKeyframeList;
				if( ! newList ){
					DETHROW( deeOutOfMemory );
				}
				
				// flags
				flagsBone = file.ReadByte();
				hasVarPos = ( flagsBone & FLAG_HAS_VAR_POS ) == FLAG_HAS_VAR_POS;
				hasVarRot = ( flagsBone & FLAG_HAS_VAR_ROT ) == FLAG_HAS_VAR_ROT;
				hasVarScale = ( flagsBone & FLAG_HAS_VAR_SCALE ) == FLAG_HAS_VAR_SCALE;
				fewKeyframes = ( flagsBone & FLAG_HAS_FEW_KEYFRAMES ) == FLAG_HAS_FEW_KEYFRAMES;
				ignoreBone = ( flagsBone & FLAG_IGNORE_BONE ) == FLAG_IGNORE_BONE;
				formatFloat = ( flagsBone & FLAG_FORMAT_FLOAT ) == FLAG_FORMAT_FLOAT;
				
				// read keyframes only if required
				if( ! ignoreBone && ( hasVarPos || hasVarRot || hasVarScale ) ){
					// read keyframe count if there are few keyframes
					if( fewKeyframes ){
						keyframeCount = file.ReadUShort();
						if( keyframeCount < 0 ){
							DETHROW( deeInvalidFormat );
						}
						
					}else{
						if( deprOldFormat ){
							keyframeCount = playtimeFrames;
							
						}else{
							keyframeCount = playtimeFrames + 1;
						}
					}
					
					// load keyframes
					for( k=0; k<keyframeCount; k++ ){
						// create keyframe
						newKeyframe = new deAnimationKeyframe;
						if( ! newKeyframe ){
							DETHROW( deeOutOfMemory );
						}
						
						// read time if we have few keyframes
						if( fewKeyframes ){
							newKeyframe->SetTime( timeFactor * ( float )file.ReadUShort() );
							
							if( newKeyframe->GetTime() < 0 ){
								DETHROW( deeInvalidFormat );
							}
							
						}else{
							newKeyframe->SetTime( timeFactor * ( float )k );
						}
						
						// read position if variable
						if( hasVarPos ){
							if( formatFloat ){
								vector.x = file.ReadFloat();
								vector.y = file.ReadFloat();
								vector.z = file.ReadFloat();
								
							}else{
								vector.x = 0.001f * file.ReadShort();
								vector.y = 0.001f * file.ReadShort();
								vector.z = 0.001f * file.ReadShort();
							}
							
							newKeyframe->SetPosition( vector );
						}
						
						// read rotation if variable
						if( hasVarRot ){
							if( formatFloat ){
								vector.x = file.ReadFloat();
								vector.y = file.ReadFloat();
								vector.z = file.ReadFloat();
								
							}else{
								vector.x = ( 0.01f * file.ReadShort() ) * DEG2RAD;
								vector.y = ( 0.01f * file.ReadShort() ) * DEG2RAD;
								vector.z = ( 0.01f * file.ReadShort() ) * DEG2RAD;
							}
							
							newKeyframe->SetRotation( vector );
						}
						
						// read scaleing if variable
						if( hasVarScale ){
							if( formatFloat ){
								vector.x = file.ReadFloat();
								vector.y = file.ReadFloat();
								vector.z = file.ReadFloat();
								
							}else{
								vector.x = 0.01f * file.ReadShort();
								vector.y = 0.01f * file.ReadShort();
								vector.z = 0.01f * file.ReadShort();
							}
							
							newKeyframe->SetScale( vector );
						}
						
						// add keyframe
						newList->AddKeyframe( newKeyframe );
						newKeyframe = NULL;
					}
				}
				
				// if there is no keyframe add a dummy one
				if( newList->GetKeyframeCount() == 0 ){
					newKeyframe = new deAnimationKeyframe;
					if( ! newKeyframe ){
						DETHROW( deeOutOfMemory );
					}
					
					newList->AddKeyframe( newKeyframe );
					newKeyframe = NULL;
				}
				
				// add bone
				newMove->AddKeyframeList( newList );
				newList = NULL;
			}
			
			// add move
			anim.AddMove( newMove );
			newMove = NULL;
		}
		
	}catch( const deException & ){
		if( newKeyframe ){
			delete newKeyframe;
		}
		if( newList ){
			delete newList;
		}
		if( newMove ){
			delete newMove;
		}
		if( newBone ){
			delete newBone;
		}
		throw;
	}
}

void deAnimModule::SaveAnimation( decBaseFileWriter &file, const deAnimation &anim ){
	// nothing yet
}
