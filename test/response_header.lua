body = [[<html><body><p>
Hello world!
</p>
</body></html>
]]

--mg.response.status = 200 -- "200 OK" is the default
mg.response.http_headers["Content-Type"] = "text/html";
mg.response.http_headers["Content-Length"] = tostring(string.len(body));
mg.response.send();
mg.write(body);
