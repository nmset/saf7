/* 
 * File:   DBusSafWorkers.h
 * Author: SET - nmset@yandex.com
 * License : GPL v2
 * Copyright SET - © 2017
 *
 * Created on 7 mai 2017, 15:18
 */

#ifndef DBUSSAFWORKERS_H
#define DBUSSAFWORKERS_H

#include "dbus_saf_adaptor.hpp"
#include "dbus_saf_proxy.hpp"
#include "SAF.h"
#include <gloox/client.h>
#include <thread>

using namespace gloox;

/**
 * The DBus application instance to which clients may talk.
 * It is run in a thread and waits for messages to send.
 */
class DBusSafAdaptor : public xmpp::saf_adaptor,
public DBus::IntrospectableAdaptor,
public DBus::ObjectAdaptor
{
public:
    DBusSafAdaptor(DBus::Connection &connection, Connector * connector, SAFConnListener * saf);
    virtual ~DBusSafAdaptor();

    /**
     * Sends a message to a recipient.
     * @param recipient : a valid jid
     * @param msg
     */
    void SendMsg(const std::string& recipient, const std::string& msg) override;

    /**
     * Sends SIGUSR1 signal, leading to disconnection from server 
     * and application exit.
     */
    void Quit() override;

    /**
     * Enters the DBus loop.
     * @return 
     */
    static int Entry();

private:
    /**
     * gloox::Client object
     */
    Connector * m_connector;
    SAFConnListener * m_saf;
};

/**
 * Unused proxy class. We don't need to talk to other application instances.
 */
class DBusSafProxy : public xmpp::saf_proxy,
public DBus::IntrospectableProxy,
public DBus::ObjectProxy
{
    DBusSafProxy(DBus::Connection &connection, const char * service); // Unused
    virtual ~DBusSafProxy();
};

#endif /* DBUSSAFWORKERS_H */

