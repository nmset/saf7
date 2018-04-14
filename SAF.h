/* 
 * File:   SAF.h
 * Author: SET - nmset@netcourrier.com
 * License : GPL v2
 * Copyright SET - © 2017
 *
 * Created on 6 mai 2017, 21:50
 */

#ifndef SAF_H
#define SAF_H

#include <iostream>
using namespace std;
#include <gloox/client.h>
#include <gloox/connectionlistener.h>
using namespace gloox;

class Connector;
class SAFConnListener;

/**
 * gloox's connection listener.
 */
class SAFConnListener : public ConnectionListener
{
public:
    SAFConnListener(Client * client);
    virtual ~SAFConnListener();

    virtual void onConnect() override;
    /**
     * Shows significant error messages.
     * @param e
     */
    virtual void onDisconnect(ConnectionError e) override;
    /**
     * Mandatory. Always returns true.
     * @param info
     * @return 
     */
    virtual bool onTLSConnect(const CertInfo& info) override;

    ConnectionError GetLastError()
    {
        return m_lastError;
    }

    Client * GetClient() const
    {
        return m_client;
    }

    void SetConnector(Connector * connector)
    {
        m_connector = connector;
    }
    /**
     * A server may disconnect an idle client.
     * Prevents the application from being seen as idle.
     * @param client
     * @param saf
     */
    static void WhiteSpacePing(Client * client, SAFConnListener * saf);
private:
    typedef map<int, string> mapErrors;
    mapErrors ConnErrorMap;

    Client * m_client;
    Connector * m_connector;
    ConnectionError m_lastError;
};

/**
 * Tries to connect to the server.
 * Tries to reconnect if the server goes down,
 * of if the server is not yet up.
 * In all other cases where Client::connect fails,
 * the application is allowed to exit.
 */
class Connector
{
public:
    Connector(SAFConnListener * saf, const uint& retry, const uint& delay);
    /**
     * Disconnects from server.
     */
    virtual ~Connector();

    void DoConnect();

    /**
     * Resets the number of times we have tried to (re)connect.
     */
    void ResetCounter()
    {
        m_count = 0;
    }
private:
    SAFConnListener * m_saf;
    uint m_retry;
    uint m_delay;
    uint m_count;
};

#endif /* SAF_H */

