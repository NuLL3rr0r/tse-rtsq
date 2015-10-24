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
 * Main entry for REST project.
 */


#include <csignal>
#include <cstdlib>
#if defined ( _WIN32 )
#include <windows.h>
#else
#include <unistd.h>
#endif  // defined ( _WIN32 )
#include <boost/algorithm/string.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <Wt/WServer>
#include <CoreLib/CoreLib.hpp>
#include <CoreLib/Database.hpp>
#include <CoreLib/Exception.hpp>
#include <CoreLib/Log.hpp>
#include <CoreLib/make_unique.hpp>
#include <CoreLib/System.hpp>
#include "Exception.hpp"
#include "Pool.hpp"
#include "PublicApiResource.hpp"
#include "VersionInfo.hpp"

void Terminate [[noreturn]] (int signo);
void InitializeDatabase();

#if defined ( __unix__ )
int main(int argc, char **argv, char **envp)
#else
int main(int argc, char **argv)
#endif  // defined ( __unix__ )
{
    try {
        /// Gracefully handling SIGTERM
        void (*prev_fn)(int);
        prev_fn = signal(SIGTERM, Terminate);
        if (prev_fn == SIG_IGN)
            signal(SIGTERM, SIG_IGN);


        /// Extract the executable path and name
        boost::filesystem::path path(boost::filesystem::initial_path<boost::filesystem::path>());
        if (argc > 0 && argv[0] != NULL)
            path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));
        std::string appId(path.filename().string());
        std::string appPath(boost::algorithm::replace_last_copy(path.string(), appId, ""));
        Rest::Pool::Storage()->AppPath = appPath;


        /// Force changing the current path to executable path
        boost::filesystem::current_path(appPath);


        /// Initializing CoreLib
        CoreLib::CoreLibInitialize(argc, argv);


        /// Initializing log system
        CoreLib::Log::Initialize(std::cout,
                                 (boost::filesystem::path(appPath)
                                  / boost::filesystem::path("..")
                                  / boost::filesystem::path("log")).string(),
                                 "JobApplicationSystemServer");


        /// Acquiring process lock
        std::string lockId;
#if defined ( __unix__ )
        int lock;
        lockId = (boost::filesystem::path(appPath)
                  / boost::filesystem::path("..")
                  / boost::filesystem::path("tmp")
                  / (appId + ".lock")).string();
#elif defined ( _WIN32 )
        HANDLE lock;
        lockId = appId;
#endif  // defined ( __unix__ )


        /// Logging application info
        LOG_INFO("Version Information", "", "BUILD_COMPILER             " VERSION_INFO_BUILD_COMPILER, "BUILD_DATE                 " VERSION_INFO_BUILD_DATE, "BUILD_HOST                 " VERSION_INFO_BUILD_HOST, "BUILD_PROCESSOR            " VERSION_INFO_BUILD_PROCESSOR, "BUILD_SYSTEM               " VERSION_INFO_BUILD_SYSTEM, "PRODUCT_COMPANY_NAME       " VERSION_INFO_PRODUCT_COMPANY_NAME, "PRODUCT_COPYRIGHT          " VERSION_INFO_PRODUCT_COPYRIGHT, "PRODUCT_INTERNAL_NAME      " VERSION_INFO_PRODUCT_INTERNAL_NAME, "PRODUCT_NAME               " VERSION_INFO_PRODUCT_NAME, "PRODUCT_VERSION            " VERSION_INFO_PRODUCT_VERSION, "PRODUCT_DESCRIPTION        " VERSION_INFO_PRODUCT_DESCRIPTION);


        if(!CoreLib::System::GetLock(lockId, lock)) {
            LOG_WARNING("Process is already running!");
        } else {
            LOG_INFO("Got the process lock!");
        }


        /// Initialize the database structure
        InitializeDatabase();

        /// Starting the server, otherwise going down
        LOG_INFO("Starting stock market RESTful server...");
        Wt::WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        Rest::PublicApiResource publicApiResource;
        server.addResource(&publicApiResource, "/StockMarket");
        if (server.start()) {
            int sig = Wt::WServer::waitForShutdown();
            server.stop();

#if defined ( __unix__ )
            /// Experimental, UNIX only
            if (sig == SIGHUP)
              Wt::WServer::restart(argc, argv, envp);
#endif  // defined ( __unix__ )
        }
    }

    catch (Rest::Exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (CoreLib::Exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (Wt::WServer::Exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (boost::exception &ex) {
        LOG_ERROR(boost::diagnostic_information(ex));
    }

    catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (...) {
        LOG_ERROR("Unknown error!");
    }

    return EXIT_SUCCESS;
}

void Terminate(int signo)
{
    LOG_WARNING((boost::format("Terminating by signal %1%...") % signo).str());
    exit(1);
}

void InitializeDatabase()
{
    try {
        Rest::Pool::Database()->RegisterTable("LAST_UPDATE", "lastupdate",
                                              " date TEXT NOT NULL, "
                                              " time TEXT NOT NULL ");
        Rest::Pool::Database()->RegisterTable("DATA_TITLES", "datatitles",
                                              " id TEXT NOT NULL, "
                                              " title TEXT NOT NULL, "
                                              " PRIMARY KEY ( id ) ");
        Rest::Pool::Database()->RegisterTable("STOCK_DATA", "stockdata",
                                              " ");
        Rest::Pool::Database()->RegisterTable("ARCHIVE", "archive",
                                              " date TEXT NOT NULL, "
                                              " time TEXT NOT NULL, "
                                              " datatitlestbl TEXT NOT NULL, "
                                              " stockdatatbl TEXT NOT NULL, "
                                              " PRIMARY KEY ( date ) ");
        Rest::Pool::Database()->Initialize();
    }

    catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (...) {
        LOG_ERROR("Unknown error!");
    }
}

