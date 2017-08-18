----------------------------------------------
-- Lua background script example
-- Global table    - mg
--
-- Fields in table 
--
-- 	mg.isterminated - true if background script need exit
-- 	mg.log          - send message to stdout
-- 	mg.handler      - thread handler, light userdata
-- 	mg.sleep        - sleep thread for milliseconds
-- 	mg.params       - store lua_background_script_params config values
--	mg.script       - script name
--      mg.root         - document roots directory

-- Comment
-- lua_background_script         - background.lua
-- lua_background_script_params  - param1=1,run_mode=database etc
-- background script terminated after all client socket closed
----------------------------------------------

local run_once = false

local mg       = mg
local thandler = mg.handler
local params   = mg.params

-- Script run once, lua_state closed when exit from script
if ( run_once ) then
	halt(0)
end 


-- Parameter test
-- Parameter format in lua_background_script_params : exit=1,param1=2
if ( params ~= nil and params.exit == "1" ) then
	halt(0)
end

mg.log(thandler ,"Background script running")
while ( true) do
	if ( mg.isterminated(thandler ) )
	then
		mg.log(thandler ,"Background script terminated by main thread")
		break
	end
	mg.sleep(500)
end
mg.log(thandler,"Background script shutdown")