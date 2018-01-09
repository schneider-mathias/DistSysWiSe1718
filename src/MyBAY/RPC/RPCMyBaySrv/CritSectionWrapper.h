#pragma once
#include <WinBase.h>

// Anwendung: 
/*scoped_lock*/
//EnterCriticalSection(critSecWrapper.getInstance());
//LeaveCriticalSection(critSecWrapper.getInstance());

class CritSectionWrapper
{
private:
	LPCRITICAL_SECTION m_critSection;
public:
	CritSectionWrapper()
	{
		if (NULL == m_critSection)
			m_critSection = new CRITICAL_SECTION();

		InitializeCriticalSection(m_critSection);

	}
	~CritSectionWrapper()
	{
		DeleteCriticalSection(m_critSection);
	}
	LPCRITICAL_SECTION getInstance()
	{
		return m_critSection;
	}
};