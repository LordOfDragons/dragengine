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
// 		if( boneCount < 1 ){
// 			DETHROW( deeInvalidFormat );
// 		}
		
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
// 		if( moveCount < 1 ){
// 			DETHROW( deeInvalidFormat );
// 		}
		
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
				
				newMove->SetFPS( ( float )fps );
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
				
				newMove->SetFPS( ( float )fps );
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

void deAnimModule::SaveAnimation( decBaseFileWriter &writer, const deAnimation &animation ){
	const char * const headerSig = "Drag[en]gine Animation  ";
	int i, j, k;
	
	// write header
	writer.Write( headerSig, 24 );
	writer.WriteByte( 1 ); // version
	writer.WriteByte( 0xf0 ); // flags (0xf0 = hack flag)
	
	// write bones
	const int boneCount = animation.GetBoneCount();
	writer.WriteByte( boneCount );
	
	for( i=0; i<boneCount; i++ ){
		writer.WriteString8( animation.GetBone( i )->GetName() );
	}
	
	// write moves
	const int moveCount = animation.GetMoveCount();
	writer.WriteUShort( moveCount );
	
	for( i=0; i<moveCount; i++ ){
		const deAnimationMove &move = *animation.GetMove( i );
		const int kflCount = move.GetKeyframeListCount();
		
		writer.WriteString8( move.GetName() );
		
		writer.WriteFloat( move.GetPlaytime() );
		
		int playtimeFrames = 0;
		for( j=0; j<kflCount; j++ ){
			playtimeFrames = decMath::max( playtimeFrames, move.GetKeyframeList( j )->GetKeyframeCount() + 1 );
		}
		writer.WriteUShort( playtimeFrames );
		
		// write bones
		for( j=0; j<boneCount; j++ ){
			const deAnimationKeyframeList &kfl = *move.GetKeyframeList( j );
			const int keyframeCount = kfl.GetKeyframeCount();
			
			// TODO optimize writing using flags
			bool hasVarPos = true;
			bool hasVarRot = true;
			bool hasVarScale = true;
			bool fewKeyframes = true;
			bool ignoreBone = keyframeCount == 0;
			bool formatFloat = true;
			
			int flags = 0;
			if( hasVarPos ){
				flags |= FLAG_HAS_VAR_POS;
			}
			if( hasVarRot ){
				flags |= FLAG_HAS_VAR_ROT;
			}
			if( hasVarScale ){
				flags |= FLAG_HAS_VAR_SCALE;
			}
			if( fewKeyframes ){
				flags |= FLAG_HAS_FEW_KEYFRAMES;
			}
			if( ignoreBone ){
				flags |= FLAG_IGNORE_BONE;
			}
			if( formatFloat ){
				flags |= FLAG_FORMAT_FLOAT;
			}
			writer.WriteByte( flags );
			
			// write keyframes if required
			if( ! ignoreBone && ( hasVarPos || hasVarRot || hasVarScale ) ){
				// we have to caluclate first the real keyframe count. this is required since
				// due to sampling two keyframes can fall on the same frame number. in this
				// case the first one is used and the second one skipped which drops the count
				const float fps = move.GetFPS();
				
				if( fewKeyframes ){
					int realKeyframeCount = 0;
					int lastFrame = -1;
					
					for( k=0; k<keyframeCount; k++ ){
						const int frame = ( int )( fps * kfl.GetKeyframe( k )->GetTime() + 0.5f );
						if( frame != lastFrame ){
							lastFrame = frame;
							realKeyframeCount++;
						}
					}
					
					writer.WriteUShort( realKeyframeCount );
				}
				
				int lastFrame = -1;
				
				for( k=0; k<keyframeCount; k++ ){
					const deAnimationKeyframe &keyframe = *kfl.GetKeyframe( k );
					
					if( fewKeyframes ){
						const int frame = ( int )( fps * kfl.GetKeyframe( k )->GetTime() + 0.5f );
						if( frame == lastFrame ){
							continue;
						}
						
						lastFrame = frame;
						writer.WriteUShort( ( unsigned short )frame );
					}
					
					if( hasVarPos ){
						const decVector &position = keyframe.GetPosition();
						
						if( formatFloat ){
							writer.WriteFloat( position.x );
							writer.WriteFloat( position.y );
							writer.WriteFloat( position.z );
							
						}else{
							writer.WriteShort( ( short )( position.x * 1000.0f + 0.5f ) );
							writer.WriteShort( ( short )( position.y * 1000.0f + 0.5f ) );
							writer.WriteShort( ( short )( position.z * 1000.0f + 0.5f ) );
						}
					}
					
					// read rotation if variable
					if( hasVarRot ){
						const decVector &rotation = keyframe.GetRotation();
						
						if( formatFloat ){
							writer.WriteFloat( rotation.x );
							writer.WriteFloat( rotation.y );
							writer.WriteFloat( rotation.z );
							
						}else{
							writer.WriteShort( ( short )( rotation.x * RAD2DEG * 100.0f + 0.5f ) );
							writer.WriteShort( ( short )( rotation.y * RAD2DEG * 100.0f + 0.5f ) );
							writer.WriteShort( ( short )( rotation.z * RAD2DEG * 100.0f + 0.5f ) );
						}
					}
					
					// read scaleing if variable
					if( hasVarScale ){
						const decVector &scale = keyframe.GetScale();
						
						if( formatFloat ){
							writer.WriteFloat( scale.x );
							writer.WriteFloat( scale.y );
							writer.WriteFloat( scale.z );
							
						}else{
							writer.WriteShort( ( short )( scale.x * 100.0f + 0.5f ) );
							writer.WriteShort( ( short )( scale.y * 100.0f + 0.5f ) );
							writer.WriteShort( ( short )( scale.z * 100.0f + 0.5f ) );
						}
					}
				}
			}
		}
	}
}
