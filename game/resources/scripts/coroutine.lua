coro = {}
function coro.wait (time)
	local finish_time = world.game_time() + time
	while world.game_time() < finish_time do
		coroutine.yield()
	end
end
