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

#ifndef _DECXMLELEMENT_H_
#define _DECXMLELEMENT_H_

#include "../../deObject.h"

class decXmlVisitor;
class decXmlContainer;
class decXmlDocument;
class decXmlComment;
class decXmlPI;
class decXmlElementTag;
class decXmlCharacterData;
class decXmlEntityReference;
class decXmlCharReference;
class decXmlCDSect;
class decXmlAttValue;
class decXmlNamespace;



/**
 * \brief Base XML Tree Element.
 */
class decXmlElement : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlElement> Ref;


public:
	/** \brief Occurance enumeration. */
	enum eOccuranceTypes{
		/** \brief Element occures exactly one. */
		eotOnce,
		
		/** \brief Element occures once or multiple times. */
		eotMultiple,
		
		/** \brief Element occures a single time or never. */
		eotOptional,
		
		/** \brief Element occures any amount of times. */
		eotAny
	};
	
	
	
private:
	int pLineNumber;
	int pPositionNumber;
	decXmlElement *pParent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml element. */
	decXmlElement();
	
protected:
	/** \brief Clean up xml element. */
	virtual ~decXmlElement();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Line number this element starts at in the source file.
	 * 
	 * The line number is set by the parser for locating the error line.
	 * The line has to be 1 or larger with 1 beeing the first line.
	 */
	inline int GetLineNumber() const{ return pLineNumber; }
	
	/**
	 * \brief Set line number this element starts at in the source file.
	 * 
	 * The line number is set by the parser for locating the error line.
	 * The line has to be 1 or larger with 1 beeing the first line.
	 */
	void SetLineNumber( int lineNumber );
	
	/**
	 * \brief Position number this element starts at in the source file.
	 * 
	 * The position number is set by the parser for locating the error position.
	 * The position has to be 0 or larger with 0 beeing the start of the line.
	 */
	inline int GetPositionNumber() const{ return pPositionNumber; }
	
	/**
	 * \brief Set position number this element starts at in the source file.
	 * 
	 * The position number is set by the parser for locating the error position.
	 * The position has to be 0 or larger with 0 beeing the start of the line.
	 */
	void SetPositionNumber( int positionNumber );
	
	/** \brief Parent or NULL. */
	inline decXmlElement *GetParent() const{ return pParent; }
	
	/** \brief Set parent or NULL. */
	void SetParent( decXmlElement *parent );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit node. */
	virtual void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	/** \brief Can cast to decXmlContainer which is by default false. */
	virtual bool CanCastToContainer() const;
	
	/** \brief Can cast to decXmlDocument which is by default false. */
	virtual bool CanCastToDocument() const;
	
	/** \brief Can cast to decXmlComment which is by default false. */
	virtual bool CanCastToComment() const;
	
	/** \brief Can cast to decXmlPI which is by default false. */
	virtual bool CanCastToPI() const;
	
	/** \brief Can cast to decXmlElementTag which is by default false. */
	virtual bool CanCastToElementTag() const;
	
	/** \brief Can cast to decXmlCharacterData which is by default false. */
	virtual bool CanCastToCharacterData() const;
	
	/** \brief Can cast to decXmlEntityReference which is by default false. */
	virtual bool CanCastToEntityReference() const;
	
	/** \brief Can cast to decXmlCharReference which is by default false. */
	virtual bool CanCastToCharReference() const;
	
	/** \brief Can cast to decXmlCDSect which is by default false. */
	virtual bool CanCastToCDSect() const;
	
	/** \brief Can cast to decXmlAttValue which is by default false. */
	virtual bool CanCastToAttValue() const;
	
	/** \brief Can cast to decXmlNamespace which is by default false. */
	virtual bool CanCastToNamespace() const;
	
	/** \brief Cast to decXmlContainer. */
	virtual decXmlContainer *CastToContainer();
	
	/** \brief Cast to decXmlDocument. */
	virtual decXmlDocument *CastToDocument();
	
	/** \brief Cast to decXmlComment. */
	virtual decXmlComment *CastToComment();
	
	/** \brief Cast to decXmlPI. */
	virtual decXmlPI *CastToPI();
	
	/** \brief Cast to decXmlElementTag. */
	virtual decXmlElementTag *CastToElementTag();
	
	/** \brief Cast to decXmlCharacterData. */
	virtual decXmlCharacterData *CastToCharacterData();
	
	/** \brief Cast to decXmlEntityReference. */
	virtual decXmlEntityReference *CastToEntityReference();
	
	/** \brief Cast to decXmlCharReference. */
	virtual decXmlCharReference *CastToCharReference();
	
	/** \brief Cast to decXmlCDSect. */
	virtual decXmlCDSect *CastToCDSect();
	
	/** \brief Cast to decXmlAttValue. */
	virtual decXmlAttValue *CastToAttValue();
	
	/** \brief Cast to decXmlNamespace. */
	virtual decXmlNamespace *CastToNamespace();
	/*@}*/
};

#endif
