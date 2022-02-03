// -------------------------
//  dynamically sized array
// -------------------------

#pragma once

#ifndef _SLD_VECTOR_H
#define _SLD_VECTOR_H

#include <sld/core.h>
#include <sld/types.h>
#include <sld/memory.h>
#include <sld/exception.h>
#include <sld/algorithm.h>

_SLD_BEGIN

template <class A> class vector {
	
public:
	
	typedef A* iterator;
	
	constexpr vector() noexcept :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{ }
	
	vector(uint64 _size) :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{
		this->resize(_size);
	}
	
	vector(uint64 _size, const A& _initial) :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{
		this->resize(_size, _initial);
	}
	
	vector(const vector <A>& _vector) noexcept :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
	{
		this->assign(_vector);
	}
	
	~vector() {
		this->clear();
	}
	
	inline void clear() noexcept {
		if (this->m_data) {
			delete [] (this->m_data);
			this->m_data = nullptr;
			this->m_size = this->m_capacity = 0;
		}
	}
	
	inline void reserve(uint64 _capacity) noexcept {
		if (_capacity <= this->capacity()) {
			return;
		}
		uint64 new_capacity = _SLD max(uint64(1), this->capacity());
		while (new_capacity < _capacity) {
			new_capacity <<= 1;
		}
		A* new_data = new A[new_capacity];
		_SLD memcpy(new_data, this->m_data, sizeof(A) * this->size());
		delete [] (this->m_data);
		this->m_data = new_data;
		this->m_capacity = new_capacity;
	}
	
	inline void resize(uint64 _size) {
		if (_size < this->size()) {
			while (_size < this->size()) {
				this->pop_back();
			}
			return;
		}
		if (_size <= this->capacity()) {
			this->m_size = _size;
			return;
		}
		this->reserve(_size);
		this->m_size = _size;
	}
	
	inline void resize(uint64 _size, const A& _initial) {
		if (_size < this->size()) {
			this->resize(_size);
			return;
		}
		uint64 old_size = this->size();
		this->resize(_size);
		_SLD fill(this->begin() + old_size, this->end(), _initial);
	}
	
	inline void push_back(const A& _value) noexcept {
		this->reserve(this->size() + 1);
		this->m_data[this->m_size++] = _value;
	}
	
	inline void pop_back() {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		(&this->m_data[--this->m_size])->~A();
	}
	
	inline void insert(_SLD vector <A>::iterator _ptr, const A& _value) {
		uint64 index_before_resize = _ptr - this->begin();
		this->resize(this->size() + 1);
		_ptr = this->begin() + index_before_resize;
		for (_SLD vector <A>::iterator it = this->end(); it != _ptr + 1; --it) {
			*(it - 1) = *(it - 2);
		}
		*_ptr = _value;
	}
	
	inline void insert(_SLD vector <A>::iterator _ptr,
	const _SLD vector <A>::iterator _begin, const _SLD vector <A>::iterator _end) {
		uint64 required_size = _end - _begin;
		uint64 index_before_resize = _ptr - this->begin();
		this->resize(this->size() + required_size);
		_ptr = this->begin() + index_before_resize;
		for (_SLD vector <A>::iterator it = this->end(); it != _ptr + required_size; --it) {
			*(it - 1) = *(it - required_size - 1);
		}
		_SLD memcpy(_ptr, _begin, sizeof(A) * required_size);
	}
	
	inline void erase(_SLD vector <A>::iterator _ptr) {
		if (_ptr < this->begin() || _ptr >= this->end()) {
			this->m_throw_bounds();
		}
		for (_SLD vector <A>::iterator it = _ptr; it + 1 != this->end(); ++it) {
			*it = *(it + 1);
		}
		this->pop_back();
	}
	
	inline void erase(_SLD vector <A>::iterator _begin, _SLD vector <A>::iterator _end) {
		if (_begin < this->begin() || _end > this->end() || _begin > _end) {
			this->m_throw_bounds();
		}
		uint64 required_size = _end - _begin;
		for (_SLD vector <A>::iterator it = _end; it != this->end(); ++it) {
			*(it - required_size) = *it;
		}
		for (uint64 i = 0; i < required_size; i++) {
			this->pop_back();
		}
	}
	
	inline void assign(const _SLD vector <A>& _vector) noexcept {
		this->clear();
		this->m_size = _vector.size();
		this->m_capacity = _vector.capacity();
		this->m_data = new A[this->capacity()];
		_SLD memcpy(this->m_data, _vector.begin(), sizeof(A) * _vector.size());
	}
	
	inline _SLD vector <A>& operator = (const _SLD vector <A>& _vector) noexcept {
		this->assign(_vector);
		return *this;
	}
	
	constexpr const A& operator [] (const uint64 _index) const noexcept {
		return this->m_data[_index];
	}
	
	inline A& operator [] (const uint64 _index) noexcept {
		return this->m_data[_index];
	}
	
	constexpr const A& at(const uint64 _index) const {
		if (_index >= this->size()) {
			this->m_throw_bounds();
		}
		return this->m_data[_index];
	}
	
	inline A& at(const uint64 _index) {
		if (_index >= this->size()) {
			this->m_throw_bounds();
		}
		return this->m_data[_index];
	}
	
	constexpr const A& front() const {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[0];
	}
	
	inline A& front() {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[0];
	}
	
	constexpr const A& back() const {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[this->size() - 1];
	}
	
	inline A& back() {
		if (this->empty()) {
			this->m_throw_bounds();
		}
		return this->m_data[this->size() - 1];
	}
	
	constexpr const _SLD vector <A>::iterator begin() const noexcept {
		return this->m_data;
	}
	
	inline _SLD vector <A>::iterator begin() noexcept {
		return this->m_data;
	}
	
	constexpr const _SLD vector <A>::iterator end() const noexcept {
		return this->m_data + this->size();
	}
	
	inline _SLD vector <A>::iterator end() noexcept {
		return this->m_data + this->size();
	}
	
	constexpr uint64 size() const noexcept {
		return this->m_size;
	}
	
	constexpr bool empty() const noexcept {
		return !(bool)this->m_size;
	}
	
	constexpr uint64 capacity() const noexcept {
		return this->m_capacity;
	}
	
private:
	
	A* m_data;
	
	uint64 m_size;
	uint64 m_capacity;
	
private:
	
	constexpr bool m_fits(const uint64 _new_size) const noexcept {
		return _new_size < this->capacity();
	}
	
private:
	
	constexpr void m_throw_bounds() {
		throw _SLD except::out_of_bounds("out of bounds sld::vector subscript");
	}
	
};

_SLD_END

#endif /// _SLD_VECTOR_H
