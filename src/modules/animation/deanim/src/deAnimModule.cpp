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
		
		const float playtime = move.GetPlaytime();
		writer.WriteFloat( playtime );
		
		int playtimeFrames = 0;
		for( j=0; j<kflCount; j++ ){
			const deAnimationKeyframeList &kfl = *move.GetKeyframeList( j );
			const int keyframeCount = kfl.GetKeyframeCount();
			const float fps = move.GetFPS();
			int realKeyframeCount = 0;
			int lastFrame = -1;
			
			for( k=0; k<keyframeCount; k++ ){
				const int frame = ( int )( fps * kfl.GetKeyframe( k )->GetTime() + 0.5f );
				if( frame != lastFrame ){
					lastFrame = frame;
					realKeyframeCount++;
				}
			}
			
			playtimeFrames = decMath::max( playtimeFrames, realKeyframeCount );
		}
		
		const int realPlaytimeFrames = decMath::max( playtimeFrames - 1, 0 );
		writer.WriteUShort( realPlaytimeFrames );
		
		// the time factor is calculated by the loader from playtime and real keyframe count.
		// we have to do the same calculation here (just reversed) to get the correct result.
		// this framerate here is not the same as the one stored in the move on purpose.
		// the one stored in the move is for use by the application. this framerate calculated
		// here is used to store times as frames. the application never gets to see those numbers
		float timeFactor = move.GetFPS();
		if( playtime > 0.001f ){
			timeFactor = ( float )realPlaytimeFrames / playtime;
		}
		
		// write bones
		for( j=0; j<boneCount; j++ ){
			const deAnimationKeyframeList &kfl = *move.GetKeyframeList( j );
			const int keyframeCount = kfl.GetKeyframeCount();
			
			// TODO optimize writing using flags
			sConfig config;
			config.hasVarPos = true;
			config.hasVarRot = true;
			config.hasVarScale = true;
			config.fewKeyframes = true;
			config.ignoreBone = keyframeCount == 0;
			config.formatFloat = true;
			
			int flags = 0;
			if( config.hasVarPos ){
				flags |= FLAG_HAS_VAR_POS;
			}
			if( config.hasVarRot ){
				flags |= FLAG_HAS_VAR_ROT;
			}
			if( config.hasVarScale ){
				flags |= FLAG_HAS_VAR_SCALE;
			}
			if( config.fewKeyframes ){
				flags |= FLAG_HAS_FEW_KEYFRAMES;
			}
			if( config.ignoreBone ){
				flags |= FLAG_IGNORE_BONE;
			}
			if( config.formatFloat ){
				flags |= FLAG_FORMAT_FLOAT;
			}
			writer.WriteByte( flags );
			
			// write keyframes if required
			if( ! config.ignoreBone && ( config.hasVarPos || config.hasVarRot || config.hasVarScale ) ){
				if( config.fewKeyframes ){
					// we have to calculate first the real keyframe count. this is required since
					// due to sampling two keyframes can fall on the same frame number. in this
					// case the first one is used and the second one skipped which drops the count
					int realKeyframeCount = 0;
					int lastFrame = -1;
					
					for( k=0; k<keyframeCount; k++ ){
						const int frame = ( int )( timeFactor * kfl.GetKeyframe( k )->GetTime() + 0.5f );
						if( frame != lastFrame ){
							lastFrame = frame;
							realKeyframeCount++;
						}
					}
					
					writer.WriteUShort( realKeyframeCount );
				}
				
				if( config.fewKeyframes ){
					int lastFrame = -1;
					
					for( k=0; k<keyframeCount; k++ ){
						const deAnimationKeyframe &keyframe = *kfl.GetKeyframe( k );
						
						const int frame = ( int )( timeFactor * kfl.GetKeyframe( k )->GetTime() + 0.5f );
						if( frame == lastFrame ){
							continue;
						}
						
						lastFrame = frame;
						writer.WriteUShort( ( unsigned short )frame );
						
						pWriteKeyframeData( writer, config, keyframe );
					}
					
				}else if( keyframeCount > 1 ){
					const int endFrameIndex = playtimeFrames - 1;
					
					int lastFrame = 0;
					pWriteKeyframeData( writer, config, *kfl.GetKeyframe( 0 ) );
					
					for( k=1; k<keyframeCount-1; k++ ){
						const deAnimationKeyframe &keyframe = *kfl.GetKeyframe( k );
						
						const int frame = decMath::clamp( ( int )( timeFactor
							* kfl.GetKeyframe( k )->GetTime() + 0.5f ), 0, endFrameIndex );
						if( frame == lastFrame ){
							continue;
						}
						
						pWriteKeyframeDataInterpolate( writer, config,
							*kfl.GetKeyframe( k - 1 ), keyframe, frame - lastFrame );
						lastFrame = frame;
					}
					
					if( lastFrame < endFrameIndex ){
						pWriteKeyframeDataInterpolate( writer, config,
							*kfl.GetKeyframe( keyframeCount - 2 ),
							*kfl.GetKeyframe( keyframeCount - 1 ), endFrameIndex - lastFrame );
					}
					
				}else if( keyframeCount == 1 ){
					for( k=0; k<playtimeFrames; k++ ){
						pWriteKeyframeData( writer, config, *kfl.GetKeyframe( 0 ) );
					}
				}
			}
		}
	}
}



