#include <iostream>
#include <sstream>
#include "LeakDetector/LeakDetector.h"

namespace helper {

#pragma init_seg(lib)
	SuspiciousObjectsList globalSuspiciousObjectsList;

	void outputObjectsInfo(const char *prefix, helper::SuspiciousObjectsList::ObjectInfo& obj)
	{
		std::string info(prefix);

		info += "with name: ";
		info += obj.name;
		info += ", file: ";
		info += obj.file;
		info += ", function: ";
		info += obj.method;
		info += ", line: ";
		info += std::to_string(obj.linenum);

		logDebug(info.c_str());
	}


	void LeakedObjectsList::append(const void *obj)
	{
		m_locker.lock();

		m_objectsList.push_back(obj);

		m_locker.unlock();
	}


	void LeakedObjectsList::remove(const void *obj)
	{
		size_t size_before;
		size_t size_after;

		m_locker.lock();

		size_before = m_objectsList.size();
		/* Dangling pointer deletion. */
		if (size_before == 0)
		{
			indicateDanglingObjects(helper::SuspiciousObjectsList::getInstance(), obj);

			ASSERT_FALSE;
		}

		for (auto it = std::begin(m_objectsList); it != std::end(m_objectsList); it++)
		{
			if (*it == obj) {
				m_objectsList.erase(it);
				break;
			}
		}

		size_after = m_objectsList.size();
		/* Dangling pointer deletion. */
		if (size_before == size_after)
		{
			indicateDanglingObjects(helper::SuspiciousObjectsList::getInstance(), obj);

			ASSERT_FALSE;
		}

		m_locker.unlock();
	}


	void LeakedObjectsList::indicateLeakedObjects(helper::SuspiciousObjectsList& suspiciousObjectsList)
	{
		m_locker.lock();
		suspiciousObjectsList.m_locker.lock();
				
		for (auto itObjectAddress = m_objectsList.begin(); itObjectAddress != m_objectsList.end(); itObjectAddress++)
		{
			for (auto itDetectionObject = suspiciousObjectsList.m_objectInfoList.begin();
				itDetectionObject != suspiciousObjectsList.m_objectInfoList.end();
				itDetectionObject++)
			{
				if (itDetectionObject->address == *itObjectAddress) {
					itDetectionObject->leaked = true;
					outputObjectsInfo("Detect leaked object ", *itDetectionObject);
					break;
				}
			}
		}

		suspiciousObjectsList.m_locker.unlock();
		m_locker.unlock();
	}


	void LeakedObjectsList::indicateDanglingObjects(helper::SuspiciousObjectsList& suspiciousObjectsList, const void *danglingObject)
	{
		m_locker.lock();
		suspiciousObjectsList.m_locker.lock();

		for (auto itSuspiciousObject = suspiciousObjectsList.m_objectInfoList.begin();
			itSuspiciousObject != suspiciousObjectsList.m_objectInfoList.end();
			itSuspiciousObject++)
		{
			if (itSuspiciousObject->address == danglingObject) {
				itSuspiciousObject->leaked = true;
				outputObjectsInfo("Detect dangling object ", *itSuspiciousObject);
				break;
			}
		}

		suspiciousObjectsList.m_locker.unlock();
		m_locker.unlock();
	}

} /* helper */
