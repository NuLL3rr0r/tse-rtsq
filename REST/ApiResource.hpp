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


#ifndef REST_API_RESOURCE_HPP
#define REST_API_RESOURCE_HPP


#include <Wt/WResource>

namespace Wt {
    namespace Http {
        class Request;
        class Response;
    }
}

#include <CoreLib/HttpStatus.hpp>

namespace Rest {
    class ApiResource;
}

class Rest::ApiResource : public Wt::WResource
{
public:
    explicit ApiResource(WObject *parent = NULL);
    virtual ~ApiResource() = 0;

public:
    virtual void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) = 0;

protected:
    std::wstring GetHttpStatus(const CoreLib::HttpStatus::HttpStatusCode &code) const;
    std::wstring GetHttpStatusJson(const CoreLib::HttpStatus::HttpStatusCode &code) const;
    std::wstring GetHttpStatusXml(const CoreLib::HttpStatus::HttpStatusCode &code) const;

    void Print(Wt::Http::Response &response, const std::wstring &text);
    void PrintJson(Wt::Http::Response &response, const std::wstring &json);
    void PrintXml(Wt::Http::Response &response, const std::wstring &xml);
};


#endif /* REST_API_RESOURCE_HPP */

