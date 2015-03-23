/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingDemangling.h"

#include "chronotext/Context.h"

#include <cxxabi.h>

using namespace std;
using namespace ci;
using namespace chr;

void TestingDemangling::run(bool force)
{
    if (force || true) testCXXAbi();
}

// ---

/*
 * REFERENCE: https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
 */

void TestingDemangling::testCXXAbi()
{
    const std::type_info &ti = typeid(*this);
    
    int status;
    char *realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);

    /*
     * RESULT: 17TestingDemangling TestingDemangling 0
     *
     * WORKS AS INTENDED ON OSX, iOS AND ANDROID
     */
    LOGI << ti.name() << " " << realname << " " << status << endl;
    
    free(realname);
}
