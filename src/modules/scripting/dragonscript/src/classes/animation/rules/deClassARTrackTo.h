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

#ifndef _DECLASSARTRACKTO_H_
#define _DECLASSARTRACKTO_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleTrackTo;
class deScriptingDragonScript;



/**
 * \brief Class animator rule track to.
 * \details Script class wrapping an attached or detached animator rule. Objects of
 *          this class hold a pointer to the rule and optional an animator. If the
 *          animator is non-NULL the rule pointer is held by the object. Otherwise
 *          only a reference to the animator is held and the pointer to the rule
 *          is not held. In addition an index is stored used to update the rule in
 *          the animator if existing. This index is not necessary the index of the
 *          rule itself but the index of the top most rule altered by changes made
 *          to this rule.
 */
class deClassARTrackTo : public dsClass{
public:
	enum eTargets{
		etBlendFactor,
		etPosition,
		etUp
	};
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsARTrackToTarget;
	dsClass *pClsARTrackToAxis;
	dsClass *pClsARTrackToUp;
	dsClass *pClsARTrackToLocked;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassARTrackTo(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassARTrackTo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Rule or \em NULL if deleted or myself is \em NULL. */
	deAnimatorRuleTrackTo *GetRule(dsRealObject *myself) const;
	
	/** \brief Assigns animator or \em NULL. */
	void AssignAnimator(dsRealObject *myself, deAnimator *animator);
	
	/** \brief Pushes a rule. */
	void PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleTrackTo *rule);
	
	inline dsClass *GetClassARTrackToTarget() const{ return pClsARTrackToTarget; }
	inline dsClass *GetClassARTrackToAxis() const{ return pClsARTrackToAxis; }
	inline dsClass *GetClassARTrackToUp() const{ return pClsARTrackToUp; }
	inline dsClass *GetClassARTrackToLocked() const{ return pClsARTrackToLocked; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARTrack, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsVec, *clsQuat, *clsVec2, *clsAr, *clsCol;
		dsClass *clsARTrackToTarget;
		dsClass *clsARTrackToAxis;
		dsClass *clsARTrackToUp;
		dsClass *clsARTrackToLocked;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfSetTrackBone);
	DEF_NATFUNC(nfSetTrackAxis);
	DEF_NATFUNC(nfSetUpAxis);
	DEF_NATFUNC(nfSetUpTarget);
	DEF_NATFUNC(nfSetLockedAxis);
	
	DEF_NATFUNC(nfTargetAddLink);
	DEF_NATFUNC(nfTargetRemoveAllLinks);
#undef DEF_NATFUNC
};

#endif
