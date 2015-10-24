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


#include <mutex>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <CoreLib/Crypto.hpp>
#include <CoreLib/Database.hpp>
#include <CoreLib/make_unique.hpp>
#include <CoreLib/Log.hpp>
#include "Pool.hpp"
#include "Captcha.hpp"

using namespace std;
using namespace Website;

struct Pool::Impl
{
    typedef std::unique_ptr<StorageStruct> Storage_ptr;
    typedef std::unique_ptr<CoreLib::Crypto> Crypto_ptr;
    typedef std::unique_ptr<CoreLib::Database> Database_ptr;

    std::mutex StorageMutex;
    Storage_ptr StorageInstance;

    std::mutex CryptoMutex;
    Crypto_ptr CryptoInstance;

    std::mutex DatabaseMutex;
    Database_ptr DatabaseInstance;
};

std::unique_ptr<Pool::Impl> Pool::s_pimpl = std::make_unique<Pool::Impl>();

const int &Pool::StorageStruct::LanguageCookieLifespan() const
{
    // 365 Days * 24 Hours * 60 Minutes * 60 Seconds = Number of Days in Seconds
    static int duration = 365 * 24 * 60 * 60;
    return duration;
}

const std::string &Pool::StorageStruct::SkeletonKey() const
{
    static string skeletonKey;
    if (skeletonKey == "") {
        const unsigned char SKELETON_KEY[] = { 0x60, 0x21, 0x40, 0x23, 0x24, 0x24, 0x23, 0x40, 0x21, 0x7e, 0x2d, 0x5f, 0x25, 0x5e, 0x2f, 0x2e, 0x3f };
        Crypto()->Hash(
                    CoreLib::Crypto::ByteArrayToString(
                        SKELETON_KEY, sizeof(SKELETON_KEY)),
                    skeletonKey);
        Crypto()->Encrypt(skeletonKey, skeletonKey);
    }
    return skeletonKey;
}

const std::string &Pool::StorageStruct::RootUsername() const
{
    static string rootUsername;
    if (rootUsername == "") {
        // root
        const unsigned char ROOT_USERNAME[] = { 0x72, 0x6f, 0x6f, 0x74 };
        rootUsername = CoreLib::Crypto::ByteArrayToString(ROOT_USERNAME, sizeof(ROOT_USERNAME));
    }
    return rootUsername;
}

const std::string &Pool::StorageStruct::RootInitialPassword() const
{
    static string rootInitialPassowrd;
    if (rootInitialPassowrd == "") {
        // `!d3Fault-/.?
        const unsigned char ROOT_INITIAL_PASSWORD[] = { 0x60, 0x21, 0x64, 0x33, 0x46, 0x61, 0x75, 0x6c, 0x74, 0x2d, 0x2f, 0x2e, 0x3f };
        CoreLib::Crypto::Hash(
                    CoreLib::Crypto::ByteArrayToString(
                        ROOT_INITIAL_PASSWORD, sizeof(ROOT_INITIAL_PASSWORD)),
                    rootInitialPassowrd);
        Crypto()->Encrypt(rootInitialPassowrd, rootInitialPassowrd);
    }
    return rootInitialPassowrd;
}

const std::string &Pool::StorageStruct::RootInitialEmail() const
{
    static string rootInitialEmail;
    if (rootInitialEmail == "") {
        // no-reply@babaei.net
        const unsigned char ROOT_INITIAL_EMAIL[] = { 0x6e, 0x6f, 0x2d, 0x72, 0x65, 0x70, 0x6c, 0x79, 0x40, 0x62, 0x61, 0x62, 0x61, 0x65, 0x69, 0x2e, 0x6e, 0x65, 0x74 };
        rootInitialEmail = CoreLib::Crypto::ByteArrayToString(ROOT_INITIAL_EMAIL, sizeof(ROOT_INITIAL_EMAIL));
    }
    return rootInitialEmail;
}

const int &Pool::StorageStruct::RootSessionLifespan() const
{
    // 7 Days * 24 Hours * 60 Minutes * 60 Seconds = Number of Days in Seconds
    static int duration = 7 * 24 * 60 * 60;
    return duration;
}

const std::string &Pool::StorageStruct::RegexEmail() const
{
    static string regex("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+(?:[a-z]{2}|com|org|net|edu|gov|mil|biz|info|mobi|name|aero|asia|jobs|museum)\\b");
    return regex;
}

const int &Pool::StorageStruct::MinRootUsernameLength() const
{
    static int len = 4;
    return len;
}

const int &Pool::StorageStruct::MinUsernameLength() const
{
    static int len = 6;
    return len;
}

const int &Pool::StorageStruct::MaxUsernameLength() const
{
    static int len = 16;
    return len;
}

const int &Pool::StorageStruct::MinPasswordLength() const
{
    static int len = 8;
    return len;
}

const int &Pool::StorageStruct::MaxPasswordLength() const
{
    static int len = 24;
    return len;
}

Pool::StorageStruct *Pool::Storage()
{
    lock_guard<mutex> lock(s_pimpl->StorageMutex);
    (void)lock;

    if (s_pimpl->StorageInstance == nullptr) {
        s_pimpl->StorageInstance = std::make_unique<Pool::StorageStruct>();
    }

    return s_pimpl->StorageInstance.get();
}

CoreLib::Crypto *Pool::Crypto()
{
    lock_guard<mutex> lock(s_pimpl->CryptoMutex);
    (void)lock;

    if (s_pimpl->CryptoInstance == nullptr) {
        // Use this nice HEX/ASCII converter and your editor's replace dialog,
        // to create your own Key and IV.
        // http://www.dolcevie.com/js/converter.html
        // To generate a random password: https://strongpasswordgenerator.com/

        // MR573J29xA76+EwF
        static constexpr CoreLib::Crypto::Byte_t KEY[] = {
            0x4d, 0x52, 0x35, 0x37, 0x33, 0x4a, 0x32, 0x39, 0x78, 0x41, 0x37, 0x36, 0x2b, 0x45, 0x77, 0x46
        };

        // 69HdAh524zuU=5kt
        static constexpr CoreLib::Crypto::Byte_t IV[] = {
            0x36, 0x39, 0x48, 0x64, 0x41, 0x68, 0x35, 0x32, 0x34, 0x7a, 0x75, 0x55, 0x3d, 0x35, 0x6b, 0x74
        };

        s_pimpl->CryptoInstance =
                std::make_unique<CoreLib::Crypto>(KEY, sizeof(KEY), IV, sizeof(IV));
    }

    return s_pimpl->CryptoInstance.get();
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

