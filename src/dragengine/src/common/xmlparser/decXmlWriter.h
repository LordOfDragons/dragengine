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

#ifndef _DECXMLWRITER_H_
#define _DECXMLWRITER_H_

#include "../file/decBaseFileWriter.h"

class decString;
class decUnicodeString;


/**
 * \brief Helper class for writing XML files without using a constructed XML document.
 */
class DE_DLL_EXPORT decXmlWriter{
private:
	const decBaseFileWriter::Ref pFile;
	int pIndent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml writer. */
	decXmlWriter(decBaseFileWriter *file);
	
	/** \brief Clean up xml writer. */
	~decXmlWriter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Indent level. */
	inline int GetIndent() const{ return pIndent; }
	
	/** \brief Set indent level. */
	void SetIndent(int indent);
	
	/** \brief Increase indent level by one. */
	void IncreaseIndent();
	
	/** \brief Decrease indent level by one if larger than 0. */
	void DecreaseIndent();
	
	/** \brief Write current indent. */
	void WriteIndent();
	
	/** \brief Write XML Declaration using UTF-8 as encoding. */
	void WriteXMLDeclaration();
	
	/** \brief Write unicode string in UTF-8 encoding. */
	void WriteUnicodeString(const decUnicodeString &string);
	
	/**
	 * \brief Write newline useful for structuring content.
	 * 
	 * Writes first the current indent and then a newline.
	 */
	void WriteNewline();
	
	/** \brief Write Document Type Declaration. */
	void WriteDocTypeDeclaration(const char *rootTag, const char *dtdSource);
	
	/** \brief Write Document Type Declaration. */
	void WriteDocTypeDeclaration(const decUnicodeString &rootTag,
		const decUnicodeString &dtdSource);
	
	/**
	 * \brief Write opening tag with no arguments.
	 * 
	 * Writes first the current indent and then '\<NAME>' or '\<NAME/>' without quotes
	 * depending on the empty parameter. If newline is true a newline is written after
	 * the tag. If empty is false and newline is true the indent is increased by one.
	 */
	void WriteOpeningTag(const char *name, bool empty = false,
		bool indent = true, bool newline = true);
	
	void WriteOpeningTag(const decUnicodeString &name, bool empty = false,
		bool indent = true, bool newline = true);
	
	/**
	 * \brief Write start of an opening tag.
	 * 
	 * Writes first the current indent and then '\<NAME' without quotes.
	 */
	void WriteOpeningTagStart(const char *name, bool indent = true);
	
	void WriteOpeningTagStart(const decUnicodeString &name, bool indent = true);
	
	/**
	 * \brief Write string argument.
	 * 
	 * Writes ' NAME="STRING"' without single quotes. The string value is properly escaped.
	 */
	void WriteAttributeString(const char *name, const char *string);
	
	void WriteAttributeString(const char *name, const decString &string);
	
	void WriteAttributeString(const decUnicodeString &name, const decUnicodeString &string);
	
	/**
	 * \brief Write boolean argument.
	 * 
	 * Writes ' NAME="VALUE"' without single quotes.
	 */
	void WriteAttributeBool(const char *name, bool value);
	
	void WriteAttributeBool(const decUnicodeString &name, bool value);
	
	/**
	 * \brief  Write integer argument.
	 * 
	 * Writes ' NAME="VALUE"' without single quotes.
	 */
	void WriteAttributeInt(const char *name, int value);
	
	void WriteAttributeInt(const decUnicodeString &name, int value);
	
	/**
	 * \brief Write float argument.
	 * 
	 * Writes ' NAME="VALUE"' without single quotes. The value is written similar to
	 * the %g format flag from the printf family of functions.
	 */
	void WriteAttributeFloat(const char *name, float value);
	
	void WriteAttributeFloat(const decUnicodeString &name, float value);
	
	/**
	 * \brief Write double argument.
	 * 
	 * Writes ' NAME="VALUE"' without single quotes. The value is written similar to
	 * the %g format flag from the printf family of functions.
	 */
	void WriteAttributeDouble(const char *name, double value);
	
