#!/bin/sh
nohup bash /usr/Alg_Doc_12_v3.2.1/start.sh > /dev/null 2>&1 &
/bin/bash             

tail -f /dev/null
