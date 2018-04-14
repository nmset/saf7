/* 
 * File:   DBusSafWorkers.cpp
 * Author: SET - nmset@netcourrier.com
 * License : GPL v2
 * Copyright SET - © 2017
 * 
 * Created on 7 mai 2017, 15:18
 */

#include "gcc_version.h"
#include "DBusSafWorkers.h"
#include <gloox/message.h>
#include <signal.h>

using namespace std;

extern bool g_verbose;

DBusSafAdaptor::DBusSafAdaptor(DBus::Connection &connection, Connector * connector, SAFConnListener * saf)
: DBus::ObjectAdaptor(connection, "/xmpp/saf")
{
    m_connector = connector;
    m_saf = saf;
    thread t(Entry);
    t.detach();
}

DBusSafAdaptor::~DBusSafAdaptor()
{
    DBus::default_dispatcher->leave();
}

int DBusSafAdaptor::Entry()
{
    DBus::default_dispatcher->enter(); // IS BLOCKING
    return 0;
}

void DBusSafAdaptor::Quit()
{
    // Handled in main.cpp
    raise(SIGTERM);
}

void DBusSafAdaptor::SendMsg(const std::string& recipient, const std::string& msg)
{
    const Message m(Message::MessageType::Chat, recipient, msg);
    m_saf->GetClient()->send(m);
}

DBusSafProxy::DBusSafProxy(DBus::Connection& connection, const char* service)
: DBus::ObjectProxy(connection, "/xmpp/saf", service)
{
}

DBusSafProxy::~DBusSafProxy()
{

}
