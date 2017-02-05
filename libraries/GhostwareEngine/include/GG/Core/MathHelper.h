#pragma once

#include "Vector.h"
#include "Json.h"

namespace GG
{
	json ToJson( const Vector2 & v)
	{
		json j;
		j[0] = v.x;
		j[1] = v.y;
		return j;
	}

	json ToJson( const Vector3 & v)
	{
		json j;
		j[0] = v.x;
		j[1] = v.y;
		j[2] = v.z;
		return j;
	}

	json ToJson(const Vector4 & v)
	{
		json j;
		j[0] = v.x;
		j[1] = v.y;
		j[2] = v.z;
		j[3] = v.w;
		return j;
	}


	json ToJson(const Vector2i & v)
	{
		json j;
		j[0] = v.x;
		j[1] = v.y;
		return j;
	}

	json ToJson(const Vector3i & v)
	{
		json j;
		j[0] = v.x;
		j[1] = v.y;
		j[2] = v.z;
		return j;
	}

	json ToJson(const Vector4i & v)
	{
		json j;
		j[0] = v.x;
		j[1] = v.y;
		j[2] = v.z;
		j[3] = v.w;
		return j;
	}

	Vector2 Vector2FromJson(const json & j)
	{
		return Vector2(j[0].asFloat(), j[1].asFloat());
	}

	Vector3 Vector3FromJson(const json & j)
	{
		return Vector3(j[0].asFloat(), j[1].asFloat(), j[2].asFloat());
	}

	Vector4 Vector4FromJson(const json & j)
	{
		return Vector4(j[0].asFloat(), j[1].asFloat(), j[2].asFloat(), j[3].asFloat());
	}

	Vector2i Vector2iFromJson(const json & j)
	{
		return Vector2i(j[0].asInt(), j[1].asInt());
	}

	Vector3i Vector3iFromJson(const json & j)
	{
		return Vector3i(j[0].asInt(), j[1].asInt(), j[2].asInt());
	}

	Vector4i Vector4iFromJson(const json & j)
	{
		return Vector4i(j[0].asInt(), j[1].asInt(), j[2].asInt(), j[3].asInt());
	}

	
}
