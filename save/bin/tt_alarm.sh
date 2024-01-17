#!/bin/bash

token='629076406:AAEp3rr5k2jfO2klj3uRoduOVIIS9M1I7yc'
chat="292469883"
#subj="Alarm"
subj=""
#message=`grep -a -e KUB -e BUF -e DEF| recode -f cp1251..utf8`
#message=`recode -f cp1251..utf8 | grep Канал | sed 's/Канал: //'`
message=`recode -f cp1251..utf8 | grep Канал | sed 's/Канал: //' | sed 's/Время: //'`

#sbj=`grep Subject`
#echo $message >/tmp/alarm

#/usr/bin/curl -s --header 'Content-Type: application/json' --request 'POST' --data "{\"chat_id\":\"${chat}\",\"text\":\"${subj}\n${message}\"}" "https://api.telegram.org/bot${token}/sendMessage"
/usr/bin/curl -s --header 'Content-Type: application/json' --request 'POST' --data "{\"chat_id\":\"${chat}\",\"text\":\"${subj}\n${message}\"}" "https://api.telegram.org/bot${token}/sendMessage"
