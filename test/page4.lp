HTTP/1.0 200 OK
Content-Type: text/html

<html><body>

<h1>Lua Pages syntax test</h1>

<h2>Intro</h2>

<p>This is a test for the 
<a href="https://keplerproject.github.io/cgilua/manual.html#templates">Kepler Syntax of Lua Pages</a>, 
served by the
<a href="https://github.com/civetweb/civetweb/">CivetWeb web server</a>.</p>

<p>
While the native CivetWeb syntax for Lua pages is
<code>&lt;? script ?&gt;</code> and <code>&lt;?= expression ?&gt;</code>,
the "Kepler syntax" uses <code>&lt;?lua chunk ?&gt;</code>, <code>&lt;?lua= expression ?&gt;</code>, <code>&lt;% chunk %&gt;</code> and <code>&lt;%= expression %&gt;</code>.
</p>


<h2>Tags</h2>

<code>
&lt;? greeting = 'CiwetWeb' ?&gt;<br/>
&lt;strong&gt;&lt;?= greeting %&gt;&lt;/strong&gt;<br/>
</code><br/>
<? greeting = 'CiwetWeb' ?>
==> <strong><?= greeting ?></strong><br/>

<br/>
<code>
&lt;?lua greeting = 'Kepler' ?&gt;<br/>
&lt;strong&gt;&lt;?lua= greeting ?&gt;&lt;/strong&gt;<br/>
</code><br/>
<?lua greeting = 'Kepler' ?>
==> <strong><?lua= greeting ?></strong><br/>

<br/>
<code>
&lt;% greeting = 'Kepler %' ?&gt;<br/>
&lt;strong&gt;&lt;%= greeting %&gt;&lt;/strong&gt;<br/>
</code><br/>
<% greeting = 'Kepler %' %>
==> <strong><%= greeting %></strong><br/>


</body></html>
