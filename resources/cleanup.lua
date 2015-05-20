-- Lua script used to clean up tabs and spaces in C, CPP and H files.
-- Copyright (c) 2014, bel
-- MIT License (http://opensource.org/licenses/mit-license.php)
--
-- It can be used from the command line:
-- Call Lua5.1 or Lua5.2 + this script file + the C/CPP/H file to clean
--
-- It can be used in Visual Studio as an external tool:
-- command: Lua5.1.exe or Lua5.2.exe
-- argument: "X:\civetweb\resources\cleanup.lua" $(ItemPath)
--

clean = arg[1]
print("Cleaning " .. clean)

lines = io.lines(clean)
if not lines then
    print("Can not open file " .. clean)
    return
end

function trimright(s)
  return s:match "^(.-)%s*$"
end

local lineend = false
local tabspace = false
local changed = false
local invalid = false
local newfile = {}

lineno = 0
incmt = false

for l in lines do
    lineno = lineno + 1
    local lt = trimright(l)
    if (lt ~= l) then
        lineend = true
        changed = true
    end
    local mcmt = l:find("%/%*");
    if mcmt then
        if incmt then
            print("line " .. lineno .. " nested comment")
        end
        if not (l:sub(mcmt):find("%*%/")) then
            -- multiline comment begins here
            incmt = true
        end
    elseif incmt then
        if not l:find("^%s*%*") then
            print("line " .. lineno .. " multiline comment without leading *")
        end
        if l:find("%*%/") then
            incmt = false
        end
    else
        local cmt = l:find("//")
        if (cmt) and (l:sub(cmt-5, cmt+1) ~= "http://") and (l:sub(cmt-6, cmt+1) ~= "https://") then
            print("line " .. lineno .. " has C++ comment //")
        end
    end
    local lts = lt:gsub('\t', '    ')
    if (lts ~= lt) then
        tabspace = true
        changed = true
    end
    for i=1,#lts do
        local b = string.byte(lts,i)
        if b<32 or b>=127 then
            print("Letter " .. string.byte(l,i) .. " (" .. b .. ") found in line " .. lts)
            invalid = true
        end
    end

    newfile[#newfile + 1] = lts
end

print("Line endings trimmed:     " .. tostring(lineend))
print("Tabs converted to spaces: " .. tostring(tabspace))
print("Invalid characters:       " .. tostring(invalid))

if changed then
    local f = io.open(clean, "wb")
    for i=1,#newfile do
        f:write(newfile[i])
        f:write("\n")
    end
    f:close()
    print("File cleaned")
end
