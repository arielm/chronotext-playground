/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingIO.h"

#include "chronotext/Context.h"

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>

using namespace std;
using namespace ci;
using namespace chr;

namespace io = boost::iostreams;

void TestingIO::run(bool force)
{
    if (force || true)
    {
        CHR_TEST(force || false, testIOStreams1);
        CHR_TEST(force || true, testIOStreams2);
    }
}

// ---

void TestingIO::testIOStreams1()
{
    vector<char> store;
    {
        double d = 123.456;
        uint32_t ui = 99;

        io::back_insert_device<vector<char>> sink(store);
        io::stream<io::back_insert_device<vector<char>>> out(sink);
        
        out << "ABC" << '\n';
        out.write(reinterpret_cast<const char*>(&d), sizeof(d));
        out.write(reinterpret_cast<const char*>(&ui), sizeof(ui));
    }
    
    LOGI << store.size() << endl;
    
    {
        io::basic_array_source<char> source(store.data(), store.size());
        io::stream<io::basic_array_source<char>> in(source);
        
        string line; std::getline(in, line);
        LOGI << line << " | " << in.rdbuf()->in_avail() << endl;
        
        double d; in.read(reinterpret_cast<char*>(&d), sizeof(d));
        LOGI << d << endl;
        
        uint32_t ui; in.read(reinterpret_cast<char*>(&ui), sizeof(ui));
        LOGI << ui << endl;
        
        LOGI << in.tellg() << endl;
        
        auto seq = in->input_sequence();
        LOGI << reinterpret_cast<void*>(seq.first) << ", " << reinterpret_cast<void*>(seq.second) << " | " << int(seq.second - seq.first) << endl;
   }
}

// ---

static void writeToStream(io::filtering_ostream &out)
{
    double d = 123.456;
    uint32_t ui = 99;
    
    out << "ABC" << '\n';
    out.write(reinterpret_cast<const char*>(&d), sizeof(d));
    out.write(reinterpret_cast<const char*>(&ui), sizeof(ui));
}

static void readFromStream(io::filtering_istream &in)
{
    string line; std::getline(in, line);
    LOGI << line << " | " << in.rdbuf()->in_avail() << endl;
    
    double d; in.read(reinterpret_cast<char*>(&d), sizeof(d));
    LOGI << d << endl;
    
    uint32_t ui; in.read(reinterpret_cast<char*>(&ui), sizeof(ui));
    LOGI << ui << endl;
    
    LOGI << in.tellg() << endl;
    
    // ---
    
    /*
     * CHECKING THE "IDENTITY" OF THE 1ST COMPONENT VIA RTTI
     */
    const std::type_info &info = in.component_type(0);
    LOGI << info.name() << endl;
    
    /*
     * ASSUMING THE 1ST COMPONENT IS A io::basic_array_source<char>
     */
    auto component = in.component<io::basic_array_source<char>>(0);
    
    auto seq = component->input_sequence();
    LOGI << reinterpret_cast<void*>(seq.first) << ", " << reinterpret_cast<void*>(seq.second) << " | " << int(seq.second - seq.first) << endl;
}

void TestingIO::testIOStreams2()
{
    vector<char> store;
    {
        io::filtering_ostream out;
        out.push(io::back_insert_device<vector<char>>(store));
        
        writeToStream(out);
    }
    
    LOGI << store.size() << endl;
    
    {
        io::filtering_istream in;
        in.push(io::basic_array_source<char>(store.data(), store.size()));
        
        readFromStream(in);
    }
}
