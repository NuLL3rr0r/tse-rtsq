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


# WARNING: YOU MUST CHANGE THESE VALUES !!
# ASCII to HEX converter  :  http://www.dolcevie.com/js/converter.html
# Random string generator :  https://strongpasswordgenerator.com/
# REST_CLIENT_TOKEN_CRYPTO_KEY: exactly 16, 24 or 32 chars
# REST_CLIENT_TOKEN_CRYPTO_IV: exactly 16, 24 or 32 chars
# REST_SERVER_TOKEN_CRYPTO_KEY: exactly 16, 24 or 32 chars
# REST_SERVER_TOKEN_CRYPTO_IV: exactly 16, 24 or 32 chars
# WEBSITE_ROOT_USERNAME: 4 to 16 chars
# WEBSITE_ROOT_INITIAL_EMAIL: ANY_VALID_EMAIL
# WEBSITE_ROOT_INITIAL_PASSWORD: 8 to 24 chars
# WEBSITE_CRYPTO_KEY: exactly 16, 24 or 32 chars
# WEBSITE_CRYPTO_IV: exactly 16, 24 or 32 chars

SET ( REST_CLIENT_TOKEN_CRYPTO_KEY  "2b:4c:3b:2f:32:5f:77:2c:7b:4f:54:40:33:3c:3f:6a" CACHE STRING "" ) # +L;/2_w,{OT@3<?j
SET ( REST_CLIENT_TOKEN_CRYPTO_IV   "6f:41:41:30:31:73:56:31:77:35:33:53:5b:71:6a:6a" CACHE STRING "" ) # oAA01sV1w53S[qjj
SET ( REST_SERVER_TOKEN_CRYPTO_KEY  "32:2b:3b:3c:5c:48:77:57:7b:20:5d:3c:5b:5b:4a" CACHE STRING "" ) # 2+;<\HwW{ ]<[[J
SET ( REST_SERVER_TOKEN_CRYPTO_IV   "3c:7a:34:5f:2a:44:36:21:5b:48:3d:30:32:42:5d:62" CACHE STRING "" ) # <z4_*D6![H=02B]b
SET ( WEBSITE_ROOT_USERNAME         "72:6f:6f:74" CACHE STRING "" ) # root
SET ( WEBSITE_ROOT_INITIAL_EMAIL    "6e:6f:2d:72:65:70:6c:79:40:62:61:62:61:65:69:2e:6e:65:74" CACHE STRING "" ) # no-reply@babaei.net
SET ( WEBSITE_ROOT_INITIAL_PASSWORD "60:21:64:33:46:61:75:6c:74:2d:2f:2e:3f" CACHE STRING "" ) # `!d3Fault-/.?
SET ( WEBSITE_CRYPTO_KEY            "4d:52:35:37:33:4a:32:39:78:41:37:36:2b:45:77:46" CACHE STRING "" ) # MR573J29xA76+EwF
SET ( WEBSITE_CRYPTO_IV             "36:39:48:64:41:68:35:32:34:7a:75:55:3d:35:6b:74" CACHE STRING "" ) # 69HdAh524zuU=5kt

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


