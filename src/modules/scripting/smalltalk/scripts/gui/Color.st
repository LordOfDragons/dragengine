" Smalltalk Class DEColor
|
| Copyright (C) 2009, Plüss Roland ( roland@rptd.ch )
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

Object subclass: DEColor [
	<comment: 'I am an immutable color. I have 4 components defined in the range from
		0 to 1. The components are red, green, blue and alpha. Alpha represents the
		transparency where 0 is fully transparent and 1 fully opaque.'>
	<shape: #byte>
	
	DEColor class [
		| scripting |
		
		new [
			self error: #shouldNotImplement
		]
		
		red: red green: green blue: blue [
			<category: 'initialization'>
			"Creates a new color."
			<cCall: 'DEColor.New1' returning: #smalltalk args: #(#self #double #double #double)>
		]
		
		red: red green: green blue: blue alpha: alpha [
			<category: 'initialization'>
			"Creates a new color."
			<cCall: 'DEColor.New2' returning: #smalltalk args: #(#self #double #double #double #double)>
		]
		
		intRed: red green: green blue: blue [
			<category: 'initialization'>
			"Creates a new color."
			<cCall: 'DEColor.NewInt1' returning: #smalltalk args: #(#self #int #int #int)>
		]
		
		intRed: red green: green blue: blue alpha: alpha [
			<category: 'initialization'>
			"Creates a new color."
			<cCall: 'DEColor.NewInt2' returning: #smalltalk args: #(#self #int #int #int #int)>
		]
	]
	
	initialize [
		<category: 'initialization'>
	]
	
	red [
		<category: 'accessors'>
		"Retrieves the red component."
		<cCall: 'DEColor.Red' returning: #double args: #(#self)>
	]
	
	green [
		<category: 'accessors'>
		"Retrieves the green component."
		<cCall: 'DEColor.Green' returning: #double args: #(#self)>
	]
	
	blue [
		<category: 'accessors'>
		"Retrieves the blue component."
		<cCall: 'DEColor.Blue' returning: #double args: #(#self)>
	]
	
	alpha [
		<category: 'accessors'>
		"Retrieves the alpha component."
		<cCall: 'DEColor.Alpha' returning: #double args: #(#self)>
	]
	
	clamp [
		<category: 'accessors'>
		"Retrieves the clamped color."
		<cCall: 'DEColor.Clamp' returning: #smalltalk args: #(#self)>
	]
	
	opaque [
		<category: 'accessors'>
		"Determines if the color is opaque."
		<cCall: 'DEColor.Opaque' returning: #boolean args: #(#self)>
	]
	
	transparent [
		<category: 'accessors'>
		"Determines if the color is transparent."
		<cCall: 'DEColor.Transparent' returning: #boolean args: #(#self)>
	]
	
	invisible [
		<category: 'accessors'>
		"Determines if the color is invisible."
		<cCall: 'DEColor.Invisible' returning: #boolean args: #(#self)>
	]
	
	copy [
		<category: 'accessors'>
		"Retrieves a copy of the color."
		<cCall: 'DEColor.Copy' returning: #smalltalk args: #(#self)>
	]
	
	
	
	= aColor [
		<category: 'operators'>
		"Determines if the color matches another color."
		<cCall: 'DEColor.Equal' returning: #boolean args: #(#self #smalltalk)>
	]
	
	+ aColor [
		<category: 'operators'>
		"Adds a color component wise to another color."
		<cCall: 'DEColor.Add' returning: #smalltalk args: #(#self #smalltalk)>
	]
	
	- aColor [
		<category: 'operators'>
		"Subtracts a color component wise to another color."
		<cCall: 'DEColor.Subtract' returning: #smalltalk args: #(#self #smalltalk)>
	]
	
	* aColor [
		<category: 'operators'>
		"Multiplies a color component wise to another color."
		<cCall: 'DEColor.Multiply' returning: #smalltalk args: #(#self #smalltalk)>
	]
	
	/ aColor [
		<category: 'operators'>
		"Divides a color component wise to another color."
		<cCall: 'DEColor.Divide' returning: #smalltalk args: #(#self #smalltalk)>
	]
	
	scalarMultiply: aScalar [
		<category: 'operators'>
		"Scalar-Multiplies a color component wise to another color."
		<cCall: 'DEColor.ScalarMultiply' returning: #smalltalk args: #(#self #float)>
	]
	
	scalarDivide: aScalar [
		<category: 'operators'>
		"Scalar-Divide a color component wise to another color."
		<cCall: 'DEColor.ScalarDivide' returning: #smalltalk args: #(#self #float)>
	]
	
	
	
	hash [
		<category: 'accessors'>
		"Retrieves the hash."
		<cCall: 'DEColor.Hash' returning: #string args: #(#self)>
	]
	
	asString [
		<category: 'accessors'>
		"Retrieves the string representation."
		<cCall: 'DEColor.AsString' returning: #smalltalk args: #(#self)>
	]
]
