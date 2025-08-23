#include "pch.h"
#include "HrCheck.h"
#include <stdexcept>
#include <string>
#include <exception>


Potator::HrCheck& Potator::HrCheck::Instance()
{
    static HrCheck instance;
    return instance;
}

std::string Potator::HrCheck::GetErrorMessage(HRESULT hr)
{
    char* errorMsg = nullptr;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&errorMsg),
        0,
        nullptr
    );

    std::string message(errorMsg ? errorMsg : "Unknown error");
    LocalFree(errorMsg);
    return message;
}

void Potator::HrCheck::Check(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::runtime_error(GetErrorMessage(hr));
    }
}

void Potator::operator>>(HRESULT hr, HrCheck& checker)
{
    checker.Check(hr);
}


