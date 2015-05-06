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
        CHR_TEST(force || true, testLayoutAdvance);
        CHR_TEST(force || true, testBIDI);
        CHR_TEST(force || true, testUnicode);
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

int checkLines(ZFont &font, InputSource::Ref source, vector<size_t> &hashes)
{
    auto lines = parseLines(source, font);
    
    if (CHR_CHECK(lines.size() == hashes.size()))
    {
        size_t index = 0;
        
        for (auto &layout : lines)
        {
            auto value = layoutHash(layout);
            auto expected = hashes[index++];
            
            stringstream ss;
            ss << "VALUE: " << value << " | EXPECTED: " << expected;
            
            CHR_CHECK(value == expected, ss.str());
        }
    }
    
    return 0;
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
    
    checkLines(*font, InputSource::getAsset("bidi.xml"), hashes);
}

void TestingZFont::testUnicode()
{
    auto font = fontManager.getFont("sans-serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(32).setCrisp());
    font->setSize(32);
    
    vector<size_t> hashes
    {
        SIZE_T2(2423061461, 2299879191927676355),
        SIZE_T2(676037056, 8126074978093623632),
        SIZE_T2(51684586, 203953228882966113),
        SIZE_T2(2160200639, 14329726771226279651),
        SIZE_T2(2829186193, 17598384365926577453),
        SIZE_T2(2273140257, 17544906773354463270),
        SIZE_T2(3846317070, 13184677768672481576),
        SIZE_T2(509205841, 17084177423896891027),
        SIZE_T2(540959090, 10056896830353472260),
        SIZE_T2(1507955890, 785683912355301265),
        SIZE_T2(2508873915, 11037216949657619652),
        SIZE_T2(1622008515, 1226742790203444453),
        SIZE_T2(2685948395, 6991658061081508337),
        SIZE_T2(4294421774, 992794805295498511),
    };
    
    checkLines(*font, InputSource::getAsset("unicode.xml"), hashes);
}
