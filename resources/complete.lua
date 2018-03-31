#!/usr/bin/lua5.2

-- CivetWeb command line completion for bash
--
-- To use it, create a file called "civetweb" in the completion folder
-- (/usr/share/bash-completion/completions/ or /etc/bash_completion)
-- This file has to contain just one line:
-- complete -C /path/to/civetweb/resources/complete.lua civetweb
--

-- The bash "complete -C" has an awkward interface:
-- see https://unix.stackexchange.com/questions/250262/how-to-use-bashs-complete-or-compgen-c-command-option
-- Command line arguments
cmd = arg[1] -- typically "./civetweb" or whatever path was used
this = arg[2] -- characters already typed for the next options
last = arg[3] -- option before this one
-- Environment variables
comp_line = os.getenv("COMP_LINE") -- entire command line
comp_point = os.getenv("COMP_POINT") -- position of cursor (index)
comp_type = os.getenv("COMP_TYPE") -- type:
-- 9 for normal completion
-- 33 when listing alternatives on ambiguous completions
-- 37 for menu completion
-- 63 when tabbing between ambiguous completions
-- 64 to list completions after a partial completion


-- Debug-Print function (must use absolute path for log file)
function dp(txt)
  local f = io.open("/tmp/complete.log", "a");
  f:write(txt .. "\n")
  f:close()
end

-- Helper function: Check if files exist
function fileexists(name)
  local f = io.open(name, "r")
  if f then
    f:close()
    return true
  end
  return false
end


-- Debug logging
dp("complete: cmd=" .. cmd .. ", this=" .. this .. ", last=" .. last .. ", type=" .. comp_type)


-- Trim command line (remove spaces)
trim_comp_line = string.match(comp_line, "^%s*(.-)%s*$")
 
if (trim_comp_line == cmd) then
  -- this is the first option
  dp("Suggest --help argument")
  print("--help ")
  os.exit(0)
end

is_h = string.find(comp_line, "^%s*" .. cmd .. "%s+%-h%s")
is_h = is_h or string.find(comp_line, "^%s*" .. cmd .. "%s+%--help%s")
is_h = is_h or string.find(comp_line, "^%s*" .. cmd .. "%s+%-H%s")

if (is_h) then
  dp("If --help is used, no additional argument is allowed")
  os.exit(0)
end

is_a = string.find(comp_line, "^%s*" .. cmd .. "%s+%-A%s")
is_c = string.find(comp_line, "^%s*" .. cmd .. "%s+%-C%s")
is_i = string.find(comp_line, "^%s*" .. cmd .. "%s+%-I%s")
is_r = string.find(comp_line, "^%s*" .. cmd .. "%s+%-R%s")

if (is_i) then
  dp("If --I is used, no additional argument is allowed")
  os.exit(0)
end

-- -A and -R require the password file as second argument
htpasswd = ".htpasswd"
if ((last == "-A") or (last == "-R")) and (this == htpasswd:sub(1,#this)) then
  print(htpasswd)
  os.exit(0)
end

-- -C requires an URL, usually starting with http:// or https://
http = "http://"
if (last == "-C") and (this == http:sub(1,#this)) then
  print(http)
  print(http.. "localhost/")
  os.exit(0)
end
http = "https://"
if (last == "-C") and (this == http:sub(1,#this)) then
  print(http)
  print(http.. "localhost/")
  os.exit(0)
end


-- Take options directly from "--help" output of executable
optfile = "/tmp/civetweb.options"
if not fileexists(optfile) then
  dp("options file " .. optfile .. " missing")
  os.execute(cmd .. " -h &> " .. optfile)
else
  dp("options file " .. optfile .. " found")
end

for l in io.lines(optfile) do
  local lkey, lval = l:match("^%s+(%-[^%s]*)%s*([^%s]*)%s*$")
  if lkey then
    local thislen = string.len(this)
    if (thislen>0) and (this == lkey:sub(1,thislen)) then
      print(lkey)
      dp("match: " .. lkey)
      keymatch = true
    end
    if last == lkey then
      valmatch = lval
    end
  end
end

if keymatch then
  -- at least one options matches
  os.exit(0)
end

if valmatch then
  -- suggest the default value
  print(valmatch)
  os.exit(0)
end

-- No context to determine next argument
dp("no specific option")
os.exit(0)

