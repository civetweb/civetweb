-- Declare a shared "timer" counter and functions "timer"
-- and "start" when the script is loading.
shared.timer = 0


function timer()
    -- Increment shared value.
    shared.timer = shared.timer + 1
    -- Return true to keep interval timer running or false to stop.
    return true
end


function start()
    -- The "start" function is called when the server is ready.
    -- At this point, a timer can be created.
    mg.set_interval(timer, 5)
end


function stop()
    -- The "stop" function is called when the server is stopping.
end

function log(ri)
    -- The "log" function can be used to
	-- (a) filter messages and return boolean: true (log) or false (do not log)
	-- (b) format log message and return it as string (empty string will not log)
	-- (c) forward the log data to an external log
	
	-- Example: Log only if the request was not made from localhost
	return (ri.remote_addr ~= "127.0.0.1");
end

-- Return false to abort server startup.
return true;
