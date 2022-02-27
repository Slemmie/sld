// ----------------------------------
//  standard outstream functionality
// ----------------------------------

#pragma once

#ifndef _SLD_STDOUT_H
#define _SLD_STDOUT_H

#include <sld/core.h>
#include <sld/types.h>
#include <sld/exception.h>
#include <sld/atexit.h>

// system header (not any C/C++ std header)
// required for (system call) 'write()' implementation
#include <unistd.h>

#ifndef _SLD_STDOUT_BUFFER_SIZE
#define _SLD_STDOUT_BUFFER_SIZE 512
#endif

#ifndef _SLD_STDERR_BUFFER_SIZE
#define _SLD_STDERR_BUFFER_SIZE 512
#endif

_SLD_BEGIN

struct _sld_stdout_buffer {
	
	static inline char buffer[_SLD_STDOUT_BUFFER_SIZE];
	static inline _SLD size_t ptr = 0;
	
	_sld_stdout_buffer() = delete;
	
};

struct _sld_stderr_buffer {
	
	static inline char buffer[_SLD_STDERR_BUFFER_SIZE];
	static inline _SLD size_t ptr = 0;
	
	_sld_stderr_buffer() = delete;
	
};

enum file_out_no : uint8 {
	stdout = 1,
	stderr = 2
};

inline void flush_stdout() noexcept {
	if (_SLD _sld_stdout_buffer::ptr) {
		write(_SLD stdout, &_SLD _sld_stdout_buffer::buffer,
		sizeof(char) * _SLD _sld_stdout_buffer::ptr);
		_SLD _sld_stdout_buffer::ptr = 0;
	}
}

inline void flush_stderr() noexcept {
	if (_SLD _sld_stderr_buffer::ptr) {
		write(_SLD stderr, &_SLD _sld_stderr_buffer::buffer,
		sizeof(char) * _SLD _sld_stderr_buffer::ptr);
		_SLD _sld_stderr_buffer::ptr = 0;
	}
}

inline void putchar(const char _ch, const _SLD file_out_no _fd = _SLD stdout) {
	if (_fd == _SLD stdout) {
		static bool is_atexit = false;
		if (!is_atexit) {
			_SLD atexit(_SLD flush_stdout);
			is_atexit = true;
		}
		if (_SLD _sld_stdout_buffer::ptr >= _SLD_STDOUT_BUFFER_SIZE) {
			throw _SLD except::stdout_buffer_pointer_overflow();
		}
		_SLD _sld_stdout_buffer::buffer[_SLD _sld_stdout_buffer::ptr++] = _ch;
		if (_SLD _sld_stdout_buffer::ptr == _SLD_STDOUT_BUFFER_SIZE) {
			_SLD flush_stdout();
		}
	} else if (_fd == _SLD stderr) {
		static bool is_atexit = false;
		if (!is_atexit) {
			_SLD atexit(_SLD flush_stderr);
			is_atexit = true;
		}
		if (_SLD _sld_stderr_buffer::ptr >= _SLD_STDOUT_BUFFER_SIZE) {
			throw _SLD except::stderr_buffer_pointer_overflow();
		}
		_SLD _sld_stderr_buffer::buffer[_SLD _sld_stderr_buffer::ptr++] = _ch;
		if (_SLD _sld_stderr_buffer::ptr == _SLD_STDOUT_BUFFER_SIZE) {
			_SLD flush_stdout();
		}
	} else {
		throw _SLD except::unsupported_file_desciptor();
	}
}

inline void putchars(const char* _chs, const _SLD file_out_no _fd = _SLD stdout) {
	for (_SLD size_t i = 0; _chs[i] != '\0'; i++) {
		_SLD putchar(_chs[i], _fd);
	}
}

void putint(int64 _arg, const _SLD size_t _base = 10, const _SLD file_out_no _fd = _SLD stdout) {
	if (_base > (_SLD size_t)16) {
		throw _SLD except::unsupported_number_base();
	}
	static char representation[] = "0123456789abcdef";
	static char buffer[256];
	char* ptr = &buffer[255];
	*(ptr--) = '\0';
	bool neg = _arg < int64(0);
	_arg = _arg < int64(0) ? -_arg : _arg;
	do {
		if (ptr == buffer) {
			throw _SLD except::stdout_putint_overflow();
		}
		*(ptr--) = representation[_arg % _base];
		_arg /= _base;
	} while (_arg);
	if (neg) {
		_SLD putchar('-', _fd);
	}
	_SLD putchars(ptr + 1, _fd);
}

void putfloat(double12 _arg, const uint64 _precision = 3, const _SLD file_out_no _fd = _SLD stdout) {
	if (_arg < double12(0)) {
		_SLD putchar('-', _fd);
		_arg = -_arg;
	}
	_SLD size_t digs_before = 0;
	double12 before_exp = double12(1);
	double12 tmp = _arg;
	do {
		digs_before++;
		before_exp *= double12(10);
		tmp /= double12(10);
	} while (tmp >= double12(1));
	while (digs_before) {
		int32 current = int32(_arg / (before_exp /= double12(10))) % 10;
		_SLD putchar('0' + current, _fd);
		digs_before--;
	}
	tmp = _arg;
	if (_precision) {
		_SLD putchar('.', _fd);
		for (uint64 i = 0; i < _precision; i++) {
			tmp *= double12(10);
			uint32 current = uint32(tmp) % 10;
			_SLD putchar('0' + current, _fd);
		}
	}
}

