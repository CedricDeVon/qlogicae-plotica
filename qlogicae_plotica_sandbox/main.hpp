#pragma once

#include "qlogicae_plotica_sandbox/includes/application.hpp"

#include "qlogicae_core/includes/all.hpp"
#include "qlogicae_plotica/includes/all.hpp"

int main(int, char**);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int
)
{
    return main(__argc, __argv);
}

namespace QLogicaePloticaSandbox
{

}
