#ifndef LEAK_DETECTOR_LEAK_DETECTOR_H
#define LEAK_DETECTOR_LEAK_DETECTOR_H

#include <vector>
#include <mutex>
#include "System/PlatformDefines.h"
#include "Logger/Logger.h"

namespace helper {

	class SuspiciousObjectsList;
	extern SuspiciousObjectsList globalSuspiciousObjectsList;

	/**
	 * List with all created objects of owner class;
	 */
	class LeakedObjectsList {
	public:
		LeakedObjectsList() : m_locker()
		{ }

		~LeakedObjectsList()
		{
			if (m_objectsList.size() > 0)
			{
				indicateLeakedObjects(globalSuspiciousObjectsList);

				ASSERT_FALSE;
			}
		}

		void append(const void *obj);
		void remove(const void *obj);

		void indicateLeakedObjects(helper::SuspiciousObjectsList& suspiciousObjectsList);
		void indicateDanglingObjects(helper::SuspiciousObjectsList& suspiciousObjectsList, const void *danglingObject);

	private:
		std::mutex m_locker;
		std::vector<const void*> m_objectsList;
	};

	/**
	* @brief
	* Observations objects list.
	* Add to list custom object with information about source code.
	* For add object to list necessary using the macros: ADD_TO_OBJECT_DETECTION_LIST.
	*/
	class SuspiciousObjectsList {
	public:
		struct ObjectInfo {
		public:
			bool leaked;
			const void *address;
			const char *name;
			const char *file;
			const char *method;
			size_t linenum;

			ObjectInfo(const void *addr, const char *n, const char *f, const char *m, size_t l) :
				leaked(false),
				address(addr),
				name(n),
				file(f),
				method(m),
				linenum(l)
			{ }
		};

		SuspiciousObjectsList() : m_locker()
		{ }

		~SuspiciousObjectsList()
		{
			clear();
		}

		void append(const void *address, const char *name, const char *file, const char *method, size_t linenum)
		{
			m_locker.lock();

			ObjectInfo objectInfo(address, name, file, method, linenum);
			m_objectInfoList.push_back(objectInfo);

			m_locker.unlock();
		}

		void clear(void)
		{
			m_locker.lock();

			if (m_objectInfoList.size() > 0) {
				m_objectInfoList.clear();
			}

			m_locker.unlock();
		}

		static SuspiciousObjectsList& getInstance()
		{
			static SuspiciousObjectsList suspiciousObjectsList;
			return suspiciousObjectsList;
		}

	private:
		friend class helper::LeakedObjectsList;
		std::mutex m_locker;
		std::vector<ObjectInfo> m_objectInfoList;
	};


#if HELPER_PLATROM_DEBUG

#define ADD_TO_SUSPICIOUS_OBJECTS_LIST( obj ) \
	helper::globalSuspiciousObjectsList.append( reinterpret_cast<const void*>(obj), #obj, __FILE__, __FUNCTION__, __LINE__ )

#define ADD_TO_LEAKED_OBJECTS_LIST \
    m_leakDetector(reinterpret_cast<void*>(this))

#define REMOVE_FROM_LEAKED_OBJECTS_LIST \
    m_leakDetector.removeObjectsFromList(reinterpret_cast<void*>(this))

#else
#define ADD_TO_SUSPICIOUS_OBJECT_LIST( obj )
#define ADD_TO_LEAKED_OBJECTS_LIST m_leakDetector(0)
#define REMOVE_FROM_LEAKED_OBJECTS_LIST
#endif

	/**
	* @brief
	* Class for embedding to other class, that should be controlled on the leaks.
	*/
	template <class OwnerClass>
	class LeakedObjectDetector
	{
	public:
		LeakedObjectDetector(void *obj) noexcept
		{
			getObjectsList().append(obj);
		}

		void removeObjectsFromList(void *obj) noexcept
		{
			getObjectsList().remove(obj);
		}

		~LeakedObjectDetector()
		{
			/*
			* call ~LeakOwnerList() when all owners objects will removed.
			*/
		}

	private:

		static LeakedObjectsList& getObjectsList() noexcept
		{
			static LeakedObjectsList objectsList;
			return objectsList;
		}
	};

#if HELPER_PLATROM_DEBUG
	/** Example of using LeakDetector macroses.
	@code
	class ExampleClass {
	public:
	ExampleClass() : ADD_TO_LEAKED_OBJECTS_LIST
	{ }

	~ExampleClass()
	{
		REMOVE_FROM_LEAKED_OBJECTS_LIST;
	}

	private:
		LEAK_DETECTOR(ExampleClass);
	};

	int main()
	{
		ExampleClass normalObject;
		ExampleClass leakedObject = new ExampleClass();

		ADD_TO_SUSPICIOUS_OBJECTS_LIST(&normalObject);
		ADD_TO_SUSPICIOUS_OBJECTS_LIST(leakedObject);
	}
	@endcode
	*/
#define LEAK_DETECTOR(OwnerClass) \
        friend class helper::LeakedObjectDetector<OwnerClass>; \
		helper::LeakedObjectDetector<OwnerClass> m_leakDetector;
#else
#define LEAK_DETECTOR(OwnerClass) \
        int m_leakDetector;
#endif

} /* helper */

#endif /* LEAK_DETECTOR_LEAK_DETECTOR_H */
