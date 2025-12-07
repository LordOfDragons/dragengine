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

#ifndef _DECLASSARLIMIT_H_
#define _DECLASSARLIMIT_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleLimit;
class deScriptingDragonScript;



/**
 * \brief Class animator rule limit.
 * \details Script class wrapping an attached or detached animator rule. Objects of
 *          this class hold a pointer to the rule and optional an animator. If the
 *          animator is non-NULL the rule pointer is held by the object. Otherwise
 *          only a reference to the animator is held and the pointer to the rule
 *          is not held. In addition an index is stored used to update the rule in
 *          the animator if existing. This index is not necessary the index of the
 *          rule itself but the index of the top most rule altered by changes made
 *          to this rule.
 */
class deClassARLimit : public dsClass{
public:
	enum eTargets{
		etBlendFactor
	};
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsARLimitTarget;
	dsClass *pClsARLimitCFrame;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassARLimit(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassARLimit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Rule or \em NULL if deleted or myself is \em NULL. */
	deAnimatorRuleLimit *GetRule(dsRealObject *myself) const;
	
	/** \brief Assigns animator or \em NULL. */
	void AssignAnimator(dsRealObject *myself, deAnimator *animator);
	
	/** \brief Pushes a rule. */
	void PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleLimit *rule);
	
	inline dsClass *GetClassARLimitTarget() const{return pClsARLimitTarget;}
	inline dsClass *GetClassARLimitCFrame() const{return pClsARLimitCFrame;}
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARLimit, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsVec, *clsQuat, *clsVec2, *clsAr, *clsCol;
		dsClass *clsARLimitTarget;
		dsClass *clsARLimitCFrame;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfSetEnablePosMin);
	DEF_NATFUNC(nfSetEnablePosMax);
	DEF_NATFUNC(nfSetEnableRotMin);
	DEF_NATFUNC(nfSetEnableRotMax);
	DEF_NATFUNC(nfSetEnableScaleMin);
	DEF_NATFUNC(nfSetEnableScaleMax);
	DEF_NATFUNC(nfSetEnableVertexPositionSetMin);
	DEF_NATFUNC(nfSetEnableVertexPositionSetMax);
	
	DEF_NATFUNC(nfSetMinimumPosition);
	DEF_NATFUNC(nfSetMaximumPosition);
	DEF_NATFUNC(nfSetMinimumRotation);
	DEF_NATFUNC(nfSetMaximumRotation);
	DEF_NATFUNC(nfSetMinimumScaling);
	DEF_NATFUNC(nfSetMaximumScaling);
	DEF_NATFUNC(nfSetMinimumVertexPositionSet);
	DEF_NATFUNC(nfSetMaximumVertexPositionSet);
	DEF_NATFUNC(nfSetCoordinateFrame);
	DEF_NATFUNC(nfSetTargetBone);
	
	DEF_NATFUNC(nfTargetAddLink);
	DEF_NATFUNC(nfTargetRemoveAllLinks);
#undef DEF_NATFUNC
};

#endif
