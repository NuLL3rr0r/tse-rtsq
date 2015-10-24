#  (The MIT License)
#
#  Copyright (c) 2015 Mohammad S. Babaei
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.


SET ( STOCK_DATA_UPDATE_INTERVAL_SECONDS "120" CACHE STRING "" )
SET ( STOCK_DATA_SOURCE_URL "http://members.tsetmc.com/tsev2/excel/MarketWatchPlus.aspx?d=0" CACHE STRING "" )

SET ( DATABASE_BACKEND "PGSQL" CACHE STRING "" )
SET_PROPERTY( CACHE DATABASE_BACKEND PROPERTY STRINGS "SQLITE3" "PGSQL" "MYSQL" )

SET ( SQLITE3_DATABASE_FILE_PATH "../db/" CACHE STRING "" )
SET ( SQLITE3_DATABASE_FILE_NAME "tse-rtsq.db" CACHE STRING "" )

SET ( PGSQL_HOST "localhost" CACHE STRING "" )
SET ( PGSQL_PORT "5432" CACHE STRING "" )
SET ( PGSQL_DATABASE "tse_rtsq_db" CACHE STRING "" )
SET ( PGSQL_USER "tse_rtsq_user" CACHE STRING "" )
SET ( PGSQL_PASSWORD "BE_SURE_TO_USE_A_STRONG_SECRET_PASSPHRASE_HERE" CACHE STRING "" )

SET ( MYSQL_HOST "localhost" CACHE STRING "" )
SET ( MYSQL_PORT "3306" CACHE STRING "" )
SET ( MYSQL_UNIX_SOCKET "" CACHE STRING "" )
SET ( MYSQL_DATABASE "tse_rtsq_db" CACHE STRING "" )
SET ( MYSQL_USER "tse_rtsq_user" CACHE STRING "" )
SET ( MYSQL_PASSWORD "BE_SURE_TO_USE_A_STRONG_SECRET_PASSPHRASE_HERE" CACHE STRING "" )

SET ( LIBB64_BUFFERSIZE "16777216" CACHE STRING "" )

SET ( GEO_LITE_COUNTRY_DB_URL "http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz" CACHE STRING "" )
SET ( GEO_LITE_CITY_DB_URL "http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz" CACHE STRING "" )

IF ( WIN32 )
    SET ( CORELIB_DEFINES "_UNICODE; UNICODE; WIN32_LEAN_AND_MEAN" CACHE STRING "" )
ELSE (  )
    SET ( CORELIB_DEFINES "" CACHE STRING "" )
ENDIF (  )

IF ( WIN32 )
    SET ( REST_DEFINES "_UNICODE; UNICODE; WIN32_LEAN_AND_MEAN" CACHE STRING "" )
ELSE (  )
    SET ( REST_DEFINES "" CACHE STRING "" )
ENDIF (  )

IF ( WIN32 )
    SET ( UTILS_DEFINES "_UNICODE; UNICODE; WIN32_LEAN_AND_MEAN" CACHE STRING "" )
ELSE (  )
    SET ( UTILS_DEFINES "" CACHE STRING "" )
ENDIF (  )

IF ( WIN32 )
    SET ( WEBSITE_DEFINES "_UNICODE; UNICODE; WIN32_LEAN_AND_MEAN" CACHE STRING "" )
ELSE (  )
    SET ( WEBSITE_DEFINES "" CACHE STRING "" )
ENDIF (  )


