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
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
    }
    
    ObservableString(const char *c)
    {
        data = strdup(c);
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
    }
    
    ObservableString(const ObservableString &other)
    {
        if (other.data)
        {
            data = strdup(other.data);
        }
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
    }
    
    ObservableString& operator=(const ObservableString &other)
    {
        if (this != &other)
        {
            free(data);
            
            if (other.data)
            {
                data = strdup(other.data);
            }
        }
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
        
        return *this;
    }
    
    ObservableString(ObservableString &&other)
    :
    data(other.data)
    {
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
        
        other.data = nullptr;
    }
    
    ObservableString& operator=(ObservableString &&other)
    {
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
        
        std::swap(data, other.data);
        return *this;
    }
    
    ~ObservableString()
    {
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
        
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

    bool operator< (const char *rhs) const
    {
        return strcmp(data, rhs) < 0;
    }
    
    friend bool operator< (const char *lhs, const ObservableString &rhs)
    {
        return strcmp(lhs, rhs.data) < 0;
    }
    
protected:
    char *data = nullptr;
};
