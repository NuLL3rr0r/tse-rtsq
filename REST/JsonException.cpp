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
 * Exceptions in JSON format.
 */


#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/WString>
#include <CoreLib/make_unique.hpp>
#include "JsonException.hpp"

using namespace std;
using namespace Wt;
using namespace Rest;

JsonException::JsonException(const std::string &message) :
    Exception(message)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"error", WString(message).value());

    boost::property_tree::write_json(stream, tree);

    m_message = stream.str();
}

JsonException::JsonException(const std::wstring &message) :
    Exception(message)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"error", message);

    boost::property_tree::write_json(stream, tree);

    m_message = stream.str();
}

const wchar_t *JsonException::What() const
{
    return m_message.c_str();
}

