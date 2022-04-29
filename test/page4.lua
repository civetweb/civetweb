-- This test checks the Lua functions:
-- get_var, get_cookie, md5, url_encode

now = os.time()
cookie_name = "CivetWeb-test-page4"

if mg.request_info.http_headers.Cookie then
   cookie_value = tonumber(mg.get_cookie(mg.request_info.http_headers.Cookie, cookie_name))
end

mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/html; charset=utf-8\r\n")
mg.write("Cache-Control: max-age=0, must-revalidate\r\n")
if not cookie_value then
    mg.write("Set-Cookie: " .. cookie_name .. "=" .. tostring(now) .. "\r\n")
end
mg.write("\r\n")

mg.write("<html>\r\n<head><title>CivetWeb Lua script test page 4</title></head>\r\n<body>\r\n")
mg.write("<p>Test of CivetWeb Lua Functions:</p>\r\n");
mg.write("<pre>\r\n");

-- get_var of query_string
mg.write("get_var test (check query string):\r\n")
if not mg.request_info.query_string then
    mg.write("  No query string. You may try <a href='?a=a1&amp;junk&amp;b=b2&amp;cc=cNotSet&amp;d=a, b and d should be set&amp;z=z'>this example</a>.\r\n")
else
    for _,var in ipairs({'a','b','c','d'}) do
       value = mg.get_var(mg.request_info.query_string, var);
       if value then
         mg.write("  Variable " .. var .. ": value " .. value .. "\r\n");
       else
         mg.write("  Variable " .. var .. " not set\r\n");
       end
    end
end
mg.write("\r\n")

-- md5
mg.write("MD5 test:\r\n")
test_string = "abcd\0efgh"
mg.write("  String with embedded 0, length " .. string.len(test_string))
test_md5 = mg.md5(test_string)
mg.write(", MD5 " .. test_md5 .. "\r\n")
if mg.md5("") == "d41d8cd98f00b204e9800998ecf8427e" then
    mg.write("  MD5 of empty string OK\r\n")
else
    mg.write("  Error: MD5 of empty string NOT OK\r\n")
end
if mg.md5("The quick brown fox jumps over the lazy dog.") == "e4d909c290d0fb1ca068ffaddf22cbd0" then
    mg.write("  MD5 of test string OK\r\n")
else
    mg.write("  Error: MD5 of test string NOT OK\r\n")
end
mg.write("\r\n")

-- get_cookie
mg.write("Cookie test:\r\n")
if not cookie_value then
    mg.write("  Cookie not set yet. Please reload the page.\r\n")
else
    mg.write("  Cookie set to " .. cookie_value .. "\r\n")
    mg.write("  You visited this page " .. os.difftime(now, cookie_value) .. " seconds before.\r\n")
end
mg.write("\r\n")

-- test 'require' of other Lua scripts
mg.write("require test\r\n")
script_path = mg.script_name:match("(.*)page%d*.lua")
if type(script_path)=='string' then
    package.path = script_path .. "?.lua;" .. package.path
    mg.write("  Lua search path: " .. package.path .. "\r\n")
    require "html_esc"
    require "require_test"
    if htmlEscape then
      for i=0,15 do
        mg.write("  ")
        for j=0,15 do
            mg.write(tostring(htmlEscape[16*i+j]))
        end
        mg.write("\r\n")
      end
    else
      mg.write("  'require' test failed (htmlEscape)\r\n")
    end
    if HugeText then
      mg.write("\r\n")
      local ht = HugeText(os.date("%a %b. %d"))
      for i=1,#ht do
        mg.write("  " .. ht[i] .. "\r\n")
      end
    else
      mg.write("  'require' test failed (HugeText)\r\n")
    end
else
    mg.write("  name match failed\r\n")
end
mg.write("\r\n")

-- test get_response_code_text
mg.write("HTTP helper methods test:\r\n")
if (htmlEscape("<a b & c d>") == "&lt;a b &amp; c d&gt;") then
    mg.write("  htmlEscape test OK\r\n")
else
    mg.write("  Error: htmlEscape test NOT OK\r\n")
