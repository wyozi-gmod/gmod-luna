#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>
#include <string>
#include <fstream>

using namespace GarrysMod::Lua;

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int TranspileLunaFile(lua_State* state)
{
	const char* cpath = LUA->CheckString(1);
	LUA->CheckType(2, GarrysMod::Lua::Type::STRING);
	
	unsigned int luaLength = 0;
	const char* lua = LUA->GetString(2, &luaLength);

	// test that it's a luna file
	std::string path(cpath);
	if (!ends_with(path, ".luna"))
	{
		LUA->PushBool(false);
		return 1;
	}

	// make sure it exists
	std::ifstream infile(path);
	if (!infile.good())
	{
		LUA->PushBool(false);
		return 1;
	}
	infile.close();

	// cool, create a new path with a .lua suffix
	std::string luaPath = path.substr(0, path.length() - 5) + ".lua";
	std::ofstream outfile(luaPath, std::ofstream::binary);
	outfile.write(lua, luaLength);
	outfile.close();

	LUA->PushBool(true);
	return 1;
}

GMOD_MODULE_OPEN()
{
	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	LUA->PushString( "TranspileLuna" );
	LUA->PushCFunction( TranspileLunaFile );
	LUA->SetTable( -3 ); 

	return 0;
}

//
// Called when your module is closed
//
GMOD_MODULE_CLOSE()
{
	return 0;
}