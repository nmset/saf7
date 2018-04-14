#/bin/sh

if [ $# -lt 2 ];then
    echo "Arg 1 : full jid without resource."
    echo "Arg 2 : message."
    echo "Arg 3 : bus name tag. Optional. Default = default ."
    exit 0
fi

RECIPIENT="$1"
MSG="$2"
TAG="default"
[ -n "$3" ] && TAG="$3"

dbus-send --system --print-reply=literal --type=method_call --dest=xmpp.saf-"$TAG" /xmpp/saf xmpp.saf.SendMsg string:"$RECIPIENT" string:"$MSG"

exit 0

