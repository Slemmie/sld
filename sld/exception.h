// ------------
//  exceptions
// ------------

#pragma once

#ifndef _SLD_EXCEPTION_H
#define _SLD_EXCEPTION_H

#include <sld/core.h>

_SLD_BEGIN

namespace except {
	
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
	
	class out_of_bounds : public _SLD except::exception {
		
	public:
		
		out_of_bounds() noexcept :
		_SLD except::exception("index out of range")
		{ }
		
		out_of_bounds(const char* _message) noexcept :
		_SLD except::exception(_message)
		{ }
		
	};
	
} /// namespace except

_SLD_END

#endif /// _SLD_EXCEPTION_H
