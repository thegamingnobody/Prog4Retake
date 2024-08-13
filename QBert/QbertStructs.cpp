#include "QbertStructs.h"

dae::TileCoordinates dae::TileCoordinates::operator-(const TileCoordinates& other) const
{
    return TileCoordinates(m_Column - other.m_Column, m_Row - other.m_Row);
}

dae::TileCoordinates dae::TileCoordinates::operator+(const TileCoordinates& other) const
{
    return TileCoordinates(m_Column + other.m_Column, m_Row + other.m_Row);
}
