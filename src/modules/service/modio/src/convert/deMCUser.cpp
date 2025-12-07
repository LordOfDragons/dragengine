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

#include "deMCUser.h"
#include "deMCDetail.h"
#include "deMCCommon.h"


// Class deMCUser
///////////////////

std::vector<Modio::UserID> deMCUser::UserIDList(const deServiceObject &so){
	const int count = so.GetChildCount();
	std::vector<Modio::UserID> list;
	int i;
	
	for(i=0; i<count; i++){
		list.push_back((Modio::UserID)deMCCommon::ID(*so.GetChildAt(i)));
	}
	
	return list;
}

deServiceObject::Ref deMCUser::User(const Modio::User &user){
	const deServiceObject::Ref so(deServiceObject::Ref::NewWith());
	
	so->SetChildAt("userId", deMCCommon::ID(user.UserId));
	
	if(! user.Username.empty()){
		so->SetStringChildAt("username", user.Username.c_str());
	}
	
	if(! user.ProfileUrl.empty()){
		so->SetStringChildAt("profileUrl", user.ProfileUrl.c_str());
	}
	
	if(! user.DisplayNamePortal.empty()){
		so->SetStringChildAt("displayNamePortal", user.DisplayNamePortal.c_str());
	}
	
	if(user.AuthToken.has_value() && user.AuthToken->GetToken().has_value()){
		so->SetStringChildAt("authToken", user.AuthToken->GetToken()->c_str());
	}
	
	so->SetChildAt("avatar", deMCDetail::Avatar(user.UserId, user.Avatar));
	
	if(user.DateOnline != 0){
		so->SetChildAt("dateOnline", deMCCommon::DateTime(user.DateOnline));
	}
	
	return so;
}
