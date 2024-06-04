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

#ifndef _IGDEXMLLOADTEXTUREPROPERTYLIST_H_
#define _IGDEXMLLOADTEXTUREPROPERTYLIST_H_

#include "../../utils/igdeBaseXML.h"

class igdeTextureProperty;
class igdeTexturePropertyList;
class decBaseFileReader;



/**
 * \brief Load texture property list.
 */
class DE_DLL_EXPORT igdeXMLLoadTexturePropertyList : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	igdeXMLLoadTexturePropertyList( deLogger *logger );
	
	/** \brief Clean up loader. */
	virtual ~igdeXMLLoadTexturePropertyList();
	/*@}*/
	
	
	
	/** \name Loading */
	/*@{*/
	/** \brief Load texture property list. */
	void ReadFromFile( igdeTexturePropertyList &list, decBaseFileReader &file );
	/*@}*/
	
	
	
private:
	void pReadList( const decXmlElementTag &root, igdeTexturePropertyList &list );
	void pReadProperty( const decXmlElementTag &root, igdeTexturePropertyList &list );
	void pReadDefault( const decXmlElementTag &root, igdeTextureProperty &property );
	void pReadAffects( const decXmlElementTag &root, igdeTextureProperty &property );
};

#endif
