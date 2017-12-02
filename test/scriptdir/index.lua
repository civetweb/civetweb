mg.write("HTTP/1.1 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")

mg.write("<!DOCTYPE html>\r\n")
mg.write("<html>\r\n")
mg.write("<head>\r\n")
mg.write("  <meta charset=\"UTF-8\">\r\n")
mg.write("  <title>HTTP reflector</title>\r\n")
mg.write([[
<style type="text/css" media="screen">
table{
border-collapse:collapse;
border:1px solid #FF0000;
text-align: left;
}
table td{
border:1px solid #FF0000;
}
table th{
border:1px solid #FF0000;
}
</style>
]]);
mg.write("</head>\r\n")
mg.write("<body>\r\n")

mg.write("  <h1>HTTP reflector</h1>\r\n")
mg.write(mg.request_info.request_method .. " " .. mg.request_info.request_uri .. "  HTTP/" .. mg.request_info.http_version .. "\r\n")

mg.write("  <h2>Header</h2>\r\n")
mg.write("    <table>\r\n");
mg.write("      <tr>\r\n");
mg.write("        <th>Header Name</th>\r\n")
mg.write("        <th>Value</th>\r\n")
mg.write("      </tr>\r\n");
for k,v in pairs(mg.request_info.http_headers) do
  mg.write("      <tr>\r\n");
  mg.write("        <td>" .. k .. "</td>\r\n")
  mg.write("        <td>" .. v .. "</td>\r\n")
mg.write("      </tr>\r\n");
end
mg.write("    </table>\r\n");

mg.write("  <h2>Body</h2>\r\n")
mg.write("<pre>\r\n");

repeat
  local r = mg.read()
  if (r) then
    mg.write(r)
  end
until not r

mg.write("\r\n</pre>\r\n");

mg.write("  <h2>-</h2>\r\n")
mg.write("</body>\r\n")
mg.write("</html>\r\n")


