#pragma once

enum class PieceType
{
    EMPTY,
    WHITE,
    BLACK
};

class PieceData 
{
public:
    PieceData();
    PieceData(int pR,int pC, PieceType pPieceType);

public:
    int r;
    int c;
    PieceType pieceType;
};