#!/usr/bin/env python
# -*- coding: utf-8 -*- 

import os.path
import string
import sys
import re


## regular expression matching a newline
reNewline = re.compile(r''' \n | : ''', re.VERBOSE)

## regular expression white spaces including line splicing but not a newline itself
reWhitespaces = re.compile(r''' ([ \t] | \\ \n)+ ''', re.VERBOSE)

## regular expression matching a single line comment
reCommentSingleLine = re.compile(r''' // [^\n]* \n ''', re.VERBOSE)

## regular expression matching a multiline comment
reCommentMultiline = re.compile(r''' / \* .*? \* / ''', re.VERBOSE | re.DOTALL)

## regular expression matching anything until the end of line including line splicing
reLineRemaining = re.compile(r''' (\\ \n | [^:\n])* (: | \n) ''', re.VERBOSE)

## regular expression for an identifier
reIdentifier = re.compile(r''' \w+ ''', re.VERBOSE | re.DOTALL)

## regular expression for an operator
reOperator = re.compile(r'''
	\<\<= | \>\>= |
	\+\+ | \-\- | \<\< | \>\> | \<= | \>= | &= | \|= | \^= | %= | \+= | \-= | \*= | /= |
	\+ | \- | \* | % | / | & | \| | \^ | \< | \> | ~
	''', re.VERBOSE)

## regular expression for a type
reType = re.compile(r'''
	\w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*
	''', re.VERBOSE | re.DOTALL)

## regular expression for an equal sign
reInitVariable = re.compile(r''' = ''', re.VERBOSE | re.DOTALL)

## regular expression for a comma
reFuncArgDelimiter = re.compile(r''' , ''', re.VERBOSE | re.DOTALL)

## regular expression for an opening paranthesis
reFuncArgBegin = re.compile(r''' \( ''', re.VERBOSE | re.DOTALL)

## regular expression for a closing paranthesis
reFuncArgEnd = re.compile(r''' \) ''', re.VERBOSE | re.DOTALL)

## regular expression for end keyword
reKeywordEnd = re.compile(r''' end ''', re.VERBOSE | re.DOTALL)

## regular expression for script up to next keyword
reScriptKeyword = re.compile(r'''
	(?P<modifiers> (((public | protected | private | abstract | static | fixed) ([ \t] | \\ \n)+)*)*)
	(?P<keyword> namespace | pin | requires | class | interface)
	''', re.VERBOSE | re.DOTALL)

## regular expression for namespace
reNamespace = re.compile(r'''
	([ \t] | \\ \n)+
	(?P<name> \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*)
	(: | \n)
	''', re.VERBOSE)

## regular expression for pinning
rePin = re.compile(r'''
	([ \t] | \\ \n)+
	(?P<name> \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*)
	(: | \n)
	''', re.VERBOSE)

## regular expression for require package
reRequires = re.compile(r'''
	([ \t] | \\ \n)+
	(?P<name> \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*)
	(: | \n)
	''', re.VERBOSE)

## regular expression for class
reClass = re.compile(r'''
	([ \t] | \\ \n)+
	(?P<name> \w+)
	([ \t] | \\ \n)*
	(extends ([ \t] | \\ \n)+
		(?P<extend> \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*))?
	(implements ([ \t] | \\ \n)+
		(?P<implements> \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*
			(, ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*)*))?
	(: | \n)
	''', re.VERBOSE)

## regular expression for modifier keywords
reModifiers = re.compile(r'''
	(((public | protected | private | abstract | static | fixed) ([ \t] | \\ \n)+)*)
	''', re.VERBOSE | re.DOTALL)

## regular expresion for class body keyword
reClassBodyKeyword = re.compile(r'''
	class | interface | enum | func | var | end
	''', re.VERBOSE | re.DOTALL)

## regular expresion for interface body keyword
reInterfaceBodyKeyword = re.compile(r'''
	class | interface | enum | func | end
	''', re.VERBOSE | re.DOTALL)

## regular expression for interface
reInterface = re.compile(r'''
	([ \t] | \\ \n)+
	(?P<name> \w+)
	([ \t] | \\ \n)*
	(extends ([ \t] | \\ \n)+ (?P<extend> \w+ ([ \t] | \\ \n)* (\. ([ \t] | \\ \n)* \w+ ([ \t] | \\ \n)*)*))?
	(: | \n)
	''', re.VERBOSE)

