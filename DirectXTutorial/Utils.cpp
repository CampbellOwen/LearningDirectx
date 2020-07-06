#include "Utils.h"

std::string Engine::Utils::GetLastErrorAsString(HRESULT hr)
{
   //Get the error message, if any.
   if (!FAILED(hr))
      return std::string(); //No error message has been recorded

   LPSTR messageBuffer = nullptr;
   size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

   std::string message(messageBuffer, size);

   //Free the buffer.
   LocalFree(messageBuffer);

   return message;
}
