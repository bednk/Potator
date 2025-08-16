#pragma once
#include <exception>
#include <string>
#include <memory>

namespace Potator
{
	class Exception : public std::exception
	{
	public:
		Exception(int framesToSkip = 3);
		Exception(const std::string& what, int framesToSkip = 3);
		char const* what() const override;
	protected:
		std::string _message;
		std::string _stackTrace;
		mutable std::unique_ptr<char[]> _what;
	};
}