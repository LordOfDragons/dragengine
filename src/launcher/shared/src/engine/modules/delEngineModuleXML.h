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

#ifndef _DELENGINEMODULEXML_H_
#define _DELENGINEMODULEXML_H_

#include "../../utils/delBaseXML.h"

#include <dragengine/common/string/decString.h>

class delEngineModule;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load Engine Module XML.
 */
class DE_DLL_EXPORT delEngineModuleXML : public delBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine module xml read. */
	delEngineModuleXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up engine module xml read. */
	virtual ~delEngineModuleXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( const char *filename, decBaseFileReader &reader, delEngineModule &module );
	/*@}*/
	
	
	
private:
	void pReadModule( const decXmlElementTag &root, delEngineModule &module );
	void pReadModuleLibrary( const decXmlElementTag &root, delEngineModule &module );
};

#endif
