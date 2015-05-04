socket = require "socket"

local civet = {}

-- default params
civet.port=12345
civet.max_retry=100
civet.start_delay=0.1

function civet.start(docroot)
  -- TODO: use a property
  docroot = docroot or 'ci/test/01_basic/docroot'
  assert(io.popen('./civetweb'
  .. " -listening_ports " .. civet.port
  .. " -document_root " .. docroot
  .. " > /dev/null 2>&1 &"
  ))
  -- wait until the server answers
  for i=1,civet.max_retry do
    local s = socket.connect('127.0.0.1', civet.port)
    if s then
      s:close()
      break
    end
    socket.select(nil, nil, civet.start_delay) -- sleep
  end
end

function civet.stop()
  os.execute('killall civetweb')
  -- wait until the server port closes
  for i=1,civet.max_retry do
    local s = socket.connect('127.0.0.1', civet.port)
    if not s then
      break
    end
    s:close()
    socket.select(nil, nil, civet.start_delay) -- sleep
  end
end

return civet
