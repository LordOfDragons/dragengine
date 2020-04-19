" Smalltalk Class DEVector2
|
| Copyright (C) 2015, Plüss Roland ( roland@rptd.ch )
|
| This program is free software; you can redistribute it and/or 
| modify it under the terms of the GNU General Public License 
| as published by the Free Software Foundation; either 
| version 2 of the License, or (at your option) any later 
| version.
| 
| This program is distributed in the hope that it will be useful,
| but WITHOUT ANY WARRANTY; without even the implied warranty of
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
| GNU General Public License for more details.
| 
| You should have received a copy of the GNU General Public License
| along with this program; if not, write to the Free Software
| Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."

Object subclass: DEVector2 [
	<comment: 'I am an immutable 2-component vector. I represent a vector in 2 dimensional
		space. The components are x and y.'>
	<shape: #byte>
	
	DEVector2 class [
		| scripting |
		
		new [
			<category: 'initialization'>
			"Create a new zero vector."
			<cCall: 'DEVector2.new' returning: #smalltalk args: #(#self)>
		]
		
		x: x y: y [
			<category: 'initialization'>
			"Create a new vector with coordinates x and y."
			<cCall: 'DEVector2.newXY' returning: #smalltalk args: #(#self #double #double)>
		]
	]
	
	initialize [
		<category: 'initialization'>
	]
	
	x [
		<category: 'accessors'>
		"X component."
		<cCall: 'DEVector2.x' returning: #double args: #(#self)>
	]
	
	y [
		<category: 'accessors'>
		"Y component."
		<cCall: 'DEVector2.y' returning: #double args: #(#self)>
	]
	
	
	
	hash [
		<category: 'accessors'>
		"Hash."
		<cCall: 'DEVector2.hash' returning: #string args: #(#self)>
	]
	
	asString [
		<category: 'accessors'>
		"String representation."
		<cCall: 'DEVector2.asString' returning: #smalltalk args: #(#self)>
	]
]