end
if (mg.get_response_code_text(200) == "OK") then
    mg.write("  get_response_code_text test OK\r\n")
else
    mg.write("  Error: get_response_code_text test NOT OK\r\n")
end
mg.write("\r\n")

-- url_encode
mg.write("URL encode/decode test:\r\n")
if mg.url_encode("") == "" then
    mg.write("  url_encode of empty string OK\r\n")
else
    mg.write("  Error: url_encode of empty string NOT OK\r\n")
end
raw_string = [[ !"#$%&'()*+,-./0123456789:;<=>?@]]
mg.write("  original string: " .. htmlEscape(raw_string) .. "\r\n")
mg_string = mg.url_encode(raw_string):upper()
ref_string = "%20!%22%23%24%25%26'()*%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40" -- from http://www.w3schools.com/tags/ref_urlencode.asp
mg.write("  mg-url:        " .. htmlEscape(mg_string) .. "\r\n")
mg.write("  reference url: " .. htmlEscape(ref_string) .. "\r\n")
dec_mg_string = mg.url_decode(mg_string)
dec_ref_string = mg.url_decode(ref_string)
mg.write("  decoded mg-url:        " .. htmlEscape(dec_mg_string) .. "\r\n")
mg.write("  decoded reference url: " .. htmlEscape(dec_ref_string) .. "\r\n")
dec_mg_string = mg.url_decode(mg_string, false)
dec_ref_string = mg.url_decode(ref_string, false)
mg.write("  decoded mg-url:        " .. htmlEscape(dec_mg_string) .. "\r\n")
mg.write("  decoded reference url: " .. htmlEscape(dec_ref_string) .. "\r\n")
dec_mg_string = mg.url_decode(mg_string, true)
dec_ref_string = mg.url_decode(ref_string, true)
mg.write("  decoded mg-url:        " .. htmlEscape(dec_mg_string) .. "\r\n")
mg.write("  decoded reference url: " .. htmlEscape(dec_ref_string) .. "\r\n")
mg.write("\r\n")

-- base64_encode
mg.write("BASE64 encode/decode test:\r\n")
raw_string = [[ !"#$%&'()*+,-./0123456789:;<=>?@]]
mg.write("  original string:  " .. htmlEscape(raw_string) .. "\r\n")
mg_string = mg.base64_encode(raw_string)
ref_string = "ICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9A" -- from http://www.base64encode.org/
mg.write("  mg-base64:        " .. htmlEscape(mg_string) .. "\r\n")
mg.write("  reference base64: " .. htmlEscape(ref_string) .. "\r\n")
dec_mg_string = mg.base64_decode(mg_string)
dec_ref_string = mg.base64_decode(ref_string)
mg.write("  decoded mg-base64:        " .. htmlEscape(dec_mg_string) .. "\r\n")
mg.write("  decoded reference base64: " .. htmlEscape(dec_ref_string) .. "\r\n")
mg.write("\r\n")
raw_string = [[<?> -?-]]
mg.write("  original string:  " .. htmlEscape(raw_string) .. "\r\n")
mg_string = mg.base64_encode(raw_string)
ref_string = "PD8+IC0/LQ==" -- from http://www.base64encode.org/
mg.write("  mg-base64:        " .. htmlEscape(mg_string) .. "\r\n")
mg.write("  reference base64: " .. htmlEscape(ref_string) .. "\r\n")
dec_mg_string = mg.base64_decode(mg_string)
dec_ref_string = mg.base64_decode(ref_string)
mg.write("  decoded mg-base64:        " .. htmlEscape(dec_mg_string) .. "\r\n")
mg.write("  decoded reference base64: " .. htmlEscape(dec_ref_string) .. "\r\n")
mg.write("\r\n")

-- random
mg.write("Random numbers:\r\n")
for i=1,10 do mg.write(string.format("%18.0f\r\n", mg.random())) end
mg.write("\r\n")

-- uuid
if mg.uuid then
mg.write("UUIDs:\r\n")
for i=1,10 do mg.write(string.format("%40s\r\n", mg.uuid())) end
mg.write("\r\n")
end

-- end of page
mg.write("</pre>\r\n</body>\r\n</html>\r\n")
