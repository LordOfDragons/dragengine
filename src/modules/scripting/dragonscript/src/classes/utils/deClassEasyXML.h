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

#ifndef _DECLASSEASYXML_H_
#define _DECLASSEASYXML_H_

#include <libdscript/libdscript.h>


class dedsXmlDocument;
class deScriptingDragonScript;


/**
 * \brief Easy xml document.
 */
class deClassEasyXML : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	deClassEasyXML( deScriptingDragonScript &ds );
	
	/** \brief Creates a new class. */
	virtual ~deClassEasyXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get xml document from object or \em NULL if \em myself is \em NULL. */
	dedsXmlDocument *GetDocument( dsRealObject *myself ) const;
	
	/** \brief Push xml document or \em NULL if \em document is \em NULL. */
	void PushDocument( dsRunTime *rt, dedsXmlDocument *document );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsXmlDocument;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsXmlElement;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewFile );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	
	DEF_NATFUNC( nfHasParseFailed );
	DEF_NATFUNC( nfGetParseLog );
	DEF_NATFUNC( nfGetRootElement );
	
	DEF_NATFUNC( nfSetRootElement );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
