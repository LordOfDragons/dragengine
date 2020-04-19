/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DECLASSARBONETRANSFORMATOR_H_
#define _DECLASSARBONETRANSFORMATOR_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleBoneTransformator;
class deScriptingDragonScript;



/**
 * \brief Class animator rule bone transformator.
 * \details Script class wrapping an attached or detached animator rule. Objects of
 *          this class hold a pointer to the rule and optional an animator. If the
 *          animator is non-NULL the rule pointer is held by the object. Otherwise
 *          only a reference to the animator is held and the pointer to the rule
 *          is not held. In addition an index is stored used to update the rule in
 *          the animator if existing. This index is not necessary the index of the
 *          rule itself but the index of the top most rule altered by changes made
 *          to this rule.
 */
class deClassARBoneTransformator : public dsClass{
public:
	enum eTargets{
		etBlendFactor,
		etTranslation,
		etRotation,
		etScaling,
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
	DEF_NATFUNC( nfSetCoordinateFrame );
	DEF_NATFUNC( nfSetTargetBone );
	
	DEF_NATFUNC( nfSetEnablePosition );
	DEF_NATFUNC( nfSetEnableOrientation );
	DEF_NATFUNC( nfSetEnableSize );
#undef DEF_NATFUNC
};

#endif
