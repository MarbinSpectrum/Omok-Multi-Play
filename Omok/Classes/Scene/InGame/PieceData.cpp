#include "PieceData.h"

PieceData::PieceData()
: r(0)
, c(0)
, pieceType(PieceType::EMPTY)
{
}

PieceData::PieceData(int pR, int pC, PieceType pPieceType)
: r(pR)
, c(pC)
, pieceType(pPieceType)
{

}

