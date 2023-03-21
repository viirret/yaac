#!/bin/bash

# check if faketime and timeout are installed
faketime --version 1>/dev/null || { echo "faketime is not installed!"; exit 1; }
timeout --version 1>/dev/null || { echo "timeout is not installed!"; exit 1; }

FAKE_TIME="2023-03-26 2:59:50"

faketime "$FAKE_TIME" ../build/yaac --now --close --time "4:00"
