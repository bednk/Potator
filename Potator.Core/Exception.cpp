#include "pch.h"
#include "Exception.h"
#include <boost/stacktrace.hpp>
#include <sstream>


Potator::Exception::Exception(const int framesToSkip)
{
	std::stringstream ss;
	ss << boost::stacktrace::stacktrace(framesToSkip, -1);
	_stackTrace = ss.str();
}

Potator::Exception::Exception(const std::string& what, const int framesToSkip) : Exception(framesToSkip)
{
	_message = what;
}

char const* Potator::Exception::what() const
{
	//I can't for my life concatenate Message and StackTrace
	//into one std::string (StackTrace always gets cut off) ¯\_(ツ)_/¯
	const int size = _message.size() + _stackTrace.size();
	_what = std::make_unique<char[]>(size);
	strcpy_s(_what.get(), size, _message.c_str());
	strcat_s(_what.get(), size, _stackTrace.c_str());
	return _what.get();
}