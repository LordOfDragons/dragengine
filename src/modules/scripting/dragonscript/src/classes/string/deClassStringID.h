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

#ifndef _DECLASSSTRINGID_H_
#define _DECLASSSTRINGID_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/collection/decPointerDictionary.h>
#include <dragengine/common/string/decStringList.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Unique string ID script class.
 * 
 * Stores table of unique strings mapped to position number. Each StringID instance stores
 * the table number. Testing strings is fast since it is only an integer comparison.
 * StringID instances are immutable like strings.
 */
class deClassStringID : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	decStringList pStrings;
	decPointerDictionary pLookUpTable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassStringID( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassStringID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	
	
	/** \brief Number of strings. */
	int GetStringCount() const;
	
	/** \brief String index. */
	int GetStringID( dsRealObject *myself );
	
	/** \brief Push string id with index. */
	void PushStringID( dsRunTime *rt, int index );
	
	/**
	 * \brief Get index for string.
	 * 
	 * If string is absent from mapping table inserts the string first. The returned
	 * index stays valid for the entire lifetime of the script module.
	 */
	int InsertString( const char *string );
	
	/** \brief String at index. */
	const decString &GetStringAt( int index ) const;
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsStringID;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsBool;
		dsClass *clsObject;
		dsClass *clsString;
		
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetString );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
