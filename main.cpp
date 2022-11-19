/* 
 * File:   main.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : GPL v2
 * Copyright Saleem Edah-Tally - © 2017
 *
 * Created on 6 mai 2017, 18:21
 */

#include "gcc_version.h"
#include <cstdlib>
#include <iostream>
#include "SAF.h"
#include "globals.h"
#include <string>
#include <fstream>
#include <signal.h>
#include <boost/program_options.hpp>
#include <gloox/client.h>
#include <gloox/disco.h>
#include "DBusSafWorkers.h"

using namespace std;
namespace po = boost::program_options;

string g_config = ""; // A configuration file.
string g_jid = "";
string g_password = "";
string g_resource = "SAF";
string g_server = "";
string g_tag = "default";
uint g_port = 5222;
uint g_retry = 0;
uint g_delay = 30;
uint g_wsping = 0;
bool g_verbose = false;

gloox::Client * g_client = NULL;
DBus::BusDispatcher g_dispatcher;
SAFConnListener * g_saf = NULL;
Connector * g_connector = NULL;
DBusSafAdaptor * g_adaptor_worker = NULL;

/**
 * TODO : Detect and react to vanishing network link.
 */

/*
 * http://www.radmangames.com/programming/how-to-use-boost-program_options
 * https://blog.knatten.org/2012/03/23/basic-gloox-tutorial/
 * https://camaya.net/api/gloox-1.0.20/
 */

/**
 * Cleaup and exit. Return code is 0 because we want to exit this way.
 * @param sn
 */
void signalhandler(int sn)
{
    delete g_connector;
    /* Cant delete adaptor_worker without a seg fault, this function is called from there.
     * Anyway, we are exiting.
     */
    //delete adaptor_worker;
    delete g_client;
    delete g_saf;
    if (g_verbose)
        cout << _("Signaled to exit.") << endl;
    exit(0);
}

bool GetAndValidateParams(int argc, char** argv)
{
    try
    {
        po::options_description desc(_ABOUT_);
        desc.add_options()
            ("help,h", "Print help messages")
            ("jid,j", po::value<string>(&g_jid), _("Full JID without resource"))
            ("password,w", po::value<string>(&g_password), _("Password"))
            ("resource,r", po::value<string>(&g_resource), _("Resource (optional)"))
            ("server,s", po::value<string>(&g_server), _("Server (optional)"))
            ("port,p", po::value<uint>(&g_port), _("Port (optional), default = 5222"))
            ("retry,y", po::value<uint>(&g_retry), _("Number of times to try to connect to server (optional), default = 0 (for ever)"))
            ("delay,d", po::value<uint>(&g_delay), _("Number of seconds between each connection attempt (optional), default = 30"))
            ("wsping,g", po::value<uint>(&g_wsping), _("Number of seconds between each whitespace ping to the server (optional), default = 0 (never), at least 55"))
            ("bustag,t", po::value<string>(&g_tag), _("Bus name tag (optional), default=default"))
            ("config,c", po::value<string>(&g_config), _("Use specified configuration file (optional), else, pass all parameters on the command line"))
            ("verbose,v", _("Show messages (optional). Command line only."));

        po::variables_map vm;
        po::variables_map vmc;
        try
        {
            po::store(po::parse_command_line(argc, argv, desc), vm);
            if (vm.count("help"))
            {
                cout << desc << endl;
                return 0;
            }
            po::notify(vm);
            if (vm.count("verbose"))
            {
                g_verbose = true;
            }
            /*
             * If a config file is in the command line,
             * its content overrides all other parameters.
             */
            if (vm.count("config"))
            {
                ifstream f(g_config);
                if (f.fail())
                {
                    cerr << _("Failure with file : ") << g_config << endl;
                    return 0;
                }
                po::store(po::parse_config_file(f, desc), vmc);
                po::notify(vmc);
            }
        }
        catch (po::error& eb)
        {
            cerr << _("Error : ") << eb.what() << endl;
            return 0;
        }
    }
    catch (exception& e)
    {
        cerr << _("Error : ") << e.what() << endl;
        return 0;
    }

    if (g_jid.empty())
    {
        cout << _("jid missing.") << endl;
        return 0;
    }
    if (g_password.empty())
    {
        cout << _("password missing.") << endl;
        return 0;
    }
    if (g_wsping > 0 && g_wsping < 55 )
    {
        cout << _("wsping must be at least 55 seconds.") << endl;
        return 0;
    }
    return true;
}

DBusSafAdaptor* StartDBusAdaptor(Connector * connector, SAFConnListener * saf)
{
    DBus::_init_threading();
    DBus::default_dispatcher = &g_dispatcher;
#ifdef DEVTIME // Defined in Debug NetBeans configuration
    DBus::Connection bus = DBus::Connection::SessionBus();
#else
    DBus::Connection bus = DBus::Connection::SystemBus();
#endif
    const string busName = string("xmpp.saf-") + g_tag;
    bus.request_name(busName.data());
    DBusSafAdaptor * adaptor_worker = new DBusSafAdaptor(bus, connector, saf);

    return adaptor_worker;
}

int main(int argc, char** argv)
{
    setlocale (LC_ALL, "");
    bindtextdomain (_APPNAME_, "/usr/local/share/locale"); // containing fr/LC_MESSAGES/
    textdomain (_APPNAME_);
    
    if (!GetAndValidateParams(argc, argv))
        return 0;

    signal(SIGINT, signalhandler);
    signal(SIGTERM, signalhandler);

    JID ajid(g_jid + "/" + g_resource);
    g_client = new Client(ajid, g_password, g_port);
    if (!g_server.empty())
        g_client->setServer(g_server);
    g_client->setPort(g_port);
    g_client->disco()->setIdentity("client", "7", "saf");

    g_saf = new SAFConnListener(g_client);
    g_connector = new Connector(g_saf, g_retry, g_delay);
    // Listen on DBus
    g_adaptor_worker = StartDBusAdaptor(g_connector, g_saf);
    // Connect to server
    g_connector->DoConnect(); // Blocks

    //cout << g_client->getStatistics().encryption << endl;

    delete g_adaptor_worker;
    delete g_connector;
    delete g_client;
    delete g_saf;

    return 0;
}

