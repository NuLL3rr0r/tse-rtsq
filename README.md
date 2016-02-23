TSE RTSQ
========

Tehran Stock Exchange. Real-Time Stock Quotes

Data provided by http://www.tsetmc.com/


## License

(The MIT License)

Copyright (c) 2015-2016 Mohammad S. Babaei

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ‘Software’), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ‘AS IS’, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


## A Work in Progress

!! This is a work in progress and is not even an MVP yet !!

__Note__: An older but fully functional version of this program can be found on one of the following repositories:

https://gitlab.com/NuLL3rr0r/kargozaar.com

https://github.com/NuLL3rr0r/kargozaar.com


## Documentation

I used a bit of doxygen inside the code. But, I'll try to add more documentation if my schedule allows me. For now, I'll put the minimum required documentation to build, configure and execute the server.

More comes when it's ready.


## Dependencies

* C++11 is the minimum required version of C++
* Bootsrap 3.x (Included)
* Boost (bzip2, date_time, filesystem, iostreams, random, regex, system, thread, zlib)
* CMake (Ninja, BSD Make and GNU Make are supported)
* CppDB
* Crypto++
* cURLpp
* GeoIP
* Git (Optional, Required by CMake during the build process)
* Gulp (Upon finding Gulp automatically installs: autoprefixer, minify-html, imagemin, minify-css, sass, uglify)
* jQuery (Included)
* libb64
* libcurl
* libstatgrab >= 0.90+
* libzip
* Magick++ (either ImageMagick or GraphicsMagick)
* MariaDB / MySQL (Optional)
* node.js (Required by Gulp)
* npm (Required by Gulp)
* PostgreSQL (Optional)
* pthread on POSIX-compliant systems
* SQLite 3 (Optional)
* VMime
* Wt (Witty) Web-toolkit

__Note__: Between PostgreSQL, SQLite 3 and MariaDB / MySQL you should choose at least one.


## Database

