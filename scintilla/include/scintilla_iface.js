/*
Copyright (C) 2019 marc2003
Usage: node scintilla_iface.js
*/

'use strict'

const types = {
	'bool': 'bool',
	'cells': 'const char*',
	'colour': 'Colour',
	'findtext': 'void*',
	'formatrange': 'void*',
	'line': 'Line',
	'pointer': 'void*',
	'position': 'Position',
	'string': 'const char*',
	'stringresult': 'char*',
	'textrange': 'void*',
	'void' : 'void',
}

const filenames = {
	'input': 'Scintilla.iface',
	'output': '../../src/ui/wtlscintilla.h',
}

const options = 'utf8'
const newline = '\r\n'

const format_one = (type, name) => {
	if (!name.length) return '0'
	if (type.endsWith('*')) return `reinterpret_cast<uintptr_t>(${name})`
	return name
}

const format_two = (type, name) => {
	if (!name.length) return '0'
	if (type.endsWith('*')) return `reinterpret_cast<intptr_t>(${name})`
	if (type == 'int' || type == 'bool' || type == 'Colour' || type == 'Line' || type == 'Position') return name
	console.log(`Parsing aborted because of unknown type: ${type} ${name}`)
	process.exit(1)
}

const get_args = (line) => {
	const start = line.indexOf('(')
	const end = line.indexOf(')')
	return line.substr(start + 1, end - start - 1).split(',').map(item => item.trim().split(' '))
}

const get_type = (type) => {
	if (!type) return ''
	return types[type] || 'int'
}

const create_header = () => {
	let tmp = []
	tmp.push('#pragma once')
	tmp.push('')
	tmp.push('template <class T>')
	tmp.push('class CScintillaImpl : public CWindowImpl<T, CWindow, CControlWinTraits>')
	tmp.push('{')
	tmp.push('public:')
	tmp.push('\tDECLARE_WND_SUPERCLASS2(_T("WTL_ScintillaCtrl"), CScintillaImpl, CWindow::GetWndClassName())')
	tmp.push('')
	tmp.push('\tusing Colour = int;')
	tmp.push('\tusing Line = int;')
	tmp.push('\tusing Position = int;')
	tmp.push('')
	tmp.push('\tvoid SetFnPtr()')
	tmp.push('\t{')
	tmp.push('\t\tATLASSERT(::IsWindow(this->m_hWnd));')
	tmp.push('\t\tfn = reinterpret_cast<FunctionDirect>(::SendMessage(this->m_hWnd, SCI_GETDIRECTFUNCTION, 0, 0));')
	tmp.push('\t\tptr = ::SendMessage(this->m_hWnd, SCI_GETDIRECTPOINTER, 0, 0);')
	tmp.push('\t}')
	tmp.push('')
	tmp.push('\t// Auto-generated from Scintilla.iface by scintilla_iface.js')
	return tmp
}

const create_body = (lines) =>
{
	let tmp = []

	for (const line of lines) {
		if (line.startsWith('cat Deprecated')) break

		const str = line.substr(0, line.indexOf('=')).split(' ')
		const feature = str[0]

		if (feature == 'fun' || feature == 'get' || feature == 'set') {
			const ret = get_type(str[1])
			const name = str[2]

			const [one, two] = get_args(line)
			const typeOne = get_type(one[0])
			const nameOne = one[1] || ''
			const typeTwo = get_type(two[0])
			const nameTwo = two[1] || ''

			let main = `\t${ret} ${name}(`
			if (typeOne.length) main += `${typeOne} ${nameOne}`
			if (typeOne.length && typeTwo.length) main += ', '
			if (typeTwo.length) main += `${typeTwo} ${nameTwo}`
			main += ') { '

			if (ret != 'void') main += 'return '

			let cast = false
			if (ret == 'void*') {
				cast = true
				main += 'reinterpret_cast<void*>('
			}

			main += `Call(SCI_${name.toUpperCase()}`

			if (typeOne.length || typeTwo.length) {
				main += `, ${format_one(typeOne, nameOne)}`
				if (typeTwo.length) {
					main += `, ${format_two(typeTwo, nameTwo)}`
				}
			}

			if (cast) main+= ')'
			main += '); }'

			tmp.push(main)
		}
	}
	tmp.sort()
	return tmp
}

const create_footer = () => {
	let tmp = []
	tmp.push('')
	tmp.push('private:')
	tmp.push('\tusing FunctionDirect = intptr_t(*)(intptr_t ptr, unsigned int msg, uintptr_t wParam, intptr_t lParam);')
	tmp.push('')
	tmp.push('\tintptr_t Call(unsigned int msg, uintptr_t wParam = 0, intptr_t lParam = 0)')
	tmp.push('\t{')
	tmp.push('\t\treturn fn(ptr, msg, wParam, lParam);')
	tmp.push('\t}')
	tmp.push('')
	tmp.push('\tFunctionDirect fn;')
	tmp.push('\tintptr_t ptr;')
	tmp.push('};')
	tmp.push('')
	return tmp
}

const fs = require('fs')
const path = require('path')

fs.readFile(path.join(__dirname, filenames.input), options, (err, content) => {
	if (err) {
		console.log(err)
		process.exit(1)
	} else {
		const out = [...create_header(), ...create_body(content.split(newline)), ...create_footer()]
		
		fs.writeFile(path.join(__dirname, filenames.output), out.join(newline), options, (err) => {
			if (err) {
				console.log(err)
				process.exit(1)
			}
			console.log('Done!')
		})
	}
})
