@echo off
python Protobuf\generate_arduino.py -s utils -p core
python Protobuf\generate_arduino.py -s utils -p status
