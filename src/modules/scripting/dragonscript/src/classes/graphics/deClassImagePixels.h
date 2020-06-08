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

#ifndef _DECLASSIMAGEPIXELS_H_
#define _DECLASSIMAGEPIXELS_H_

#include <libdscript/libdscript.h>


class deEngine;
class deImage;
class deScriptingDragonScript;
class deClassResourceListener;


/**
 * \brief Image pixels class.
 */
class deClassImagePixels : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassImagePixels( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassImagePixels();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsImagePixels;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsImage;
		dsClass *clsColor;
		dsClass *clsArray;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetImage );
	DEF_NATFUNC( nfGetWidth );
	DEF_NATFUNC( nfGetHeight );
	DEF_NATFUNC( nfGetDepth );
	
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfGetAt2 );
	DEF_NATFUNC( nfGetRange );
	DEF_NATFUNC( nfGetRange2 );
#undef DEF_NATFUNC
};

#endif
