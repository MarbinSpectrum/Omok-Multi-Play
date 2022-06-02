#pragma once

#include <string>

typedef __int64 int64;
typedef unsigned int uint;

class RoomData
{
public:
    RoomData(std::string roomName, uint roomNum, int64 roomKey, int personNum)
    : roomName(roomName)
    , roomNum(roomNum)
    , roomKey(roomKey)
    , personNum(personNum)
    {

    }
    static const int maxPerson = 2;

    std::string roomName;
    uint roomNum;
    int64 roomKey;
    int personNum;
};