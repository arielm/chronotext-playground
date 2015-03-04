/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * FEATURES:
 *
 * - DOWNLOADING DATA WITH PROGRESS
 * - HTTPS SUPPORT
 * - POSSIBILITY TO CANCEL (TOUCH THE SCREEN TO TRIGGER)
 */

#pragma once

#include "TestingBase.h"

#include "Tasks/HttpTask.h"

class TestingNetwork : public TestingBase, public HttpTask::Handler
{
public:
    void setup() final;
    void shutdown() final;
    
    void addTouch(int index, float x, float y) final;
    
    void requestStarted(const HttpTask::Request &request) final;
    void requestCancelled(const HttpTask::Request &request) final;
    void requestFailed(const HttpTask::Request &request, const HttpTask::Response &response, HttpTask::Error error) final;
    void requestSucceeded(const HttpTask::Request &request, const HttpTask::Response &response) final;
    void requestProgress(const HttpTask::Request &request, std::streamsize downloaded, std::streamsize total) final;

protected:
    int taskId = 0;
};