// Private Functions
//////////////////////

void deAnimModule::pWriteKeyframeData( decBaseFileWriter &writer, const sConfig &config,
const deAnimationKeyframe &keyframe ){
	if( config.hasVarPos ){
		pWriteKeyframePosition( writer, config, keyframe.GetPosition() );
	}
	if( config.hasVarRot ){
		pWriteKeyframeRotation( writer, config, keyframe.GetRotation() );
	}
	if( config.hasVarScale ){
		pWriteKeyframeScale( writer, config, keyframe.GetScale() );
	}
}

void deAnimModule::pWriteKeyframeDataInterpolate( decBaseFileWriter &writer,
const sConfig &config, const deAnimationKeyframe &keyframePrev,
const deAnimationKeyframe &keyframeNext, int frameSteps ){
	int i;
	for( i=1; i<frameSteps; i++ ){
		const float blend = ( float )i / ( float )frameSteps;
		
		if( config.hasVarPos ){
			pWriteKeyframePosition( writer, config,
				keyframePrev.GetPosition().Mix( keyframeNext.GetPosition(), blend ) );
		}
		if( config.hasVarRot ){
			pWriteKeyframeRotation( writer, config,
				keyframePrev.GetRotation().Mix( keyframeNext.GetRotation(), blend ) );
		}
		if( config.hasVarScale ){
			pWriteKeyframeScale( writer, config,
				keyframePrev.GetScale().Mix( keyframeNext.GetScale(), blend ) );
		}
	}
	
	pWriteKeyframeData( writer, config, keyframeNext );
}

void deAnimModule::pWriteKeyframePosition( decBaseFileWriter &writer,
const sConfig &config, const decVector &position ){
	if( config.formatFloat ){
		writer.WriteFloat( position.x );
		writer.WriteFloat( position.y );
		writer.WriteFloat( position.z );
		
	}else{
		writer.WriteShort( ( short )( position.x * 1000.0f + 0.5f ) );
		writer.WriteShort( ( short )( position.y * 1000.0f + 0.5f ) );
		writer.WriteShort( ( short )( position.z * 1000.0f + 0.5f ) );
	}
}

void deAnimModule::pWriteKeyframeRotation( decBaseFileWriter &writer,
const sConfig &config, const decVector &rotation ){
	if( config.formatFloat ){
		writer.WriteFloat( rotation.x );
		writer.WriteFloat( rotation.y );
		writer.WriteFloat( rotation.z );
		
	}else{
		writer.WriteShort( ( short )( rotation.x * RAD2DEG * 100.0f + 0.5f ) );
		writer.WriteShort( ( short )( rotation.y * RAD2DEG * 100.0f + 0.5f ) );
		writer.WriteShort( ( short )( rotation.z * RAD2DEG * 100.0f + 0.5f ) );
	}
}

void deAnimModule::pWriteKeyframeScale( decBaseFileWriter &writer,
const sConfig &config, const decVector &scale ){
	if( config.formatFloat ){
		writer.WriteFloat( scale.x );
		writer.WriteFloat( scale.y );
		writer.WriteFloat( scale.z );
		
	}else{
		writer.WriteShort( ( short )( scale.x * 100.0f + 0.5f ) );
		writer.WriteShort( ( short )( scale.y * 100.0f + 0.5f ) );
		writer.WriteShort( ( short )( scale.z * 100.0f + 0.5f ) );
	}
}
