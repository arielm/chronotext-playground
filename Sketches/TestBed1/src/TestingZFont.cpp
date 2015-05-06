/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingZFont.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

void TestingZFont::setup()
{
    FontManager::LOG_VERBOSE = true;
    
    fontManager.loadConfig(InputSource::getAsset("font-config.xml"));
}

void TestingZFont::shutdown()
{
    FontManager::LOG_VERBOSE = false;
}

void TestingZFont::run(bool force)
{
    if (force || true)
    {
        if (force || true) testLayoutAdvance();
        if (force || true) testBIDI();
    }
}

// ---

size_t LayoutHasher::operator()(const LineLayout& layout) const
{
    using boost::hash_value;
    using boost::hash_combine;
    
    size_t seed = 0;
    
    for (auto &cluster : layout.clusters)
    {
        hash_combine(seed, hash_value(cluster.combinedAdvance));
        
        for (auto &shape : cluster.shapes)
        {
            hash_combine(seed, hash_value(shape.codepoint));
        }
    }
    
    return seed;
}

vector<shared_ptr<LineLayout>> parseLines(InputSource::Ref source, ZFont &font)
{
    vector<shared_ptr<LineLayout>> lines;
    
    XmlTree doc(source->loadDataSource());
    const auto &rootElement = doc.getChild("Text");
    
    for (const auto &lineElement : rootElement.getChildren())
    {
        string text;
        string lang;
        hb_direction_t dir;
        
        tie(text, lang, dir) = parseLine(*lineElement);
        lines.emplace_back(font.getLineLayout(text, lang, dir));
    }
    
    return lines;
}

tuple<string, string, hb_direction_t> parseLine(const XmlTree &element)
{
    auto line = element.getValue();
    
    boost::algorithm::trim(line);
    boost::replace_all(line, "&lrm;", "\u200e");
    boost::replace_all(line, "&rlm;", "\u200f");
    
    auto lang = element.getAttributeValue<string>("lang", "");
    auto dir = ZFont::stringToDirection(element.getAttributeValue<string>("dir", ""));
    
    return make_tuple(line, lang, dir);
}

// ---

void TestingZFont::testLayoutAdvance()
{
    auto font = fontManager.getFont("sans-serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(32).setCrisp());
    font->setSize(32);
    
    CHR_CHECK(font->getAdvance(*font->getLineLayout("Which way to the station?")) == 366);
}

// ---

void TestingZFont::testBIDI()
{
    auto font = fontManager.getFont("sans-serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(32).setCrisp());
    font->setSize(32);
    
    auto lines = parseLines(InputSource::getAsset("TextBIDI.xml"), *font);
    
    vector<size_t> hashes
    {
        SIZE_T2(520534658, 4572554708016581871),
        SIZE_T2(3001945655, 1323500750991593886),
        SIZE_T2(4098485350, 16616302104612900688),
        SIZE_T2(467935369, 18024912760393654653),
        SIZE_T2(3613247354, 7317353174500663839),
        SIZE_T2(3946649848, 2265622829494731629),
        SIZE_T2(85789293, 11704902527822747174),
    };
    
    if (CHR_CHECK(lines.size() == hashes.size()))
    {
        size_t index = 0;
        
        for (auto &layout : lines)
        {
            CHR_CHECK(layoutHash(layout) == hashes[index++]);
        }
    }
}