## regular expression for handling function body skipping
reFuncBodyFirstKeyword = re.compile(r'''
	(?P<keyword> if | select | for | while | try | end)
	[ \t:\n/"']
	''', re.VERBOSE)

reFuncBodyInlineKeyword = re.compile(r'''
	(
		(/[*] .* [*]/)
		| (" (\\" | [^"\n])* ")
		| (' (\\' | [^'\n])* ')
		| [^"':\n]
	)+
	block [ \t:\n/"']
	''', re.VERBOSE)

reFuncBodySkip = re.compile(r'''
	(.* | \\n)* (\n | :)
	''', re.VERBOSE)



## parse comments and empty lines. returns content afterwards
def parseWhitespaces(content, printContent = True):
	while True:
		m = reWhitespaces.match(content)
		if not m:
			m = reCommentSingleLine.match(content)
		if not m:
			m = reCommentMultiline.match(content)
		if not m:
			break
		if printContent:
			sys.stdout.write(content[:m.end()])
		content = content[m.end():]
	return content

## parse whitespaces and newlines
def parseWhitespacesNewline(content, printContent = True):
	while True:
		m = reWhitespaces.match(content)
		if not m:
			m = reCommentSingleLine.match(content)
		if not m:
			m = reCommentMultiline.match(content)
		if not m:
			m = reNewline.match(content)
		if not m:
			break
		if printContent:
			sys.stdout.write(content[:m.end()])
		content = content[m.end():]
	return content

## parse remaining line until newline
def parseRemainingLine(content, printContent = False):
	m = reLineRemaining.match(content)
	if m:
		if printContent:
			sys.stdout.write(content[:m.end()])
		content = content[m.end():]
	return content

## parse remaining line until newline returning the parsed content
def parseRemainingLineReturn(content):
	m = reLineRemaining.match(content)
	if m:
		return (content[m.end():], content[:m.end()])
	return (content, '')

## parse script content
def parseScript(content):
	namespaceOpen = False
	while True:
		content = parseWhitespacesNewline(content)
		
		m = reScriptKeyword.match(content)
		if not m:
			sys.stdout.write(content)
			break
		
		#sys.stdout.write(content[:m.start('keyword')])
		content = content[m.end():]
		
		modifiers = m.group('modifiers').strip() if m.group('modifiers') else None
		keyword = m.group('keyword')
		if keyword == 'namespace':
			if namespaceOpen:
				sys.stdout.write('};\n')
				namespaceOpen = False
			
			m = reNamespace.match(content)
			if not m:
				sys.stdout.write('namespace')
				continue
			sys.stdout.write('package {};\n'.format(m.group('name')))
			content = content[m.end():]
			namespaceOpen = True
			
		elif keyword == 'pin':
			m = rePin.match(content)
			if not m:
				sys.stdout.write('pin')
				continue
			sys.stdout.write('import {}.*;\n'.format(m.group('name')))
			content = content[m.end():]
			
		elif keyword == 'requires':
			m = reRequires.match(content)
			if not m:
				sys.stdout.write('requires')
				continue
			sys.stdout.write('import {}.*;\n'.format(m.group('name')))
			content = content[m.end():]
			
		elif keyword == 'class':
			content = parseClass(content, modifiers)
			
		elif keyword == 'interface':
			content = parseInterface(content, modifiers)
			
		else:
			sys.stdout.write(content)
			content = ''

## parse class. return content after class end
def parseClass(content, modifiers = None):
	m = reClass.match(content)
	if not m:
		sys.stdout.write('class')
		return content
	
	content = content[m.end():]
	className = m.group('name').strip()
	extendName = m.group('extend').strip() if m.group('extend') else 'Object'
	implements = m.group('implements').strip() if m.group('implements') else None
	
	if modifiers:
		printModifiers(modifiers)
	else:
		sys.stdout.write('public ')
	sys.stdout.write('class {}'.format(className))
	sys.stdout.write(' extends {}'.format(extendName))
	if implements:
		sys.stdout.write(' implements {}'.format(implements))
	sys.stdout.write(' {\n')
	
	# parse body
	while True:
		content = parseWhitespacesNewline(content)
		
		m = reModifiers.match(content)
		if not m:
			sys.stdout.write(content)
			content = ''
			break
		modifiers = m.group().strip().split()
		content = content[m.end():]
		
		content = parseWhitespaces(content, False)
		m = reClassBodyKeyword.match(content)
		if not m:
			sys.stdout.write(content)
			content = ''
			break
		keyword = m.group().strip()
		content = content[m.end():]
		
		if keyword == 'class':
			content = parseClass(content, modifiers)
			
		elif keyword == 'interface':
			content = parseInterface(content, modifiers)
			
		elif keyword == 'enum':
			content = parseClassEnum(content, modifiers)
			
		elif keyword == 'var':
			content = parseClassVar(content, modifiers)
			
		elif keyword == 'func':
			content = parseClassFunc(content, modifiers, className)
			
		elif keyword == 'end':
			content = parseRemainingLine(content, False)
			break
	
	sys.stdout.write('};\n')
	return content

