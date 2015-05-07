/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "Testing/TestingBase.h"

#include <boost/functional/hash.hpp>

#include "chronotext/font/zf/FontManager.h"

struct LayoutHasher
{
    std::size_t operator()(const chr::zf::LineLayout& layout) const;
};

static inline std::size_t layoutHash(std::shared_ptr<chr::zf::LineLayout> layout)
{
    return LayoutHasher()(*layout);
}

static std::vector<std::shared_ptr<chr::zf::LineLayout>> parseLines(chr::InputSource::Ref source, chr::ZFont &font);
static std::tuple<std::string, hb_language_t, hb_direction_t> parseLine(const ci::XmlTree &element);
static int checkLines(chr::ZFont &font, chr::InputSource::Ref source, std::vector<size_t> &expectedHashes);

// ---

class TestingZFont : public TestingBase
{
public:
    chr::zf::FontManager fontManager;
    
    void setup() final;
    void shutdown() final;
    void run(bool force = false) final;
    
    // ---
    
    void testLayoutAdvance();
    void testBIDI();
    void testUnicode();
};