void printf(const char* _format) {
	_SLD putchars(_format);
}

template <class... ARGS> inline void _sld_putchar(const ARGS&... _args);
template <> inline void _sld_putchar <signed char  >
(const signed char& _arg) { _SLD putchar((char)_arg); }
template <> inline void _sld_putchar <unsigned char>
(const unsigned char& _arg) { _SLD putchar((char)_arg); }
template <> inline void _sld_putchar <char         >
(const char& _arg) { _SLD putchar((char)_arg); }
template <class... ARGS> inline void _sld_putchar
(const ARGS&... _args) { throw _SLD except::invalid_put_args(); }

template <class... ARGS> inline void _sld_putchars(ARGS... _args);
template <> inline void _sld_putchars <const char*>
(const char* _arg) { _SLD putchars(_arg); }
template <> inline void _sld_putchars <char*>
(char* _arg) { _SLD putchars(_arg); }
template <class... ARGS> inline void _sld_putchars
(ARGS... _args) { throw _SLD except::invalid_put_args(); }

#define _sld_overload(TYPE)                                \
template <> inline void _sld_putint <TYPE>                 \
(const TYPE& _arg) { _SLD putint(_arg, (_SLD size_t)10); } \
template <> inline void _sld_putint <TYPE, _SLD size_t>    \
(const TYPE& _arg, const _SLD size_t& _base) { _SLD putint(_arg, _base); }
template <class... ARGS> inline void _sld_putint(const ARGS&... _args);
_sld_overload( uint8)
_sld_overload(  int8)
_sld_overload(uint16)
_sld_overload( int16)
_sld_overload(uint32)
_sld_overload( int32)
_sld_overload(uint64)
_sld_overload( int64)
_sld_overload(  char)
_sld_overload(  long)
_sld_overload( double4)
_sld_overload( double8)
_sld_overload(double12)
template <class... ARGS> inline void _sld_putint
(const ARGS&... _args) { throw _SLD except::invalid_put_args(); }
#undef _sld_overload

template <class... ARGS> inline void _sld_putfloat(const ARGS&... _args);
template <> inline void _sld_putfloat <double12, uint64>
(const double12& _arg, const uint64& _precision) { _SLD putfloat(_arg, _precision); }
template <> inline void _sld_putfloat <double8, uint64>
(const double8& _arg, const uint64& _precision) { _SLD putfloat(_arg, _precision); }
template <> inline void _sld_putfloat <double4, uint64>
(const double4& _arg, const uint64& _precision) { _SLD putfloat(_arg, _precision); }
template <class... ARGS> inline void _sld_putfloat
(const ARGS&... _args) { throw _SLD except::invalid_put_args(); }

template <class ARG, class... ARGS>
void printf(const char* _format, const ARG& _arg, const ARGS&... _args) {
	for (_SLD size_t i = 0; _format[i] != '\0'; i++) {
		if (_format[i] != '%') {
			_sld_putchar(_format[i]);
			continue;
		}
		if (_format[++i] == '\0') {
			break;
		}
		switch (_format[i]) {
			case 'c':
				_SLD _sld_putchar(_arg);
				_SLD printf(_format + ++i, _args...);
				return;
			case 'd':
				_SLD _sld_putint(_arg);
				_SLD printf(_format + ++i, _args...);
				return;
			case 's':
				_SLD _sld_putchars(_arg);
				_SLD printf(_format + ++i, _args...);
				return;
			case 'o':
				_SLD _sld_putint(_arg, (_SLD size_t)8);
				_SLD printf(_format + ++i, _args...);
				return;
			case 'x':
				_SLD _sld_putint(_arg, (_SLD size_t)16);
				_SLD printf(_format + ++i, _args...);
				return;
			case '%':
				_SLD _sld_putchar('%');
				_SLD printf(_format + ++i, _args...);
				return;
			default: {
				if (_format[i] != '.' && _format[i] != 'f' &&
				(_format[i] < '0' || _format[i] > '9')) {
					_SLD _sld_putchar(_format[i]);
					break;
				}
				uint64 precision = 0;
				while (_format[i] != '\0' && _format[i] != 'f') {
					if (_format[i] == '.') {
						precision = 0;
					} else if (_format[i] >= '0' && _format[i] <= '9') {
						precision = precision * 10 + (_format[i] - '0');
					}
					i++;
				}
				if (_format[i] == '\0') {
					return;
				}
				_SLD _sld_putfloat(_arg, precision ? precision : (uint64)3);
				_SLD printf(_format + ++i, _args...);
				return;
			}
		};
	}
}

