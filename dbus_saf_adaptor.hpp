
/*
 *	This file was automatically generated by dbusxx-xml2cpp; DO NOT EDIT!
 */

#ifndef __dbusxx__dbus_saf_adaptor_hpp__ADAPTOR_MARSHAL_H
#define __dbusxx__dbus_saf_adaptor_hpp__ADAPTOR_MARSHAL_H

#include <dbus-c++/dbus.h>
#include <cassert>

namespace xmpp {

class saf_adaptor
: public ::DBus::InterfaceAdaptor
{
public:

    saf_adaptor()
    : ::DBus::InterfaceAdaptor("xmpp.saf")
    {
        register_method(saf_adaptor, Quit, _Quit_stub);
        register_method(saf_adaptor, SendMsg, _SendMsg_stub);
    }

    ::DBus::IntrospectedInterface *introspect() const 
    {
        static ::DBus::IntrospectedArgument Quit_args[] = 
        {
            { 0, "", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedArgument SendMsg_args[] = 
        {
            { "recipient", "s", true },
            { "msg", "s", true },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedMethod saf_adaptor_methods[] = 
        {
            { "Quit", Quit_args },
            { "SendMsg", SendMsg_args },
            { 0, 0 }
        };
        static ::DBus::IntrospectedMethod saf_adaptor_signals[] = 
        {
            { 0, 0 }
        };
        static ::DBus::IntrospectedProperty saf_adaptor_properties[] = 
        {
            { 0, 0, 0, 0 }
        };
        static ::DBus::IntrospectedInterface saf_adaptor_interface = 
        {
            "xmpp.saf",
            saf_adaptor_methods,
            saf_adaptor_signals,
            saf_adaptor_properties
        };
        return &saf_adaptor_interface;
    }

public:

    /* properties exposed by this interface, use
     * property() and property(value) to get and set a particular property
     */

public:

    /* methods exported by this interface,
     * you will have to implement them in your ObjectAdaptor
     */
    virtual void Quit() = 0;
    virtual void SendMsg(const std::string& recipient, const std::string& msg) = 0;

public:

    /* signal emitters for this interface
     */

private:

    /* unmarshalers (to unpack the DBus message before calling the actual interface method)
     */
    ::DBus::Message _Quit_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        Quit();
        ::DBus::ReturnMessage reply(call);
        return reply;
    }
    ::DBus::Message _SendMsg_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        std::string argin1; ri >> argin1;
        std::string argin2; ri >> argin2;
        SendMsg(argin1, argin2);
        ::DBus::ReturnMessage reply(call);
        return reply;
    }
};

} 
#endif //__dbusxx__dbus_saf_adaptor_hpp__ADAPTOR_MARSHAL_H
