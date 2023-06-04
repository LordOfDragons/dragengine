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
	deClassAnimator( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	~deClassAnimator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** Retrieves the animator or NULL if object is NULL. */
	deAnimator *GetAnimator( dsRealObject *object ) const;
	/** Pushes an animator on the stack (pushes nil if instance is NULL). */
	void PushAnimator( dsRunTime *rt, deAnimator *animator );
	
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
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetRig );
	DEF_NATFUNC( nfSetRig );
	DEF_NATFUNC( nfGetAnimation );
	DEF_NATFUNC( nfSetAnimation );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfSetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	
	DEF_NATFUNC( nfGetLinkCount );
	DEF_NATFUNC( nfAddLink );
	DEF_NATFUNC( nfRemoveAllLinks );
	DEF_NATFUNC( nfSetLinkController );
	DEF_NATFUNC( nfSetLinkCurve );
	DEF_NATFUNC( nfSetLinkRepeat );
	DEF_NATFUNC( nfSetLinkBone );
	DEF_NATFUNC( nfSetLinkBoneParameter );
	DEF_NATFUNC( nfSetLinkBoneValueRange );
	DEF_NATFUNC( nfSetLinkBoneValueRangeRotation );
	DEF_NATFUNC( nfSetLinkVertexPositionSet );
	DEF_NATFUNC( nfSetLinkVertexPositionSetValueRange );
	DEF_NATFUNC( nfSetLinkWrapY );
	
	DEF_NATFUNC( nfGetRuleCount );
	DEF_NATFUNC( nfAddRule );
	DEF_NATFUNC( nfGetRuleAt );
	DEF_NATFUNC( nfRemoveRuleAt );
	DEF_NATFUNC( nfRemoveAllRules );
	
	DEF_NATFUNC( nfAddBone );
	DEF_NATFUNC( nfRemoveAllBones );
	
	DEF_NATFUNC( nfAddVertexPositionSet );
	DEF_NATFUNC( nfRemoveAllVertexPositionSets );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
