
local civet = {}

civet.port=12345

function civet.start(docroot)
  -- TODO: use a property
  docroot = docroot or 'test/ci_tests/01_basic/docroot'
  assert(io.popen('./civetweb'
  .. " -listening_ports " .. civet.port
  .. " -document_root " .. docroot
  .. " > /dev/null 2>&1 &"
  ))
  -- wait until the server is listening (TODO: Linux only)
  while true do
    local f = assert(io.popen('netstat -an | grep '
      .. civet.port .. ' | grep -i listen'))
    local out = f:read('*all')
    if string.match(out, civet.port) then break end
    f:close()
  end
end

function civet.stop()
  os.execute('killall civetweb')
  -- wait until the server is listening (TODO: Linux only)
  while true do
    local f = assert(io.popen('netstat -an | grep '
      .. civet.port .. ' | grep -i listen'))
    local out = f:read('*all')
    if not string.match(out, civet.port) then break end
    f:close()
  end
end

return civet
