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
 * Manage news section of the website.
 */


#include "CmsNews.hpp"
#include "Div.hpp"

using namespace std;
using namespace Wt;
using namespace Website;

struct CmsNews::Impl : public Wt::WObject
{
public:
    CmsNews *m_parent;

public:
    explicit Impl(CmsNews *parent);

public:
};

CmsNews::CmsNews(CgiRoot *cgi) :
    Page(cgi),
    m_pimpl(std::make_unique<CmsNews::Impl>(this))
{
    this->clear();
    this->setId("CmsNewsPage");
    this->addWidget(Layout());
    m_htmlRoot->addWidget(this);
}

WWidget *CmsNews::Layout()
{
    Div *container = new Div("CmsNews", "container");



    return container;
}

CmsNews::Impl::Impl(CmsNews *parent)
    : m_parent(parent)
{

}

