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

#ifndef _DECLASSSAFEARRAY_H_
#define _DECLASSSAFEARRAY_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Safe array script class.
 * \details Array class safe to be modified while iterating using forEach.
 */
class deClassSafeArray : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassSafeArray( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassSafeArray();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsSA, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool, *clsBlock;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCount );
	DEF_NATFUNC( nfHas );
	DEF_NATFUNC( nfIndexOf );
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfSetAt );
	DEF_NATFUNC( nfAdd );
	DEF_NATFUNC( nfAddIfAbsent );
	DEF_NATFUNC( nfRemove );
	DEF_NATFUNC( nfRemoveFrom );
	DEF_NATFUNC( nfRemoveIfPresent );
	DEF_NATFUNC( nfRemoveAll );
	DEF_NATFUNC( nfRemoveAll2 );
	
	DEF_NATFUNC( nfForEach );
	DEF_NATFUNC( nfFind );
#undef DEF_NATFUNC
};

#endif
