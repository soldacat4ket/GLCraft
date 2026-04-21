#include "OpenGLMinecraft/OpenGLMinecraft.h"
#include "OpenGLMinecraft/Utility/Log.h"

int main(int argc, char** argv)
{
    fprintf(stderr, "WE ARE ALIVE!!!\n"); // ensure program starts, you'd be suprised how useful it is

    Log::Init();
    LOG_INFO("Hello, Logs!");

    OpenGLMinecraft* GameInstance = new OpenGLMinecraft();

    GameInstance->Start();

    delete GameInstance;

    return 0;
}