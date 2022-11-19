/* 
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : GPL v2
 * Copyright Saleem Edah-Tally - © 2017
 *
 * Created on 2 juin 2017, 17:35
 */

#ifndef GCC_VERSION_H
#define GCC_VERSION_H

// Include at top of every file dealing with DBus

#if __GNUC__ == 7
#define DBUS_HAS_RECURSIVE_MUTEX
#endif

#endif /* GCC_VERSION_H */

