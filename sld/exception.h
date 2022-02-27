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
	
#define _SLD_MAKE_EXCEPTION(_class_name, _default_message)                    \
class _class_name : public _SLD exception {                                   \
	                                                                          \
public:                                                                       \
	                                                                          \
	_class_name() noexcept : _SLD exception(_default_message) { }             \
	                                                                          \
	_class_name(const char* _message) noexcept : _SLD exception(_message) { } \
	                                                                          \
};
	
	_SLD_MAKE_EXCEPTION(out_of_bounds, "index out of range")
	
	_SLD_MAKE_EXCEPTION(stdout_buffer_pointer_overflow, "stdout buffer pointer overflow")
	
	_SLD_MAKE_EXCEPTION(stderr_buffer_pointer_overflow, "stderr buffer pointer overflow")
	
	_SLD_MAKE_EXCEPTION(unsupported_number_base, "unsupported number base")
	
	_SLD_MAKE_EXCEPTION(stdout_putint_overflow, "stdout putint() overflow")
	
	_SLD_MAKE_EXCEPTION(invalid_put_args, "stdout putint() overflow")
	
	_SLD_MAKE_EXCEPTION(unsupported_file_desciptor, "unsupported file desciptor")
	
#undef _SLD_MAKE_EXCEPTION
	
} /// namespace except

_SLD_END

#endif /// _SLD_EXCEPTION_H
