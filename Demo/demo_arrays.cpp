#include <iostream>
#include "demo_arrays.h"

void java_array_link_test(helper::JavaArray<int> &m) {
	for (int i = 0; i < m.length; i++) {
		m[i] = i + 1;
	}
}

void java_array_pointer_test(helper::JavaArray<int> *m) {
	for (int i = 0; i < m->length; i++) {
		m->operator[](i) = i + 1;
	}
}

helper::MatrixArray<int> move_matrix() {
	helper::MatrixArray<int> m(2, 3);

	for (int i = 0; i < m.getNumRows(); i++) {
		for (int j = 0; j < m.getNumCols(); j++) {
			m(i, j, i + j);
		}
	}

	return m;
}

void test_containers()
{
	// Test java array implementation.
	{
		helper::JavaArray<int> m(5);

		std::cout << "*** Cycle assign ***" << std::endl;

		std::cout << "Array(new): ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;

		for (int i = 0; i < m.length; i++) {
			m[i] = i + 1;
		}

		std::cout << "Array: ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;

		//m[m.length + 1] = 0;
	}

	{
		helper::JavaArray<int> m(5);

		std::cout << std::endl << "*** Function link assign ***" << std::endl;

		std::cout << "Array(new): ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;

		java_array_link_test(m);

		std::cout << "Array: ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;
	}

	{
		helper::JavaArray<int> m(5);

		std::cout << std::endl << "*** Function pointer assign ***"
				<< std::endl;

		std::cout << "Array(new): ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;

		java_array_pointer_test(&m);

		std::cout << "Array: ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** java array alloc ***" << std::endl;

		helper::JavaArray<int> m;

		m.alloc(3);

		std::cout << "Array(new)[0]: ";
		for (int i = 0; i < m.length; i++) {
			std::cout << m[i];
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** java array copy data from ***"
				<< std::endl;

		//helper::JavaArray<int> m1;
		helper::JavaArray<int> m1(2);
		helper::JavaArray<int> m2(3);

		m2.fill(9);

		std::cout << "m1(new): ";
		for (int i = 0; i < m1.length; i++) {
			std::cout << m1[i];
		}
		std::cout << std::endl;
		std::cout << "m2(new): ";
		for (int i = 0; i < m2.length; i++) {
			std::cout << m2[i];
		}
		std::cout << std::endl;

		m1.copyDataFrom(&m2);

		std::cout << "m1(after): ";
		for (int i = 0; i < m1.length; i++) {
			std::cout << m1[i];
		}
		std::cout << std::endl;
		std::cout << "m2(after): ";
		for (int i = 0; i < m2.length; i++) {
			std::cout << m2[i];
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** Matrix array create-delete ***"
				<< std::endl;

		helper::MatrixArray<int> m(2, 3);

		std::cout << "Array(bedore)" << std::endl;

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		std::cout << "Array(after)" << std::endl;

		for (int i = 0; i < m.getNumRows(); i++) {
			for (int j = 0; j < m.getNumCols(); j++) {
				m(i, j, i + j);
			}
		}

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** Matrix copy constructor ***"
				<< std::endl;

		helper::MatrixArray<int> m1(2, 3);

		for (int i = 0; i < m1.getNumRows(); i++) {
			for (int j = 0; j < m1.getNumCols(); j++) {
				m1(i, j, i + j);
			}
		}

		helper::MatrixArray<int> m2(m1);

		for (int i = 0; i < m1.getNumRows(); i++) {
			std::cout << "m1[" << i << "]: ";
			for (int j = 0; j < m1.getNumCols(); j++) {
				std::cout << m1(i, j);
			}
			std::cout << std::endl;
		}
		for (int i = 0; i < m2.getNumRows(); i++) {
			std::cout << "m2[" << i << "]: ";
			for (int j = 0; j < m2.getNumCols(); j++) {
				std::cout << m2(i, j);
			}
			std::cout << std::endl;
		}
	}

	{
		std::cout << std::endl << "*** Matrix copy operator ***" << std::endl;

		helper::MatrixArray<int> m1(2, 3);
		helper::MatrixArray<int> m2(1, 4);

		for (int i = 0; i < m1.getNumRows(); i++) {
			for (int j = 0; j < m1.getNumCols(); j++) {
				m1(i, j, i + j);
			}
		}

		std::cout << "Array(before)" << std::endl;

		for (int i = 0; i < m1.getNumRows(); i++) {
			std::cout << "m1[" << i << "]: ";
			for (int j = 0; j < m1.getNumCols(); j++) {
				std::cout << m1(i, j);
			}
			std::cout << std::endl;
		}
		for (int i = 0; i < m2.getNumRows(); i++) {
			std::cout << "m2[" << i << "]: ";
			for (int j = 0; j < m2.getNumCols(); j++) {
				std::cout << m2(i, j);
			}
			std::cout << std::endl;
		}

		std::cout << "Array(after)" << std::endl;

		m2 = m1;

		for (int i = 0; i < m1.getNumRows(); i++) {
			std::cout << "m1[" << i << "]: ";
			for (int j = 0; j < m1.getNumCols(); j++) {
				std::cout << m1(i, j);
			}
			std::cout << std::endl;
		}
		for (int i = 0; i < m2.getNumRows(); i++) {
			std::cout << "m2[" << i << "]: ";
			for (int j = 0; j < m2.getNumCols(); j++) {
				std::cout << m2(i, j);
			}
			std::cout << std::endl;
		}
	}

	{
		std::cout << std::endl << "*** Matrix move constructor ***"
				<< std::endl;

		helper::MatrixArray<int> m = move_matrix();

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
	}

	{
		std::cout << std::endl << "*** Matrix fill test ***" << std::endl;

		helper::MatrixArray<int> m(2, 3);

		std::cout << "Array(before)" << std::endl;

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}

		std::cout << "Array(after)" << std::endl;

		m.fill(9);

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
	}

	{
		std::cout << std::endl << "*** Matrix get JavaArray ***" << std::endl;

		helper::MatrixArray<int> m(2, 3);
		helper::JavaArray<int> a(5);

		m.fill(9);

		std::cout << "Array(before)" << std::endl;

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
		std::cout << "a[new]: ";
		for (int i = 0; i < a.length; i++) {
			std::cout << a[i];
		}
		std::cout << std::endl;

		std::cout << "Array(after)" << std::endl;

		a = m[0];

		std::cout << "a[from m]: ";
		for (int i = 0; i < a.length; i++) {
			std::cout << a[i];
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** Matrix set JavaArray ***" << std::endl;

		helper::MatrixArray<int> m(2, 3);
		helper::JavaArray<int> a(5);

		a.fill(9);

		std::cout << "Array(before)" << std::endl;

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
		std::cout << "a[new]: ";
		for (int i = 0; i < a.length; i++) {
			std::cout << a[i];
		}
		std::cout << std::endl;

		std::cout << "Array(after)" << std::endl;

		m[0] = a;

		for (int i = 0; i < m.getNumRows(); i++) {
			std::cout << "m[" << i << "]: ";
			for (int j = 0; j < m.getNumCols(); j++) {
				std::cout << m(i, j);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** JavaArray iterator test ***" << std::endl;

		helper::JavaArray<int> a(3);

		std::cout << "a[new]: ";
		for (int v : a) {
			std::cout << v;
		}
		std::cout << std::endl;
		
		for (int& v : a) {
			v = 3;
		}
		std::cout << "a[edit]: ";
		for (int v : a) {
			std::cout << v;
		}
		std::cout << std::endl;
	}

	{
		std::cout << std::endl << "*** MatrixArray iterator test ***" << std::endl;

		helper::MatrixArray<int> m(2, 3);
		
		for (helper::JavaArray<int> ary : m) {
			std::cout << "m[]: ";
			for (int v : ary) {
				std::cout << v;
			}
			std::cout << std::endl;
		}

		for (helper::JavaArray<int>& ary : m) {
			for (int& v : ary) {
				v = 3;
			}
		}

		for (helper::JavaArray<int> ary : m) {
			std::cout << "m[]: ";
			for (int v : ary) {
				std::cout << v;
			}
			std::cout << std::endl;
		}
	}

	{
		std::cout << std::endl << "*** MatrixArray operator [][] test ***" << std::endl;

		helper::MatrixArray<int> m(2, 3);

		for (size_t x = 0; x < m.getNumRows(); x++) {
			std::cout << "m[]: ";
			for (size_t y = 0; y < m.getNumCols(); y++) {
				std::cout << m[x][y];
			}
			std::cout << std::endl;
		}

		for (size_t x = 0; x < m.getNumRows(); x++) {
			for (size_t y = 0; y < m.getNumCols(); y++) {
				m[x][y] = x + y;
			}
		}

		for (size_t x = 0; x < m.getNumRows(); x++) {
			std::cout << "m[]: ";
			for (size_t y = 0; y < m.getNumCols(); y++) {
				std::cout << m[x][y];
			}
			std::cout << std::endl;
		}
	}

	//Test string Array
	{
		std::cout << std::endl << "*** String Array test ***" << std::endl;

		helper::StringArray sa;

		sa << "first";
		sa << "second";
		sa << "last";

		for (std::string s : sa) {
			std::cout << s.c_str() << std::endl;
		}

		sa.erase(1);
		std::cout << "------------------" << std::endl;

		for (size_t i = 0; i < sa.size(); i++) {
			std::cout << sa[i].c_str() << std::endl;
		}
	}

}
