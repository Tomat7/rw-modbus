#!/bin/bash

token='629076406:AAEp3rr5k2jfO2klj3uRoduOVIIS9M1I7yc'
chat="292469883"
subj="Mode"
message=`grep Subject | sed 's/Subject: //'`

#sbj=`grep Subject`
#echo $message

#/usr/bin/curl -s --header 'Content-Type: application/json' --request 'POST' --data "{\"chat_id\":\"${chat}\",\"text\":\"${subj}\n${message}\"}" "https://api.telegram.org/bot${token}/sendMessage"

/usr/bin/curl -s --header 'Content-Type: application/json' --request 'POST' --data "{\"chat_id\":\"${chat}\",\"text\":\"${message}\"}" "https://api.telegram.org/bot${token}/sendMessage"

