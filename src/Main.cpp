#include "OpenGLMinecraft/OpenGLMinecraft.h"
#include "OpenGLMinecraft/Utility/Log.h"

// currently hardcoded main for released window only builds
#ifdef NDEBUG
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nShowCmd)
#else
int main(int argc, char** argv)
#endif
{
    fprintf(stderr, "WE ARE ALIVE!!!\n"); // ensure program starts, you'd be suprised how useful it is

    Log::Init();
    LOG_INFO("Hello, Logs!");

    OpenGLMinecraft* GameInstance = new OpenGLMinecraft();

    GameInstance->Start();

    delete GameInstance;

    return 0;
}