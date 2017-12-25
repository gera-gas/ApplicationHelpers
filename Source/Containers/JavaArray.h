#ifndef CONTAINERS_JAVA_ARRAY_H
#define CONTAINERS_JAVA_ARRAY_H

#include "Logger/Logger.h"
#include "LeakDetector/LeakDetector.h"

namespace helper {

	template<typename T>
	class JavaArray {
	public:
		const size_t length;

		JavaArray();
		JavaArray(size_t newLength);
		JavaArray(const helper::JavaArray<T> &other);
		JavaArray(helper::JavaArray<T> &&other);
		~JavaArray();

		void alloc(size_t);
		void release();
		void copyDataFrom(helper::JavaArray<T> *other);
		void copyDataFrom(T *data, size_t len);
		T* get();

		T& operator[](size_t index);
		JavaArray<T>& operator=(const helper::JavaArray<T> &other);
		JavaArray<T>& operator=(helper::JavaArray<T> &&other);

		void fill(T value);
		void fill(T value, size_t fromIndex, size_t toIndex);

		bool isZero();

		inline T* begin()
		{
			return m_pointer;
		}

		inline T* end()
		{
			if (m_pointer == nullptr || length == 0) {
				return m_pointer;
			}

			return m_pointer + length;
		}

	private:
		T *m_pointer;

		LEAK_DETECTOR(JavaArray<T>);
	};

	template<typename T>
	JavaArray<T>::JavaArray() :
		ADD_TO_LEAKED_OBJECTS_LIST,
		length(0),
		m_pointer(nullptr)
	{ }

	template<typename T>
	JavaArray<T>::JavaArray(size_t newLength) :
		ADD_TO_LEAKED_OBJECTS_LIST,
		length(0),
		m_pointer(nullptr)
	{
		alloc(newLength);
	}

	template<typename T>
	JavaArray<T>::~JavaArray()
	{
		release();

		REMOVE_FROM_LEAKED_OBJECTS_LIST;
	}

	template<typename T>
	void JavaArray<T>::release()
	{
		if (m_pointer != nullptr)
		{
			delete[] m_pointer;
			m_pointer = nullptr;

			*((size_t *)(&length)) = 0;
		}
	}

	template<typename T>
	void JavaArray<T>::alloc(size_t newLength)
	{
		if (newLength > 0) {
			if (m_pointer != nullptr) {
				release();
			}

			m_pointer = new T[newLength]{ 0 };
			*((size_t *)(&length)) = newLength;
		}
	}

	template<typename T>
	void JavaArray<T>::copyDataFrom(T *data, size_t len)
	{
		if (data != nullptr && len > 0) {
			alloc(len);

			for (size_t i = 0; i < len; i++) {
				m_pointer[i] = data[i];
			}
		}
		else {
			release();
		}
	}

	template<typename T>
	void JavaArray<T>::copyDataFrom(helper::JavaArray<T> *other)
	{
		copyDataFrom(other->get(), other->length);
	}

	template<typename T>
	JavaArray<T>::JavaArray(const helper::JavaArray<T> &other) :
		ADD_TO_LEAKED_OBJECTS_LIST,
		length(0),
		m_pointer(nullptr)
	{
		copyDataFrom((JavaArray<T>*)&other);
	}

	template<typename T>
	JavaArray<T>& JavaArray<T>::operator=(const helper::JavaArray<T> &other)
	{
		if (&other != this) {
			copyDataFrom((JavaArray<T>*)&other);
		}

		return *this;
	}

	template<typename T>
	JavaArray<T>::JavaArray(helper::JavaArray<T> &&other) :
		ADD_TO_LEAKED_OBJECTS_LIST,
		length(0),
		m_pointer(nullptr)
	{
		if (other.length > 0) {
			m_pointer = other.m_pointer;
			*((size_t *)(&length)) = other.length;
		}

		*((size_t *)(&other.length)) = 0;
		other.m_pointer = nullptr;
	}

	template<typename T>
	JavaArray<T>& JavaArray<T>::operator=(helper::JavaArray<T> &&other)
	{
		if (&other != this) {
			release();

			if (other.length > 0) {
				m_pointer = other.m_pointer;
				*((size_t *)(&length)) = other.length;
			}

			*((size_t *)(&other.length)) = 0;
			other.m_pointer = nullptr;
		}

		return *this;
	}

	template<typename T>
	T* JavaArray<T>::get()
	{
		return m_pointer;
	}

	template<typename T>
	T& JavaArray<T>::operator[](size_t index)
	{
		ASSERT(index < length);
		
		return m_pointer[index];
	}

	template<typename T>
	void JavaArray<T>::fill(T value)
	{
		for (size_t i = 0; i < length; i++)
		{
			m_pointer[i] = value;
		}
	}

	template<typename T>
	void JavaArray<T>::fill(T value, size_t fromIndex, size_t toIndex)
	{
		assert(fromIndex <= length && toIndex <= length);

		for (size_t i = fromIndex; i < toIndex; i++)
		{
			m_pointer[i] = value;
		}
	}

	template<typename T>
	bool JavaArray<T>::isZero()
	{
		T result = 0;

		for (size_t i = 0; i < length; i++) {
			result += m_pointer[i];
		}

		if (result == 0) {
			return true;
		}

		return false;
	}

} /* helper */

#endif /* CONTAINERS_JAVA_ARRAY_H */
