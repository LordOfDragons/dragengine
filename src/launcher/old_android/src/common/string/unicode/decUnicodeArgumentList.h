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

// include only once
#ifndef _DECUNICODEARGUMENTLIST_H_
#define _DECUNICODEARGUMENTLIST_H_



// definitions
class decUnicodeString;



/**
 * @brief Manages argument list with unicode strings.
 *
 * Provides support to parse unicode strings into an argument list
 * using common separation schemas. Easy to use for obtaining
 * commands in unicode format and turning them into a simple to
 * use argument list.
 */
class decUnicodeArgumentList{
private:
	decUnicodeString **pArguments;
	int pArgumentCount, pArgumentSize;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new empty argument list object. */
	decUnicodeArgumentList();
	/** Frees the argument list object. */
	~decUnicodeArgumentList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of arguments. */
	inline int GetArgumentCount() const{ return pArgumentCount; }
	/** Retrieves the argument at the given index. */
	const decUnicodeString *GetArgumentAt(int index) const;
	/** Adds an argument to the end of the list. */
	void AddArgument(const decUnicodeString &argument);
	/** Determines if the given argument matches the provided utf8 string. */
	bool MatchesArgumentAt(int index, const char *string) const;
	/** Removes all arguments. */
	void RemoveAllArguments();
	/**
	 * Parses command line passed into arguments which are then added
	 * to the end of the list. Arguments are considered separated by
	 * a white space. Quoted text strings are considered one argument.
	 * This function is present for operating systems not providing
	 * an entry point function which already splits up the arguments.
	 */
	void ParseCommand(const decUnicodeString &command);
	/*@}*/
};

// end of include only once
#endif
