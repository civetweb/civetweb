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

-- Return false to abort server startup.
return true;
