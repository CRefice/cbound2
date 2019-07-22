function rising(fn)
	return function(self, val)
		if val > 0.0 then
			fn(self)
		end
	end
end

function falling(fn)
	return function(self, val)
		if val < 0.0 then
			fn(self)
		end
	end
end

