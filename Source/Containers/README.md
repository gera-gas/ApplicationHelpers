## Containers
Helpers classes with implementation of various data containers.

## Usage
Have a three basic container classes.
* JavaArray -- Linear array implementation to small similar on Java array.
* MatrixArray -- 2D array based on JavaArray.
* StringArray -- String array implementation.

#### JavaArray
```cpp
helper::JavaArray<int> m(5);

std::cout << "Array(new): ";
for (int i = 0; i < m.length; i++) {
	std::cout << m[i];
}
std::cout << std::endl;

for (int i = 0; i < m.length; i++) {
	m[i] = i + 1;
}

std::cout << "Array: ";
for (int e : m) {
	std::cout << e;
}
std::cout << std::endl;
```

#### MatrixArray
```cpp
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

for (helper::JavaArray<int> ary : m) {
	std::cout << "m[]: ";
	for (int v : ary) {
		std::cout << v;
	}
	std::cout << std::endl;
}
```

#### StringArray
```cpp
helper::StringArray sa;

sa << "first";
sa << "second";
sa << "last";

for (std::string s : sa) {
	std::cout << s.c_str() << std::endl;
}
```
