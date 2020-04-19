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
#ifndef _DEDSCLASSPROPFIELD_H_
#define _DEDSCLASSPROPFIELD_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deScriptingDragonScript;

class dePropField;



/**
 * @brief Prop Field Script Class.
 *
 * Native script class providing access to prop fields.
 */
class deClassPropField : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassPropField( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the prop field or NULL if myself is NULL. */
	dePropField *GetPropField( dsRealObject *myself ) const;
	/** Pushes the prop field which can be NULL. */
	void PushPropField( dsRunTime *rt, dePropField *propfield );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsPF, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsDVec, *clsVec, *clsMdl, *clsSkin, *clsPFL, *clsCF;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetPosition );
	
	DEF_NATFUNC( nfAddType );
	DEF_NATFUNC( nfRemoveAllTypes );
	DEF_NATFUNC( nfSetInstanceAt );
	
	DEF_NATFUNC( nfGetListener );
	DEF_NATFUNC( nfSetListener );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

// end of include only once
#endif
