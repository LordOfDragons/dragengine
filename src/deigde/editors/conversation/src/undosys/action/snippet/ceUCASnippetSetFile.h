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

#ifndef _CEUCASNIPPETSETFILE_H_
#define _CEUCASNIPPETSETFILE_H_

#include <deigde/undo/igdeUndo.h>

class ceCASnippet;
class ceConversationTopic;



/**
 * \brief Undo Action Snippet Conversation Action Set File.
 */
class ceUCASnippetSetFile : public igdeUndo{
private:
	ceConversationTopic::Ref pTopic;
	ceCASnippet::Ref pSnippet;
	decString pOldFile;
	decString pNewFile;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCASnippetSetFile> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCASnippetSetFile(ceConversationTopic::Ref topic, ceCASnippet::Ref snippet, const char *newFile);
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCASnippetSetFile();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
