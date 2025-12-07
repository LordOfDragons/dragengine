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

#ifndef _DEOALDELAYEDDELETION_H_
#define _DEOALDELAYEDDELETION_H_

class deoalAudioThread;


/** \brief Delete OpenAL objects properly after audioing is done in the audio thread. */
class deoalDelayedDeletion{
private:
	/** \brief Previous entry in the linked list or \em NULL. */
	deoalDelayedDeletion *pLLPrev;
	
	/** \brief Next entry in the linked list or \em NULL. */
	deoalDelayedDeletion *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create delayed deletion object. */
	deoalDelayedDeletion();
	
	/** \brief Clean up delayed file write object. */
	virtual ~deoalDelayedDeletion();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Delete objects. */
	virtual void DeleteObjects(deoalAudioThread &audioThread) = 0;
	/*@}*/
	
	
	
	/**
	 * \name Linked list.
	 * \warning For use by deoalDelayedOperations only.
	 */
	/*@{*/
	/** \brief Previous entry in the linked list or \em NULL. */
	inline deoalDelayedDeletion *GetLLPrev() const{return pLLPrev;}
	
	/** \brief Set previous entry in the linked list or \em NULL. */
	void SetLLPrev(deoalDelayedDeletion *prev);
	
	/** \brief Next entry in the linked list or \em NULL. */
	inline deoalDelayedDeletion *GetLLNext() const{return pLLNext;}
	
	/** \brief Set next entry in the linked list or \em NULL. */
	void SetLLNext(deoalDelayedDeletion *next);
	/*@}*/
};

#endif
