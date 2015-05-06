/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "chronotext/Log.h"

class ObservableString
{
public:
    ObservableString(const std::string &s)
    {
        data = strdup(s.data());
        
        dump(__PRETTY_FUNCTION__);
    }
    
    ObservableString(const char *c)
    {
        data = strdup(c);
        
        dump(__PRETTY_FUNCTION__);
    }
    
    ObservableString(const ObservableString &other)
    {
        data = strdup(other.data);
        
        dump(__PRETTY_FUNCTION__);
    }
    
    ObservableString& operator=(const ObservableString &other)
    {
        if (this != &other)
        {
            free(data);
            data = strdup(other.data);
        }
        
        dump(__PRETTY_FUNCTION__);
        
        return *this;
    }
    
    ObservableString(ObservableString &&other) noexcept
    :
    data(nullptr)
    {
        std::swap(data, other.data);
        
        dump(__PRETTY_FUNCTION__);
    }
    
    /*
     * TODO: TEST
     */
    ObservableString& operator=(ObservableString &&other) noexcept
    {
        std::swap(data, other.data);
        
        dump(__PRETTY_FUNCTION__);
        
        return *this;
    }
    
    ~ObservableString()
    {
        dump(__PRETTY_FUNCTION__);
        
        free(data);
    }
    
    operator const char* () const
    {
        return data;
    }
    
    bool operator< (const ObservableString &rhs) const
    {
        return strcmp(data, rhs.data) < 0;
    }
    
    bool operator== (const ObservableString &rhs) const
    {
        return (data == rhs.data) || (strcmp(data, rhs.data) == 0);
    }
    
protected:
    char *data = nullptr;
    
    void dump(const char *prefix)
    {
        void *instanceAddress = this;
        void *dataAddress = data;
        
        LOGI << prefix << " " << instanceAddress << " | " << (data ? data : "") << (data ? " " : "") << dataAddress << std::endl;
    }
};
