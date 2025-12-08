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

// include only once
#ifndef _DECLASSANIMATOR_H_
#define _DECLASSANIMATOR_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deAnimator;
class deClassRig;
class deClassAnimation;
class deClassAnimatorRule;
class deClassVector;
class deClassVector2;
class deClassQuaternion;
class deScriptingDragonScript;



/**
 * \brief Animator Class.
 */
class deClassAnimator : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsAnimatorLinkBoneParameter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassAnimator(deScriptingDragonScript *ds);
	/** Cleans up the script class. */
	~deClassAnimator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Creates the class members. */
	void CreateClassMembers(dsEngine *engine);
	/** Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** Retrieves the animator or NULL if object is NULL. */
	deAnimator *GetAnimator(dsRealObject *object) const;
	/** Pushes an animator on the stack (pushes nil if instance is NULL). */
	void PushAnimator(dsRunTime *rt, deAnimator *animator);
	
	inline dsClass *GetClassAnimatorLinkBoneParameter() const{ return pClsAnimatorLinkBoneParameter; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsAr, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsArR, *clsRig, *clsAni, *clsCurveBezier;
		dsClass *clsAnimatorCtrl;
		dsClass *clsAnimatorLinkBoneParameter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetRig);
	DEF_NATFUNC(nfSetRig);
	DEF_NATFUNC(nfGetAnimation);
	DEF_NATFUNC(nfSetAnimation);
	
	DEF_NATFUNC(nfGetControllerCount);
	DEF_NATFUNC(nfSetControllerCount);
	DEF_NATFUNC(nfGetControllerAt);
	DEF_NATFUNC(nfGetControllerNamed);
	DEF_NATFUNC(nfIndexOfControllerNamed);
	
	DEF_NATFUNC(nfGetLinkCount);
	DEF_NATFUNC(nfAddLink);
	DEF_NATFUNC(nfRemoveAllLinks);
	DEF_NATFUNC(nfSetLinkController);
	DEF_NATFUNC(nfSetLinkCurve);
	DEF_NATFUNC(nfSetLinkRepeat);
	DEF_NATFUNC(nfSetLinkBone);
	DEF_NATFUNC(nfSetLinkBoneParameter);
	DEF_NATFUNC(nfSetLinkBoneValueRange);
	DEF_NATFUNC(nfSetLinkBoneValueRangeRotation);
	DEF_NATFUNC(nfSetLinkVertexPositionSet);
	DEF_NATFUNC(nfSetLinkVertexPositionSetValueRange);
	DEF_NATFUNC(nfSetLinkWrapY);
	
	DEF_NATFUNC(nfGetRuleCount);
	DEF_NATFUNC(nfAddRule);
	DEF_NATFUNC(nfGetRuleAt);
	DEF_NATFUNC(nfRemoveRuleAt);
	DEF_NATFUNC(nfRemoveAllRules);
	
	DEF_NATFUNC(nfAddBone);
	DEF_NATFUNC(nfRemoveAllBones);
	
	DEF_NATFUNC(nfAddVertexPositionSet);
	DEF_NATFUNC(nfRemoveAllVertexPositionSets);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfEquals2);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
