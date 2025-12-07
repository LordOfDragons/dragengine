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

#ifndef _CEACTORGESTURE_H_
#define _CEACTORGESTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class deAnimator;



/**
 * \brief Conversation actor gesture.
 */
class ceActorGesture : public deObject{
private:
	igdeEnvironment &pEnvironment;
	deAnimator *pEngAnimator;
	
	decString pName;
	decString pPathAnimator;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceActorGesture> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create actor gesture. */
	ceActorGesture(igdeEnvironment &environment, const char *name);
	
	/** \brief Create copy of actor gesture. */
	ceActorGesture(const ceActorGesture &gesture);
	
protected:
	/** \brief Clean up actor gesture. */
	virtual ~ceActorGesture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{return pEnvironment;}
	
	/** \brief Engine animator or \em NULL. */
	inline deAnimator *GetEngineAnimator() const{return pEngAnimator;}
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Animator path. */
	inline const decString &GetPathAnimator() const{return pPathAnimator;}
	
	/** \brief Set animator path. */
	void SetPathAnimator(const char *path);
	/*@}*/
	
	
	
private:
	void pLoadAnimator();
};

#endif
