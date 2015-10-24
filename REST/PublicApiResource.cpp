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
 * The main RESTful interface of the application.
 */


#include <chrono>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/Utils>
#include <Wt/WString>
#include <CoreLib/Crypto.hpp>
#include <CoreLib/Database.hpp>
#include <CoreLib/Log.hpp>
#include <CoreLib/make_unique.hpp>
#include "JsonException.hpp"
#include "Pool.hpp"
#include "PublicApiResource.hpp"
#include "ServiceContract.hpp"
#include "XmlException.hpp"

#define     MAX_TOKEN_MILLISECONDS_DIFFERENCE        240000

#define     INVALID_TOKEN_ERROR                      L"INVALID_TOKEN"

#define     DataByDateJSON_URI_TEMPLATE              L"StockMarket/DataByDate/JSON/{DATE}/{TOKEN}"
#define     DataByDateXML_URI_TEMPLATE               L"StockMarket/DataByDate/XML/{DATE}/{TOKEN}"
#define     LatestDataJSON_URI_TEMPLATE              L"StockMarket/LatestData/JSON/{TOKEN}"
#define     LatestDataXML_URI_TEMPLATE               L"StockMarket/LatestData/XML/{TOKEN}"
#define     TokenJSON_URI_TEMPLATE                   L"StockMarket/Token/JSON"
#define     TokenXML_URI_TEMPLATE                    L"StockMarket/Token/XML"

using namespace std;
using namespace boost;
using namespace Wt;
using namespace Rest;

struct PublicApiResource::Impl
{
    typedef vector<vector<std::string> > Table_t;
    typedef vector<std::string> Row_t;

    enum class OutputType : unsigned char {
        JSON,
        XML
    };

    std::unique_ptr<Rest::ServiceContract> ServiceContractPtr;

    bool IsValidToken(const std::wstring &encryptedToken);

    void GetDataTree(const OutputType &outputType,
                     const std::string &dateId, const std::string &time,
                     const Row_t &titles, const Table_t &data,
                     boost::property_tree::wptree &out_tree);
    void GetDataByDate(const OutputType &outputType,
                       const std::string &date, boost::property_tree::wptree &out_tree);
    void GetLatestData(const OutputType &outputType, boost::property_tree::wptree &out_tree);
    void GetToken(boost::property_tree::wptree &out_tree);

    void DataByDateJson(const std::wstring &date, std::wstring &out_response);
    void DataByDateXml(const std::wstring &date, std::wstring &out_response);
    void LatestDataJson(std::wstring &out_response);
    void LatestDataXml(std::wstring &out_response);
    void TokenJson(std::wstring &out_response);
    void TokenXml(std::wstring &out_response);

    Impl();
    ~Impl();
};

