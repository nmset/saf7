/* 
 * File:   SAF.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : GPL v2
 * Copyright Saleem Edah-Tally - © 2017
 * 
 * Created on 6 mai 2017, 21:50
 */

#include "SAF.h"
#include "globals.h"
#include <unistd.h>
#include <thread>

extern bool g_verbose;
extern uint g_wsping;
extern Client * g_client;

SAFConnListener::SAFConnListener(Client * client)
{
    m_client = client;
    m_lastError = ConnectionError::ConnNotConnected;

    ConnErrorMap[ConnectionError::ConnNoError] = "Not really an error. Everything went just fine. ";
    ConnErrorMap[ConnectionError::ConnStreamError] = "A stream error occured. The stream has been closed.";
    ConnErrorMap[ConnectionError::ConnStreamVersionError] = "The incoming stream's version is not supported.";
    ConnErrorMap[ConnectionError::ConnStreamClosed] = "The stream has been closed (by the server).";
    ConnErrorMap[ConnectionError::ConnProxyAuthRequired] = "The HTTP/SOCKS5 proxy requires authentication.";
    ConnErrorMap[ConnectionError::ConnProxyAuthFailed] = "HTTP/SOCKS5 proxy authentication failed.";
    ConnErrorMap[ConnectionError::ConnProxyNoSupportedAuth] = "The HTTP/SOCKS5 proxy requires an unsupported auth mechanism.";
    ConnErrorMap[ConnectionError::ConnIoError] = "An I/O error occured. ";
    ConnErrorMap[ConnectionError::ConnParseError] = "An XML parse error occurred. ";
    ConnErrorMap[ConnectionError::ConnConnectionRefused] = "The connection was refused by the server (on the socket level).";
    ConnErrorMap[ConnectionError::ConnDnsError] = "Resolving the server's hostname failed.";
    ConnErrorMap[ConnectionError::ConnOutOfMemory] = "Out of memory. Uhoh.";
    ConnErrorMap[ConnectionError::ConnNoSupportedAuth] = "The auth mechanisms the server offers are not supported.";
    ConnErrorMap[ConnectionError::ConnTlsFailed] = "The server's certificate could not be verified or the TLS handshake did not complete successfully.";
    ConnErrorMap[ConnectionError::ConnTlsNotAvailable] = "The server didn't offer TLS while it was set to be required, or TLS was not compiled in.";
    ConnErrorMap[ConnectionError::ConnCompressionFailed] = "Negotiating/initializing compression failed.";
    ConnErrorMap[ConnectionError::ConnAuthenticationFailed] = "Authentication failed. Username/password wrong or account does not exist. Use ClientBase::authError() to find the reason.";
    ConnErrorMap[ConnectionError::ConnUserDisconnected] = "The user (or higher-level protocol) requested a disconnect.";
    ConnErrorMap[ConnectionError::ConnNotConnected] = "There is no active connection.";

    m_client->registerConnectionListener(this);
    thread t(WhiteSpacePing, g_client, this);
    t.detach();
}

SAFConnListener::~SAFConnListener()
{
}

void SAFConnListener::onConnect()
{
    m_lastError = ConnectionError::ConnNoError;
    m_connector->ResetCounter();
    if (g_verbose)
        cout << _("Connected (") << m_client->jid().bare() << ")." << endl;
}

void SAFConnListener::onDisconnect(ConnectionError e)
{
    m_lastError = e;
    if (g_verbose)
        cout << _("Disconnected (") << m_client->jid().bare() << ")." << endl;
    if (e == ConnNoError || e == ConnUserDisconnected) return;
    cout << ConnErrorMap[e] << endl;
}

bool SAFConnListener::onTLSConnect(const CertInfo& info)
{
    return true;
}

void SAFConnListener::WhiteSpacePing(Client* client, SAFConnListener * saf)
{
    if (g_wsping == 0) return;
    while (true)
    {
        sleep(g_wsping);
        if (saf->GetLastError() == ConnectionError::ConnNoError)
            client->whitespacePing();
        //cout << "Pinging the server." << endl;
    }
}

Connector::Connector(SAFConnListener * saf, const uint& retry, const uint& delay)
{
    m_saf = saf;
    m_retry = retry;
    m_delay = delay;
    m_count = 0;
    m_saf->SetConnector(this);
}

Connector::~Connector()
{
    m_saf->GetClient()->disconnect();
}

void Connector::DoConnect()
{
    // At start, SAFConnListener has ConnNotConnected as last error
    while (m_saf->GetLastError() != ConnectionError::ConnNoError)
    {
        bool res = m_saf->GetClient()->connect(); // Blocks on successful connect()
        //If no connection is established, res can still be true !
        if (!res
            /* If the server has been shutdown, we want to reconnect.
             * Same thing if the server is not yet up.
             */
            && m_saf->GetLastError() != ConnectionError::ConnConnectionRefused)
        {
            cout << _("Cannot initiate a connection."
                    " Check network, server, or parameters.") << endl;
            break;
        }
        if (g_verbose)
            cout << _("Try ") << m_count
                << _(" : Waiting ") << m_delay << " seconds"
                << _(" before connecting...") << endl;
        sleep(m_delay);
        m_count++;
        if (m_retry > 0 && m_count == m_retry)
        {
            if (g_verbose)
                cout << _("Giving up.") << endl;
            break;
        }
    }
}

