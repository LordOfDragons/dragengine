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

#ifndef _PROJDISTRIBUTORLOCALXML_H_
#define _PROJDISTRIBUTORLOCALXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class projProject;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save XML Project Local.
 */
class projProjectLocalXml : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml project. */
	projProjectLocalXml(deLogger *logger, const char *loggerSource);
	
	/** \brief Clean up xml project. */
	virtual ~projProjectLocalXml();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read configuration from xml file. */
	void ReadFromFile(decBaseFileReader &reader, projProject &project);
	
	/** \brief Write configuration to xml file. */
	void WriteToFile(decBaseFileWriter &writer, const projProject &project);
	/*@}*/
	
	
	
private:
	void pWriteProjectLocal(decXmlWriter &writer, const projProject &project);
	
	void pReadProjectLocal(const decXmlElementTag &root, projProject &project);
};

#endif
