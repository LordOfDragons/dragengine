#if 0

/* 
 * DragonScript Scripting Module
 *
 * Copyright (C) 2013, Plüss Roland ( roland@rptd.ch )
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

#ifndef _DECLASSDESTRING_H_
#define _DECLASSDESTRING_H_

#include <libdscript/libdscript.h>

class decString;
class deEngine;
class deScriptingDragonScript;



/**
 * @brief Class DE String.
 * Wrapper around the decString class. This is not a subclass of the String class to avoid
 * potential problems in the future.
 */
class deClassDEString : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassDEString( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassDEString();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript &GetDS(){ return *pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the string. */
	const decString &GetString( dsRealObject *myself );
	/** Push a new string. */
	void PushString( dsRunTime *rt, const decString &string );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsDEStr, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsObj, *clsStr, *clsArr;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetString );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

// end of include only once
#endif

#endif
