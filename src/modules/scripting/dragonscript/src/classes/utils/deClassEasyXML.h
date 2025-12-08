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
	deClassEasyXML(deScriptingDragonScript &ds);
	
	/** \brief Creates a new class. */
	virtual ~deClassEasyXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Get xml document from object or \em NULL if \em myself is \em NULL. */
	dedsXmlDocument *GetDocument(dsRealObject *myself) const;
	
	/** \brief Push xml document or \em NULL if \em document is \em NULL. */
	void PushDocument(dsRunTime *rt, dedsXmlDocument *document);
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
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewFile);
	DEF_NATFUNC(nfNewFile2);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	
	DEF_NATFUNC(nfHasParseFailed);
	DEF_NATFUNC(nfGetParseLog);
	DEF_NATFUNC(nfGetRootElement);
	
	DEF_NATFUNC(nfSetRootElement);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
