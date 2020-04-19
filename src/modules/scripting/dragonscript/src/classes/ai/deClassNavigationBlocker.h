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

#ifndef _DEDSCLASSNAVIGATIONBLOCKER_H_
#define _DEDSCLASSNAVIGATIONBLOCKER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigationBlocker;



/**
 * \brief Navigation Mesh Script Class.
 */
class deClassNavigationBlocker : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsNavigationSpaceType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassNavigationBlocker( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassNavigationBlocker();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the navigation blocker or NULL if myself is NULL. */
	deNavigationBlocker *GetNavigationBlocker( dsRealObject *myself ) const;
	/** \brief Pushes the navigation blocker which can be NULL. */
	void PushNavigationBlocker( dsRunTime *rt, deNavigationBlocker *blocker );
	
	inline dsClass *GetClassNavigationSpaceType() const{ return pClsNavigationSpaceType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsNavBlocker, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsDVec, *clsVec, *clsQuat, *clsShaList;
		dsClass *clsNavigationSpaceType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetLayer );
	DEF_NATFUNC( nfSetLayer );
	DEF_NATFUNC( nfGetSpaceType );
	DEF_NATFUNC( nfSetSpaceType );
	DEF_NATFUNC( nfGetBlockingPriority );
	DEF_NATFUNC( nfSetBlockingPriority );
	DEF_NATFUNC( nfGetEnabled );
	DEF_NATFUNC( nfSetEnabled );
	
	DEF_NATFUNC( nfGetShapeList );
	DEF_NATFUNC( nfSetShapeList );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
