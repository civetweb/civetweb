conn.write("HTTP/1.0 200 OK\r\n")
conn.write("Content-Type: text/html\r\n")
conn.write("\r\n")
conn.write("<html><body>\r\n")
conn.write("<p>This is an example of a server side JavaScript, served by the ")
conn.write('<a href="https://github.com/civetweb/civetweb/">CivetWeb web server</a>.')
conn.write("</p>\r\n<p>")


elms = Object.getOwnPropertyNames(conn)

for (var i = 0; i < elms.length; i++) {
  conn.write(JSON.stringify(elms[i]))
  conn.write(JSON.stringify(Object.getOwnPropertyDescriptor(conn, elms[i])))
  conn.write("<br>\r\n")
}


conn.write('</p>\r\n')
conn.write('</body></html>\r\n')
