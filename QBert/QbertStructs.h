#pragma once
namespace dae
{
	struct TileCoordinates
	{
		TileCoordinates(int column, int row)
			: m_Column(column)
			, m_Row(row)
		{}

		TileCoordinates operator-(const TileCoordinates& other) const;
		TileCoordinates operator+(const TileCoordinates& other) const;

		int m_Column;
		int m_Row;
	};

	struct TileData
	{
		TileData(float tileSide, float zoomLevel)
			: m_TileSide(tileSide)
			, m_ZoomLevel(zoomLevel)
		{}

		float m_TileSide;
		float m_ZoomLevel;
	};

	struct RoundData
	{
		RoundData(int level = 1, int round = -1, int tileset = 1, int maxToggles = 1, bool allowTileDecrease = false)
			: m_IsValid(true)
			, m_Level(level)
			, m_Round(round)
			, m_TileSet(tileset)
			, m_MaxToggles(maxToggles)
			, m_AllowTileDecrease(allowTileDecrease)
		{}

		bool m_IsValid;
		int m_Level;
		int m_Round;
		int m_TileSet;
		int m_MaxToggles;
		bool m_AllowTileDecrease;
	};

}