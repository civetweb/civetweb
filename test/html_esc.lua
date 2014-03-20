htmlEscape = {    "&#x263a;", "&#x263b;", "&#x2665;", "&#x2666;", "&#x2663;", "&#x2660;", "&#x2022;", -- ASCII 1-7 (symbols for control characters, see code page 437)
      "&#x25d8;", "&#x25cb;", "&#x25d9;", "&#x2642;", "&#x2640;", "&#x266a;", "&#x266b;", "&#x263c;", -- ASCII 8-15
      "&#x25ba;", "&#x25c4;", "&#x2195;", "&#x203c;", "&#x00b6;", "&#x00a7;", "&#x25ac;", "&#x21a8;", -- ASCII 16-23
      "&#x2191;", "&#x2193;", "&#x21a8;", "&#x2190;", "&#x221f;", "&#x2192;", "&#x25b2;", "&#x25bc;", -- ASCII 24-31
      " ",        "!",        "&quot;",   "#",        "$",        "%",        "&amp;",    "'",        -- ASCII 32-39
      "(",        ")",        "*",        "+",        ",",        "-",        ".",        "/",        -- ASCII 40-47
      "0",        "1",        "2",        "3",        "4",        "5",        "6",        "7",        -- ASCII 48-55
      "8",        "9",        ":",        ";",        "&lt;",     "=",        "&gt;",     "?",        -- ASCII 56-63
      "@",        "A",        "B",        "C",        "D",        "E",        "F",        "G",        -- ASCII 64-71
      "H",        "I",        "J",        "K",        "L",        "M",        "N",        "O",        -- ASCII 72-79
      "P",        "Q",        "R",        "S",        "T",        "U",        "V",        "W",        -- ASCII 80-87
      "X",        "Y",        "Z",        "[",        "\\",       "]",        "^",        "_",        -- ASCII 88-95
      "`",        "a",        "b",        "c",        "d",        "e",        "f",        "g",        -- ASCII 96-103
      "h",        "i",        "j",        "k",        "l",        "m",        "n",        "o",        -- ASCII 104-111
      "p",        "q",        "r",        "s",        "t",        "u",        "v",        "w",        -- ASCII 112-119
      "x",        "y",        "z",        "{",        "|",        "}",        "~",        "&#x2302;", -- ASCII 120-127
      "&Ccedil;", "&uuml;",   "&eacute;", "&acirc;",  "&auml;",   "&agrave;", "&aring;",  "&ccedil;", -- 128-135 (dos code page 850)
      "&ecirc;",  "&euml;",   "&egrave;", "&iuml;",   "&icirc;",  "&igrave;", "&Auml;",   "&Aring;",  -- 136-143
      "&Eacute;", "&aelig;",  "&AElig;",  "&ocirc;",  "&ouml;",   "&ograve;", "&ucirc;",  "&ugrave;", -- 144-151
      "&yuml;",   "&Ouml;",   "&Uuml;",   "&oslash;", "&#x00a3;", "&Oslash;", "&#x00d7;", "&#x0192;", -- 152-159
      "&aacute;", "&iacute;", "&oacute;", "&uacute;", "&ntilde;", "&Ntilde;", "&#x00aa;", "&#x00ba;", -- 160-167
      "&#x00bf;", "&#x00ae;", "&#x00ac;", "&#x00bd;", "&#x00bc;", "&#x00a1;", "&#x00ab;", "&#x00bb;", -- 168-175
      "&#x2591;", "&#x2592;", "&#x2593;", "&#x2502;", "&#x2524;", "&Aacute;", "&Acirc;",  "&Agrave;", -- 176-183
      "&#x00a9;", "&#x2563;", "&#x2551;", "&#x2557;", "&#x255d;", "&cent;",   "&#x00a5;", "&#x2510;", -- 184-191
      "&#x2514;", "&#x2534;", "&#x252c;", "&#x251c;", "&#x2500;", "&#x253c;", "&atilde;", "&Atilde;", -- 192-199
      "&#x255a;", "&#x2554;", "&#x2569;", "&#x2566;", "&#x2560;", "&#x2550;", "&#x256c;", "&#x00a4;", -- 200-207
      "&eth;",    "&ETH;",    "&Ecirc;",  "&Euml;",   "&Egrave;", "&#x0131;", "&Iacute;", "&Icirc;",  -- 208-215
      "&Iuml;",   "&#x2518;", "&#x250c;", "&#x2588;", "&#x2584;", "&#x00a6;", "&Igrave;", "&#x2580;", -- 216-223
      "&Oacute;", "&szlig;",  "&Ocirc;",  "&Ograve;", "&otilde;", "&Otilde;", "&#x00b5;", "&thorn;",  -- 224-231
      "&THORN;",  "&Uacute;", "&Ucirc;",  "&Ugrave;", "&yacute;", "&Yacute;", "&#x00af;", "&#x00b4;", -- 232-239
      "&equiv;",  "&#x00b1;", "&#x2017;", "&#x00be;", "&#x00b6;", "&#x00a7;", "&#x00f7;", "&#x00b8;", -- 240-247
      "&#x00b0;", "&#x00a8;", "&#x00b7;", "&#x00b9;", "&#x00b3;", "&#x00b2;", "&#x25a0;", "&#9633;",  -- 248-255 (use empty box for 255)
};
htmlEscape[0] = "&middot;" -- in this table, we use a 8 bit character set, where every has a different graphical representation

-- the conversion table should work as a convertion function for strings as well
setmetatable(htmlEscape, {__call = function (tab,str) return string.gsub(str, ".", function (c) return tab[c:byte()] end) end})


function htmlEsc(txt)
    s = txt:gsub("%&", "&amp;")
    s = s:gsub("%<", "&lt;")
    return s:gsub("%>", "&gt;")
end


function iso8859_1_to_utf8(txt)
    local s = txt:gsub(".",
      function (c)
        local b = c:byte()
        if b < 128 then
          return c
        elseif b < 192 then
          return string.char(194, b)
        else
          return string.char(195, b-64)
        end
      end)
    return s
end
