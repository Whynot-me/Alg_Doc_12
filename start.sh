# ! /bin/sh

SERVICE_PATH="/usr/Alg_Doc_12_v3.2.1"

SERVICE_NAME="algorithmv_12_v3.2.1"

START_CMD="./$SERVICE_NAME"

LOG_FILE="restart.log"


cd $SERVICE_PATH

pwd

while true 

do

    procnum=`ps -ef|grep $SERVICE_NAME|grep -v grep|wc -l`

    if [ $procnum -eq 0 ]

    then

        echo "start service...................."

        echo `date +%Y-%m-%d` `date +%H:%M:%S`  $SERVICE_NAME >>$LOG_FILE

        ${START_CMD}
        /bin/bash

    fi

    sleep 1m

done
