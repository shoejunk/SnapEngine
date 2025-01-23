/**
 * \file RendererBase.cpp
 * \brief Implements the RendererBase class test() method.
 */

#include "RendererBase.h"
#include <iostream>
#include <cassert>

void RendererBase::test()
{
    std::cout << "[RendererBase] Running basic tests...\n";

    // Since RendererBase is abstract, we can't instantiate it directly.
    // We'll just do a smoke test for now.
    // In practice, your D3D11Renderer test will be more thorough.

    std::cout << "[RendererBase] Tests completed (abstract class).\n";
}
