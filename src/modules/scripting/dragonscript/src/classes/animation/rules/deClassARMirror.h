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

#ifndef _DECLASSARMIRROR_H_
#define _DECLASSARMIRROR_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleMirror;
class deScriptingDragonScript;



/**
 * Class animator rule mirror.
 * 
 * Script class wrapping an attached or detached animator rule. Objects of
 * this class hold a pointer to the rule and optional an animator. If the
 * animator is non-NULL the rule pointer is held by the object. Otherwise
 * only a reference to the animator is held and the pointer to the rule
 * is not held. In addition an index is stored used to update the rule in
 * the animator if existing. This index is not necessary the index of the
 * rule itself but the index of the top most rule altered by changes made
 * to this rule.
 */
class deClassARMirror : public dsClass{
public:
	enum eTargets{
		etBlendFactor
	};
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsARMirrorTarget;
	dsClass *pClsARMirrorMirrorAxis;
	dsClass *pClsARMirrorMatchNameType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create script class. */
	deClassARMirror(deScriptingDragonScript &ds);
	
	/** Clean up script class. */
	virtual ~deClassARMirror();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** Rule or nullptr if deleted or myself is nullptr. */
	deAnimatorRuleMirror *GetRule(dsRealObject *myself) const;
	
	/** Assigns animator or nullptr. */
	void AssignAnimator(dsRealObject *myself, deAnimator *animator);
	
	/** Pushes a rule. */
	void PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleMirror *rule);
	
	inline dsClass *GetClassARMirrorTarget() const{ return pClsARMirrorTarget; }
	inline dsClass *GetClassARMirrorMirrorAxis() const{ return pClsARMirrorMirrorAxis; }
	inline dsClass *GetClassARMirrorMatchNameType() const{ return pClsARMirrorMatchNameType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARMirror;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsObj;
		dsClass *clsVec;
		dsClass *clsQuat;
		dsClass *clsVec2;
		dsClass *clsAr;
		dsClass *clsCol;
		dsClass *clsARMirrorMirrorAxis;
		dsClass *clsARMirrorMatchNameType;
		dsClass *clsARMirrorTarget;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfTargetAddLink);
	DEF_NATFUNC(nfTargetRemoveAllLinks);
	
	DEF_NATFUNC(nfSetMirrorMirrorAxis);
	DEF_NATFUNC(nfSetMirrorBone);
	DEF_NATFUNC(nfAddMatchName);
	
	DEF_NATFUNC(nfSetEnablePosition);
	DEF_NATFUNC(nfSetEnableOrientation);
	DEF_NATFUNC(nfSetEnableSize);
	DEF_NATFUNC(nfSetEnableVertexPositionSet);
#undef DEF_NATFUNC
};

#endif
