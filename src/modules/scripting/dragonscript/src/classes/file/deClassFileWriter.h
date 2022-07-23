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

#ifndef _DECLASSFILEWRITER_H_
#define _DECLASSFILEWRITER_H_

#include <libdscript/libdscript.h>

class decBaseFileWriter;
class deScriptingDragonScript;



/**
 * @brief File Writer Script Class.
 */
class deClassFileWriter : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassFileWriter( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	virtual ~deClassFileWriter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the fileWriter or NULL if myself is NULL. */
	decBaseFileWriter *GetFileWriter( dsRealObject *myself ) const;
	/** Retrieves the stream version. */
	int GetStreamVersion( dsRealObject *myself ) const;
	/** Pushes the file writer on the stack. File writer can be NULL to push a null object. */
	void PushFileWriter( dsRunTime *rt, decBaseFileWriter *fileWriter );
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsFileWriter;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsTimeDate;
		
		dsClass *clsFileReader;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewZCompressed );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	
	DEF_NATFUNC( nfGetStreamVersion );
	DEF_NATFUNC( nfSetStreamVersion );
	
	DEF_NATFUNC( nfWriteChar );
	DEF_NATFUNC( nfWriteByte );
	DEF_NATFUNC( nfWriteShort );
	DEF_NATFUNC( nfWriteUShort );
	DEF_NATFUNC( nfWriteInt );
	DEF_NATFUNC( nfWriteUInt );
	DEF_NATFUNC( nfWriteFloat );
	DEF_NATFUNC( nfWriteString8 );
	DEF_NATFUNC( nfWriteString16 );
	DEF_NATFUNC( nfWriteString );
	DEF_NATFUNC( nfWriteData );
	DEF_NATFUNC( nfWriteData2 );
	DEF_NATFUNC( nfWriteTimeDate );
#undef DEF_NATFUNC
};

#endif
