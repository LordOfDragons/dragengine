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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deAnimModule.h"
#include "dragengine/resources/animation/deAnimation.h"
#include "dragengine/resources/animation/deAnimationBone.h"
#include "dragengine/resources/animation/deAnimationKeyframe.h"
#include "dragengine/resources/animation/deAnimationKeyframeList.h"
#include "dragengine/resources/animation/deAnimationKeyframeVertexPositionSet.h"
#include "dragengine/resources/animation/deAnimationKeyframeVertexPositionSetList.h"
#include "dragengine/resources/animation/deAnimationMove.h"
#include "dragengine/common/math/decMath.h"
#include "dragengine/common/file/decBaseFileReader.h"
#include "dragengine/common/file/decBaseFileWriter.h"
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define FLAG_KF_HAS_VAR_POS			0x1
#define FLAG_KF_HAS_VAR_ROT			0x2
#define FLAG_KF_HAS_VAR_SCALE		0x4
#define FLAG_KF_HAS_FEW_KEYFRAMES	0x8
#define FLAG_KF_IGNORE_BONE			0x10
#define FLAG_KF_FORMAT_FLOAT		0x20

#define FLAG_KFVPS_HAS_VAR_WEIGHT		0x1
#define FLAG_KFVPS_HAS_FEW_KEYFRAMES	0x2
#define FLAG_KFVPS_IGNORE_SET			0x4
#define FLAG_KFVPS_FORMAT_FLOAT			0x8



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
	try{
		return new deAnimModule( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



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

void deAnimModule::LoadAnimation( decBaseFileReader &reader, deAnimation &animation ){
	const char * const headerSig = "Drag[en]gine Animation  ";
	char checkSig[ 24 ];
	
	reader.Read( &checkSig, 24 );
	if( strncmp( checkSig, headerSig, 24 ) != 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	sInfo info;
	memset( &info, 0, sizeof( info ) );
	
	info.version = reader.ReadByte();
	info.flags = reader.ReadByte();
	
	switch( info.version ){
	case 1:
		info.deprOldFormat = ( info.flags & 0xf0 ) == 0;
		pReadBones( reader, animation, info );
		pReadMoves( reader, animation, info );
		break;
		
	case 2:
		pReadBones( reader, animation, info );
		pReadVertexPositionSets( reader, animation, info );
		pReadMoves( reader, animation, info );
		break;
		
	default:
		DETHROW( deeInvalidFormat );
	}
}

void deAnimModule::SaveAnimation( decBaseFileWriter &writer, const deAnimation &animation ){
	const char * const headerSig = "Drag[en]gine Animation  ";
	int i, j, k;
	
	// write header
	writer.Write( headerSig, 24 );
	writer.WriteByte( 2 ); // version
	writer.WriteByte( 0 ); // flags
	
	// write bones
	const int boneCount = animation.GetBoneCount();
	writer.WriteUShort( boneCount );
	
	for( i=0; i<boneCount; i++ ){
		writer.WriteString8( animation.GetBone( i )->GetName() );
	}
	
	// write vertex position sets
	const decStringList &vertexPositionSets = animation.GetVertexPositionSets();
	const int vertexPositionSetCount = vertexPositionSets.GetCount();
	writer.WriteUShort( vertexPositionSetCount );
	
	for( i=0; i< vertexPositionSetCount; i++ ){
		writer.WriteString8( vertexPositionSets.GetAt( i ) );
	}
	
	// write moves
	const int moveCount = animation.GetMoveCount();
	writer.WriteUShort( moveCount );
	
	for( i=0; i<moveCount; i++ ){
		const deAnimationMove &move = *animation.GetMove( i );
		const int kflCount = move.GetKeyframeListCount();
		const int kflvpsCount = move.GetVertexPositionSetKeyframeListCount();
		
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
		
		for( j=0; j<kflvpsCount; j++ ){
			const deAnimationKeyframeVertexPositionSetList &kfl = *move.GetVertexPositionSetKeyframeList( j );
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
				flags |= FLAG_KF_HAS_VAR_POS;
			}
			if( config.hasVarRot ){
				flags |= FLAG_KF_HAS_VAR_ROT;
			}
			if( config.hasVarScale ){
				flags |= FLAG_KF_HAS_VAR_SCALE;
			}
			if( config.fewKeyframes ){
				flags |= FLAG_KF_HAS_FEW_KEYFRAMES;
			}
			if( config.ignoreBone ){
				flags |= FLAG_KF_IGNORE_BONE;
			}
			if( config.formatFloat ){
				flags |= FLAG_KF_FORMAT_FLOAT;
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
		
		// write vertex position sets
		for( j=0; j<vertexPositionSetCount; j++ ){
			const deAnimationKeyframeVertexPositionSetList &kfl = *move.GetVertexPositionSetKeyframeList( j );
			const int keyframeCount = kfl.GetKeyframeCount();
			
			sConfig2 config;
			config.hasVarWeight = true;
			config.fewKeyframes = true;
			config.ignoreSet = keyframeCount == 0;
			config.formatFloat = true;
			
			int flags = 0;
			if( config.hasVarWeight ){
				flags |= FLAG_KFVPS_HAS_VAR_WEIGHT;
			}
			if( config.fewKeyframes ){
				flags |= FLAG_KFVPS_HAS_FEW_KEYFRAMES;
			}
			if( config.ignoreSet ){
				flags |= FLAG_KFVPS_IGNORE_SET;
			}
			if( config.formatFloat ){
				flags |= FLAG_KFVPS_FORMAT_FLOAT;
			}
			writer.WriteByte( flags );
			
			// write keyframes if required
			if( ! config.ignoreSet && config.hasVarWeight ){
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
						const deAnimationKeyframeVertexPositionSet &keyframe = *kfl.GetKeyframe( k );
						
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
						const deAnimationKeyframeVertexPositionSet &keyframe = *kfl.GetKeyframe( k );
						
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

void deAnimModule::pReadBones( decBaseFileReader &reader, deAnimation &animation, sInfo &info ){
	if( info.version == 1 ){
		info.boneCount = reader.ReadByte();
		
	}else{
		info.boneCount = reader.ReadUShort();
	}
	
	int i;
	for( i=0; i<info.boneCount; i++ ){
		deAnimationBone * const bone = new deAnimationBone;
		
		try{
			bone->SetName( reader.ReadString8() );
			animation.AddBone( bone );
			
		}catch( const deException & ){
			delete bone;
			throw;
		}
	}
}

void deAnimModule::pReadVertexPositionSets( decBaseFileReader &reader, deAnimation &animation, sInfo &info ){
	if( info.version == 1 ){
		info.vertexPositionSetCount = reader.ReadByte();
		
	}else{
		info.vertexPositionSetCount = reader.ReadUShort();
	}
	
	decStringList &list = animation.GetVertexPositionSets();
	int i;
	
	for( i=0; i<info.vertexPositionSetCount; i++ ){
		const decString name( reader.ReadString8() );
		if( list.Has( name ) ){
			DETHROW( deeInvalidFormat );
		}
		list.Add( name );
	}
}

void deAnimModule::pReadMoves( decBaseFileReader &reader, deAnimation &animation, sInfo &info ){
	info.moveCount = reader.ReadUShort();
	
	int i;
	for( i=0; i<info.moveCount; i++ ){
		deAnimationMove * const move = new deAnimationMove;
		
		try{
			move->SetName( reader.ReadString8() );
			
			pReadMoveFps( reader, *move, info );
			pReadMoveBones( reader, *move, info );
			
			if( info.version > 1 ){
				pReadMoveVertexPositionSets( reader, *move, info );
			}
			
			animation.AddMove( move );
			
		}catch( const deException & ){
			delete move;
			throw;
		}
	}
}

void deAnimModule::pReadMoveFps( decBaseFileReader &reader, deAnimationMove &move, sInfo &info ){
	if( info.deprOldFormat ){
		const int fps = reader.ReadByte();
		info.timeFactor = 1.0f / ( float )fps;
		
		info.playtimeFrames = reader.ReadUShort();
		
		move.SetFPS( ( float )fps );
		move.SetPlaytime( info.timeFactor * ( float )info.playtimeFrames );
		
	}else{
		const float playtimeSeconds = reader.ReadFloat();
		float fps;
		
		info.playtimeFrames = reader.ReadUShort();
		
		if( info.playtimeFrames > 0 ){
			info.timeFactor = playtimeSeconds / ( float )info.playtimeFrames;
			fps = 1.0f / info.timeFactor;
			
		}else{
			info.timeFactor = 0.0f;
			fps = 50.0f;
		}
		
		move.SetFPS( fps );
		move.SetPlaytime( playtimeSeconds );
	}
}

void deAnimModule::pReadMoveBones( decBaseFileReader &reader, deAnimationMove &move, sInfo &info ){
	int i;
	
	for( i=0; i<info.boneCount; i++ ){
		deAnimationKeyframeList * const list = new deAnimationKeyframeList;
		
		try{
			info.boneFlags = reader.ReadByte();
			info.hasVarPos = ( info.boneFlags & FLAG_KF_HAS_VAR_POS ) == FLAG_KF_HAS_VAR_POS;
			info.hasVarRot = ( info.boneFlags & FLAG_KF_HAS_VAR_ROT ) == FLAG_KF_HAS_VAR_ROT;
			info.hasVarScale = ( info.boneFlags & FLAG_KF_HAS_VAR_SCALE ) == FLAG_KF_HAS_VAR_SCALE;
			info.fewKeyframes = ( info.boneFlags & FLAG_KF_HAS_FEW_KEYFRAMES ) == FLAG_KF_HAS_FEW_KEYFRAMES;
			info.ignoreBone = ( info.boneFlags & FLAG_KF_IGNORE_BONE ) == FLAG_KF_IGNORE_BONE;
			info.formatFloat = ( info.boneFlags & FLAG_KF_FORMAT_FLOAT ) == FLAG_KF_FORMAT_FLOAT;
			
			if( ! info.ignoreBone && ( info.hasVarPos || info.hasVarRot || info.hasVarScale ) ){
				pReadKeyframes( reader, *list, info );
			}
			
			if( list->GetKeyframeCount() == 0 ){
				// if there is no keyframe add a dummy one
				deAnimationKeyframe * const keyframe = new deAnimationKeyframe;
				
				try{
					list->AddKeyframe( keyframe );
					
				}catch( const deException & ){
					delete keyframe;
					throw;
				}
			}
			
			move.AddKeyframeList( list );
			
		}catch( const deException & ){
			delete list;
			throw;
		}
	}
}

void deAnimModule::pReadKeyframes( decBaseFileReader &reader, deAnimationKeyframeList &list, sInfo &info ){
	int i, keyframeCount;
	
	if( info.fewKeyframes ){
		keyframeCount = reader.ReadUShort();
		
	}else{
		if( info.deprOldFormat ){
			keyframeCount = info.playtimeFrames;
			
		}else{
			keyframeCount = info.playtimeFrames + 1;
		}
	}
	
	for( i=0; i<keyframeCount; i++ ){
		pReadKeyframe( reader, list, info, i );
	}
}

void deAnimModule::pReadKeyframe( decBaseFileReader &reader, deAnimationKeyframeList &list,
sInfo &info, int frameNumber ){
	deAnimationKeyframe * const keyframe = new deAnimationKeyframe;
	
	try{
		// read time if we have few keyframes
		if( info.fewKeyframes ){
			keyframe->SetTime( info.timeFactor * ( float )reader.ReadUShort() );
			if( keyframe->GetTime() < 0 ){
				DETHROW( deeInvalidFormat );
			}
			
		}else{
			keyframe->SetTime( info.timeFactor * ( float )frameNumber );
		}
		
		if( info.hasVarPos ){
			decVector position;
			
			if( info.formatFloat ){
				position.x = reader.ReadFloat();
				position.y = reader.ReadFloat();
				position.z = reader.ReadFloat();
				
			}else{
				position.x = 0.001f * ( float )reader.ReadShort();
				position.y = 0.001f * ( float )reader.ReadShort();
				position.z = 0.001f * ( float )reader.ReadShort();
			}
			
			keyframe->SetPosition( position );
		}
		
		if( info.hasVarRot ){
			decVector rotation;
			
			if( info.formatFloat ){
				rotation.x = reader.ReadFloat();
				rotation.y = reader.ReadFloat();
				rotation.z = reader.ReadFloat();
				
			}else{
				rotation.x = ( 0.01f * ( float )reader.ReadShort() ) * DEG2RAD;
				rotation.y = ( 0.01f * ( float )reader.ReadShort() ) * DEG2RAD;
				rotation.z = ( 0.01f * ( float )reader.ReadShort() ) * DEG2RAD;
			}
			
			keyframe->SetRotation( rotation );
		}
		
		if( info.hasVarScale ){
			decVector scale;
			
			if( info.formatFloat ){
				scale.x = reader.ReadFloat();
				scale.y = reader.ReadFloat();
				scale.z = reader.ReadFloat();
				
			}else{
				scale.x = 0.01f * ( float )reader.ReadShort();
				scale.y = 0.01f * ( float )reader.ReadShort();
				scale.z = 0.01f * ( float )reader.ReadShort();
			}
			
			keyframe->SetScale( scale );
		}
		
		list.AddKeyframe( keyframe );
		
	}catch( const deException & ){
		if( keyframe ){
			delete keyframe;
		}
		throw;
	}
}

void deAnimModule::pReadMoveVertexPositionSets( decBaseFileReader &reader, deAnimationMove &move, sInfo &info ){
	int i;
	
	for( i=0; i<info.vertexPositionSetCount; i++ ){
		deAnimationKeyframeVertexPositionSetList * const list = new deAnimationKeyframeVertexPositionSetList;
		
		try{
			info.vertexPositionSetFlags = reader.ReadByte();
			info.hasVarWeight = ( info.vertexPositionSetFlags & FLAG_KFVPS_HAS_VAR_WEIGHT ) == FLAG_KFVPS_HAS_VAR_WEIGHT;
			info.fewKeyframes = ( info.vertexPositionSetFlags & FLAG_KFVPS_HAS_FEW_KEYFRAMES ) == FLAG_KFVPS_HAS_FEW_KEYFRAMES;
			info.ignoreSet = ( info.vertexPositionSetFlags & FLAG_KFVPS_IGNORE_SET ) == FLAG_KFVPS_IGNORE_SET;
			info.formatFloat = ( info.vertexPositionSetFlags & FLAG_KFVPS_FORMAT_FLOAT ) == FLAG_KFVPS_FORMAT_FLOAT;
			
			if( ! info.ignoreSet && info.hasVarWeight ){
				pReadKeyframes( reader, *list, info );
			}
			
			if( list->GetKeyframeCount() == 0 ){
				// if there is no keyframe add a dummy one
				deAnimationKeyframeVertexPositionSet * const keyframe = new deAnimationKeyframeVertexPositionSet;
				
				try{
					list->AddKeyframe( keyframe );
					
				}catch( const deException & ){
					delete keyframe;
					throw;
				}
			}
			
			move.AddVertexPositionSetKeyframeList( list );
			
		}catch( const deException & ){
			delete list;
			throw;
		}
	}
}

void deAnimModule::pReadKeyframes( decBaseFileReader &reader,
deAnimationKeyframeVertexPositionSetList &list, sInfo &info ){
	int i, keyframeCount;
	
	if( info.fewKeyframes ){
		keyframeCount = reader.ReadUShort();
		
	}else{
		keyframeCount = info.playtimeFrames + 1;
	}
	
	for( i=0; i<keyframeCount; i++ ){
		pReadKeyframe( reader, list, info, i );
	}
}

void deAnimModule::pReadKeyframe( decBaseFileReader &reader,
deAnimationKeyframeVertexPositionSetList &list, sInfo &info, int frameNumber ){
	deAnimationKeyframeVertexPositionSet * const keyframe = new deAnimationKeyframeVertexPositionSet;
	
	try{
		// read time if we have few keyframes
		if( info.fewKeyframes ){
			keyframe->SetTime( info.timeFactor * ( float )reader.ReadUShort() );
			if( keyframe->GetTime() < 0 ){
				DETHROW( deeInvalidFormat );
			}
			
		}else{
			keyframe->SetTime( info.timeFactor * ( float )frameNumber );
		}
		
		if( info.hasVarWeight ){
			if( info.formatFloat ){
				keyframe->SetWeight( reader.ReadFloat() );
				
			}else{
				keyframe->SetWeight( 0.001f * ( float )reader.ReadShort() );
			}
		}
		
		list.AddKeyframe( keyframe );
		
	}catch( const deException & ){
		if( keyframe ){
			delete keyframe;
		}
		throw;
	}
}

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

void deAnimModule::pWriteKeyframeData( decBaseFileWriter &writer, const sConfig2 &config,
const deAnimationKeyframeVertexPositionSet &keyframe ){
	if( config.hasVarWeight ){
		pWriteKeyframeWeight( writer, config, keyframe.GetWeight() );
	}
}

void deAnimModule::pWriteKeyframeDataInterpolate( decBaseFileWriter &writer,
const sConfig2 &config, const deAnimationKeyframeVertexPositionSet &keyframePrev,
const deAnimationKeyframeVertexPositionSet &keyframeNext, int frameSteps ) {
	int i;
	for( i=1; i<frameSteps; i++ ){
		const float blend = ( float )i / ( float )frameSteps;
		
		if( config.hasVarWeight ){
			const float a = keyframePrev.GetWeight();
			const float b = keyframeNext.GetWeight();
			pWriteKeyframeWeight( writer, config, a * ( 1.0f - blend ) + b * blend );
		}
	}
	
	pWriteKeyframeData( writer, config, keyframeNext );
}

void deAnimModule::pWriteKeyframeWeight( decBaseFileWriter &writer, const sConfig2 &config, float weight ){
	if( config.formatFloat ){
		writer.WriteFloat( weight );
		
	}else{
		writer.WriteShort( ( short )( weight * 1000.0f + 0.5f ) );
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deanimModuleInternal : public deInternalModule{
public:
deanimModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DEAnim");
		SetDescription("Handles animations in the binary Drag[en]gine animation format.");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtAnimation);
		SetDirectoryName("deanim");
		GetPatternList().Add(".deanim");
		SetDefaultExtension(".deanim");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(DEAnimCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deanimRegisterInternalModule(deModuleSystem *system){
	return new deanimModuleInternal(system);
}
#endif
