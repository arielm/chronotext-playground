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
    if (force || true) testCXXAbi1();
    if (force || true) testCXXAbi2();
    if (force || true) testCXXAbi3();
}

// ---

/*
 * TESTING __cxa_demangle:
 *
 * PROS: WORKS AS INTENDED ON OSX, iOS AND ANDROID
 * CONS: NOT COMPATIBLE WITH VISUAL STUDIO!
 *
 *
 * REFERENCE:
 *
 * - https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
 * - http://en.wikipedia.org/wiki/Name_mangling#Name_mangling_in_C.2B.2B
 * - http://www.int0x80.gr/papers/name_mangling.pdf
 */

void TestingDemangling::testCXXAbi1()
{
    const std::type_info &ti = typeid(*this);
    
    int status;
    char *realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);

    /*
     * RESULT: 17TestingDemangling TestingDemangling 0
     */
    LOGI << ti.name() << " " << realname << " " << status << endl;
    free(realname);
}

// ---

namespace test
{
    struct Foo
    {
        void bar()
        {}
    };
}

void TestingDemangling::testCXXAbi2()
{
    test::Foo foo;
    const std::type_info &ti = typeid(foo);
    
    int status;
    char *realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
    
    /*
     * RESULT: N4test3FooE test::Foo 0
     */
    LOGI << ti.name() << " " << realname << " " << status << endl;
    free(realname);
}

// ---

struct Base
{
    virtual ~Base() {}
};

struct Derived : public Base
{};

void TestingDemangling::testCXXAbi3()
{
    Derived derived;
    const std::type_info &ti = typeid(derived);
    
    int status;
    char *realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
    
    /*
     * RESULT: 7Derived Derived 0
     */
    LOGI << ti.name() << " " << realname << " " << status << endl;
    free(realname);
}

