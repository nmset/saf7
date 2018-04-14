#/bin/sh

TAG="default"
[ -n "$1" ] && TAG="$1"

dbus-send --system --type=method_call --dest=xmpp.saf-"$TAG" /xmpp/saf xmpp.saf.Quit

exit 0

