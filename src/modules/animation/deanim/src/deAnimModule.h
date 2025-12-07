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
	deAnimModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deAnimModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load animation. */
	virtual void LoadAnimation(decBaseFileReader &reader, deAnimation &animation);
	
	/** \brief Save animation. */
	virtual void SaveAnimation(decBaseFileWriter &writer, const deAnimation &animation);
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
	
	void pReadBones(decBaseFileReader &reader, deAnimation &animation, sInfo &info);
	void pReadVertexPositionSets(decBaseFileReader &reader, deAnimation &animation, sInfo &info);
	void pReadMoves(decBaseFileReader &reader, deAnimation &animation, sInfo &info);
	void pReadMoveFps(decBaseFileReader &reader, deAnimationMove &move, sInfo &info);
	void pReadMoveBones(decBaseFileReader &reader, deAnimationMove &move, sInfo &info);
	void pReadKeyframes(decBaseFileReader &reader, deAnimationKeyframeList &list, sInfo &info);
	void pReadKeyframe(decBaseFileReader &reader, deAnimationKeyframeList &list, sInfo &info, int frameNumber);
	void pReadMoveVertexPositionSets(decBaseFileReader &reader, deAnimationMove &move, sInfo &info);
	void pReadKeyframes(decBaseFileReader &reader, deAnimationKeyframeVertexPositionSetList &list, sInfo &info);
	void pReadKeyframe(decBaseFileReader &reader, deAnimationKeyframeVertexPositionSetList &list, sInfo &info, int frameNumber);
	
	void pWriteKeyframeData(decBaseFileWriter &writer, const sConfig &config,
		const deAnimationKeyframe &keyframe);
	
	void pWriteKeyframeDataInterpolate(decBaseFileWriter &writer, const sConfig &config,
		const deAnimationKeyframe &keyframePrev, const deAnimationKeyframe &keyframeNext,
		int frameSteps);
	
	void pWriteKeyframePosition(decBaseFileWriter &writer, const sConfig &config, const decVector &position);
	void pWriteKeyframeRotation(decBaseFileWriter &writer, const sConfig &config, const decVector &rotation);
	void pWriteKeyframeScale(decBaseFileWriter &writer, const sConfig &config, const decVector &scale);
	
	void pWriteKeyframeData(decBaseFileWriter &writer, const sConfig2 &config,
		const deAnimationKeyframeVertexPositionSet &keyframe);
	
	void pWriteKeyframeDataInterpolate(decBaseFileWriter &writer, const sConfig2 &config,
		const deAnimationKeyframeVertexPositionSet &keyframePrev,
		const deAnimationKeyframeVertexPositionSet &keyframeNext, int frameSteps);
	
	void pWriteKeyframeWeight(decBaseFileWriter &writer, const sConfig2 &config, float weight);
};

#endif
