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
	deClassARLimit( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassARLimit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Rule or \em NULL if deleted or myself is \em NULL. */
	deAnimatorRuleLimit *GetRule( dsRealObject *myself ) const;
	
	/** \brief Assigns animator or \em NULL. */
	void AssignAnimator( dsRealObject *myself, deAnimator *animator );
	
	/** \brief Pushes a rule. */
	void PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleLimit *rule );
	
	inline dsClass *GetClassARLimitTarget() const{ return pClsARLimitTarget; }
	inline dsClass *GetClassARLimitCFrame() const{ return pClsARLimitCFrame; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARLimit, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsVec, *clsQuat, *clsVec2, *clsAr, *clsCol;
		dsClass *clsARLimitTarget;
		dsClass *clsARLimitCFrame;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetEnablePosMin );
	DEF_NATFUNC( nfSetEnablePosMax );
	DEF_NATFUNC( nfSetEnableRotMin );
	DEF_NATFUNC( nfSetEnableRotMax );
	DEF_NATFUNC( nfSetEnableScaleMin );
	DEF_NATFUNC( nfSetEnableScaleMax );
	DEF_NATFUNC( nfSetEnableVertexPositionSetMin );
	DEF_NATFUNC( nfSetEnableVertexPositionSetMax );
	
	DEF_NATFUNC( nfSetMinimumPosition );
	DEF_NATFUNC( nfSetMaximumPosition );
	DEF_NATFUNC( nfSetMinimumRotation );
	DEF_NATFUNC( nfSetMaximumRotation );
	DEF_NATFUNC( nfSetMinimumScaling );
	DEF_NATFUNC( nfSetMaximumScaling );
	DEF_NATFUNC( nfSetMinimumVertexPositionSet );
	DEF_NATFUNC( nfSetMaximumVertexPositionSet );
	DEF_NATFUNC( nfSetCoordinateFrame );
	DEF_NATFUNC( nfSetTargetBone );
	
	DEF_NATFUNC( nfTargetAddLink );
	DEF_NATFUNC( nfTargetRemoveAllLinks );
#undef DEF_NATFUNC
};

#endif
