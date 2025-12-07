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

#ifndef _DESYNCOMMANDEXECUTER_H_
#define _DESYNCOMMANDEXECUTER_H_

class deDESynthesizer;
class decUnicodeArgumentList;
class decUnicodeString;



/**
 * \brief Command executer.
 */
class desynCommandExecuter{
private:
	deDESynthesizer &pModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create command executer. */
	desynCommandExecuter(deDESynthesizer &module);
	
	/** \brief Clean up command executer. */
	~desynCommandExecuter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deDESynthesizer &GetModule() const{ return pModule; }
	
	
	
	/** \brief Run command. */
	void ExecuteCommand(const decUnicodeArgumentList &command, decUnicodeString &answer);
	
	/** \brief Display help message. */
	void CmdHelp(const decUnicodeArgumentList &command, decUnicodeString &answer);
	/*@}*/
};

#endif
