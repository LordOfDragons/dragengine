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

#ifndef _DEALCONFIGXML_H_
#define _DEALCONFIGXML_H_

#include "../dealBaseXML.h"

#include "../common/string/decString.h"

class dealConfigWindow;
class decBaseFileReader;
class decBaseFileWriter;
class dealConfiguration;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Configuration XML.
 */
class dealConfigXML : public dealBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration xml read/save. */
	dealConfigXML(deLogger *logger, const char *loggerSource);
	
	/** \brief Clean up configuration xml read/save. */
	~dealConfigXML() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile(decBaseFileReader &reader, dealConfiguration &config);
	
	/** \brief Write to XML file. */
	void WriteToFile(decBaseFileWriter &writer, const dealConfiguration &config);
	/*@}*/
	
	
	
private:
	void pWriteConfig(decXmlWriter &writer, const dealConfiguration &config);
	void pWriteWindow(decXmlWriter &writer, const dealConfigWindow &window, const char *tagName);
	
	void pReadConfig(const decXmlElementTag &root, dealConfiguration &config);
	void pReadWindow(const decXmlElementTag &root, dealConfigWindow &window);
};

#endif
