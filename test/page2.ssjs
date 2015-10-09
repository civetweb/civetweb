conn.write("HTTP/1.0 200 OK\r\n")
conn.write("Content-Type: text/html\r\n")
conn.write("\r\n")
conn.write("<html><body>\r\n")
conn.write("<p>This is an example of a server side JavaScript, served by the ")
conn.write('<a href="https://github.com/civetweb/civetweb/">CivetWeb web server</a>.')
conn.write("</p>\r\n")


function print_elements(title, obj)
{
  conn.write("<p>\r\n");
  conn.write("<b>" + title + "</b><br>\r\n");
  elms = Object.getOwnPropertyNames(obj)

  for (var i = 0; i < elms.length; i++) {
    conn.write(JSON.stringify(elms[i]) + ":<br>\r\n")
    conn.write("Type: " + typeof(obj[elms[i]]) + "<br>\r\n")
    conn.write(JSON.stringify(Object.getOwnPropertyDescriptor(obj, elms[i]))  + "<br>\r\n")
    conn.write("<br>\r\n")
  }
  conn.write('<br></p>\r\n')
}


print_elements("conn", conn)
print_elements("civetweb", civetweb)


conn.write('</body></html>\r\n')
