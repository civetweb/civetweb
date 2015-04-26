local lunit = require('lunitx')
local cURL = require("cURL")

_ENV = lunit.module('enhanced','seeall')

function test_success() 
  assert_true (true, 'tests work')
end
