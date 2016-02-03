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


#ifndef REST_STOCK_UPDATE_WORKER_HPP
#define REST_STOCK_UPDATE_WORKER_HPP


#include <memory>
#include <mutex>
#include <string>
#include <boost/thread/thread.hpp>

namespace Rest {
class StockUpdateWorker;
}

class Rest::StockUpdateWorker
{
public:
    typedef boost::int_least64_t Interval;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    StockUpdateWorker();
    StockUpdateWorker(const std::string &sourceURL,
                      const Interval interval,
                      bool startImmediately = false);
    ~StockUpdateWorker();

public:
    std::string GetSourceURL();
    boost::int_least64_t GetInterval();

    void SetSourceURL(const std::string &sourceFile);
    void SetInterval(Interval interval);

public:
    bool IsRunning();
    void Start();
    void Stop();
};


#endif /* REST_STOCK_UPDATE_WORKER_HPP */

