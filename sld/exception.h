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
	
} /// namespace except

_SLD_END

#endif /// _SLD_EXCEPTION_H
