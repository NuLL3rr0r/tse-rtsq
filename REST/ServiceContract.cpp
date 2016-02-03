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


#include <map>
#include <vector>
#include <utility>
#include <cstddef>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <CoreLib/make_unique.hpp>
#include "ServiceContract.hpp"

using namespace std;
using namespace boost;
using namespace Rest;

struct ServiceContract::Impl
{
    typedef std::multimap<std::wstring, std::pair<std::wstring, std::size_t>> ContractsMultimap;

    ContractsMultimap Contracts;
};

ServiceContract::ServiceContract() :
    m_pimpl(std::make_unique<ServiceContract::Impl>())
{

}

ServiceContract::~ServiceContract()
{

}

void ServiceContract::Register(const std::wstring &uriTemplate)
{
    std::vector<std::wstring> vec;
    boost::split(vec, uriTemplate, boost::is_any_of(L"/"));

    std::size_t argsCount = 0;
    for (std::vector<std::wstring>::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if (boost::starts_with((*it), "{")
                && boost::ends_with((*it), "}")) {
            ++argsCount;
        }
    }
    static const boost::wregex eOmmiter(L"\\/{.*?\\}");
    static const std::wstring emptyText = L"";
    std::wstring webMethod = boost::regex_replace(uriTemplate, eOmmiter, emptyText);

    m_pimpl->Contracts.insert(
                std::make_pair(webMethod,
                               std::make_pair(uriTemplate, argsCount)));
}

bool ServiceContract::Resolve(const std::wstring &uri, std::wstring &out_uriTemplate,
                              std::vector<std::wstring> &out_args) const
{
    out_uriTemplate.clear();

    if (!boost::ends_with(uri, "/")) {
        for (Impl::ContractsMultimap::const_iterator it = m_pimpl->Contracts.begin();
             it != m_pimpl->Contracts.end(); ++it) {
            if (boost::starts_with(uri, it->first)) {
                std::wstring args(uri.substr(it->first.size()));

                std::size_t argsCount = 0;
                std::vector<std::wstring> vec;
                if (args.find(L"/") != std::wstring::npos) {
                    boost::split(vec, args, boost::is_any_of(L"/"));

                    for (std::vector<std::wstring>::const_iterator vecIt = vec.begin();
                         vecIt != vec.end(); ++vecIt) {
                        if (!(*vecIt).empty()) {
                            out_args.push_back(*vecIt);
                            ++argsCount;
                        }
                    }
                }

                if (it->second.second == argsCount) {
                    out_uriTemplate = it->second.first;
                    return true;
                }
            }
        }
    }

    return false;
}

