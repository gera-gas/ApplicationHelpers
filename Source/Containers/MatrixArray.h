#ifndef CONTAINERS_MATRIX_ARRAY_H
#define CONTAINERS_MATRIX_ARRAY_H

#include "JavaArray.h"

namespace helper {

	template<typename T>
	class MatrixArray {
	public:
		MatrixArray();
		MatrixArray(size_t numRows, size_t numCols);
		~MatrixArray();

		size_t getNumRows();
		size_t getNumCols();

		void release();
		void alloc(size_t numRows, size_t numCols);
		JavaArray<T>* get();

		T operator()(size_t rowIndex, size_t colIndex);
		void operator()(size_t rowIndex, size_t colIndex, T newValue);

		void copyDataFrom(helper::MatrixArray<T> *other);

		MatrixArray(const helper::MatrixArray<T> &other);
		MatrixArray<T>& operator=(const helper::MatrixArray<T> &other);

		MatrixArray(helper::MatrixArray<T> &&other);
		MatrixArray<T>& operator=(helper::MatrixArray<T> &&other);

		void fill(T value);

		JavaArray<T>& operator[](size_t index);

		inline JavaArray<T>* begin()
		{
			return m_pointer;
		}

		inline JavaArray<T>* end()
		{
			if (m_pointer == nullptr || m_numRows == 0) {
				return m_pointer;
			}

			return m_pointer + m_numRows;
		}

		bool isZero();

	private:
		size_t m_numRows;
		size_t m_numCols;

		JavaArray<T> *m_pointer;

		LEAK_DETECTOR(MatrixArray<T>);
	};

	template<typename T>
	size_t MatrixArray<T>::getNumRows()
	{
		return m_numRows;
	}

	template<typename T>
	size_t MatrixArray<T>::getNumCols()
	{
		return m_numCols;
	}

	template<typename T>
	JavaArray<T>* MatrixArray<T>::get()
	{
		return m_pointer;
	}

	template<typename T>
	MatrixArray<T>::MatrixArray() :
		ADD_TO_LEAKED_OBJECTS_LIST,
		m_numRows(0),
		m_numCols(0),
		m_pointer(nullptr)
	{ }

	template<typename T>
	MatrixArray<T>::MatrixArray(size_t numRows, size_t numCols) :
		ADD_TO_LEAKED_OBJECTS_LIST,
		m_numRows(0),
		m_numCols(0),
		m_pointer(nullptr)
	{
		alloc(numRows, numCols);
	}

	template<typename T>
	MatrixArray<T>::~MatrixArray()
	{
		release();

		REMOVE_FROM_LEAKED_OBJECTS_LIST;
	}

	template<typename T>
	void MatrixArray<T>::release()
	{
		if (m_pointer != nullptr) {
			for (size_t i = 0; i < m_numRows; i++) {
				m_pointer[i].release();
			}
			delete[] m_pointer;
		}
	}

	template<typename T>
	void MatrixArray<T>::alloc(size_t numRows, size_t numCols)
	{
		if (numRows > 0 && numCols > 0) {
			if (m_pointer != nullptr) {
				release();
			}

			m_pointer = new JavaArray<T>[numRows];
			for (size_t i = 0; i < numRows; i++) {
				m_pointer[i].alloc(numCols);
			}

			m_numRows = numRows;
			m_numCols = numCols;
		}
	}

	template<typename T>
	T MatrixArray<T>::operator()(size_t rowIndex, size_t colIndex)
	{
		ASSERT(rowIndex < m_numRows && colIndex < m_numCols);

		return m_pointer[rowIndex][colIndex];
	}

	template<typename T>
	void MatrixArray<T>::operator()(size_t rowIndex, size_t colIndex, T newValue)
	{
		ASSERT(rowIndex < m_numRows && colIndex < m_numCols);

		m_pointer[rowIndex][colIndex] = newValue;
	}

	template<typename T>
	void MatrixArray<T>::copyDataFrom(helper::MatrixArray<T> *other)
	{
		size_t rows = other->getNumRows();

		if (rows > 0) {
			alloc(rows, other->getNumCols());
			for (size_t i = 0; i < rows; i++) {
				m_pointer[i].copyDataFrom(other->m_pointer[i].get(), other->m_pointer[i].length);
			}
		}
		else {
			release();
		}
	}

	template<typename T>
	MatrixArray<T>::MatrixArray(const MatrixArray<T> &other) :
		ADD_TO_LEAKED_OBJECTS_LIST,
		m_numRows(0),
		m_numCols(0),
		m_pointer(nullptr)
	{
		copyDataFrom((MatrixArray<T>*)&other);
	}

	template<typename T>
	MatrixArray<T>& MatrixArray<T>::operator=(const helper::MatrixArray<T> &other)
	{
		if (&other != this) {
			copyDataFrom((MatrixArray<T>*)&other);
		}

		return *this;
	}

	template<typename T>
	MatrixArray<T>::MatrixArray(helper::MatrixArray<T> &&other) :
		ADD_TO_LEAKED_OBJECTS_LIST,
		m_numRows(0),
		m_numCols(0),
		m_pointer(nullptr)
	{
		if (other.m_numRows > 0 && other.m_numCols > 0) {
			m_pointer = other.m_pointer;
			m_numRows = other.m_numRows;
			m_numCols = other.m_numCols;
		}

		other.m_pointer = nullptr;
		other.m_numRows = 0;
		other.m_numCols = 0;
	}

	template<typename T>
	MatrixArray<T>& MatrixArray<T>::operator=(helper::MatrixArray<T> &&other)
	{
		if (&other != this) {
			release();

			if (other.m_numRows > 0 && other.m_numCols > 0) {
				m_pointer = other.m_pointer;
				m_numRows = other.m_numRows;
				m_numCols = other.m_numCols;
			}

			other.m_pointer = nullptr;
			other.m_numRows = 0;
			other.m_numCols = 0;
		}

		return *this;
	}

	template<typename T>
	void MatrixArray<T>::fill(T value)
	{
		for (size_t i = 0; i < m_numRows; i++) {
			m_pointer[i].fill(value);
		}
	}

	template<typename T>
	JavaArray<T>& MatrixArray<T>::operator[](size_t index)
	{
		ASSERT(index < m_numRows);

		return m_pointer[index];
	}

	template<typename T>
	bool MatrixArray<T>::isZero()
	{
		for (size_t i = 0; i < m_numRows; i++) {
			if (!m_pointer[i].isZero()) {
				return false;
			}
		}

		return true;
	}

} /* helper */

#endif /* CONTAINERS_MATRIX_ARRAY_H */
