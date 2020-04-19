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

#ifndef _DECLASSGAME_H_
#define _DECLASSGAME_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Game script class.
 */
class deClassGame : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassGame( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsGame;
		
		dsClass *clsInteger;
		dsClass *clsVoid;
		dsClass *clsString;
		dsClass *clsInputEvent;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfInputEvent );
	DEF_NATFUNC( nfInitGame );
	DEF_NATFUNC( nfCleanUp );
	DEF_NATFUNC( nfOnFrameUpdate );
	DEF_NATFUNC( nfOnResizeRenderWindow );
	
	DEF_NATFUNC( nfGetArgumentCount );
	DEF_NATFUNC( nfGetArgumentAt );
#undef DEF_NATFUNC
};

#endif
