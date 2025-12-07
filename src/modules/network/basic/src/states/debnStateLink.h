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

#ifndef _DEBNSTATELINK_H_
#define _DEBNSTATELINK_H_

class debnState;
class debnConnection;



/**
 * \brief State link.
 */
class debnStateLink{
public:
	/** \brief Link states. */
	enum eLinkStates{
		elsDown,
		elsListening,
		elsUp
	};
	
	
	
private:
	debnState *pState;
	int pIdentifier;
	int pLinkState;
	
	debnConnection &pConnection;
	bool pChanged;
	
	bool *pValueChanged;
	int pValueCount;
	
	debnStateLink *pPreviousLink;
	debnStateLink *pNextLink;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create state link. */
	debnStateLink(debnState *bnState, debnConnection &connection);
	
	/** \brief Clean up state link. */
	~debnStateLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief State or NULL if dropped. */
	inline debnState *GetState() const{return pState;}
	
	/** \brief Drop state. */
	void DropState();
	
	/** \brief Unique identifier. */
	inline int GetIdentifier() const{return pIdentifier;}
	
	/** \brief Set unique identifier. */
	void SetIdentifier(int identifier);
	
	/** \brief Link state. */
	inline int GetLinkState() const{return pLinkState;}
	
	/** \brief Set link state. */
	void SetLinkState(int linkState);
	
	/** \brief Connection. */
	inline debnConnection &GetConnection() const{return pConnection;}
	
	/** \brief State link changed. */
	inline bool GetChanged() const{return pChanged;}
	
	/** \brief Set if state link changed. */
	void SetChanged(bool changed);
	
	/** \brief Value changed. */
	bool GetValueChangedAt(int index) const;
	
	/** \brief Set if value changed. */
	void SetValueChangedAt(int index, bool changed);
	
	/** \brief One or more values are marked changed. */
	bool HasChangedValues() const;
	
	/** \brief Reset changed of all values and state link. */
	void ResetChanged();
	/*@}*/
	
	
	
	/** \name Linked List ( for debnLinkQueue only ) */
	/*@{*/
	inline debnStateLink *GetPreviousLink() const{return pPreviousLink;}
	void SetPreviousLink(debnStateLink *link);
	inline debnStateLink *GetNextLink() const{return pNextLink;}
	void SetNextLink(debnStateLink *link);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
