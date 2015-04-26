local lunit = require('lunitx')
local cURL = require("cURL")

_ENV = lunit.module('simple','seeall')

function test_success_again() 
  assert_true (true, 'more tests work')
end

function test_fail() 
  assert_true ("not something true", 'fail test')
end


