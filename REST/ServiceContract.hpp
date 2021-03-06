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
 * This class takes care of translating RESTful urls to C++ functions.
 */


#ifndef REST_SERVICE_CONTRACT_HPP
#define REST_SERVICE_CONTRACT_HPP


#include <string>
#include <vector>

namespace Rest {
    class ServiceContract;
}

class Rest::ServiceContract
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    ServiceContract();
    ~ServiceContract();

public:
    void Register(const std::wstring &uriTemplate);
    bool Resolve(const std::wstring &uri, std::wstring &out_uriTemplate,
                 std::vector<std::wstring> &out_args) const;
};


#endif /* REST_SERVICE_CONTRACT_HPP */

