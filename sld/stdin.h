// ---------------------------------
//  standard instream functionality
// ---------------------------------

#pragma once

#ifndef _SLD_STDIN_H
#define _SLD_STDIN_H

#include <sld/core.h>
#include <sld/types.h>
#include <sld/exception.h>

// system header (not any C/C++ std header)
// required for (system call) 'read()' implementation
#include <unistd.h>

_SLD_BEGIN

enum file_in_no : uint8 {
	stdin = 1
};

inline char& _previous_read_character(const _SLD file_in_no _fd = _SLD stdin) {
	static char ch = '\0';
	return ch;
}

template <_SLD file_in_no FD = _SLD stdin> inline void _null_read_until_newline() {
	if (_SLD _previous_read_character(FD) == '\0' || _SLD _previous_read_character(FD) == '\n') {
		return;
	}
	char ch;
	do {
		read(FD, &ch, 1);
		_SLD _previous_read_character(FD) = ch;
	} while (ch != '\0' && ch != '\n');
}

inline char getchar(const _SLD file_in_no _fd = _SLD stdin) {
	if (_fd == _SLD stdin) {
		static bool is_atexit = false;
		if (!is_atexit) {
			_SLD atexit(_SLD _null_read_until_newline <_SLD stdin>);
			is_atexit = true;
		}
		char ch;
		read(_SLD stdin, &ch, 1);
		_SLD _previous_read_character(_fd) = ch;
		return ch;
	} else {
		throw _SLD except::unsupported_file_desciptor();
	}
}

inline void getchars(char* _buffer, const _SLD file_in_no _fd = _SLD stdin) {
	_SLD size_t i = 0;
	char ch;
	do {
		_buffer[i++] = ch = _SLD getchar(_fd);
	} while(ch != ' ' && ch != '\n' && ch != '\0' && ch != '\32');
	_buffer[i - 1] = '\0';
}

template <class A = int32> inline A getint(const _SLD size_t _base = 10, const _SLD file_in_no _fd = _SLD stdin) {
	if (_base > (_SLD size_t)16) {
		throw _SLD except::unsupported_number_base();
	}
	char ch = getchar(_fd);
	bool neg = false;
	if (ch == '-') {
		ch = getchar(_fd);
		neg = true;
	}
	if (ch != '-' && (ch < '0' || ch > '9')) {
		return A(0);
	}
	A result = A(0);
	for (; ch >= '0' && ch <= '9'; ch = getchar(_fd)) {
		if (ch < '0' || ch > '9') {
			result = result * A(10) + A(ch - 'a');
		} else {
			result = result * A(10) + A(ch - '0');
		}
	}
	while (ch != ' ' && ch != '\n' && ch != '\0' && ch != '\32') {
		ch = _SLD getchar();
	}
	return neg ? -result : result;
}

template <class A = double8> inline A getfloat(const _SLD file_in_no _fd = _SLD stdin) {
	char ch = getchar(_fd);
	bool neg = false;
	if (ch == '-') {
		ch = getchar(_fd);
		neg = true;
	}
	if (ch != '-' && ch != '.' && (ch < '0' || ch > '9')) {
		return A(0);
	}
	A result = A(0);
	for (; ch >= '0' && ch <= '9'; ch = getchar(_fd)) {
		result = result * A(10) + A(ch - '0');
	}
	if (ch == '.') {
		A after_exp = A(1);
		for (ch = getchar(_fd); ch >= '0' && ch <= '9'; ch = getchar(_fd)) {
			result += A(1) / (after_exp *= A(10)) * A(ch - '0');
		}
	}
	while (ch != ' ' && ch != '\n' && ch != '\0' && ch != '\32') {
		ch = _SLD getchar();
	}
	return neg ? -result : result;
}

void scanf(const char* _format) { }

template <class ARG, class... ARGS>
void scanf(const char* _format, ARG* _arg, ARGS*... _args) {
	for (_SLD size_t i = 0; _format[i] != '\0'; i++) {
		if (_format[i] != '%') {
			_SLD getchar();
			continue;
		}
		if (_format[++i] == '\0') {
			break;
		}
		switch (_format[i]) {
			case 'c':
				*_arg = _SLD getchar();
				_SLD scanf(_format + ++i, _args...);
				return;
			case 'd':
				*_arg = _SLD getint <ARG> ();
				_SLD scanf(_format + ++i, _args...);
				return;
			case 's':
				_SLD getint(*_arg);
				_SLD scanf(_format + ++i, _args...);
				return;
			case 'o':
				*_arg = _SLD getint <ARG> (8);
				_SLD scanf(_format + ++i, _args...);
				return;
			case 'x':
				*_arg = _SLD getint <ARG> (16);
				_SLD scanf(_format + ++i, _args...);
				return;
			case 'f':
				*_arg = _SLD getfloat <ARG> ();
				_SLD scanf(_format + ++i, _args...);
				return;
			default:
				_SLD getchar();
				break;
		};
	}
}

_SLD_END

#endif /// _SLD_STDIN_H
