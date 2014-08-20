-- This test checks the Lua functions:
-- get_var, get_cookie, md5, url_encode

now = os.time()
cookie_name = "civetweb-test-page4"

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

mg.write("<html>\r\n<head><title>Civetweb Lua script test page 5</title></head>\r\n<body>\r\n")
mg.write("<p>Test of Civetweb Lua Import Functions:</p>\r\n");
mg.write("<pre>\r\n");
-- begin of page

require 'lxp'

-- end of page
mg.write("</pre>\r\n</body>\r\n</html>\r\n")
