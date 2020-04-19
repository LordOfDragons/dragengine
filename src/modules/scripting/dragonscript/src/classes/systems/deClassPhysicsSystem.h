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

#ifndef _DECLASSPHYSICSSYSTEM_H_
#define _DECLASSPHYSICSSYSTEM_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Physics system script class.
 */
class deClassPhysicsSystem : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassPhysicsSystem( deScriptingDragonScript &ds );
	
	/** \brief Cleans up the class. */
	virtual ~deClassPhysicsSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsPhySys;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsModParam;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfGetParameterCount );
	DEF_NATFUNC( nfGetParameterInfo );
	DEF_NATFUNC( nfGetParameterInfo2 );
	DEF_NATFUNC( nfGetParameterValue );
	DEF_NATFUNC( nfSetParameterValue );
	DEF_NATFUNC( nfSendCommand );
#undef DEF_NATFUNC
};

#endif
