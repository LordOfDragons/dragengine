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

#ifndef _DECLASSFONT_H_
#define _DECLASSFONT_H_

#include <libdscript/libdscript.h>

class deFont;
class deEngine;
class deScriptingDragonScript;



/**
 * \brief Font script class.
 */
class deClassFont : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassFont( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassFont();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Font or \em NULL if object is \em NULL. */
	deFont *GetFont( dsRealObject *object ) const;
	
	/** \brief Font size or 0 if object is \em NULL. */
	int GetFontSize( dsRealObject *object ) const;
	
	/** \brief Pushe font on stack or null object if \em font is \em NULL. */
	void PushFont( dsRunTime *rt, deFont *font, int size );
	
	
	
private:
	struct sInitData{
		dsClass *clsFont;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsByte;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsPoint;
		dsClass *clsUnicodeString;
		dsClass *clsResourceListener;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewSize );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	DEF_NATFUNC( nfGetSize );
	
	DEF_NATFUNC( nfGetTextSize );
	DEF_NATFUNC( nfGetCharWidth );
	DEF_NATFUNC( nfGetCharAdvance );
	DEF_NATFUNC( nfGetCharBearing );
	DEF_NATFUNC( nfGetMaxWidth );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
