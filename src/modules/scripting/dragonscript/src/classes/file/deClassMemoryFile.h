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

#ifndef _DECLASSMEMORYFILE_H_
#define _DECLASSMEMORYFILE_H_

#include <libdscript/libdscript.h>

class decMemoryFile;
class deScriptingDragonScript;



/**
 * \brief Memory file script class.
 */
class deClassMemoryFile : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassMemoryFile( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassMemoryFile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Memory file or \em NULL if myself is \em NULL. */
	decMemoryFile *GetMemoryFile( dsRealObject *myself ) const;
	
	/**
	 * \brief Pushe memory file on stack.
	 * \details Memory file can be \em NULL in which case a null object is pushed. */
	void PushMemoryFile( dsRunTime *rt, decMemoryFile *memoryFile );
	
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsMemFile;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsInt;
		dsClass *clsFlt;
		
		dsClass *clsFRead;
		dsClass *clsFWrite;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfSetSize );
	
	DEF_NATFUNC( nfGetReader );
	DEF_NATFUNC( nfGetWriter );
#undef DEF_NATFUNC
};

#endif