## print modifiers
def printModifiers(modifiers):
	if 'public' in modifiers:
		sys.stdout.write('public ')
	elif 'protected' in modifiers:
		sys.stdout.write('protected ')
	elif 'private' in modifiers:
		sys.stdout.write('private ')
	else:
		sys.stdout.write('public ')
	
	if 'static' in modifiers:
		sys.stdout.write('static ')
	if 'fixed' in modifiers:
		sys.stdout.write('final ')

## parse class body enumeration
def parseClassEnum(content, modifiers):
	content = parseWhitespaces(content, False)
	m = reIdentifier.match(content)
	if not m:
		sys.stdout.write(content)
		return ''
	name = m.group().strip()
	content = content[m.end():]
	
	content = parseRemainingLine(content)
	
	printModifiers(modifiers)
	sys.stdout.write('enum {}{{\n'.format(name))
	
	# parse entries
	while True:
		content = parseWhitespacesNewline(content)
		
		m = reKeywordEnd.match(content)
		if m:
			content = parseRemainingLine(content[m.end():])
			break
		
		m = reIdentifier.match(content)
		if not m:
			sys.stdout.write(content)
			content = ''
			break
		name = m.group().strip()
		
		content = parseRemainingLine(content[m.end():], False)
		
		sys.stdout.write('{},\n'.format(name.strip()))
	
	sys.stdout.write('};\n')
	
	return content

## parse class body variable
def parseClassVar(content, modifiers):
	content = parseWhitespaces(content, False)
	m = reType.match(content)
	if not m:
		sys.stdout.write(content)
		return ''
	vartype = m.group().strip()
	content = content[m.end():]
	
	content = parseWhitespaces(content, False)
	m = reIdentifier.match(content)
	if not m:
		sys.stdout.write(content)
		return ''
	name = m.group().strip()
	content = content[m.end():]
	
	content = parseWhitespaces(content, False)
	init = None
	m = reInitVariable.match(content)
	if m:
		content = parseWhitespaces(content[m.end():], False)
		(content, remaining) = parseRemainingLineReturn(content)
		init = remaining.strip()
	else:
		content = parseRemainingLine(content)
	
	printModifiers(modifiers)
	if init:
		sys.stdout.write('{} {} = {};\n'.format(vartype, name, init))
	else:
		sys.stdout.write('{} {};\n'.format(vartype, name))
	
	return content

## parse class body function
def parseClassFunc(content, modifiers, constrType):
	printModifiers(modifiers)
	content = parseFuncDef(content, constrType)
	
	# super constructor if present
	content = parseRemainingLine(content, False)
	
	# function body
	sys.stdout.write(';\n')
	content = parseFuncBody(content)
	
	return content

