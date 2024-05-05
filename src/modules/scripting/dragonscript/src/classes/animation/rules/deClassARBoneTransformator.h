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

#ifndef _DECLASSARBONETRANSFORMATOR_H_
#define _DECLASSARBONETRANSFORMATOR_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleBoneTransformator;
class deScriptingDragonScript;



/**
 * \brief Class animator rule bone transformator.
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
class deClassARBoneTransformator : public dsClass{
public:
	enum eTargets{
		etBlendFactor,
		etTranslation,
		etRotation,
		etScaling
	};
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsARBoneTransformatorTarget;
	dsClass *pClsARBoneTransformatorCFrame;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassARBoneTransformator( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassARBoneTransformator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Rule or \em NULL if deleted or myself is \em NULL. */
	deAnimatorRuleBoneTransformator *GetRule( dsRealObject *myself ) const;
	
	/** \brief Assigns animator or \em NULL. */
	void AssignAnimator( dsRealObject *myself, deAnimator *animator );
	
	/** \brief Pushes a rule. */
	void PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleBoneTransformator *rule );
	
	inline dsClass *GetClassARBoneTransformatorTarget() const{ return pClsARBoneTransformatorTarget; }
	inline dsClass *GetClassARBoneTransformatorCFrame() const{ return pClsARBoneTransformatorCFrame; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARBoneTrans, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsVec, *clsQuat, *clsVec2, *clsAr, *clsCol;
		dsClass *clsARBoneTransformatorTarget;
		dsClass *clsARBoneTransformatorCFrame;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfTargetAddLink );
	DEF_NATFUNC( nfTargetRemoveAllLinks );
	
	DEF_NATFUNC( nfSetMinimumTranslation );
	DEF_NATFUNC( nfSetMaximumTranslation );
	DEF_NATFUNC( nfSetMinimumRotation );
	DEF_NATFUNC( nfSetMaximumRotation );
	DEF_NATFUNC( nfSetMinimumScaling );
	DEF_NATFUNC( nfSetMaximumScaling );
	DEF_NATFUNC( nfSetAxis );
	DEF_NATFUNC( nfSetMinimumAngle );
	DEF_NATFUNC( nfSetMaximumAngle );
	DEF_NATFUNC( nfSetUseAxis );
	DEF_NATFUNC( nfSetCoordinateFrame );
	DEF_NATFUNC( nfSetTargetBone );
	
	DEF_NATFUNC( nfSetEnablePosition );
	DEF_NATFUNC( nfSetEnableOrientation );
	DEF_NATFUNC( nfSetEnableSize );
#undef DEF_NATFUNC
};

#endif
