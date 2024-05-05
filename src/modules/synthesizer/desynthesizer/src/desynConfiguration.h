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

#ifndef _DESYNCONFIGURATION_H_
#define _DESYNCONFIGURATION_H_

#include <dragengine/common/string/decString.h>

class decXmlAttValue;
class decXmlElementTag;
class decBaseFileReader;
class deDESynthesizer;



/**
 * \brief DESynthesizer configuration.
 */
class desynConfiguration{
private:
	deDESynthesizer &pModule;
	
	int pStreamBufSizeThreshold;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	desynConfiguration( deDESynthesizer &module );
	
	/** \brief Clean up configuration. */
	~desynConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Buffer size threshold to stream sound samples. */
	inline int GetStreamBufSizeThreshold() const{ return pStreamBufSizeThreshold; }
	
	/** \brief Set buffer size threshold to stream sound samples. */
	void SetStreamBufSizeThreshold( int threshold );
	
	
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configuration. */
	void SaveConfig();
	
	
	
private:
	void pCleanUp();
	void pLoadConfig( decBaseFileReader *file );
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name );
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name );
	const char *pGetCData( const decXmlElementTag &tag, const char *defaultValue );
	bool pGetCDataBool( const decXmlElementTag &tag, bool defaultValue );
	int pGetCDataInt( const decXmlElementTag &tag, int defaultValue );
};

#endif
