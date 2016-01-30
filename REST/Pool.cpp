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
 * A class that provides access to other objects using proxy design pattern.
 */


#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <CoreLib/Crypto.hpp>
#include <CoreLib/Database.hpp>
#include <CoreLib/make_unique.hpp>
#include <CoreLib/Log.hpp>
#include "Pool.hpp"

using namespace std;
using namespace Rest;

struct Pool::Impl
{
    typedef std::unique_ptr<StorageStruct> Storage_ptr;
    typedef std::unique_ptr<CoreLib::Crypto> Crypto_ptr;
    typedef std::unique_ptr<CoreLib::Database> Database_ptr;
    typedef std::unique_ptr<Rest::StockUpdateWorker> StockUpdateWorker_ptr;

    std::mutex StorageMutex;
    Storage_ptr StorageInstance;

    std::mutex DatabaseMutex;
    Database_ptr DatabaseInstance;

    std::mutex StockUpdateWorkerMutex;
    StockUpdateWorker_ptr StockUpdateWorkerInstance;

    std::mutex ClientTokenMutex;
    Crypto_ptr ClientTokenInstance;

    std::mutex ServerTokenMutex;
    Crypto_ptr ServerTokenInstance;
};

std::unique_ptr<Pool::Impl> Pool::s_pimpl = std::make_unique<Pool::Impl>();

Pool::StorageStruct *Pool::Storage()
{
    lock_guard<mutex> lock(s_pimpl->StorageMutex);
    (void)lock;

    if (s_pimpl->StorageInstance == nullptr) {
        s_pimpl->StorageInstance = std::make_unique<Pool::StorageStruct>();
    }

    return s_pimpl->StorageInstance.get();
}

CoreLib::Database *Pool::Database()
{
    lock_guard<mutex> lock(s_pimpl->DatabaseMutex);
    (void)lock;

    if (s_pimpl->DatabaseInstance == nullptr) {

#if DATABASE_BACKEND == PGSQL

#ifdef CORELIB_STATIC
#if defined ( HAS_CPPDB_PGSQL_DRIVER )
        if (!Database::IsPgSqlDriverLoaded()) {
            Database::LoadPgSqlDriver();
        }
#endif  // defined ( HAS_CPPDB_PGSQL_DRIVER )
#endif  // CORELIB_STATIC

        std::string parameters;
        std::string pgSqlHost(PGSQL_HOST);
        std::string pgSqlPort(PGSQL_PORT);
        std::string pgSqlDatabase(PGSQL_DATABASE);
        std::string pgSqlUser(PGSQL_USER);
        std::string pgSqlPassword(PGSQL_PASSWORD);

        if (boost::trim_copy(pgSqlHost) != "")
            parameters += (boost::format("host=%1%;") % pgSqlHost).str();

        if (boost::trim_copy(pgSqlPort) != "")
            parameters += (boost::format("port=%1%;") % pgSqlPort).str();

        if (boost::trim_copy(pgSqlDatabase) != "")
            parameters += (boost::format("dbname=%1%;") % pgSqlDatabase).str();

        if (boost::trim_copy(pgSqlUser) != "")
            parameters += (boost::format("user=%1%;") % pgSqlUser).str();

        if (boost::trim_copy(pgSqlPassword) != "")
            parameters += (boost::format("password=%1%;") % pgSqlPassword).str();

        static const std::string CONNECTION_STRING(
                    (boost::format("postgresql:%1%")
                     % parameters).str());

#elif DATABASE_BACKEND == MYSQL

#ifdef CORELIB_STATIC
#if defined ( HAS_CPPDB_MYSQL_DRIVER )
        if (!Database::IsMySqlDriverLoaded()) {
            Database::LoadMySqlDriver();
        }
#endif  // defined ( HAS_CPPDB_MYSQL_DRIVER )
#endif  // CORELIB_STATIC

        std::string parameters;
        std::string mySqlHost(MYSQL_HOST);
        std::string mySqlPort(MYSQL_PORT);
        std::string mySqlUnixSocket(MYSQL_UNIX_SOCKET);
        std::string mySqlDatabase(MYSQL_DATABASE);
        std::string mySqlUser(MYSQL_USER);
        std::string mySqlPassword(MYSQL_PASSWORD);

        if (boost::trim_copy(mySqlHost) != "")
            parameters += (boost::format("host=%1%;") % mySqlHost).str();

        if (boost::trim_copy(mySqlPort) != "")
            parameters += (boost::format("port=%1%;") % mySqlPort).str();

        if (boost::trim_copy(mySqlUnixSocket) != "")
            parameters += (boost::format("unix_socket=%1%;") % mySqlUnixSocket).str();

        if (boost::trim_copy(mySqlDatabase) != "")
            parameters += (boost::format("database=%1%;") % mySqlDatabase).str();

        if (boost::trim_copy(mySqlUser) != "")
            parameters += (boost::format("user=%1%;") % mySqlUser).str();

        if (boost::trim_copy(mySqlPassword) != "")
            parameters += (boost::format("password=%1%;") % mySqlPassword).str();

        static const std::string CONNECTION_STRING(
                    (boost::format("mysql:%1%")
                     % parameters).str());

#else   // SQLITE3

#if defined ( CORELIB_STATIC )
#if defined ( HAS_CPPDB_SQLITE3_DRIVER )
        if (!Database::IsSqlite3DriverLoaded()) {
            Database::LoadSQLite3Driver();
        }
#endif  // defined ( HAS_CPPDB_SQLITE3_DRIVER )
#endif  // defined ( CORELIB_STATIC )

        static const std::string DB_FILE((boost::filesystem::path(Storage()->AppPath)
                                          / boost::filesystem::path(SQLITE3_DATABASE_FILE_PATH)
                                          / boost::filesystem::path(SQLITE3_DATABASE_FILE_NAME)).string());
        static const std::string CONNECTION_STRING(
                    (boost::format("sqlite3:db=%1%")
                     % DB_FILE).str());

#if defined ( HAS_SQLITE3 )
        CoreLib::Database::Sqlite3Vacuum(DB_FILE);
#endif  // defined ( HAS_SQLITE3 )

#endif // DATABASE_BACKEND == PGSQL

        s_pimpl->DatabaseInstance = std::make_unique<CoreLib::Database>(CONNECTION_STRING);
    }

    return s_pimpl->DatabaseInstance.get();
}

