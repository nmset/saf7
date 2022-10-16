/* 
 * File:   globals.h
 * Author: SET - nmset@yandex.com
 * License : GPL v2
 * Copyright SET - © 2017
 *
 * Created on 6 mai 2017, 18:23
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
using namespace std;

#define _APPNAME_ "saf7" // Send and forget
#define _APP_DESCRIPTION_ "Send and forget an XMPP message"
#define _VERSION_ "5"
#define _AUTHOR_ "SET, M.D., nmset@yandex.com"
#define _ABOUT_ string(_APPNAME_) + string(" - version ") + string(_VERSION_) \
        + string(" - ") + string(_APP_DESCRIPTION_) \
        + string("\nAuthor : ") + string(_AUTHOR_) \
        + string("\nLicense : GPL v2")

#endif /* GLOBALS_H */

