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

#ifndef _DECLASSANIMATORINSTANCE_H_
#define _DECLASSANIMATORINSTANCE_H_

#include <libdscript/libdscript.h>

class deEngine;
class deAnimatorInstance;
class deScriptingDragonScript;



/**
 * \brief Animator instance script class.
 */
class deClassAnimatorInstance : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsAnimatorRuleBlendMode;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassAnimatorInstance(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassAnimatorInstance();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Animator instance for object or \em NULL if object is \em NULL. */
	deAnimatorInstance *GetAnimatorInstance(dsRealObject *object) const;
	
	/** \brief Push animator instance or NULL on the stack. */
	void PushAnimatorInstance(dsRunTime *rt, deAnimatorInstance::Ref instance);
	
	inline dsClass *GetClassAnimatorRuleBlendMode() const{ return pClsAnimatorRuleBlendMode; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsAnimatorInstance;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsObj;
		dsClass *clsAr;
		dsClass *clsComp;
		dsClass *clsAnimator;
		dsClass *clsAnimatorCtrl;
		dsClass *clsAnimatorRuleBlendMode;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetAnimator);
	DEF_NATFUNC(nfSetAnimator);
	DEF_NATFUNC(nfSetAnimator2);
	DEF_NATFUNC(nfGetComponent);
	DEF_NATFUNC(nfSetComponent);
	DEF_NATFUNC(nfGetAnimation);
	DEF_NATFUNC(nfSetAnimation);
	
	DEF_NATFUNC(nfGetBlendMode);
	DEF_NATFUNC(nfSetBlendMode);
	DEF_NATFUNC(nfGetBlendFactor);
	DEF_NATFUNC(nfSetBlendFactor);
	DEF_NATFUNC(nfGetEnableRetargeting);
	DEF_NATFUNC(nfSetEnableRetargeting);
	DEF_NATFUNC(nfGetProtectDynamicBones);
	DEF_NATFUNC(nfSetProtectDynamicBones);
	
	DEF_NATFUNC(nfGetControllerCount);
	DEF_NATFUNC(nfGetControllerAt);
	DEF_NATFUNC(nfGetControllerNamed);
	DEF_NATFUNC(nfIndexOfControllerNamed);
	DEF_NATFUNC(nfCopyControllerStates);
	DEF_NATFUNC(nfCopyControllerStates2);
	DEF_NATFUNC(nfCopyNamedControllerStates);
	
	DEF_NATFUNC(nfApply);
	DEF_NATFUNC(nfApplyDirect);
	DEF_NATFUNC(nfCaptureState);
	DEF_NATFUNC(nfStoreFrame);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
