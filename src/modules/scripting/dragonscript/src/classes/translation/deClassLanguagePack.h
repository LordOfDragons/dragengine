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

#ifndef _DECLASSLANGUAGEPACK_H_
#define _DECLASSLANGUAGEPACK_H_

#include <libdscript/libdscript.h>

class deEngine;
class deLanguagePack;
class deClassUnicodeString;
class deScriptingDragonScript;



/**
 * \brief Language pack script class.
 */
class deClassLanguagePack : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassLanguagePack( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassLanguagePack();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Language pack or \em NULL if myself is \em NULL. */
	deLanguagePack *GetLanguagePack( dsRealObject *myself ) const;
	
	/** \brief Push language pack which can be \em NULL. */
	void PushLanguagePack( dsRunTime *rt, deLanguagePack *languagePack );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsLP;
		dsClass *clsVoid;
		dsClass *clsInt;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsBool;
		dsClass *clsUS;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfGetDescription );
	DEF_NATFUNC( nfGetFilename );
	
	DEF_NATFUNC( nfTranslate );
	DEF_NATFUNC( nfTranslate2 );
	DEF_NATFUNC( nfGetMissingText );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
