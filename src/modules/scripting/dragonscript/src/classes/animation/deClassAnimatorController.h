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

#ifndef _DEDSCLASSANIMATORCONTROLLER_H_
#define _DEDSCLASSANIMATORCONTROLLER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deAnimator;
class deAnimatorInstance;



/**
 * \brief Animator controller script class.
 */
class deClassAnimatorController : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassAnimatorController( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassAnimatorController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get parameters from object. */
	void GetController( dsRealObject *myself, deAnimator *&animator,
		deAnimatorInstance *&instance, int &index ) const;
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deAnimator *animator, int index );
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deAnimatorInstance *instance, int index );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsAnimatorCtrl;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsVector;
		dsClass *clsAnimator;
		dsClass *clsAnimatorInst;
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
	DEF_NATFUNC( nfGetAnimatorInstance );
	DEF_NATFUNC( nfGetControllerIndex );
	
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfSetName );
	DEF_NATFUNC( nfGetLower );
	DEF_NATFUNC( nfGetUpper );
	DEF_NATFUNC( nfSetRange );
	DEF_NATFUNC( nfGetValue );
	DEF_NATFUNC( nfSetValue );
	DEF_NATFUNC( nfGetRelativeValue );
	DEF_NATFUNC( nfSetRelativeValue );
	DEF_NATFUNC( nfGetVector );
	DEF_NATFUNC( nfSetVector );
	DEF_NATFUNC( nfGetRotation );
	DEF_NATFUNC( nfSetRotation );
	DEF_NATFUNC( nfGetFrozen );
	DEF_NATFUNC( nfSetFrozen );
	DEF_NATFUNC( nfGetClamp );
	DEF_NATFUNC( nfSetClamp );
	
	DEF_NATFUNC( nfIncrement );
	DEF_NATFUNC( nfSetToLower );
	DEF_NATFUNC( nfSetToCenter );
	DEF_NATFUNC( nfSetToUpper );
	DEF_NATFUNC( nfReverse );
	DEF_NATFUNC( nfSetValueFrom );
	DEF_NATFUNC( nfSetAllFrom );
	DEF_NATFUNC( nfIsAtLower );
	DEF_NATFUNC( nfIsAtUpper );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
