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

#ifndef _CEWPSTOPTOPIC_H_
#define _CEWPSTOPTOPIC_H_

#include "ceWPAction.h"

class ceCAStopTopic;



/**
 * \brief Stop topic conversation action property window.
 */
class ceWPAStopTopic : public ceWPAction{
public:
	typedef deTObjectReference<ceWPAStopTopic> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation action property window. */
	ceWPAStopTopic(ceWPTopic &parentPanel);
	
	/** \brief Clean up conversation action property window. */
	~ceWPAStopTopic() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active action or \em nullptr if none is active. */
	ceCAStopTopic *GetAction() const;
	
	/** \brief Update active action. */
	void UpdateAction();
	/*@}*/
};

#endif
