#!/bin/bash

make clean
make debug

#Hello message------------------------------------------------------------------------
./encode  testPcaps/M_hello testPcaps/M_hello.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m M_hello.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m M_hello.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/M_hello testPcaps/M_hello.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > M_helloValgrind.txt
fi

./decode testPcaps/M_hello.pcap > decoded.d > /dev/null 2>&1
diff decoded.d M_hello > diffDeEn > /dev/null 2>&1
diffCode=$?
if [[ $diffCode == 1 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m M_hello diff failed"
    cat diffDeEn > failDeEnMhello
fi

#Empty message------------------------------------------------------------------------
./encode  testPcaps/M_empty testPcaps/M_empty.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m M_empty.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m M_empty.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/M_empty testPcaps/M_empty.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

./decode testPcaps/M_empty.pcap > decoded.d > /dev/null 2>&1
diff decoded.d M_empty > diffDeEn > /dev/null 2>&1
diffCode=$?
if [[ $diffCode == 1 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m M_empty diff failed"
    cat diffDeEn > failDeEnHempty
fi

#Normal status------------------------------------------------------------------------
./encode  testPcaps/S_normal testPcaps/S_normal.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m S_normal.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m S_normal.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/S_normal testPcaps/S_normal.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

./decode testPcaps/S_normal.pcap > decoded.d > /dev/null 2>&1
diff decoded.d S_normal > diffDeEn > /dev/null 2>&1
diffCode=$?
if [[ $diffCode == 1 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m S_normal diff failed"
    cat diffDeEn > failDeEnSnormal
fi

#No name status------------------------------------------------------------------------
./encode  testPcaps/S_noname testPcaps/S_noname.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m S_noname.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m S_noname.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/S_noname testPcaps/S_noname.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

./decode testPcaps/S_noname.pcap > decoded.d > /dev/null 2>&1
diff decoded.d S_noname > diffDeEn > /dev/null 2>&1
diffCode=$?
if [[ $diffCode == 1 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m S_noname diff failed"
    cat diffDeEn > failDeEnSnoname
fi

#Command repeat------------------------------------------------------------------------
./encode  testPcaps/C_repeat testPcaps/C_repeat.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_repeat.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_repeat.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/C_repeat testPcaps/C_repeat.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Command add setgroup -17------------------------------------------------------------------------
./encode  testPcaps/C_addsetgroup-17 testPcaps/C_addsetgroup-17.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m add group -17"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m add group -17 with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/C_addsetgroup-17 testPcaps/C_addsetgroup-17.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Command remove setgroup -17------------------------------------------------------------------------
./encode  testPcaps/C_removesetgroup-17 testPcaps/C_removesetgroup-17.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m remove group -17"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m remove group -17 with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/C_removesetgroup-17 testPcaps/C_removesetgroup-17.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Command status------------------------------------------------------------------------
./encode  testPcaps/C_status testPcaps/C_status.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_status.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_status.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/C_status testPcaps/C_status.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#GPS normal------------------------------------------------------------------------
./encode  testPcaps/G_normal testPcaps/G_normal.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m G_normal.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m G_normal.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/G_normal testPcaps/G_normal.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Multi message/status/gps------------------------------------------------------------------------
./encode  testPcaps/multiM_S_G testPcaps/multiM_S_G.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m multiM_S_G.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m multiM_S_G.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/multiM_S_G testPcaps/multiM_S_G.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Goto command------------------------------------------------------------------------
./encode  testPcaps/C_goto testPcaps/C_goto.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 0 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m C_goto.pcap"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m C_goto.pcap with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/C_goto testPcaps/C_goto.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Main zerg headers mixed------------------------------------------------------------------------
./encode  testPcaps/mainHeaderMixed testPcaps/mainHeaderMixed.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 51 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m main zerg headers mixed"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m main zerg headers mixed with exit code $error"
fi

valgrind --leak-check=full  -v ./encode testPcaps/mainHeaderMixed testPcaps/mainHeaderMixed.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
fi

#Zerg message header missing------------------------------------------------------------------------
./encode  testPcaps/messageHeaderGone testPcaps/messageHeaderGone.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 17 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m message header missing"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m message header missing with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/messageHeaderGone testPcaps/messageHeaderGone.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > messageHeaderGone.txt
fi

#Zerg message header mixed------------------------------------------------------------------------
./encode  testPcaps/messageHeaderMix testPcaps/messageHeaderMix.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 51 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m message header mixed"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m message header mixed with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/messageHeaderMix testPcaps/messageHeaderMix.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > messageHeaderMix.txt
fi

#Zerg status header missing------------------------------------------------------------------------
./encode  testPcaps/statusHeaderMissing testPcaps/statusHeaderMissing.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 29 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m status header missing"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m status header missing with exit code $error"
fi

valgrind --leak-check=full --show-leak-kinds=all -v ./encode testPcaps/statusHeaderMissing testPcaps/statusHeaderMissing.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > statusHeaderMissing.txt
fi

#Zerg status header mixed------------------------------------------------------------------------
./encode  testPcaps/statusHeaderMix testPcaps/statusHeaderMix.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 29 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m status header mixed"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m status header mixed with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/statusHeaderMix testPcaps/statusHeaderMix.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > statusHeaderMix.txt
fi

#Zerg status bad hp------------------------------------------------------------------------
./encode  testPcaps/statusBadHP testPcaps/statusBadHP.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 21 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m status header bad HP"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m status header bad HP with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/statusBadHP testPcaps/statusBadHP.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > statusBadHP.txt
fi

#Zerg status bad breed------------------------------------------------------------------------
./encode  testPcaps/statusBadBreed testPcaps/statusBadBreed.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 24 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m status header bad breed"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m status header bad breed with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/statusBadBreed testPcaps/statusBadBreed.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > statusBadBreed.txt
fi

#Zerg status bad name------------------------------------------------------------------------
./encode  testPcaps/statusBadName testPcaps/statusBadName.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 29 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m command header bad name"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m command header bad name with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/statusBadName testPcaps/statusBadName.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > statusBadName.txt
fi

#Zerg command bad command------------------------------------------------------------------------
./encode  testPcaps/badCommand testPcaps/badCommand.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 31 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m command header bad command"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m command bad command with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/badCommand testPcaps/badCommand.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > badCommand.txt
fi

#Zerg command bad set group------------------------------------------------------------------------
./encode  testPcaps/badSetGroup testPcaps/badSetGroup.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 38 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m command header bad set group"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m command header bad set group with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/badSetGroup testPcaps/badSetGroup.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > badSetGroup.txt
fi

#Zerg command header mix------------------------------------------------------------------------
./encode  testPcaps/cmdHeaderMix testPcaps/cmdHeaderMix.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 39 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m command header mixed"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m command header mixed with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/cmdHeaderMix testPcaps/cmdHeaderMix.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > cmdHeaderMix.txt
fi

#Zerg command header missing------------------------------------------------------------------------
./encode  testPcaps/cmdHeaderMissing testPcaps/cmdHeaderMissing.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 39 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m command header missing"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m command header missing with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/cmdHeaderMissing testPcaps/cmdHeaderMissing.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > cmdHeaderMissing.txt
fi

#Zerg command bad repeat------------------------------------------------------------------------
./encode  testPcaps/badrepeat testPcaps/badrepeat.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 37 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m command header bad repeat"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m command header bad repeat with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/badrepeat testPcaps/badrepeat.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > badrepeat.txt
fi

#Zerg gps missing header------------------------------------------------------------------------
./encode  testPcaps/gpsHeaderMissing testPcaps/gpsHeaderMissing.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 49 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m gps header missing"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m gps header missing with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/gpsHeaderMissing testPcaps/gpsHeaderMissing.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > gpsHeaderMissing.txt
fi

#Zerg gps mixed header------------------------------------------------------------------------
./encode  testPcaps/gpsHeaderMix testPcaps/gpsHeaderMix.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 49 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m gps header mixed"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m gps header mixed with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/gpsHeaderMix testPcaps/gpsHeaderMix.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > gpsHeaderMix.txt
fi

#Zerg gps bad lat------------------------------------------------------------------------
./encode  testPcaps/badLat testPcaps/badLat.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 42 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m gps bad lat"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m gps bad lat with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/badLat testPcaps/badLat.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > badLat.txt
fi

#Zerg gps bad long------------------------------------------------------------------------
./encode  testPcaps/badLong testPcaps/badLong.pcap > /dev/null 2>&1
error=$?
if [[ $error -eq 41 ]]
then
    echo -e "[\e[32m+\e[0m]\e[32mPassed\e[0m gps bad long"
else
    echo -e "[\e[31m-\e[0m]\e[31mFailed\e[0m gps bad long with exit code $error"
fi

valgrind --leak-check=full -v ./encode testPcaps/badLong testPcaps/badLong.pcap > valgrind.txt 2>&1
cat valgrind.txt | grep -o "All heap blocks were freed" > /dev/null
error=$?
if [[ $error -ne 0 ]]
then
    echo -e "-----[\e[31m-\e[0m]\e[31mFailed\e[0m valgrind leaks"
    cat valgrind.txt > badLong.txt
fi






#Clean up
if [[ -e valgrind.txt ]]
then
    rm valgrind.txt
fi

if [[ -e diffDeEn ]]
then
    rm diffDeEn
fi
