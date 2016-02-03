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
 * This class updates the stock quotes upon start in a specified interval.
 */


#include <vector>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/regex.hpp>
#include <CoreLib/Archiver.hpp>
#include <CoreLib/Database.hpp>
#include <CoreLib/FileSystem.hpp>
#include <CoreLib/Http.hpp>
#include <CoreLib/Log.hpp>
#include <CoreLib/make_unique.hpp>
#include "Pool.hpp"
#include "StockUpdateWorker.hpp"

#define         STOCK_DATA_LOCAL_TEMP_FILE_NAME             "stock-quotes-latest.xlsx"
#define         STOCK_DATA_TEMP_WORK_DIR_NAME               "stock-quotes-excel-temp"

using namespace std;
using namespace boost;
using namespace CoreLib;
using namespace Rest;

struct StockUpdateWorker::Impl
{
    typedef std::unique_ptr<boost::thread> thread_ptr;

    bool Running;
    bool StartImmediately;

    std::string SourceURL;
    StockUpdateWorker::Interval Interval;

    thread_ptr WorkerThread;
    std::mutex WorkerMutex;

    Impl();
    ~Impl();

    void Cron();
    void Update();

    std::string GetTableNameFromDate(const std::string &id, const std::string &date);
};

StockUpdateWorker::StockUpdateWorker() :
    m_pimpl(std::make_unique<StockUpdateWorker::Impl>())
{

}

StockUpdateWorker::StockUpdateWorker(const std::string &sourceURL,
                                     const Interval interval,
                                     bool startImmediately) :
    m_pimpl(std::make_unique<StockUpdateWorker::Impl>())
{
    m_pimpl->SourceURL = sourceURL;
    m_pimpl->Interval = interval;
    m_pimpl->StartImmediately = startImmediately;
}

StockUpdateWorker::~StockUpdateWorker()
{

}

std::string StockUpdateWorker::GetSourceURL()
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    return m_pimpl->SourceURL;
}

boost::int_least64_t StockUpdateWorker::GetInterval()
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    return m_pimpl->Interval;
}

void StockUpdateWorker::SetSourceURL(const std::string &sourceFile)
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    if (m_pimpl->Running)
        return;

    m_pimpl->SourceURL = sourceFile;
}

void StockUpdateWorker::SetInterval(Interval interval)
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    if (m_pimpl->Running)
        return;

    m_pimpl->Interval = interval;
}

bool StockUpdateWorker::IsRunning()
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    return m_pimpl->Running;
}

void StockUpdateWorker::Start()
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    assert(m_pimpl->SourceURL != "");
    assert(m_pimpl->Interval != 0);

    if (m_pimpl->Running) {
        return;
    }

    LOG_INFO("Starting stock data updater process...");

    m_pimpl->Running = true;

    m_pimpl->WorkerThread = std::make_unique<thread>(&StockUpdateWorker::Impl::Cron, m_pimpl.get());
    m_pimpl->WorkerThread->detach();
}

void StockUpdateWorker::Stop()
{
    std::lock_guard<std::mutex> lock(m_pimpl->WorkerMutex);
    (void)lock;

    if (!m_pimpl->Running) {
        return;
    }

    LOG_INFO("Stopping stock data updater process...");

    m_pimpl->Running = false;
    m_pimpl->WorkerThread->interrupt();

    m_pimpl->WorkerThread.reset();
}

StockUpdateWorker::Impl::Impl() :
    Running(false),
    StartImmediately(false)
{

}

StockUpdateWorker::Impl::~Impl()
{
    std::lock_guard<std::mutex> lock(WorkerMutex);
    (void)lock;

    WorkerThread->interrupt();
    WorkerThread.reset();
}

void StockUpdateWorker::Impl::Cron()
{
    if (!StartImmediately) {
        boost::this_thread::interruption_point();
        boost::this_thread::sleep_for(boost::chrono::seconds(Interval));
    }

    for (;;) {
        boost::this_thread::disable_interruption di;

        {
            std::lock_guard<std::mutex> lock(WorkerMutex);
            (void)lock;

            Update();

            if (!Running)
                break;
        }

        boost::this_thread::restore_interruption ri(di);
        (void)ri;
        boost::this_thread::interruption_point();
        boost::this_thread::sleep_for(boost::chrono::seconds(Interval));
    }
}