void printf(const _SLD file_out_no _fd, const char* _format) {
	_SLD putchars(_format, _fd);
}

template <class... ARGS> inline void _sld_putchar(const _SLD file_out_no _fd, const ARGS&... _args);
template <> inline void _sld_putchar <signed char  >
(const _SLD file_out_no _fd, const signed char& _arg) { _SLD putchar((char)_arg, _fd); }
template <> inline void _sld_putchar <unsigned char>
(const _SLD file_out_no _fd, const unsigned char& _arg) { _SLD putchar((char)_arg, _fd); }
template <> inline void _sld_putchar <char         >
(const _SLD file_out_no _fd, const char& _arg) { _SLD putchar((char)_arg, _fd); }
template <class... ARGS> inline void _sld_putchar
(const _SLD file_out_no _fd, const ARGS&... _args) { throw _SLD except::invalid_put_args(); }

template <class... ARGS> inline void _sld_putchars(const _SLD file_out_no _fd, ARGS... _args);
template <> inline void _sld_putchars <const char*>
(const _SLD file_out_no _fd, const char* _arg) { _SLD putchars(_arg, _fd); }
template <class... ARGS> inline void _sld_putchars
(const _SLD file_out_no _fd, ARGS... _args) { throw _SLD except::invalid_put_args(); }

#define _sld_overload(TYPE)                           \
template <> inline void _sld_putint <TYPE>            \
(const _SLD file_out_no _fd, const TYPE& _arg) { _SLD putint(_arg, (_SLD size_t)10, _fd); } \
template <> inline void _sld_putint <TYPE, _SLD size_t>    \
(const _SLD file_out_no _fd, const TYPE& _arg, const _SLD size_t& _base) { _SLD putint(_arg, _base, _fd); }
template <class... ARGS> inline void _sld_putint(const _SLD file_out_no _fd, const ARGS&... _args);
_sld_overload( uint8)
_sld_overload(  int8)
_sld_overload(uint16)
_sld_overload( int16)
_sld_overload(uint32)
_sld_overload( int32)
_sld_overload(uint64)
_sld_overload( int64)
_sld_overload( double4)
_sld_overload( double8)
_sld_overload(double12)
template <class... ARGS> inline void _sld_putint
(const _SLD file_out_no _fd, const ARGS&... _args) { throw _SLD except::invalid_put_args(); }
#undef _sld_overload

template <class... ARGS> inline void _sld_putfloat(const _SLD file_out_no _fd, const ARGS&... _args);
template <> inline void _sld_putfloat <double12, uint64>
(const _SLD file_out_no _fd, const double12& _arg, const uint64& _precision)
{ _SLD putfloat(_arg, _precision, _fd); }
template <> inline void _sld_putfloat <double8, uint64>
(const _SLD file_out_no _fd, const double8& _arg, const uint64& _precision)
{ _SLD putfloat(_arg, _precision, _fd); }
template <> inline void _sld_putfloat <double4, uint64>
(const _SLD file_out_no _fd, const double4& _arg, const uint64& _precision)
{ _SLD putfloat(_arg, _precision, _fd); }
template <class... ARGS> inline void _sld_putfloat
(const _SLD file_out_no _fd, const ARGS&... _args) { throw _SLD except::invalid_put_args(); }

template <class ARG, class... ARGS>
void printf(const _SLD file_out_no _fd, const char* _format, const ARG& _arg, const ARGS&... _args) {
	for (_SLD size_t i = 0; _format[i] != '\0'; i++) {
		if (_format[i] != '%') {
			_sld_putchar(_fd, _format[i]);
			continue;
		}
		if (_format[++i] == '\0') {
			break;
		}
		switch (_format[i]) {
			case 'c':
				_SLD _sld_putchar(_fd, _arg);
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			case 'd':
				_SLD _sld_putint(_fd, _arg);
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			case 's':
				_SLD _sld_putchars(_fd, _arg);
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			case 'o':
				_SLD _sld_putint(_fd, _arg, (_SLD size_t)8);
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			case 'x':
				_SLD _sld_putint(_fd, _arg, (_SLD size_t)16);
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			case '%':
				_SLD _sld_putchar(_fd, '%');
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			default: {
				if (_format[i] != '.' && _format[i] != 'f' &&
				(_format[i] < '0' || _format[i] > '9')) {
					_SLD _sld_putchar(_fd, _format[i]);
					break;
				}
				uint64 precision = 0;
				while (_format[i] != '\0' && _format[i] != 'f') {
					if (_format[i] == '.') {
						precision = 0;
					} else if (_format[i] >= '0' && _format[i] <= '9') {
						precision = precision * 10 + (_format[i] - '0');
					}
					i++;
				}
				if (_format[i] == '\0') {
					return;
				}
				_SLD _sld_putfloat(_fd, _arg, precision ? precision : (uint64)3);
				_SLD printf(_fd, _format + ++i, _args...);
				return;
			}
		};
	}
}

_SLD_END

#endif /// _SLD_STDOUT_H
