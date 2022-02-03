// -------------------------------------------------------------------------------
//  push function pointers to a last-in-first-out stack - unwound on program exit
// -------------------------------------------------------------------------------

#pragma once

#ifndef _SLD_ALGORITHM_H
#define _SLD_ALGORITHM_H

#include <sld/core.h>
#include <sld/types.h>

#ifndef _SLD_ATEXIT_BUFFER_SIZE
#define _SLD_ATEXIT_BUFFER_SIZE 32
#endif

_SLD_BEGIN

class _sld_atexit_stack {
	
public:
	
	static inline _sld_atexit_stack& instance() {
		static _SLD _sld_atexit_stack inst;
		return inst;
	}
	
	inline bool push(void (*_function) (void)) noexcept {
		if (m_ptr == _SLD_ATEXIT_BUFFER_SIZE) {
			return false;
		}
		m_functions[m_ptr++] = _function;
		return true;
	}
	
	_sld_atexit_stack(_SLD _sld_atexit_stack const&) = delete;
	void operator = (_sld_atexit_stack const&) = delete;
	
private:
	
	_sld_atexit_stack() { }
	
	~_sld_atexit_stack() {
		for (; m_ptr; m_ptr--) {
			m_functions[m_ptr - 1]();
		}
	}
	
	static inline void (*m_functions[_SLD_ATEXIT_BUFFER_SIZE]) (void);
	static inline uint64 m_ptr = 0;
	
};

inline bool atexit(void (*_function) (void)) noexcept {
	return _SLD _sld_atexit_stack::instance().push(_function);
}

_SLD_END

#endif /// _SLD_ALGORITHM_H
