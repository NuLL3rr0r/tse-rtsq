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
 * Change admin password.
 */


#include "CmsChangePassword.hpp"
#include "Div.hpp"

using namespace std;
using namespace Wt;
using namespace Website;

struct CmsChangePassword::Impl : public Wt::WObject
{
public:
    CmsChangePassword *m_parent;

public:
    explicit Impl(CmsChangePassword *parent);

public:
};

CmsChangePassword::CmsChangePassword(CgiRoot *cgi) :
    Page(cgi),
    m_pimpl(std::make_unique<CmsChangePassword::Impl>(this))
{
    this->clear();
    this->setId("CmsChangePasswordPage");
    this->addWidget(Layout());
    m_htmlRoot->addWidget(this);
}

WWidget *CmsChangePassword::Layout()
{
    Div *container = new Div("CmsChangePassword", "container");



    return container;
}

CmsChangePassword::Impl::Impl(CmsChangePassword *parent)
    : m_parent(parent)
{

}

