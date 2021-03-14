
function start()
    -- The "start" function is called when the server is ready.
	logfile = io.open("C:\\tmp\\lualog.txt", "w");
	if (logfile) then
	    logfile:write("Server start: " .. os.date('%Y-%m-%dT%H:%M:%S') .. "\n\n");
	end
end


function stop()
    -- The "stop" function is called when the server is stopping.
    logfile:close();
end


function log(req, resp)
    -- The "log" function can be used to
	-- (a) filter messages and return boolean: true (log) or false (do not log)
	-- (b) format log message and return it as string (empty string will not log)
	-- (c) forward the log data to an external log

	if logfile then
	  logfile:write(os.date('%Y-%m-%dT%H:%M:%S').."\n");
	  logfile:write("request:\n");
	  for k,v in pairs(req) do
	    logfile:write("  " .. tostring(k) .. " = " .. tostring(v) .. "\n");
		if type(v) == "table" then
		  for k2,v2 in pairs(v) do
	        logfile:write("    " .. tostring(k2) .. " = " .. tostring(v2) .. "\n");
		  end
		end
	  end
	  logfile:write("response:\n");
	  for k,v in pairs(resp) do
	    logfile:write("  " .. tostring(k) .. " = " .. tostring(v) .. "\n");
		if type(v) == "table" then
		  for k2,v2 in pairs(v) do
	        logfile:write("    " .. tostring(k2) .. " = " .. tostring(v2) .. "\n");
		  end
		end
	  end
	  logfile:write("----\n\n");
	  logfile:flush();
	end

	-- Loging already done here
	return false;
end

-- Return true to continue server startup.
return true;
