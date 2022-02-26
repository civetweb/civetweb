/* Reimplementation of pattern matching */
/* This file is part of the CivetWeb web server.
 * See https://github.com/civetweb/civetweb/
 */

#if !defined(MG_MATCH_CONTEXT_MAX_MATCHES)
#define MG_MATCH_CONTEXT_MAX_MATCHES (32)
#endif

struct mg_match_context {
	int case_sensitive;
	size_t num_matches;
	const char *match_str[MG_MATCH_CONTEXT_MAX_MATCHES];
	size_t match_len[MG_MATCH_CONTEXT_MAX_MATCHES];
};


/* Add a new match to the list of matches */
static void
match_context_push(const char *str, size_t len, struct mg_match_context *mcx)
{
	if (mcx->num_matches < MG_MATCH_CONTEXT_MAX_MATCHES) {
		mcx->match_str[mcx->num_matches] = str;
		mcx->match_len[mcx->num_matches] = len;
		mcx->num_matches++;
	}
}


static ptrdiff_t
mg_match_impl(const char *pat,
              size_t pat_len,
              const char *str,
              struct mg_match_context *mcx)
{
	/* Parse string */
	size_t i_pat = 0; /* Pattern index */
	size_t i_str = 0; /* Pattern index */

	uint8_t case_sensitive = ((mcx != NULL) ? mcx->case_sensitive : 0);

	while (i_pat < pat_len) {

		/* Pattern ? matches one character, except / and NULL character */
		if ((pat[i_pat] == '?') && (str[i_str] != '\0')
		    && (str[i_str] != '/')) {
			size_t i_str_start = i_str;
			do {
				/* Advance as long as there are ? */
				i_pat++;
				i_str++;
			} while ((pat[i_pat] == '?') && (str[i_str] != '\0')
			         && (str[i_str] != '/') && (i_pat < pat_len));

			/* If we have a match context, add the substring we just found */
			if (mcx) {
				match_context_push(str + i_str_start, i_str - i_str_start, mcx);
			}

			/* Reached end of pattern ? */
			if (i_pat == pat_len) {
				return (ptrdiff_t)i_str;
			}
		}

		/* Pattern $ matches end of string */
		if (pat[i_pat] == '$') {
			return (str[i_str] == '\0') ? i_str : -1;
		}

		/* Pattern * or ** matches multiple characters */
		if (pat[i_pat] == '*') {
			size_t len; /* lenght matched by "*" or "**" */
			ptrdiff_t ret;

			i_pat++;
			if ((pat[i_pat] == '*') && (i_pat < pat_len)) {
				/* Pattern ** matches all */
				i_pat++;
				len = strlen(str + i_str);
			} else {
				/* Pattern * matches all except / character */
				len = strcspn(str + i_str, "/");
			}

			if (i_pat == pat_len) {
				/* End of pattern reached. Add all to match context. */
				if (mcx) {
					match_context_push(str + i_str, len, mcx);
				}
				return (i_str + len);
			}

			/* This loop searches for the longest possible match */
			do {
				ret = mg_match_impl(pat + i_pat,
				                    (pat_len - (size_t)i_pat),
				                    str + i_str + len,
				                    mcx);
			} while ((ret == -1) && (len-- > 0));

			/* If we have a match context, add the substring we just found */
			if (ret >= 0) {
				if (mcx) {
					match_context_push(str + i_str, len, mcx);
				}
				return (i_str + ret + len);
			}

			return -1;
		}


		/* Single character compare */
		if (case_sensitive) {
			if (pat[i_pat] != str[i_str]) {
				/* case sensitive compare: mismatch */
				return -1;
			}
		} else if (lowercase(&pat[i_pat]) != lowercase(&str[i_str])) {
			/* case insensitive compare: mismatch */
			return -1;
		}

		i_pat++;
		i_str++;
	}
	return (ptrdiff_t)i_str;
}


static ptrdiff_t
mg_match_alternatives(const char *pat,
                      size_t pat_len,
                      const char *str,
                      struct mg_match_context *mcx)
{
	const char *match_alternative = (const char *)memchr(pat, '|', pat_len);
	while (match_alternative != NULL) {
		/* Split at | for alternative match */
		size_t left_size = (size_t)(match_alternative - pat);

		/* Try left string first */
		ptrdiff_t ret = mg_match_impl(pat, left_size, str, mcx);
		if (ret >= 0) {
			/* A 0-byte match is also valid */
			return ret;
		}

		/* If no match: try right side */
		pat += left_size + 1;
		pat_len -= left_size + 1;
		match_alternative = (const char *)memchr(pat, '|', pat_len);
	}

	/* Handled all | operators. This is the final string. */
	return mg_match_impl(pat, pat_len, str, mcx);
}


/* Export as public API? */
static ptrdiff_t
mg_match(const char *pat,
         size_t pat_len,
         const char *str,
         struct mg_match_context *mcx)
{
	ptrdiff_t ret;
	if (mcx != NULL) {
		mcx->num_matches = 0;
		memset((void *)(mcx->match_str), 0, sizeof(mcx->match_str));
		memset(mcx->match_len, 0, sizeof(mcx->match_len));
	}
	ret = mg_match_alternatives(pat, pat_len, str, mcx);
	if (mcx != NULL) {
		if (ret < 0) {
			mcx->num_matches = 0;
			memset((void *)(mcx->match_str), 0, sizeof(mcx->match_str));
			memset(mcx->match_len, 0, sizeof(mcx->match_len));
		} else {
			/* TODO: Join matches */
		}
	}
	return ret;
}


static ptrdiff_t
match_prefix(const char *pattern, size_t pattern_len, const char *str)
{
	if (pattern == NULL) {
		return -1;
	}
	return mg_match_alternatives(pattern, pattern_len, str, NULL);
}


static ptrdiff_t
match_prefix_strlen(const char *pattern, const char *str)
{
	if (pattern == NULL) {
		return -1;
	}
	return mg_match_alternatives(pattern, strlen(pattern), str, NULL);
}

/* End of match.inl */
