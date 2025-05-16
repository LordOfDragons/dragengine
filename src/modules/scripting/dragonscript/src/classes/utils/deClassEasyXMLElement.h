/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	DEF_NATFUNC(nfInsertElement);
	DEF_NATFUNC( nfRemoveElement );
	DEF_NATFUNC( nfRemoveAllElements );
	
	DEF_NATFUNC( nfAddTag );
	DEF_NATFUNC( nfAddDataTagString );
	DEF_NATFUNC( nfAddDataTagInteger );
	DEF_NATFUNC( nfAddDataTagFloat );
	DEF_NATFUNC( nfAddDataTagBool );
	DEF_NATFUNC( nfAddCData );
	DEF_NATFUNC( nfAddComment );
	
	DEF_NATFUNC( nfForEachTag );
	DEF_NATFUNC( nfHasTags );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