PublicApiResource::PublicApiResource(WObject *parent) :
    ApiResource(parent),
    m_pimpl(std::make_unique<PublicApiResource::Impl>())
{
    m_pimpl->ServiceContractPtr = std::make_unique<Rest::ServiceContract>();
    m_pimpl->ServiceContractPtr->Register(DataByDateJSON_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(DataByDateXML_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(LatestDataJSON_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(LatestDataXML_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(TokenJSON_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(TokenXML_URI_TEMPLATE);
}

PublicApiResource::~PublicApiResource()
{
    beingDeleted();
}

void PublicApiResource::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
    try {
        WString uri((boost::format("%1%%2%")
                     % request.path().substr(request.path().find_last_of("/") + 1)
                     % request.pathInfo()
                     ).str());
        wstring uriTemplate;
        vector<wstring> args;

        if (m_pimpl->ServiceContractPtr->Resolve(uri.value(), uriTemplate, args)) {
            wstring outResponse;

            /// Validating the token
            if (boost::algorithm::contains(uriTemplate, L"/{TOKEN}")) {
                if (!m_pimpl->IsValidToken(args[args.size() - 1])) {
                    if (boost::algorithm::contains(uriTemplate, L"/JSON")) {
                        throw Rest::JsonException(INVALID_TOKEN_ERROR);
                    } else if (boost::algorithm::contains(uriTemplate, L"/XML")) {
                        throw Rest::XmlException(INVALID_TOKEN_ERROR);
                    } else {
                        throw Rest::Exception(INVALID_TOKEN_ERROR);
                    }
                }
            }

            if (uriTemplate == DataByDateJSON_URI_TEMPLATE) {

                m_pimpl->DataByDateJson(args[0], outResponse);
                PrintJson(response, outResponse);

            } else if (uriTemplate == DataByDateXML_URI_TEMPLATE) {

                m_pimpl->DataByDateXml(args[0], outResponse);
                PrintXml(response, outResponse);

            } else if (uriTemplate == LatestDataJSON_URI_TEMPLATE) {

                m_pimpl->LatestDataJson(outResponse);
                PrintJson(response, outResponse);

            } else if (uriTemplate == LatestDataXML_URI_TEMPLATE) {

                m_pimpl->LatestDataXml(outResponse);
                PrintXml(response, outResponse);

            } else if (uriTemplate == TokenJSON_URI_TEMPLATE) {

                m_pimpl->TokenJson(outResponse);
                PrintJson(response, outResponse);

            } else if (uriTemplate == TokenXML_URI_TEMPLATE) {

                m_pimpl->TokenXml(outResponse);
                PrintXml(response, outResponse);

            }
        } else {
            if (boost::algorithm::contains(uri.value(), L"/JSON")) {
                PrintJson(response, GetHttpStatusJson(CoreLib::HttpStatus::HttpStatusCode::HTTP_400));
            } else if (boost::algorithm::contains(uri.value(), L"/XML")) {
                PrintXml(response, GetHttpStatusXml(CoreLib::HttpStatus::HttpStatusCode::HTTP_400));
            } else {
                Print(response, GetHttpStatus(CoreLib::HttpStatus::HttpStatusCode::HTTP_400));
            }
        }
    }

    catch (const Rest::JsonException &ex) {
        PrintJson(response, ex.What());
    }

    catch (const Rest::XmlException &ex) {
        PrintXml(response, ex.What());
    }

    catch (const Rest::Exception &ex) {
        Print(response, ex.What());
    }

    catch (...) {
        Print(response, GetHttpStatus(CoreLib::HttpStatus::HttpStatusCode::HTTP_500));
    }
}

PublicApiResource::Impl::Impl()
{

}

PublicApiResource::Impl::~Impl()
{

}

bool PublicApiResource::Impl::IsValidToken(const std::wstring &encryptedToken)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double token;

    try {
        std::string decryptedToken;
        Pool::ClientToken()->Decrypt(WString(encryptedToken).toUTF8(), decryptedToken);
        token = boost::lexical_cast<double>(decryptedToken);
    } catch (...) {
        return false;
    }

    if (std::abs((std::chrono::system_clock::to_time_t(now) * 1000.0) - token)
            > MAX_TOKEN_MILLISECONDS_DIFFERENCE) {
        return false;
    }

    return true;
}

void PublicApiResource::Impl::GetDataTree(const OutputType &outputType,
                                          const std::string &date, const std::string &time,
                                          const Row_t &titles, const Table_t &data,
                                          boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    out_tree.put(L"StockMarket.version.major", "1");
    out_tree.put(L"StockMarket.version.minor", "0");
    out_tree.put(L"StockMarket.version.compat.major", "1");
    out_tree.put(L"StockMarket.version.compat.minor", "0");

    out_tree.put(L"StockMarket.date", WString(date).value());
    out_tree.put(L"StockMarket.time", WString(time).value());

    boost::property_tree::wptree titlesTree;
    for (Row_t::const_iterator it = titles.begin(); it != titles.end(); ++it) {
        boost::property_tree::wptree nameTree;
        nameTree.put(L"", WString(*it).value());

        switch (outputType) {
        case OutputType::JSON:
            titlesTree.push_back(std::make_pair(L"", nameTree));
            break;
        case OutputType::XML:
            titlesTree.add_child(L"n", nameTree);
            break;
        }
    }
    out_tree.add_child(L"StockMarket.titles", titlesTree);

    boost::property_tree::wptree dataTree;
    for (Table_t::const_iterator it = data.begin(); it != data.end(); ++it) {
        boost::property_tree::wptree rowTree;
        for (Row_t::const_iterator rowIt = (*it).begin(); rowIt != (*it).end(); ++rowIt) {
            boost::property_tree::wptree colTree;
            colTree.put(L"", WString(*rowIt).value());

            switch (outputType) {
            case OutputType::JSON:
                rowTree.push_back(std::make_pair(L"", colTree));
                break;
            case OutputType::XML:
                rowTree.add_child(L"c", colTree);
                break;
            }
        }

        switch (outputType) {
        case OutputType::JSON:
            dataTree.push_back(std::make_pair(L"", rowTree));
            break;
        case OutputType::XML:
            dataTree.add_child(L"r", rowTree);
            break;
        }
    }
    out_tree.add_child(L"StockMarket.data", dataTree);
}

void PublicApiResource::Impl::GetDataByDate(const OutputType &outputType, const std::string &dateId,
                                            boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    std::string date;
    std::string time;
    std::string dataTitlesTable;
    std::string stockDataTable;
    Row_t titles;
    Table_t data;

    cppdb::transaction guard(Pool::Database()->Sql());

    cppdb::result r = Pool::Database()->Sql()
            << (boost::format("SELECT date, time, datatitlestbl, stockdatatbl"
                              " FROM %1%"
                              " WHERE date=? "
                              " ORDER BY ROWID ASC"
                              " LIMIT 1;")
                % Pool::Database()->GetTableName("ARCHIVE")).str()
            << boost::replace_all_copy(dateId, "-", "/")
            << cppdb::row;

    if (r.empty()) {
        out_tree.put(L"status", CoreLib::HttpStatus::GetHttpResponse(
                         CoreLib::HttpStatus::HttpStatusCode::HTTP_404));
        return;
    }

    r >> date >> time >> dataTitlesTable >> stockDataTable;

    r = Pool::Database()->Sql()
            << (boost::format("SELECT title"
                              " FROM %1%"
                              " ORDER BY ROWID ASC;")
                % dataTitlesTable).str();

    std::string value;
    while(r.next()) {
        r >> value;
        titles.push_back(value);
    }

    r = Pool::Database()->Sql()
            << (boost::format("SELECT *"
                              " FROM %1%"
                              " ORDER BY ROWID ASC;")
                % stockDataTable).str();

    size_t rowId = 0;
    while(r.next()) {
        Row_t row;
        data.push_back(row);
        for (int i = 0; i < r.cols(); ++i) {
            r >> value;

            // skip the 'r' column
            if (i == 0)
                continue;

            data[rowId].push_back(value);
        }
        ++rowId;
    }

    guard.rollback();

    GetDataTree(outputType, date, time, titles, data, out_tree);
}

void PublicApiResource::Impl::GetLatestData(const OutputType &outputType,
                                            boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    std::string date;
    std::string time;
    Row_t titles;
    Table_t data;

    cppdb::transaction guard(Pool::Database()->Sql());

    cppdb::result r = Pool::Database()->Sql()
            << (boost::format("SELECT date, time"
                              " FROM %1%"
                              " ORDER BY ROWID ASC"
                              " LIMIT 1;")
                % Pool::Database()->GetTableName("LAST_UPDATE")).str()
            << cppdb::row;

    if (r.empty()) {
        return;
    }

    r >> date >> time;

    r = Pool::Database()->Sql()
            << (boost::format("SELECT title"
                              " FROM %1%"
                              " ORDER BY ROWID ASC;")
                % Pool::Database()->GetTableName("DATA_TITLES")).str();

    std::string value;
    while(r.next()) {
        r >> value;
        titles.push_back(value);
    }

    r = Pool::Database()->Sql()
            << (boost::format("SELECT *"
                              " FROM %1%"
                              " ORDER BY ROWID ASC;")
                % Pool::Database()->GetTableName("STOCK_DATA")).str();

    size_t rowId = 0;
    while(r.next()) {
        Row_t row;
        data.push_back(row);
        for (int i = 0; i < r.cols(); ++i) {
            r >> value;

            // skip the 'r' column
            if (i == 0)
                continue;

            data[rowId].push_back(value);
        }
        ++rowId;
    }

    guard.rollback();

    GetDataTree(outputType, date, time, titles, data, out_tree);
}

void PublicApiResource::Impl::GetToken(boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> millisecondsSinceEpoch =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    std::string token;
    Pool::ServerToken()->Encrypt(lexical_cast<std::string>(millisecondsSinceEpoch.count()), token);

    out_tree.put(L"token", WString(token).value());
}

void PublicApiResource::Impl::DataByDateJson(const std::wstring &date, std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetDataByDate(OutputType::JSON, WString(date).toUTF8(), tree);

    boost::property_tree::write_json(stream, tree);

    out_response.assign(stream.str());
}

void PublicApiResource::Impl::DataByDateXml(const std::wstring &date, std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetDataByDate(OutputType::XML, WString(date).toUTF8(), tree);

    boost::property_tree::write_xml(stream, tree);

    out_response.assign(stream.str());
}

void PublicApiResource::Impl::LatestDataJson(std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetLatestData(OutputType::JSON, tree);

    boost::property_tree::write_json(stream, tree);

    out_response.assign(stream.str());
}

void PublicApiResource::Impl::LatestDataXml(std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetLatestData(OutputType::XML, tree);

    boost::property_tree::write_xml(stream, tree);

    out_response.assign(stream.str());
}

void PublicApiResource::Impl::TokenJson(std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetToken(tree);

    boost::property_tree::write_json(stream, tree);

    out_response.assign(stream.str());
}

void PublicApiResource::Impl::TokenXml(std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetToken(tree);

    boost::property_tree::write_xml(stream, tree);

    out_response.assign(stream.str());
}

