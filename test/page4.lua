-- This test checks the Lua functions:
-- get_var, get_cookie, md5, url_encode

mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/html; charset=utf-8\r\n")
mg.write("\r\n")

mg.write("<html>\r\n<head><title>Civetweb Lua script test page 4</title></head>\r\n<body><pre>\r\n")

-- get_var of query_string 
if not mg.request_info.query_string then    
    mg.write("No query string!\r\n")
else
    mg.write("Variables within query string:\r\n")
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
mg.write("MD5:\r\n")
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
mg.write("TODO: Test get_cookie\r\n")
mg.write("\r\n")

-- url_encode
mg.write("URL:\r\n")
if mg.url_encode("") == "" then
    mg.write("  url_encode of empty string OK\r\n")
else
    mg.write("  Error: url_encode of empty string NOT OK\r\n")
end
raw_string = [[ !"#$%&'()*+,-./0123456789:;<=>?@]]
mg.write("  original string: " .. raw_string .. "\r\n")
url_string = mg.url_encode(raw_string):upper()
ref_string = "%20!%22%23%24%25%26'()*%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40" -- from http://www.w3schools.com/tags/ref_urlencode.asp
mg.write("  mg-url:        " .. url_string .. "\r\n")
mg.write("  reference url: " .. ref_string .. "\r\n")
dec_url_string = mg.url_decode(url_string)
dec_ref_string = mg.url_decode(ref_string)
mg.write("  decoded mg-url:        " .. dec_url_string .. "\r\n")
mg.write("  decoded reference url: " .. dec_ref_string .. "\r\n")
dec_url_string = mg.url_decode(url_string, false)
dec_ref_string = mg.url_decode(ref_string, false)
mg.write("  decoded mg-url:        " .. dec_url_string .. "\r\n")
mg.write("  decoded reference url: " .. dec_ref_string .. "\r\n")
dec_url_string = mg.url_decode(url_string, true)
dec_ref_string = mg.url_decode(ref_string, true)
mg.write("  decoded mg-url:        " .. dec_url_string .. "\r\n")
mg.write("  decoded reference url: " .. dec_ref_string .. "\r\n")
mg.write("\r\n")


-- end of page
mg.write("</pre></body>\r\n</html>\r\n")