#pragma once

#include <string>

typedef __int64 int64;
typedef unsigned int uint;

struct RoomData
{
    RoomData(uint roomNum, int64 roomKey, int personNum)
    : roomNum(roomNum)
    , roomKey(roomKey)
    , personNum(personNum)
    {

    }
    static const int maxPerson = 2;

    uint roomNum;
    int64 roomKey;
    int personNum;
};