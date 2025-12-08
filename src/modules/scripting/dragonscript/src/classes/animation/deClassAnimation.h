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

#ifndef _DECLASSANIMATION_H_
#define _DECLASSANIMATION_H_

#include <libdscript/libdscript.h>

class deEngine;
class deAnimation;
class deScriptingDragonScript;
class deClassResourceListener;

/**
 * \brief Animation script class.
 */
class deClassAnimation : public dsClass{
private:
	deEngine &pGameEngine;
	deScriptingDragonScript &pDS;
	
	dsClass *pClsBoneParameter;
	
public:
	deClassAnimation(deEngine &gameEngine, deScriptingDragonScript &ds);
	~deClassAnimation() override;
	
	void CreateClassMembers(dsEngine *engine) override;
	inline deEngine &GetGameEngine() const{ return pGameEngine; }
	
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	deAnimation *GetAnimation(dsRealObject *This) const;
	void PushAnimation(dsRunTime *rt, deAnimation::Ref anim);
	
	inline dsClass *GetClassAnimationBoneParameter() const{ return pClsBoneParameter; }
	
private:
	struct sInitData{
		dsClass *clsAnim, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj, *clsRN;
		dsClass *clsCurveBezier;
		dsClass *clsAnimationBoneParameter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfLoad);
	DEF_NATFUNC(nfLoadAsynchron);
	DEF_NATFUNC(nfSave);
	DEF_NATFUNC(nfDestructor);
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetMovePlaytime);
	DEF_NATFUNC(nfGetBoneCount);
	DEF_NATFUNC(nfIndexOfBoneNamed);
	DEF_NATFUNC(nfGetBoneName);
	DEF_NATFUNC(nfGetVertexPositionSetCount);
	DEF_NATFUNC(nfIndexOfVertexPositionSetNamed);
	DEF_NATFUNC(nfGetVertexPositionSetName);
	DEF_NATFUNC(nfGetMoveFPS);
	DEF_NATFUNC(nfGetMoveCount);
	DEF_NATFUNC(nfGetMoveName);
	DEF_NATFUNC(nfIndexOfMoveNamed);
	DEF_NATFUNC(nfGetKeyframeCurve);
	DEF_NATFUNC(nfGetVertexPositionSetKeyframeCurve);
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
