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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sePropertyNode.h"
#include "sePropertyNodeList.h"

#include <dragengine/common/exceptions.h>



// Class sePropertyNodeList
/////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeList::sePropertyNodeList(){
}

sePropertyNodeList::sePropertyNodeList(const sePropertyNodeList &list) : pNodes(list.pNodes){
}

sePropertyNodeList::~sePropertyNodeList(){
}



// Management
///////////////

int sePropertyNodeList::GetCount() const{
	return pNodes.GetCount();
}

sePropertyNode *sePropertyNodeList::GetAt(int index) const{
	return (sePropertyNode*)pNodes.GetAt(index);
}

bool sePropertyNodeList::Has(sePropertyNode *node) const{
	return pNodes.Has(node);
}

int sePropertyNodeList::IndexOf(sePropertyNode *node) const{
	return pNodes.IndexOf(node);
}

void sePropertyNodeList::Add(sePropertyNode *node){
	if(!node){
		DETHROW(deeInvalidParam);
	}
	pNodes.Add(node);
}

void sePropertyNodeList::AddIfAbsent(sePropertyNode *node){
	if(!node){
		DETHROW(deeInvalidParam);
	}
	pNodes.AddIfAbsent(node);
}

void sePropertyNodeList::Insert(int index, sePropertyNode *node){
	if(!node){
		DETHROW(deeInvalidParam);
	}
	pNodes.Insert(node, index);
}

void sePropertyNodeList::Remove(sePropertyNode *node){
	pNodes.Remove(node);
}

void sePropertyNodeList::RemoveIfPresent(sePropertyNode *node){
	pNodes.RemoveIfPresent(node);
}

void sePropertyNodeList::RemoveAll(){
	pNodes.RemoveAll();
}

void sePropertyNodeList::Move(sePropertyNode *node, int index){
	pNodes.Move(node, index);
}



// Operators
//////////////

sePropertyNodeList &sePropertyNodeList::operator=(const sePropertyNodeList &list){
	pNodes = list.pNodes;
	return *this;
}