Rest::StockUpdateWorker *Pool::StockUpdateWorker()
{
    lock_guard<mutex> lock(s_pimpl->StockUpdateWorkerMutex);
    (void)lock;

    if (s_pimpl->StockUpdateWorkerInstance == nullptr) {
        s_pimpl->StockUpdateWorkerInstance = std::make_unique<Rest::StockUpdateWorker>(
                    STOCK_DATA_SOURCE_URL,
                    STOCK_DATA_UPDATE_INTERVAL_SECONDS,
                    true
                    );
    }

    return s_pimpl->StockUpdateWorkerInstance.get();
}

CoreLib::Crypto *Pool::ClientToken()
{
    lock_guard<mutex> lock(s_pimpl->ClientTokenMutex);
    (void)lock;

    if (s_pimpl->ClientTokenInstance == nullptr) {
        static const string KEY = CoreLib::Crypto::HexStringToString(CLIENT_TOKEN_CRYPTO_KEY);
        static const string IV = CoreLib::Crypto::HexStringToString(CLIENT_TOKEN_CRYPTO_IV);

        s_pimpl->ClientTokenInstance =
                std::make_unique<CoreLib::Crypto>(reinterpret_cast<const CoreLib::Crypto::Byte_t *>(KEY.c_str()), KEY.size(),
                                                  reinterpret_cast<const CoreLib::Crypto::Byte_t *>(IV.c_str()), IV.size());
    }

    return s_pimpl->ClientTokenInstance.get();
}

CoreLib::Crypto *Pool::ServerToken()
{
    lock_guard<mutex> lock(s_pimpl->ServerTokenMutex);
    (void)lock;

    if (s_pimpl->ServerTokenInstance == nullptr) {
        static const string KEY = CoreLib::Crypto::HexStringToString(SERVER_TOKEN_CRYPTO_KEY);
        static const string IV = CoreLib::Crypto::HexStringToString(SERVER_TOKEN_CRYPTO_IV);

        s_pimpl->ServerTokenInstance =
                std::make_unique<CoreLib::Crypto>(reinterpret_cast<const CoreLib::Crypto::Byte_t *>(KEY.c_str()), KEY.size(),
                                                  reinterpret_cast<const CoreLib::Crypto::Byte_t *>(IV.c_str()), IV.size());
    }

    return s_pimpl->ServerTokenInstance.get();
}

