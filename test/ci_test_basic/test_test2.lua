local lunit = require('lunitx')
local cURL = require("cURL")

_ENV = lunit.module('simple','seeall')

function test_success_again() 
  assert_true (true, 'more tests work')
end


