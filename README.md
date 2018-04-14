SAF7 - Send And Forget an XMPP message.
License : GPL v2

This application is designed to send XMPP messages to arbitrary recipients 
without expecting a reply. It is intended to run on servers to forward messages 
to admins. Repeated problems with similar tools after system upgrade on Arch 
Linux motivated writing this application.

It uses gloox as XMPP library, and messages are sent to the application via 
DBus. It runs on the system bus, and hence, must be run as root.

USAGE
It must be started with two mandatory parameters :
    --jid (-j) : a valid XMPP jid, the sender : ex. srv-admin@srv.domain.tld
    --password (-w) : the sender's authotization token to connect to the server
Optional parameters may be used :
    --resource (-r) : XMPP ressource label, default : SAF
    --server (-s) : overrides the server part of the sender's jid
    --port (-p) : overrides default XMPP 5222 port, default : 5222 nevertheless
    --retry (-y) : Number of times to try to connect to server, default = 0 
(for ever)
    --delay (-d) : Number of seconds between each connection attempt,default = 
30
    --wsping (-g) : Number of seconds between each whitespace ping to the server 
, default = 0 (never), at least 55
    --bustag (-t) : a suffix to the 'xmpp.saf-' bus name, default : default
    --config (-c) : a simple key=value file, where above parameters can be 
saved for simplicty
    --verbose (-v) : Show messages. Command line only.

DBUS CONSIDERATIONS
A policy file must be created in /etc/dbus-1/system.d/ , because the 
application requests the system bus. A sample file is in the release package. 
If --bustag is used, another policy file must be created to reflect this 
parameter's significance.

The default instance creates a DBus destination 'xmpp.saf-default', with path 
/xmpp/saf. Two methods are available : Quit, and SendMsg. They can simply be 
called as such :

dbus-send --system --print-reply=literal --type=method_call 
--dest=xmpp.saf-default /xmpp/saf xmpp.saf.SendMsg 
string:recipient@srv.domain.tld string:"Server is about to crash!"

dbus-send --system --print-reply=literal --type=method_call 
--dest=xmpp.saf-default /xmpp/saf xmpp.saf.Quit

If a bustag parameter is used, the created DBus destination is 
'xmpp.saf-$(bustag)'. It is therefore possible to run multiple instances of the 
application simultaneously, each instance having a different configuration file.

XMPP NOTE
If the same sender's XMPP account is used in multiple instances, or on multiple 
hosts, a different resource parameter MUST be used for each instance.

FILES IN THE RELEASE ARCHIVE
 -All project files as a NetBeans project.
 -Convenience files :
    saf7_SendMsg.sh : a bash script to send a message. It accepts the 
recipient's address, the message, and optionally the bustag parameter if 
relevant.
    saf7_Quit.sh : a bash script to stop the application. Only root can do that, 
as per applied DBus policy. It optionally accepts the bustag parameter.
    saf7-default.rc.template : can simplify creating a configuration file.
    saf7-default.conf : a DBus policy file for the default instance. Create 
another one if bustag is used.
    saf7-default.service : a systemd service file for the default instance. 
Create another service file if bustag is used.
    PKGBUILD : to create an Arch's package. After installation, create a 
default /usr/local/etc/saf7/saf7-default.rc file with valid parameters.

DISCLAIMER
I don't claim the application is fit for any purpose.Programming is just my hobby.

Cheers !