## parse function definition
def parseFuncDef(content, constrType = ''):
	content = parseWhitespaces(content, False)
	m = reType.match(content)
	if not m:
		sys.stdout.write(content)
		return ''
	vartype = m.group().strip()
	content = content[m.end():]
	operator = ''
	
	m = reFuncArgBegin.match(content)
	if m:
		name = vartype
		vartype = constrType
		
	else:
		content = parseWhitespaces(content, False)
		m = reOperator.match(content)
		if m:
			operator = True
		else:
			m = reIdentifier.match(content)
			if not m:
				sys.stdout.write(content)
				return ''
		name = m.group().strip()
		content = content[m.end():]
	
	m = reFuncArgBegin.match(content)
	if not m:
		sys.stdout.write(content)
		return ''
	content = content[m.end():]
	
	if operator:
		sys.stdout.write('{} operator{}('.format(vartype, name))
	else:
		sys.stdout.write('{} {}('.format(vartype, name))
	
	m = reFuncArgEnd.match(content)
	if m:
		content = content[m.end():]
		
	else:
		while True:
			content = parseWhitespaces(content, False)
			m = reType.match(content)
			if not m:
				sys.stdout.write(content)
				return ''
			vartype = m.group().strip()
			content = content[m.end():]
			
			content = parseWhitespaces(content, False)
			m = reIdentifier.match(content)
			if not m:
				sys.stdout.write(content)
				return ''
			name = m.group().strip()
			content = content[m.end():]
			
			sys.stdout.write('{} {}'.format(vartype, name))
			
			content = parseWhitespaces(content, False)
			m = reFuncArgEnd.match(content)
			if m:
				content = content[m.end():]
				break
			
			m = reFuncArgDelimiter.match(content)
			if not m:
				sys.stdout.write(content)
				return ''
			content = content[m.end():]
			sys.stdout.write(', ')
			
	sys.stdout.write(')')
	
	return content

## parse function body skipping all content
#counter = 0
def parseFuncBody(content):
	#global counter
	#counter = counter+1
	while True:
		# skip white spaces from the start of the line
		content = parseWhitespacesNewline(content, False)
		
		# check if this is a keyword with a body at the start of the line
		m = reFuncBodyFirstKeyword.match(content)
		if m:
			#print(counter, 'reFuncBodyFirstKeyword', content[:m.end()])
			if m.group('keyword') == 'end':
				#counter = counter-1
				return content[m.end():]
			
			content = parseFuncBody(content[m.end():])
			continue
		
		# check if the line contains an inline keyword with a body
		m = reFuncBodyInlineKeyword.match(content)
		if m:
			#print(counter, 'reFuncBodyInlineKeyword', content[:m.end()])
			content = parseFuncBody(content[m.end():])
			continue
		
		# skip the rest of the line
		m = reFuncBodySkip.match(content)
		if m:
			#print(counter, 'reFuncBodySkip', content[:m.end()])
			content = content[m.end():]
		else:
			return ''
	
	return content

## parse interface. return content after interface end
def parseInterface(content, modifiers = None):
	m = reInterface.match(content)
	if not m:
		sys.stdout.write('class')
		return content
	
	content = content[m.end():]
	interfaceName = m.group('name').strip()
	extendName = m.group('extend').strip() if m.group('extend') else None
	
	if modifiers:
		printModifiers(modifiers)
	sys.stdout.write('public interface {}'.format(interfaceName))
	if extendName:
		sys.stdout.write(' extends {}'.format(extendName))
	sys.stdout.write(' {\n')
	
	# parse body
	while True:
		content = parseWhitespacesNewline(content)
		
		m = reModifiers.match(content)
		if not m:
			sys.stdout.write(content)
			content = ''
			break
		modifiers = m.group().split()
		content = content[m.end():]
		
		content = parseWhitespaces(content, False)
		m = reInterfaceBodyKeyword.match(content)
		if not m:
			sys.stdout.write(content)
			content = ''
			break
		keyword = m.group()
		content = content[m.end():]
		
		if keyword == 'class':
			content = parseClass(content, modifiers)
			
		elif keyword == 'interface':
			content = parseInterface(content, modifiers)
			
		elif keyword == 'enum':
			content = parseClassEnum(content, modifiers)
			
		elif keyword == 'func':
			content = parseInterfaceFunc(content, modifiers)
			
		elif keyword == 'end':
			content = parseRemainingLine(content, False)
			break
	
	sys.stdout.write('};\n')
	return content

## parse interface body function
def parseInterfaceFunc(content, modifiers):
	printModifiers(modifiers)
	sys.stdout.write('public ')
	content = parseFuncDef(content)
	sys.stdout.write(';\n')
	return content

## open file and filter content
def filter(filename):
	try:
		with open(filename, 'r') as f:
			content = f.read()
		parseScript(content)
	except UnicodeDecodeError as e:
		sys.stderr.write('{}: contains non-UTF8 characters!\n'.format(filename))
		sys.exit(2)
	except IOError as e:
		sys.stderr.write('{}\n'.format(e[1]))
		sys.exit(3)

if len(sys.argv) != 2:
	print('usage: {} filename'.format(sys.argv[0]))
	sys.exit(1)

filter(sys.argv[1])
sys.exit(0)
