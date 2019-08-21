#! /bin/sh
# Print the IP address
_IP=$(hostname -I) || true
if [ "$_IP" ]; then
    printf "=================================================\n"
    printf "IP BJ-Machine is %s\n" "$_IP"
    printf "=================================================\n"
fi

# cd folder buil
cd /home/pi/resource/coffee-machine/raspberry/

# run only tty1
if [[ "$(tty)" == "/dev/tty1" ]]
 then
    printf "TTY1 Start service BJMachine"
    sudo sh run.sh
fi
