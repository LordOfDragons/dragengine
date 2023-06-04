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

#ifndef _DEANIMMODULE_H_
#define _DEANIMMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/animation/deBaseAnimationModule.h>

class deAnimationMove;
class deAnimationKeyframeList;
class deAnimationKeyframe;
class deAnimationKeyframeVertexPositionSetList;
class deAnimationKeyframeVertexPositionSet;


/**
 * @brief Drag[en]gine Animation Module.
 */
class deAnimModule : public deBaseAnimationModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deAnimModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deAnimModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load animation. */
	virtual void LoadAnimation( decBaseFileReader &reader, deAnimation &animation );
	
	/** \brief Save animation. */
	virtual void SaveAnimation( decBaseFileWriter &writer, const deAnimation &animation );
	/*@}*/
	
	
	
private:
	struct sInfo{
		int version;
		int flags;
		bool deprOldFormat;
		
		int boneCount;
		int vertexPositionSetCount;
		int moveCount;
		
		float timeFactor;
		int playtimeFrames;
		
		int boneFlags;
		bool hasVarPos;
		bool hasVarRot;
		bool hasVarScale;
		bool fewKeyframes;
		bool ignoreBone;
		bool formatFloat;
		
		int vertexPositionSetFlags;
		bool hasVarWeight;
		bool ignoreSet;
	};
	
	struct sConfig{
		bool hasVarPos;
		bool hasVarRot;
		bool hasVarScale;
		bool fewKeyframes;
		bool ignoreBone;
		bool formatFloat;
	};
	
	struct sConfig2{
		bool hasVarWeight;
		bool fewKeyframes;
		bool ignoreSet;
		bool formatFloat;
	};
	
	void pReadBones( decBaseFileReader &reader, deAnimation &animation, sInfo &info );
	void pReadVertexPositionSets( decBaseFileReader &reader, deAnimation &animation, sInfo &info );
	void pReadMoves( decBaseFileReader &reader, deAnimation &animation, sInfo &info );
	void pReadMoveFps( decBaseFileReader &reader, deAnimationMove &move, sInfo &info );
	void pReadMoveBones( decBaseFileReader &reader, deAnimationMove &move, sInfo &info );
	void pReadKeyframes( decBaseFileReader &reader, deAnimationKeyframeList &list, sInfo &info );
	void pReadKeyframe( decBaseFileReader &reader, deAnimationKeyframeList &list, sInfo &info, int frameNumber );
	void pReadMoveVertexPositionSets( decBaseFileReader &reader, deAnimationMove &move, sInfo &info );
	void pReadKeyframes( decBaseFileReader &reader, deAnimationKeyframeVertexPositionSetList &list, sInfo &info );
	void pReadKeyframe( decBaseFileReader &reader, deAnimationKeyframeVertexPositionSetList &list, sInfo &info, int frameNumber );
	
	void pWriteKeyframeData( decBaseFileWriter &writer, const sConfig &config,
		const deAnimationKeyframe &keyframe );
	
	void pWriteKeyframeDataInterpolate( decBaseFileWriter &writer, const sConfig &config,
		const deAnimationKeyframe &keyframePrev, const deAnimationKeyframe &keyframeNext,
		int frameSteps );
	
	void pWriteKeyframePosition( decBaseFileWriter &writer, const sConfig &config, const decVector &position );
	void pWriteKeyframeRotation( decBaseFileWriter &writer, const sConfig &config, const decVector &rotation );
	void pWriteKeyframeScale( decBaseFileWriter &writer, const sConfig &config, const decVector &scale );
	
	void pWriteKeyframeData( decBaseFileWriter &writer, const sConfig2 &config,
		const deAnimationKeyframeVertexPositionSet &keyframe );
	
	void pWriteKeyframeDataInterpolate( decBaseFileWriter &writer, const sConfig2 &config,
		const deAnimationKeyframeVertexPositionSet &keyframePrev,
		const deAnimationKeyframeVertexPositionSet &keyframeNext, int frameSteps );
	
	void pWriteKeyframeWeight( decBaseFileWriter &writer, const sConfig2 &config, float weight );
};

#endif