Although the core library of the project supports PostgreSQL and MariaDB / MySQL in addition to SQLite 3, the intention for all top-level projects like the REST API and Website is to work only on PostgreSQL. You may need to get your hands a bit dirty if you need PostgreSQL or MariaDB / MySQL support. Pull requests are very welcome due to my limited time. Right now the REST project` relies on SQLite 3 - which will change to PostgreSQL soon - and the frontend project relies on PostgreSQL as its database;

    sudo -u pgsql psql -d template1
    template1=# CREATE ROLE tse_rtsq_user LOGIN ENCRYPTED PASSWORD '${SECRET_PASSWORD}' NOINHERIT VALID UNTIL 'infinity';
    template1=# CREATE DATABASE tse_rtsq_db WITH ENCODING='UTF8' OWNER tse_rtsq_user;
    template1=# \q
    sudo -u pgsql -H psql -d tse_rtsq_db
    tse_rtsq_db=> \q
    sudo -u pgsql pg_dump tse_rtsq_db

After creating the database, in the next step you have to modify ${PROJECT_ROOT}/definitions.cmake.


## Supported Platforms

I developed this on a FreeBSD x64 machine and tested it on another FreeBSD x86 VPS successfully. It should work 99.99 percent on most Linux boxes. Support for Microsoft Windows needs a bit of work, I tried my best for most parts, but did not have the time to make the whole project work on Windows, too.


## Build

You can build either using Ninja or traditional build systems such as BSD Make or GNU Make. It also supports Qt Creator as an IDE. To start building Qt Creator which supports CMake, Ninja and Make simply drag CMakeLists.txt - located in the root directory of the project - to Qt Creator's main window. Alternatively follow the following instructions if you want to build from command line:

To build using Ninja:

    $ cd tse-rtsq
    $ mkdir build
    $ cd build
    $ sudo cmake -GNinja -DCMAKE_BUILD_TYPE=ClangNativeMaxSpeedRel -DAPP_ROOT_DIR=/srv/babaei.net/tse-rtsq ../
    $ ninja
    $ sudo ninja install

CMAKE_BUILD_TYPE possible values
* None
* Debug
* Release
* RelWithDebInfo
* MinSizeRel
* GCCMinSizeRel
* GCCMaxSpeedRel
* GCCNativeMinSizeRel
* GCCNativeMaxSpeedRel
* ClangMinSizeRel
* ClangMaxSpeedRel
* ClangNativeMinSizeRel
* ClangNativeMaxSpeedRel

__Note__: For other available options please build using CMake GUI.

To build using either BSD Make or GNU Make:

    $ cd tse-rtsq
    $ mkdir build
    $ cd build
    $ sudo cmake -GNinja -DCMAKE_BUILD_TYPE=ClangNativeMaxSpeedRel -DAPP_ROOT_DIR=/srv/babaei.net/tse-rtsq ../

make -j<NUMBER_OF_CPU_CORES+1>, e.g.

    $ make -j9
    $ sudo make install

Or, if you are using GNU Make on *BSD operating systems:

    $ gmake -j9
    $ sudo gmake install

__Note__: In the ${PROJECT_ROOT} directory you can customize the build by modifying the following files:

* build-config.cmake
* definitions.cmake
* product-info.cmake


# spawn-whttpd.db and Nginx example configuration

spawn-whttpd.db

*spawn-whttpd.db* should be placed inside *${APP_ROOT_DIRECTORY}/db* directory.

    {
        "apps" :
        [
            {
                "app"                     :  "/srv/babaei.net/tse-rtsq/www/tse-rtsq-backend.app",
                "workdir"                 :  "/srv/babaei.net/tse-rtsq/www/",
                "threads"                 :  "-1",
                "servername"              :  "",
                "docroot"                 :  "/srv/babaei.net/tse-rtsq/www/;/favicon.ico,/favicon.png,/css,/img,/js,/resources",
                "approot"                 :  "/srv/babaei.net/tse-rtsq/www/",
                "errroot"                 :  "/srv/.nginx/err/",
                "accesslog"               :  "",
                "compression"             :  "no",
                "deploy-path"             :  "/",
                "session-id-prefix"       :  "",
                "pid-file"                :  "/srv/babaei.net/tse-rtsq/tmp/tse-rtsq-backend.app",
                "config"                  :  "/srv/babaei.net/tse-rtsq/etc/wt_config.xml",
                "max-memory-request-size" :  "8388608",
                "gdb"                     :  "no",
                "http-address"            :  "127.0.0.1",
                "http-port"               :  "10100",
                "https-address"           :  "",
                "https-port"              :  "",
                "ssl-certificate"         :  "",
                "ssl-private-key"         :  "",
                "ssl-tmp-dh"              :  "",
                "ssl-enable-v3"           :  "",
                "ssl-client-verification" :  "",
                "ssl-verify-depth"        :  "",
                "ssl-ca-certificates"     :  "",
                "ssl-cipherlist"          :  ""
            },
            {
                "app"                     :  "/srv/babaei.net/tse-rtsq/www/tse-rtsq-frontend.app",
                "workdir"                 :  "/srv/babaei.net/tse-rtsq/www/",
                "threads"                 :  "-1",
                "servername"              :  "",
                "docroot"                 :  "/srv/babaei.net/tse-rtsq/www/;/favicon.ico,/favicon.png,/css,/img,/js,/resources",
                "approot"                 :  "/srv/babaei.net/tse-rtsq/www/",
                "errroot"                 :  "/srv/.nginx/err/",
                "accesslog"               :  "",
                "compression"             :  "no",
                "deploy-path"             :  "/",
                "session-id-prefix"       :  "",
                "pid-file"                :  "/srv/babaei.net/tse-rtsq/tmp/tse-rtsq-frontend.app",
                "config"                  :  "/srv/babaei.net/tse-rtsq/etc/wt_config.xml",
                "max-memory-request-size" :  "8388608",
                "gdb"                     :  "no",
                "http-address"            :  "127.0.0.1",
                "http-port"               :  "10200",
                "https-address"           :  "",
                "https-port"              :  "",
                "ssl-certificate"         :  "",
                "ssl-private-key"         :  "",
                "ssl-tmp-dh"              :  "",
                "ssl-enable-v3"           :  "",
                "ssl-client-verification" :  "",
                "ssl-verify-depth"        :  "",
                "ssl-ca-certificates"     :  "",
                "ssl-cipherlist"          :  ""
            }
        ]
    }

Nginx

    map $http_upgrade $connection_upgrade {
        default upgrade;
        '' close;
    }

    upstream TseRtsqBackendServer {
        server 127.0.0.1:10100;
    }

    upstream TseRtsqFrontendServer {
        server 127.0.0.1:10200;
    }

    server {
        listen        80;
        server_tokens off;
        server_name   tse-rtsq.babaei.net;

        #error_log     /srv/babaei.net/log/tse-rtsq_error_log;
        #access_log    /srv/babaei.net/log/tse-rtsq_access_log;

        charset utf-8;
        merge_slashes on;

        location /StockMarket {
            gzip off;

            proxy_read_timeout 300;
            proxy_connect_timeout 300;
            proxy_redirect off;

            proxy_http_version 1.1;
            proxy_set_header Upgrade $http_upgrade;
            proxy_set_header Connection $connection_upgrade;
            proxy_set_header Host $http_host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-Ssl on;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
            proxy_set_header X-Frame-Options SAMEORIGIN;

            proxy_pass http://TseRtsqBackendServer;
        }

        location / {
            gzip off;

            proxy_read_timeout 300;
            proxy_connect_timeout 300;
            proxy_redirect off;

            proxy_http_version 1.1;
            proxy_set_header Upgrade $http_upgrade;
            proxy_set_header Connection $connection_upgrade;
            proxy_set_header Host $http_host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-Ssl on;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
            proxy_set_header X-Frame-Options SAMEORIGIN;

            proxy_pass http://TseRtsqFrontendServer;
        }
    }


# spawn-fastcgi.db and Nginx example configuration

spawn-fastcgi.db

*spawn-fastcgi.db* should be placed inside *${APP_ROOT_DIRECTORY}/db* directory.

    {
        "apps" :
        [
            {
                "app"     :  "/srv/babaei.net/tse-rtsq/www/tse-rtsq-backend.app",
                "config"  :  "/srv/babaei.net/tse-rtsq/etc/wt_config.xml",
                "socket"  :  "/srv/babaei.net/tse-rtsq/tmp/tse-rtsq-backend.socket",
                "workdir" :  "/srv/babaei.net/tse-rtsq/www/"
            },
            {
                "app"     :  "/srv/babaei.net/tse-rtsq/www/tse-rtsq-frontend.app",
                "config"  :  "/srv/babaei.net/tse-rtsq/etc/wt_config.xml",
                "socket"  :  "/srv/babaei.net/tse-rtsq/tmp/tse-rtsq-frontend.socket",
                "workdir" :  "/srv/babaei.net/tse-rtsq/www/"
            }
        ]
    }

Nginx

    server {
        listen        80;
        server_tokens off;
        server_name   tse-rtsq.babaei.net;

        #error_log     /srv/babaei.net/log/tse-rtsq_error_log;
        #access_log    /srv/babaei.net/log/tse-rtsq_access_log;

        root           /srv/babaei.net/tse-rtsq/www;
        index          index.html tse-rtsq-backend.app;
        fastcgi_index  tse-rtsq-backend.app;

        charset utf-8;
        merge_slashes on;

        location /tse-rtsq-backend.app {
            fastcgi_pass   unix:/srv/babaei.net/tse-rtsq/tmp/tse-rtsq-backend.socket;

            fastcgi_param  QUERY_STRING       $query_string;
            fastcgi_param  REQUEST_METHOD     $request_method;
            fastcgi_param  CONTENT_TYPE       $content_type;
            fastcgi_param  CONTENT_LENGTH     $content_length;

            if ($document_uri ~ "^/tse-rtsq-backend.app/(.*)") {
                set $apache_path_info /$1;
            }

            fastcgi_param  SCRIPT_NAME        /babaei.net/tse-rtsq/www/tse-rtsq-backend.app;
            fastcgi_param  PATH_INFO          $apache_path_info;
            fastcgi_param  REQUEST_URI        $request_uri;
            fastcgi_param  DOCUMENT_URI       $document_uri;
            fastcgi_param  DOCUMENT_ROOT      $document_root;
            fastcgi_param  SERVER_PROTOCOL    $server_protocol;

            fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
            fastcgi_param  SERVER_SOFTWARE    nginx/$nginx_version;

            fastcgi_param  REMOTE_ADDR        $remote_addr;
            fastcgi_param  REMOTE_PORT        $remote_port;
            fastcgi_param  SERVER_ADDR        $server_addr;
            fastcgi_param  SERVER_PORT        $server_port;
            fastcgi_param  SERVER_NAME        $server_name;
        }

        location /tse-rtsq-frontend.app {
            fastcgi_pass   unix:/srv/babaei.net/tse-rtsq/tmp/tse-rtsq-frontend.socket;

            fastcgi_param  QUERY_STRING       $query_string;
            fastcgi_param  REQUEST_METHOD     $request_method;
            fastcgi_param  CONTENT_TYPE       $content_type;
            fastcgi_param  CONTENT_LENGTH     $content_length;

            if ($document_uri ~ "^/tse-rtsq-frontend.app/(.*)") {
                set $apache_path_info /$1;
            }

            fastcgi_param  SCRIPT_NAME        /babaei.net/tse-rtsq/www/tse-rtsq-frontend.app;
            fastcgi_param  PATH_INFO          $apache_path_info;
            fastcgi_param  REQUEST_URI        $request_uri;
            fastcgi_param  DOCUMENT_URI       $document_uri;
            fastcgi_param  DOCUMENT_ROOT      $document_root;
            fastcgi_param  SERVER_PROTOCOL    $server_protocol;

            fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
            fastcgi_param  SERVER_SOFTWARE    nginx/$nginx_version;

            fastcgi_param  REMOTE_ADDR        $remote_addr;
            fastcgi_param  REMOTE_PORT        $remote_port;
            fastcgi_param  SERVER_ADDR        $server_addr;
            fastcgi_param  SERVER_PORT        $server_port;
            fastcgi_param  SERVER_NAME        $server_name;
        }
    }


## Sreenshots

![Admin Login](/Screenshots/admin_login.png?raw=true "Admin Login")
![Admin Login (Responsive)](/Screenshots/admin-login-resonsive.png?raw=true "Admin Login (Responsive)")
![Farsi Admin Login, Recover Password](/Screenshots/admin-login-recover-password-fa.png?raw=true "Farsi Admin Login, Recover Password")

![System Monitor](/Screenshots/cms-system-monitor-1.png?raw=true "System Monitor")

![System Monitor](/Screenshots/cms-system-monitor-2.png?raw=true "System Monitor")

![Farsi System Monitor](/Screenshots/cms-system-monitor-fa.png?raw=true "Farsi System Monitor")

More screenshots to come, as it makes progress.


