#ifndef CONTAINERS_STRING_ARRAY_H
#define CONTAINERS_STRING_ARRAY_H

#include "Logger/Logger.h"
#include "LeakDetector/LeakDetector.h"

namespace helper {

	class StringArray {
	public:
		StringArray() { }
		~StringArray() { m_stringArray.clear(); }

		void operator<< (const char *str)
		{
			m_stringArray.push_back(std::string(str));
		}

		void operator<< (std::string& str)
		{
			m_stringArray.push_back(str);
		}

		std::string& operator[] (size_t i)
		{
			ASSERT(i < m_stringArray.size());

			return m_stringArray[i];
		}

		inline std::vector<std::string>::iterator begin()
		{
			return m_stringArray.begin();
		}

		inline std::vector<std::string>::iterator end()
		{
			return m_stringArray.end();
		}

		size_t size() const
		{
			return m_stringArray.size();
		}

		size_t find(const char *str)
		{
			size_t idx = 0;
			size_t result = -1;

			for (auto it : m_stringArray) {
				if (it == str) {
					result = idx;
					break;
				}
				idx++;
			}

			return result;
		}

		size_t find(std::string &str)
		{
			return find(str.c_str());
		}

		void erase(size_t idx)
		{
			if (idx < m_stringArray.size()) {
				m_stringArray.erase(m_stringArray.begin() + idx);
			}
		}

		void erase(std::string& str)
		{
			erase(find(str));
		}

	private:
		std::vector<std::string> m_stringArray;
	};

} /* helper */

#endif /* CONTAINERS_STRING_ARRAY_H */
