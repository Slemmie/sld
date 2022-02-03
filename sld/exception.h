// ------------
//  exceptions
// ------------

#pragma once

#ifndef _SLD_EXCEPTION_H
#define _SLD_EXCEPTION_H

#include <sld/core.h>

_SLD_BEGIN

class exception {
	
public:
	
	const char* what() const noexcept {
		return this->m_what;
	}
	
protected:
	
	exception() = delete;
	
	exception(const char* _message) noexcept :
	m_what(_message)
	{ }
	
	const char* m_what;
	
};

namespace except {
	
	class out_of_bounds : public _SLD exception {
		
	public:
		
		out_of_bounds() noexcept :
		_SLD exception("index out of range")
		{ }
		
		out_of_bounds(const char* _message) noexcept :
		_SLD exception(_message)
		{ }
		
	};
	
	class stdout_buffer_pointer_overflow : public _SLD exception {
		
	public:
		
		stdout_buffer_pointer_overflow() noexcept :
		_SLD exception("stdout buffer pointer overflow")
		{ }
		
		stdout_buffer_pointer_overflow(const char* _message) noexcept :
		_SLD exception(_message)
		{ }
		
	};
	
	class unsupported_number_base : public _SLD exception {
		
	public:
		
		unsupported_number_base() noexcept :
		_SLD exception("unsupported number base")
		{ }
		
		unsupported_number_base(const char* _message) noexcept :
		_SLD exception(_message)
		{ }
		
	};
	
	class stdout_putint_overflow : public _SLD exception {
		
	public:
		
		stdout_putint_overflow() noexcept :
		_SLD exception("stdout putint() overflow")
		{ }
		
		stdout_putint_overflow(const char* _message) noexcept :
		_SLD exception(_message)
		{ }
		
	};
	
	class invalid_put_args : public _SLD exception {
		
	public:
		
		invalid_put_args() noexcept :
		_SLD exception("stdout putint() overflow")
		{ }
		
		invalid_put_args(const char* _message) noexcept :
		_SLD exception(_message)
		{ }
		
	};
	
} /// namespace except

_SLD_END

#endif /// _SLD_EXCEPTION_H
