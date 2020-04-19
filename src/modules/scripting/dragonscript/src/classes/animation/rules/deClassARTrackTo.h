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
	deClassARTrackTo( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassARTrackTo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Rule or \em NULL if deleted or myself is \em NULL. */
	deAnimatorRuleTrackTo *GetRule( dsRealObject *myself ) const;
	
	/** \brief Assigns animator or \em NULL. */
	void AssignAnimator( dsRealObject *myself, deAnimator *animator );
	
	/** \brief Pushes a rule. */
	void PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleTrackTo *rule );
	
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
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetTrackBone );
	DEF_NATFUNC( nfSetTrackAxis );
	DEF_NATFUNC( nfSetUpAxis );
	DEF_NATFUNC( nfSetUpTarget );
	DEF_NATFUNC( nfSetLockedAxis );
	
	DEF_NATFUNC( nfTargetAddLink );
	DEF_NATFUNC( nfTargetRemoveAllLinks );
#undef DEF_NATFUNC
};

#endif
