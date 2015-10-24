/**
 * @file
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @version 0.1.0
 *
 * @section LICENSE
 *
 * (The MIT License)
 *
 * Copyright (c) 2015 Mohammad S. Babaei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Abstract class to create RESTful interfaces.
 */


#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/Http/Response>
#include <CoreLib/make_unique.hpp>
#include "ApiResource.hpp"

using namespace std;
using namespace boost;
using namespace Wt;
using namespace CoreLib;
using namespace Rest;

ApiResource::ApiResource(WObject *parent) :
    WResource(parent)
{

}

ApiResource::~ApiResource()
{

}

std::wstring ApiResource::GetHttpStatus(const CoreLib::HttpStatus::HttpStatusCode &code) const
{
    return HttpStatus::GetHttpResponse(code);
}

std::wstring ApiResource::GetHttpStatusJson(const CoreLib::HttpStatus::HttpStatusCode &code) const
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"status", HttpStatus::GetHttpResponse(code));

    boost::property_tree::write_json(stream, tree);

    return stream.str();
}

std::wstring ApiResource::GetHttpStatusXml(const CoreLib::HttpStatus::HttpStatusCode &code) const
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"status", HttpStatus::GetHttpResponse(code));

    boost::property_tree::write_xml(stream, tree);

    return stream.str();
}

void ApiResource::Print(Wt::Http::Response &response, const std::wstring &text)
{
    response.addHeader("Content-type", "text/plain; charset=utf-8");
    response.out() << text;
}

void ApiResource::PrintJson(Wt::Http::Response &response, const std::wstring &json)
{
    response.addHeader("Content-type", "application/json; charset=utf-8");
    response.out() << json;
}

void ApiResource::PrintXml(Wt::Http::Response &response, const std::wstring &xml)
{
    response.addHeader("Content-type", "application/xml; charset=utf-8");
    response.out() << xml;
}

