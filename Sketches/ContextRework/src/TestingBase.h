#pragma once

#include "chronotext/Log.h"

class TestingBase
{
public:
    template<class TestingBase>
    static void execute(bool proceed = true, bool force = false)
    {
        if (proceed)
        {
            auto test = std::make_shared<TestingBase>();
            
            try
            {
                test->setup();
                test->run(force);
                test->shutdown();
            }
            catch (std::exception &e)
            {
                LOGI << "TEST FAILED | REASON: " << e.what() << std::endl;
            }
        }
    }
    
    virtual ~TestingBase() {}
    
    virtual void setup() {}
    virtual void shutdown() {}
    virtual void run(bool force = false) = 0;
};