	void WriteAttributeDouble(const decUnicodeString &name, double value);
	
	/**
	 * \brief Write end of an opening tag.
	 * 
	 * Writes either "/>" or ">". If empty is true an empty tag using the first text is
	 * written otherwise an opening tag with the second text. If newline is true a
	 * newline is written after the text. If newline is true and empty is false the
	 * indent is increased by one.
	 */
	void WriteOpeningTagEnd(bool empty = false, bool newline = true);
	
	/** \brief Write text with proper escaping. */
	void WriteTextString(const char *text);
	
	void WriteTextString(const decString &text);
	
	void WriteTextString(const decUnicodeString &text);
	
	/** \brief Write bool text. */
	void WriteTextBool(bool value);
	
	/** \brief Write integer text. */
	void WriteTextInt(int value);
	
	/** \brief Write float text. */
	void WriteTextFloat(float value);
	
	/** \brief Write double text. */
	void WriteTextDouble(double value);
	
	/**
	 * \brief Write closing tag.
	 * 
	 * Writes first the current indent and then '\</NAME>' without the quotes.
	 * If newline is true a newline is written after the closing tag and the
	 * indent decreased by one.
	 */
	void WriteClosingTag(const char *name, bool indent = true, bool newline = true);
	
	void WriteClosingTag(const decUnicodeString &name, bool indent = true, bool newline = true);
	
	/** \brief Write comment. */
	void WriteComment(const char *comment, bool indent = true, bool newline = true);
	
	void WriteComment(const decString &comment, bool indent = true, bool newline = true);
	
	void WriteComment(const decUnicodeString &comment, bool indent = true, bool newline = true);
	
	/**
	 * \brief Write entire data text tag.
	 * 
	 * A data tag composes of an opening tag with no parameters, a text and an ending tag
	 * on the same line ( if the text is not multi line ).
	 */
	void WriteDataTagString(const char *tagName, const char *string,
		bool indent = true, bool newline = true);
	
	void WriteDataTagString(const char *tagName, const decString &string,
		bool indent = true, bool newline = true);
	
	void WriteDataTagString(const decUnicodeString &tagName, const decUnicodeString &string,
		bool indent = true, bool newline = true);
	
	/**
	 * \brief Write entire boolean data tag.
	 * 
	 * A data tag composes of an opening tag with no parameters, a text and an ending
	 * tag on the same line ( if the text is not multi line ).
	 */
	void WriteDataTagBool(const char *tagName, bool value,
		bool indent = true, bool newline = true);
	
	void WriteDataTagBool(const decUnicodeString &tagName,
		bool value, bool indent = true, bool newline = true);
	
	/**
	 * \brief Write entire integer data tag.
	 * 
	 * A data tag composes of an opening tag with no parameters, a text and an ending
	 * tag on the same line ( if the text is not multi line ).
	 */
	void WriteDataTagInt(const char *tagName, int value,
		bool indent = true, bool newline = true);
	
	void WriteDataTagInt(const decUnicodeString &tagName,
		int value, bool indent = true, bool newline = true);
	
	/**
	 * \brief Write entire float data tag.
	 * 
	 * A data tag composes of an opening tag with no parameters, a text and an ending
	 * tag on the same line ( if the text is not multi line ).
	 */
	void WriteDataTagFloat(const char *tagName, float value,
		bool indent = true, bool newline = true);
	
	void WriteDataTagFloat(const decUnicodeString &tagName,
		float value, bool indent = true, bool newline = true);
	
	/**
	 * \brief Write entire double data tag.
	 * 
	 * A data tag composes of an opening tag with no parameters, a text and an ending
	 * tag on the same line ( if the text is not multi line ).
	 */
	void WriteDataTagDouble(const char *tagName, double value,
		bool indent = true, bool newline = true);
	
	void WriteDataTagDouble(const decUnicodeString &tagName,
		double value, bool indent = true, bool newline = true);
	
	
	
	/** \brief Escape XML Text. */
	static decString EscapeText(const char *text);
	/*@}*/
};

#endif
