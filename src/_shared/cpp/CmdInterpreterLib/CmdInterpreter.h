#pragma once
#include <queue>
#include <atomic>
#include <string>
#include <sstream>

//https ://stackoverflow.com/questions/19485536/redirect-output-of-an-function-printing-to-console-to-string

enum CmdModes
{
	Reading = 0,
	Writing,
	Stopped
};

class CmdInterpreter
{
public:
	CmdInterpreter();
	~CmdInterpreter();

	bool registerCmd();
	void run();
	void stop();

private:
	void cout();
	std::atomic<CmdModes> m_mode;
	std::stringstream m_outBuffer;
	std::streambuf* m_previousBuffer;
};