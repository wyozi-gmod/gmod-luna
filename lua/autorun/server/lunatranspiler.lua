local function FindLuna(folder, fn)
	local fils, _ = file.Find(folder .. "*.luna", "MOD")
	
	for _,fil in pairs(fils) do
		local path = folder .. fil
		fn(path)
	end
	
	local _, fols = file.Find(folder .. "*", "MOD")
	for _,fol in pairs(fols) do
		FindLuna(folder .. fol .. "/", fn)
	end
end


require("lunatranspiler")

local _lexer, _parser, _toLua = include("luna/bin/lexer.lua"), include("luna/bin/parser.lua"), include("luna/bin/to_lua.lua").toLua
local function toAST(code)
	local l = _lexer.new(code)
	local p = _parser.new(l)
	return p:block()
end

local function luna2lua(l)
	return _toLua(toAST(l))
end

luna = {}
function luna.TranspileCode(luna)
	return luna2lua(luna)
end

local function TranspileAll()
	local function cb(path)
		local lunac = file.Read(path, "MOD")
		local b = TranspileLuna("./garrysmod/" .. path, luna.TranspileCode(lunac))
		if b == false then
			print("transpiling luna file at ", path, " failed!")
		end
	end

	print("Transpiling .luna files..")
	FindLuna("addons/", cb)
	FindLuna("gamemodes/", cb)
	print("Transpiling done")
end

TranspileAll()
hook.Add("ShutDown", "LunaTranspiler", TranspileAll)