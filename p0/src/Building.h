#pragma once
struct Building
{
	enum Type : size_t
	{
		TD,
		APARTMENT,
		BMO,
		CONDO,
		DUPLEX,
		OFFICE,
		PENTA,
		PINK,
		COUNT
	} type;

	Color color = { 0.75f, 0.75f, 0.75f };
	float hitpoints;
};
