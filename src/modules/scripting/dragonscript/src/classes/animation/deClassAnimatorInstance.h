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
	deClassAnimatorInstance( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassAnimatorInstance();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Animator instance for object or \em NULL if object is \em NULL. */
	deAnimatorInstance *GetAnimatorInstance( dsRealObject *object ) const;
	
	/** \brief Push animator instance or NULL on the stack. */
	void PushAnimatorInstance( dsRunTime *rt, deAnimatorInstance *instance );
	
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
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAnimator );
	DEF_NATFUNC( nfSetAnimator );
	DEF_NATFUNC( nfSetAnimator2 );
	DEF_NATFUNC( nfGetComponent );
	DEF_NATFUNC( nfSetComponent );
	DEF_NATFUNC( nfGetAnimation );
	DEF_NATFUNC( nfSetAnimation );
	
	DEF_NATFUNC( nfGetBlendMode );
	DEF_NATFUNC( nfSetBlendMode );
	DEF_NATFUNC( nfGetBlendFactor );
	DEF_NATFUNC( nfSetBlendFactor );
	DEF_NATFUNC( nfGetEnableRetargeting );
	DEF_NATFUNC( nfSetEnableRetargeting );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	DEF_NATFUNC( nfCopyControllerStates );
	DEF_NATFUNC( nfCopyControllerStates2 );
	DEF_NATFUNC( nfCopyNamedControllerStates );
	
	DEF_NATFUNC( nfApply );
	DEF_NATFUNC( nfApplyDirect );
	DEF_NATFUNC( nfCaptureState );
	DEF_NATFUNC( nfStoreFrame );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
