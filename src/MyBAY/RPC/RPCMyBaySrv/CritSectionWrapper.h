/************************************************************/
/*                                                          */
/* Inhalt:    CriticalSectionWrapper - Header	            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#pragma once
#include <WinBase.h>

// CriticalSection - Singleton
// Um Threadsafety zu gewährleisten
class CritSectionWrapper
{
private:
	LPCRITICAL_SECTION m_critSection;
public:
	/// <summary>
	/// Konstruktor
	/// </summary>
	CritSectionWrapper()
	{
		if (NULL == m_critSection)
			m_critSection = new CRITICAL_SECTION();

		InitializeCriticalSection(m_critSection);		// Initialisiert die Critical Section 

	}
	/// <summary>
	/// Destruktor
	/// </summary>
	~CritSectionWrapper()
	{
		DeleteCriticalSection(m_critSection);
	}
	/// <summary>
	/// Instanz der Klasse 
	/// </summary>
	/// <returns> Instanz der Klasse </returns>
	LPCRITICAL_SECTION getInstance()
	{
		return m_critSection;
	}
};