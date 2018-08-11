#!/bin/bash

make clean
make debug

#Hello message
./decode pcaps/M_hello.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m M_hello.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m M_hello.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/M_hello.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Empty message
./decode pcaps/M_empty.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m M_empty.pcap."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m M_empty.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/M_empty.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#ipv6 message
./decode pcaps/M_ipv6.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m M_ipv6.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m M_ipv6.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/M_ipv6.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#utf8 message
./decode pcaps/M_utf8.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m M_utf8.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m M_utf8.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/M_utf8.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#802.1q status
./decode pcaps/S_802.1q.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m S_802.1q.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m S_802.1q.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/S_801.1q.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#No name status
./decode pcaps/S_noname.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m S_noname.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m S_noname.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/S_noname.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi


#Normal status
./decode pcaps/S_normal.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m S_normal.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m S_normal.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/S_normal.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi


#Normal GPS
./decode pcaps/G_normal.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m G_normal.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m G_normal.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/G_normal.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Status command
./decode pcaps/C_getstatus.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_getstatus.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_getstatus.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/C_getstatus.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#repeat command
./decode pcaps/C_repeat.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_repeat.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_repeat.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/C_repeat.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#repeat two command
./decode pcaps/C_repeat2.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_repeat2.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_repeat2.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/C_repeat2.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Set group two command
./decode pcaps/C_setgroup-17.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_setgroup-17.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_setgroup-17.pcap with exit code $error"
fi

valgrind -v ./decode pcaps/C_setgroup-17.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#No udp header
./decode pcaps/nonudp.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 54 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m not UDP header."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m not UDP header."
fi

valgrind -v ./decode pcaps/nonudp-17.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Bad latitude multi packet
./decode pcaps/multi_status_gps.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 42 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m invalid latitude."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m invalid latitude."
fi

valgrind -v ./decode pcaps/multi_status_gps.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Good multi packet
./decode pcaps/multi_command_message_status_command.pcap > /dev/null 2>&1 
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m valid multi_C_M_S_C."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m invalid multi_C_M_S_C."
fi

valgrind -v ./decode pcaps/multi_command_message_status_command.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Bad multi command packet
./decode pcaps/bad_multi_command.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 11 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m wrong IP version."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m wrong IP version."
fi

valgrind -v ./decode pcaps/bad_multi_command.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Bad multi gps packet
./decode pcaps/bad_multi_gps.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m mismatch IP version."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m mismatch IP version."
fi

valgrind -v ./decode pcaps/bad_multi_gps.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Non zerg packet
./decode pcaps/nonzerg.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 57 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m nonzerg."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m nonzerg with error code $error."
fi

valgrind -v ./decode pcaps/nonzerg.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#IP IHL less than 5
./decode pcaps/invalidIpsize.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 55 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m IP size \< 5."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m IP size \< 5."
fi

valgrind -v ./decode pcaps/invalidIpsize.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Get status and status packet
./decode pcaps/C_getstatus_S_normal.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m GET STATUS and normal status."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m GET STATUS and normal status."
fi

valgrind -v ./decode pcaps/C_getstatus_S_normal.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Big endian
./decode pcaps/M_bigendian.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m big endian."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m big endian."
fi

valgrind -v ./decode pcaps/M_bigendian.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Invalid Magic number
./decode pcaps/badMagicNumber.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 56 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m bad magic number."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m bad magic number, exit code $error."
fi

valgrind -v ./decode pcaps/badMagicNumber.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Invalid pcap version
./decode pcaps/badPcapVersion.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 56 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m bad pcap version."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m bad pcap version, exit code $error."
fi

valgrind -v ./decode pcaps/badPcapVersion.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Invalid pcap link layer
./decode pcaps/badLinkType.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 56 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m bad pcap link layer."
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m bad pcap link layer, exit code $error."
fi

valgrind -v ./decode pcaps/badLinkType.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne  0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi


#Clean up
if [[ -e valgrind.txt ]]
then
    rm valgrind.txt
fi
