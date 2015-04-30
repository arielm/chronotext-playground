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
        data = (char*)malloc(s.size());
        memcpy(data, s.data(), s.size());
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
    }
    
    ObservableString(const char *c)
    {
        data = (char*)malloc(strlen(c));
        memcpy(data, c, strlen(c));
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
    }
    
    ObservableString(const ObservableString &other)
    {
        if (other.data)
        {
            data = (char*)malloc(strlen(other.data));
            memcpy(data, other.data, strlen(other.data));
        }
        
        LOGI << __PRETTY_FUNCTION__ << " " << reinterpret_cast<void*>(this) << " | " << data << std::endl;
    }
    
    ObservableString& operator=(const ObservableString &other)
    {
        if (this != &other)
        {
            if (data)
            {
                free(data);
            }
            
            if (other.data)
            {
                data = (char*)malloc(strlen(other.data));
                memcpy(data, other.data, strlen(other.data));
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
    
protected:
    char *data = nullptr;
};
