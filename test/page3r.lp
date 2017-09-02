HTTP/1.0 200 OK
Content-Type: text/html

<html><body>


<p>This is a test page of mg.include in a Lua server page, served by the
<a href="https://github.com/civetweb/civetweb/">CivetWeb web server</a>.
</p><p>
<?
  script = "page2.lp"
  mg.write("Output of " .. script .. " (path type: \"rel\"):\n")
?>
</p><p>
<?
  mg.include(script, "rel")
?>
</p>

</body></html>
