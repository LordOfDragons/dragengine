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

#ifndef _DECLASSARSTATEMANIPULATOR_H_
#define _DECLASSARSTATEMANIPULATOR_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleStateManipulator;
class deScriptingDragonScript;



/**
 * \brief Class animator rule state manipulator.
 * \details Script class wrapping an attached or detached animator rule. Objects of
 *          this class hold a pointer to the rule and optional an animator. If the
 *          animator is non-NULL the rule pointer is held by the object. Otherwise
 *          only a reference to the animator is held and the pointer to the rule
 *          is not held. In addition an index is stored used to update the rule in
 *          the animator if existing. This index is not necessary the index of the
 *          rule itself but the index of the top most rule altered by changes made
 *          to this rule.
 */
class deClassARStateManipulator : public dsClass{
public:
	enum eTargets{
		etBlendFactor,
		etPosition,
		etOrientation,
		etSize,
		etVertexPositionSet
	};
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsARStateManipulatorTarget;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassARStateManipulator( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassARStateManipulator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Rule or \em NULL if deleted or myself is \em NULL. */
	deAnimatorRuleStateManipulator *GetRule( dsRealObject *myself ) const;
	
	/** \brief Assigns animator or \em NULL. */
	void AssignAnimator( dsRealObject *myself, deAnimator *animator );
	
	/** \brief Pushes a rule. */
	void PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleStateManipulator *rule );
	
	inline dsClass *GetClassARStateManipulatorTarget() const{ return pClsARStateManipulatorTarget; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARStaM, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsVec, *clsQuat, *clsVec2, *clsAr, *clsCol;
		dsClass *clsARStateManipulatorTarget;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetEnablePosition );
	DEF_NATFUNC( nfSetEnableOrientation );
	DEF_NATFUNC( nfSetEnableSize );
	DEF_NATFUNC( nfSetEnableVertexPositionSet );
	
	DEF_NATFUNC( nfSetMinimumPosition );
	DEF_NATFUNC( nfSetMaximumPosition );
	DEF_NATFUNC( nfSetMinimumRotation );
	DEF_NATFUNC( nfSetMaximumRotation );
	DEF_NATFUNC( nfSetMinimumSize );
	DEF_NATFUNC( nfSetMaximumSize );
	DEF_NATFUNC( nfSetMinimumVertexPositionSet );
	DEF_NATFUNC( nfSetMaximumVertexPositionSet );
	
	DEF_NATFUNC( nfTargetAddLink );
	DEF_NATFUNC( nfTargetRemoveAllLinks );
#undef DEF_NATFUNC
};

#endif
