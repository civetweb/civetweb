shared.timer = 0

function timer()
	shared.timer = shared.timer + 1
end

mg.set_interval(timer,5)

return true;
