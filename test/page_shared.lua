mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Cache-Control: no-cache, no-store, must-revalidate, max-age=0\r\n")
mg.write("Content-Type: text/plain\r\n")
mg.write("\r\n")

if not shared then
  mg.write("\"shared\" does not exist\n")
  return
elseif type(shared) ~= "userdata" then
  mg.write("\"shared\" is not userdata\n")
  return
end

-- Test with number
mg.write("\nNumber:\n")
x = shared.count
mg.write("Previous count was " .. tostring(x) .. " (type " .. type(x) .. ")\n")
x = x or 0
x = x + 1
shared.count = x
mg.write("Store new count " .. tostring(x) .. " (type " .. type(x) .. ")\n")
x = shared.count
mg.write("New count is " .. tostring(x) .. " (type " .. type(x) .. ")\n")

-- Test with name
mg.write("\nString:\n")
x = shared.name
mg.write("Previous name was " .. tostring(x) .. " (type " .. type(x) .. ")\n")
x = x or ""
l = string.len(x) % 26
x = x .. string.char(string.byte("a") + l)
shared.name = x
mg.write("Store new name " .. tostring(x) .. " (type " .. type(x) .. ")\n")
x = shared.name
mg.write("New name is " .. tostring(x) .. " (type " .. type(x) .. ")\n")


-- Test with boolean
mg.write("\nBoolean:\n")
x = shared.condition
mg.write("Previous condition was " .. tostring(x) .. " (type " .. type(x) .. ")\n")
x = not x
shared.condition = x
mg.write("Store new condition " .. tostring(x) .. " (type " .. type(x) .. ")\n")
x = shared.condition
mg.write("New condition is " .. tostring(x) .. " (type " .. type(x) .. ")\n")


-- Test using "shared" as array
mg.write("\nArray element:\n")
mg.write("Previous array was: ")
for i=1,10 do
  x = shared[i]
  mg.write(tostring(x) .. " (" .. type(x):sub(1,1) .. ") ")
end
mg.write("\n")
for i=1,10 do
  shared[i] = shared[(i + 1) % 10 + 1] or i
end
mg.write("Shifted array is:   ")
for i=1,10 do
  x = shared[i]
  mg.write(tostring(x) .. " (" .. type(x):sub(1,1) .. ") ")
end
mg.write("\n")


-- Test using "shared" as array
mg.write("\nBoolean indexed element:\n")
x = shared[true]
y = shared[false]
mg.write("Previous elements were "
         .. tostring(x) .. " (type " .. type(x) .. ") / "
         .. tostring(y) .. " (type " .. type(y) .. ")\n")
x = not x
y = not x
shared[true] = x
shared[false] = y
mg.write("New elements are "
         .. tostring(x) .. " (type " .. type(x) .. ") / "
         .. tostring(y) .. " (type " .. type(y) .. ")\n")


-- Check if experimental functions (starting with __) are available
if not shared.__inc then
  mg.write("\nExperimental functions not available\n")
  return
else
  mg.write("\nTesting experimental functions\n")
end


-- Test __inc/__dec functions
if not shared.x then
  shared.x = 0
  shared.y = 0
end
mg.write("__inc(x) = " .. shared.__inc("x") .. "\n")
mg.write("__dec(y) = " .. shared.__dec("y") .. "\n")


-- Test __add function
if not shared.x then
  shared.x = 0
  shared.y = 0
end
mg.write("__add(x, 10) = " .. shared.__add("x", 10) .. "\n")
mg.write("__add(y, -10) = " .. shared.__add("y", -10) .. "\n")


-- end
