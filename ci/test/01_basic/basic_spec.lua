civet = require "ci/test/civet"
local curl = require "cURL"

describe("civetweb basic", function()

  setup(function()
    civet.start()
  end)

  teardown(function()
    civet.stop()
  end)


  it("should serve a simple get request", function()

    local out = ""
    function capture(str)
      out = out .. str
    end

    local c = curl.easy()
      :setopt_url('http://localhost:' .. civet.port .. "/")
      :setopt_writefunction(capture)
      :perform()
    :close()

    --print('rescode:' .. c.getinfo(curl.INFO_RESPONSE_CODE))

    assert.are.equal('Index of', string.match(out, 'Index of'))
    assert.are.equal('01_basic_test_dir', string.match(out, '01_basic_test_dir'))
    assert.are.equal('01_basic_test_file', string.match(out, '01_basic_test_file'))
  end)

end)