void StockUpdateWorker::Impl::Update()
{
    static const std::string TEMP_FILE(
                (boost::filesystem::path(Pool::Storage()->AppPath)
                 / boost::filesystem::path("..")
                 / boost::filesystem::path("tmp")
                 / STOCK_DATA_LOCAL_TEMP_FILE_NAME).string()
                );
    static const std::string WORK_DIR(
                (boost::filesystem::path(Pool::Storage()->AppPath)
                 / boost::filesystem::path("..")
                 / boost::filesystem::path("tmp")
                 / STOCK_DATA_TEMP_WORK_DIR_NAME).string()
                );

    string err;

    if (FileSystem::DirExists(WORK_DIR))
        FileSystem::Erase(WORK_DIR);

    if (FileSystem::FileExists(TEMP_FILE))
        FileSystem::Erase(TEMP_FILE);

    if (Http::Download(SourceURL, TEMP_FILE, err)) {
        if (Archiver::UnZip(TEMP_FILE, WORK_DIR, err)) {

            try {
                static const std::string SHARED_STRINGS_FILE(
                            (boost::filesystem::path(WORK_DIR)
                             / boost::filesystem::path("xl")
                             / boost::filesystem::path("sharedStrings.xml")).string()
                            );
                static const std::string SHEET1_FILE(
                            (boost::filesystem::path(WORK_DIR)
                             / boost::filesystem::path("xl")
                             / boost::filesystem::path("worksheets")
                             / boost::filesystem::path("sheet1.xml")).string()
                            );

                property_tree::ptree sharedStringsTree;
                property_tree::read_xml(SHARED_STRINGS_FILE, sharedStringsTree);

                vector<std::string> sharedStrings;

                BOOST_FOREACH(property_tree::ptree::value_type &siNode,
                              sharedStringsTree.get_child("sst")) {
                    auto t = siNode.second.get_child_optional("t");
                    if (t) {
                        sharedStrings.push_back(siNode.second.get<std::string>("t"));
                    }
                }

                property_tree::ptree sheet1Tree;
                property_tree::read_xml(SHEET1_FILE, sheet1Tree);

                string date;
                string time;
                string createTableFields = " r INTEGER NOT NULL, ";
                vector<string> tableFieldsId;

                cppdb::transaction guard(Pool::Database()->Sql());

                BOOST_FOREACH(property_tree::ptree::value_type &rowNode,
                              sheet1Tree.get_child("worksheet.sheetData")) {

                    long rRow(rowNode.second.get<long>("<xmlattr>.r", -1));

                    if (rRow == 2) {
                        Pool::Database()->DropTable("DATA_TITLES");
                        Pool::Database()->CreateTable("DATA_TITLES");
                    } else if (rRow == 3) {
                        createTableFields += " PRIMARY KEY ( r ) ";

                        /// We should set this each and every time
                        /// due to any possible changes in original
                        /// .xlsx file
                        Pool::Database()->SetTableFields("STOCK_DATA",
                                                         createTableFields);

                        Pool::Database()->DropTable("STOCK_DATA");
                        Pool::Database()->CreateTable("STOCK_DATA");
                    }

                    if (rRow > 2) {
                        Pool::Database()->Insert("STOCK_DATA",
                                                 "r",
                                                 { boost::lexical_cast<string>(rRow) });
                    }

                    size_t col = 0;

                    BOOST_FOREACH(property_tree::ptree::value_type &cNode,
                                  rowNode.second) {
                        if (cNode.first == "c") {
                            if (rRow == 1) {
                                string rC(cNode.second.get<std::string>("<xmlattr>.r", ""));
                                string tC(cNode.second.get<std::string>("<xmlattr>.t", ""));

                                if (rC == "A1" && tC == "s") {
                                    std::string v(sharedStrings[cNode.second.get<size_t>("v")]);

                                    static const regex eDate("(13)[1-9][1-9][\\/]((0[1-9]|1[012])|([1-9]))[\\/]((0[1-9]|[12][0-9]|3[01])|([1-9]))");
                                    static const regex eTime("[0-2][1-9][\\:](([0-9][0-9])|([0-9]))[\\:](([0-9][0-9])|([0-9]))");

                                    boost::smatch result;
                                    if (boost::regex_search(v, result, eDate)) {
                                        date.assign(result[0].first, result[0].second);
                                        if (date.size() != 10 || date.size() != 0) {
                                            std::vector<std::string> vec;
                                            boost::split(vec, date, boost::is_any_of(L"/"));
                                            if (vec.size() == 3) {
                                                date = (boost::format("%1%/%2%/%3%")
                                                        % vec[0]
                                                        % (vec[1].size() == 2 ? vec[1] : (boost::format("0%1%") % vec[1]).str())
                                                        % (vec[2].size() == 2 ? vec[2] : (boost::format("0%1%") % vec[2]).str())
                                                  ).str();
                                            }
                                        }
                                    }
                                    if (boost::regex_search(v, result, eTime)) {
                                        time.assign(result[0].first, result[0].second);
                                        if (time.size() != 10 || time.size() != 0) {
                                            std::vector<std::string> vec;
                                            boost::split(vec, time, boost::is_any_of(L":"));
                                            if (vec.size() == 3) {
                                                time = (boost::format("%1%:%2%:%3%")
                                                        % vec[0]
                                                        % (vec[1].size() == 2 ? vec[1] : (boost::format("0%1%") % vec[1]).str())
                                                        % (vec[2].size() == 2 ? vec[2] : (boost::format("0%1%") % vec[2]).str())
                                                  ).str();
                                            }
                                        }
                                    }

                                    try {
                                        cppdb::result r = Pool::Database()->Sql()
                                                << (boost::format("SELECT date, time"
                                                                  " FROM %1%"
                                                                  " ORDER BY ROWID ASC"
                                                                  " LIMIT 1;")
                                                    % Pool::Database()->GetTableName("LAST_UPDATE")).str()
                                                << cppdb::row;

                                        if (!r.empty()) {
                                            string lastUpdateDate;
                                            string lastUpdateTime;
                                            r >> lastUpdateDate >> lastUpdateTime;

                                            if (lastUpdateDate == date && lastUpdateTime == time) {
                                                guard.rollback();
                                                return;
                                            } else {
                                                if (lastUpdateDate != date) {
                                                    std::string archiveDataTitlesTableName(
                                                                GetTableNameFromDate("DATA_TITLES", lastUpdateDate));
                                                    std::string archiveStockDataTableName(
                                                                GetTableNameFromDate("STOCK_DATA", lastUpdateDate));

                                                    Pool::Database()->RenameTable("STOCK_DATA", archiveStockDataTableName);
                                                    Pool::Database()->RenameTable("DATA_TITLES", archiveDataTitlesTableName);

                                                    Pool::Database()->Insert("ARCHIVE",
                                                                             "date, time, datatitlestbl, stockdatatbl ",
                                                                             {
                                                                                 lastUpdateDate,
                                                                                 lastUpdateTime,
                                                                                 archiveDataTitlesTableName,
                                                                                 archiveStockDataTableName
                                                                             });
                                                }
                                            }
                                        }
                                    } catch (...) {
                                    }

                                    break;
                                }
                            } else if (rRow == 2) {
                                string tC(cNode.second.get<std::string>("<xmlattr>.t", ""));
                                if (tC == "s") {
                                    string rC(cNode.second.get<std::string>("<xmlattr>.r", ""));
                                    string v(sharedStrings[cNode.second.get<size_t>("v")]);

                                    Pool::Database()->Insert("DATA_TITLES",
                                                             "id, title",
                                                             { rC, v });

                                    tableFieldsId.push_back(rC);
                                    createTableFields += (boost::format(" [%1%] TEXT, ") % rC).str();
                                }
                            } else {
                                string tC(cNode.second.get<std::string>("<xmlattr>.t", ""));
                                string v(tC == "s"
                                         ? sharedStrings[cNode.second.get<size_t>("v")]
                                         : boost::lexical_cast<string>(cNode.second.get<double>("v")));
                                Pool::Database()->Update("STOCK_DATA",
                                                         "r",
                                                         boost::lexical_cast<string>(rRow),
                                                         (boost::format("%1%=?") % tableFieldsId[col]).str(),
                                                         { v });
                            }
                            ++col;
                        }
                    }
                }

                Pool::Database()->DropTable("LAST_UPDATE");
                Pool::Database()->CreateTable("LAST_UPDATE");
                Pool::Database()->Insert("LAST_UPDATE",
                                         "date, time",
                                         { date, time });

                guard.commit();
            }

            catch (boost::exception &ex) {
                LOG_ERROR(boost::diagnostic_information(ex));
            }

            catch (std::exception &ex) {
                LOG_ERROR(ex.what());
            }

            catch (...) {
                LOG_ERROR("StockUpdateWorker::Impl::Update(): Unknown error!");
            }

        } else {
            LOG_ERROR(err);
        }
    } else {
        LOG_ERROR(err);
    }

    if (FileSystem::DirExists(WORK_DIR))
        FileSystem::Erase(WORK_DIR);

    if (FileSystem::FileExists(TEMP_FILE))
        FileSystem::Erase(TEMP_FILE);
}

std::string StockUpdateWorker::Impl::GetTableNameFromDate(const std::string &id, const std::string &date)
{
    return  (boost::format("archive__%1%__%2%")
             % Pool::Database()->GetTableName(id)
             % boost::replace_all_copy(date, "/", "_")).str();
}

