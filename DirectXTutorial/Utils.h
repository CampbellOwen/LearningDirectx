#pragma once

#include <string>

#include <Windows.h>

namespace Engine
{
namespace Utils
{
	std::string GetHRErrorString(HRESULT hr);

}
}