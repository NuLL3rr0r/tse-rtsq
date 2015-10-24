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
 * Provides some useful information about both the web server and client.
 */


#ifndef WEBSITE_CGIENV_HPP
#define WEBSITE_CGIENV_HPP


#include <string>

namespace Wt {
class WEnvironment;
}

namespace Website {
class CgiEnv;
}

class Website::CgiEnv
{
public:
    struct ClientInfoTag {
        std::string country_code;
        std::string country_code3;
        std::string country_name;
        std::string region;
        std::string city;
        std::string postal_code;
        std::string latitude;
        std::string longitude;
        std::string metro_code;
        std::string dma_code;
        std::string area_code;
        std::string charset;
        std::string continent_code;
        std::string netmask;
    } ClientInfoRecord;

    enum class ClientInfo : unsigned char {
        IP,
        Browser,
        Referer,
        Location
    };

    enum class ServerInfo : unsigned char {
        Host,
        URL,
        RootLoginUrl,
        NoReplyAddr
    };

    enum class Language : unsigned char {
        None,
        Invalid,
        En,
        Fa
    };

    enum class LanguageDirection : unsigned char {
        None,
        LeftToRight,
        RightToLeft
    };

public:
    struct SignedIn {
        std::string Username;
        std::string Email;
        struct LastLoginData {
            std::string IP;
            std::string Location;
            std::string LoginRawTime;
            std::string LoginGDate;
            std::string LoginJDate;
            std::string LoginTime;
            std::string UserAgent;
            std::string Referer;
        } LastLogin;
    } SignedInUser;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    explicit CgiEnv(const Wt::WEnvironment &env);
    virtual ~CgiEnv();

public:
    std::string GetClientInfo(const ClientInfo &key) const;
    std::string GetServerInfo(const ServerInfo &key) const;

    std::string GetInitialQueryString() const;

    const Language &GetCurrentLanguage() const;
    std::string GetCurrentLanguageString() const;

    const LanguageDirection &GetLanguageDirection(const Language &language) const;
    const LanguageDirection &GetCurrentLanguageDirection() const;

    bool FoundXSS() const;
    bool IsRootLoginRequested() const;
    bool IsRootLogoutRequested() const;
};


#endif /* WEBSITE_CGIENV_HPP */


