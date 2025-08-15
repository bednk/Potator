#pragma once
#include <string>

namespace Potator
{
	class HrCheck
	{
	public:
		HrCheck() = default;
		HrCheck(const HrCheck&) = delete;
		HrCheck& operator=(const HrCheck&) = delete;

		static HrCheck& Instance();
		friend void operator>>(HRESULT hr, HrCheck& checker);
	private:
		static std::string GetErrorMessage(HRESULT hr);
		static void Check(HRESULT hr);
	};
}