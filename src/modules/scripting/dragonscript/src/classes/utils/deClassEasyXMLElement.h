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

#ifndef _DECLASSEASYXMLELEMENT_H_
#define _DECLASSEASYXMLELEMENT_H_

#include <libdscript/libdscript.h>


class decXmlElement;
class deScriptingDragonScript;


/**
 * \brief Easy xml element.
 */
class deClassEasyXMLElement : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsEasyXMLElementType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	deClassEasyXMLElement( deScriptingDragonScript &ds );
	
	/** \brief Creates a new class. */
	virtual ~deClassEasyXMLElement();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get xml element from object or \em NULL if \em myself is \em NULL. */
	decXmlElement *GetElement( dsRealObject *myself ) const;
	
	/** \brief Push xml element or \em NULL if \em element is \em NULL. */
	void PushElement( dsRunTime *rt, decXmlElement *element );
	
	inline dsClass *GetClassEasyXMLElementType() const{ return pClsEasyXMLElementType; }
	
	
	
private:
	struct sInitData{
		dsClass *clsXmlElement;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsBlock;
		dsClass *clsEasyXMLElementType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetLine );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfGetType );
	
	DEF_NATFUNC( nfGetTagName );
	DEF_NATFUNC( nfSetTagName );
	
	DEF_NATFUNC( nfGetCDataContent );
	DEF_NATFUNC( nfSetCDataContent );
	
	DEF_NATFUNC( nfGetComment );
	DEF_NATFUNC( nfSetComment );
	
	DEF_NATFUNC( nfGetAttributeCount );
	DEF_NATFUNC( nfGetAttributeNamed );
	DEF_NATFUNC( nfGetAttributeIntNamed );
	DEF_NATFUNC( nfGetAttributeFloatNamed );
	DEF_NATFUNC( nfGetAttributeBoolNamed );
	DEF_NATFUNC( nfHasAttributeNamed );
	DEF_NATFUNC( nfSetAttributeString );
	DEF_NATFUNC( nfSetAttributeInteger );
	DEF_NATFUNC( nfSetAttributeFloat );
	DEF_NATFUNC( nfSetAttributeBool );
	DEF_NATFUNC( nfRemoveAttributeNamed );
	DEF_NATFUNC( nfRemoveAllAttributes );
	
	DEF_NATFUNC( nfGetFirstCData );
	DEF_NATFUNC( nfGetFirstCDataInteger );
	DEF_NATFUNC( nfGetFirstCDataFloat );
	DEF_NATFUNC( nfGetFirstCDataBool );
	
	DEF_NATFUNC( nfGetElementCount );
	DEF_NATFUNC( nfGetElementAt );
	DEF_NATFUNC( nfHasElement );
	DEF_NATFUNC( nfAddElement );
	DEF_NATFUNC( nfRemoveElement );
	DEF_NATFUNC( nfRemoveAllElements );
	
	DEF_NATFUNC( nfAddTag );
	DEF_NATFUNC( nfAddDataTagString );
	DEF_NATFUNC( nfAddDataTagInteger );
	DEF_NATFUNC( nfAddDataTagFloat );
	DEF_NATFUNC( nfAddDataTagBool );
	DEF_NATFUNC( nfAddComment );
	
	DEF_NATFUNC( nfForEachTag );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
