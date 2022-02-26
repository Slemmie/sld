// ---------------------------------
//  contains simple memory material
// ---------------------------------

#pragma once

#ifndef _SLD_MEMORY_H
#define _SLD_MEMORY_H

#include <sld/core.h>
#include <sld/types.h>

_SLD_BEGIN

void memcpy(void* _destination, const void* _source, const uint64 _bytes) noexcept {
	byte* bytes_dest = (byte*)_destination;
	const byte* bytes_source = (byte*)_source;
	for (uint64 i = 0; i < _bytes; i++) {
		bytes_dest[i] = bytes_source[i];
	}
}

void memmove(void* _destination, const void* _source, const uint64 _bytes) noexcept {
	byte* bytes_dest = (byte*)_destination;
	const byte* bytes_source = (byte*)_source;
	byte* tmp_buf = new byte[_bytes];
	for (uint64 i = 0; i < _bytes; i++) {
		tmp_buf[i] = bytes_source[i];
	}
	for (uint64 i = 0; i < _bytes; i++) {
		bytes_dest[i] = tmp_buf[i];
	}
	delete [] (tmp_buf);
}

template <class IT, class A> void fill(IT _begin, IT _end, const A& _value) noexcept {
	for (; _begin != _end; ++_begin) {
		*_begin = _value;
	}
}

void* malloc(uint64 _size) noexcept {
	return (void*) (new char* [_size]);
}

void free(void* _ptr) noexcept {
	delete [] ((char*) _ptr);
	_ptr = nullptr;
}

_SLD_END

#endif /// _SLD_MEMORY_H
