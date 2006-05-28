static int read_properties(FILE *pipe, const char *separators, int (*read)(char *, int, char *, int));
struct int_map {
	const char *name;
	int namelen;
	int value;
};

static int
set_from_int_map(struct int_map *map, size_t map_size,
		 int *value, const char *name, int namelen)
{

	int i;

	for (i = 0; i < map_size; i++)
		if (namelen == map[i].namelen &&
		    !strncasecmp(name, map[i].name, namelen)) {
			*value = map[i].value;
			return OK;
		}

	return ERR;
}

static char *
chomp_string(char *name)
{
	int namelen;

	while (isspace(*name))
		name++;

	namelen = strlen(name) - 1;
	while (namelen > 0 && isspace(name[namelen]))
		name[namelen--] = 0;

	return name;
}

"  -b[N], --tab-size[=N]       Set number of spaces for tab expansion\n"
/**
 * FILES
 * -----
 * '~/.tig'::
 *	User configuration file. See "<<config-options, Configuration options>>"
 *	section for examples.
 *
 * '.git/config'::
 *	Repository config file. Read on startup with the help of
 *	git-repo-config(1).
 **/
/**
 * [[config-options]]
 * User Configuration file
 * -----------------------
 * You can permanently set an option by putting it in the `~/.tig` file.
 * The file consists of a series of 'commands'.  Each
 * line of the file may contain only one command.
 *
 * The hash mark ('#'), or semi-colon (';') is used as a 'comment' character.
 * All text after the comment character to the end of the line is ignored.
 * You can use comments to annotate your initialization file.
 *
 * Some sample options:
 *
 * ==========================================================================
 *	# Diff colors
 *	color diff-header	yellow	default
 *	color diff-index	blue	default
 *	color diff-chunk	magenta	default
 *	# UI colors
 *	color title-blur	white	blue
 *	color title-focus	white	blue	bold
 * ==========================================================================
 *
 * [[color-options]]
 * Color options
 * ~~~~~~~~~~~~~
 * Color options control highlighting and the user interface styles.
 * If  your terminal supports color, these commands can be used to assign
 * foreground/backgound combinations to certain areas. Optionally, an
 * attribute can be given as the last parameter. The syntax is:
 *
 * [verse]
 * ..........................................................................
 *	*color* 'area' 'fgcolor' 'bgcolor' '[attributes]'
 * ..........................................................................
 *
 * Valid colors include: *white*, *black*, *green*, *magenta*, *blue*, *cyan*,
 * *yellow*, *red*, *default*. Use *default* to refer to the default terminal
 * colors.
 **/

static struct int_map color_map[] = {
#define COLOR_MAP(name) { #name, STRING_SIZE(#name), COLOR_##name }
	COLOR_MAP(DEFAULT),
	COLOR_MAP(BLACK),
	COLOR_MAP(BLUE),
	COLOR_MAP(CYAN),
	COLOR_MAP(GREEN),
	COLOR_MAP(MAGENTA),
	COLOR_MAP(RED),
	COLOR_MAP(WHITE),
	COLOR_MAP(YELLOW),
};

/**
 * Valid attributes include: *normal*, *blink*, *bold*, *dim*, *reverse*, *standout*,
 * and *underline*. Note, not all attributes may be supported by the terminal.
 **/
static struct int_map attr_map[] = {
#define ATTR_MAP(name) { #name, STRING_SIZE(#name), A_##name }
	ATTR_MAP(NORMAL),
	ATTR_MAP(BLINK),
	ATTR_MAP(BOLD),
	ATTR_MAP(DIM),
	ATTR_MAP(REVERSE),
	ATTR_MAP(STANDOUT),
	ATTR_MAP(UNDERLINE),
};

/**
 * Valid area names are described below. Note, all names are case-insensitive,
 * and you may use '-', '_', and '.' interchangeably. So "Diff-Header",
 * "DIFF_HEADER", and "diff.header" are the same.
 *
 * --
 **/
/**
 * Diff markup::
 *
 * Options concerning diff start, chunks and lines added and deleted.
 *
 * *diff-header*, *diff-chunk*, *diff-add*, *diff-del*
 **/ \
LINE(DIFF_HEADER,  "diff --git ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
/**
 * Enhanced git diff markup::
 *
 * Extra diff information emitted by the git diff machinery, such as mode
 * changes, rename detection, and similarity.
 *
 * *diff-oldmode*, *diff-newmode*, *diff-copy-from*, *diff-copy-to*,
 * *diff-rename-from*, *diff-rename-to*, *diff-similarity* *diff-dissimilarity*
 * *diff-tree*, *diff-index*
 **/ \
LINE(DIFF_INDEX,	"index ",	  COLOR_BLUE,	COLOR_DEFAULT,	0), \
LINE(DIFF_OLDMODE,	"old file mode ", COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_NEWMODE,	"new file mode ", COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_COPY_FROM,	"copy from",	  COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_COPY_TO,	"copy to",	  COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_RENAME_FROM,	"rename from",	  COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_RENAME_TO,	"rename to",	  COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_SIMILARITY,   "similarity ",	  COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_DISSIMILARITY,"dissimilarity ", COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_TREE,		"diff-tree ",	  COLOR_BLUE,	COLOR_DEFAULT,	0), \
/**
 * Pretty print commit headers::
 *
 * Commit diffs and the revision logs are usually formatted using pretty
 * printed headers , unless `--pretty=raw` was given. This includes lines,
 * such as merge info, commit ID, and author and comitter date.
 *
 * *pp-author*, *pp-commit*, *pp-merge*, *pp-date*, *pp-adate*, *pp-cdate*
 **/ \
/**
 * Raw commit header::
 *
 * Usually shown when `--pretty=raw` is given, however 'commit' is pretty
 * much omnipresent.
 *
 * *commit*, *parent*, *tree*, *author*, *committer*
 **/ \
/**
 * Commit message::
 *
 * For now only `Signed-off-by lines` are colorized.
 *
 * *signoff*
 **/ \
/**
 * UI colors::
 *
 * Colors for text not matching any of the above: *default*
 *
 * Status window colors: *status*
 *
 * Title window colors: *title-blur*, *title-focus*
 *
 * Cursor line colors: *cursor*
 *
 * Main view specific: *main-date*, *main-author*, *main-commit*, *main-delim*,
 * *main-tag*, *main-ref*
 **/ \
LINE(MAIN_REF,     "",			COLOR_CYAN,	COLOR_DEFAULT,	A_BOLD), \
/**
 * --
 **/


/*
 * Line-oriented content detection.
 */
	const char *name;	/* Option name. */
	int namelen;		/* Size of option name. */
	{ #type, STRING_SIZE(#type), (line), STRING_SIZE(line), (fg), (bg), (attr) }
static struct line_info *
get_line_info(char *name, int namelen)
{
	enum line_type type;
	int i;

	/* Diff-Header -> DIFF_HEADER */
	for (i = 0; i < namelen; i++) {
		if (name[i] == '-')
			name[i] = '_';
		else if (name[i] == '.')
			name[i] = '_';
	}

	for (type = 0; type < ARRAY_SIZE(line_info); type++)
		if (namelen == line_info[type].namelen &&
		    !strncasecmp(line_info[type].name, name, namelen))
			return &line_info[type];

	return NULL;
}

#define set_color(color, name, namelen) \
	set_from_int_map(color_map, ARRAY_SIZE(color_map), color, name, namelen)

#define set_attribute(attr, name, namelen) \
	set_from_int_map(attr_map, ARRAY_SIZE(attr_map), attr, name, namelen)

static int
read_option(char *opt, int optlen, char *value, int valuelen)
{
	optlen = strcspn(opt, "#;");
	if (optlen == 0)
		/* The whole line is a comment. */
		return OK;

	else if (opt[optlen] != 0)
		/* Part of the option name is a comment, so the value part
		 * should be ignored. */
		valuelen = 0;
	else
		/* Else look for comment endings in the value. */
		valuelen = strcspn(value, "#;");

	opt[optlen] = value[valuelen] = 0;

	/* Reads: "color" object fgcolor bgcolor [attr] */
	if (!strcmp(opt, "color")) {
		struct line_info *info;

		value = chomp_string(value);
		valuelen = strcspn(value, " \t");
		info = get_line_info(value, valuelen);
		if (!info)
			return ERR;

		value = chomp_string(value + valuelen);
		valuelen = strcspn(value, " \t");
		if (set_color(&info->fg, value, valuelen) == ERR)
			return ERR;

		value = chomp_string(value + valuelen);
		valuelen = strcspn(value, " \t");
		if (set_color(&info->bg, value, valuelen) == ERR)
			return ERR;

		value = chomp_string(value + valuelen);
		if (*value &&
		    set_attribute(&info->attr, value, strlen(value)) == ERR)
			return ERR;

		return OK;
	}

	return ERR;
}

static int
load_options(void)
{
	char *home = getenv("HOME");
	char buf[1024];
	FILE *file;

	if (!home ||
	    snprintf(buf, sizeof(buf), "%s/.tig", home) >= sizeof(buf))
		return ERR;

	/* It's ok that the file doesn't exist. */
	file = fopen(buf, "r");
	if (!file)
		return OK;

	return read_properties(file, " \t", read_option);
}


	return read_properties(popen(cmd, "r"), "\t", read_ref);
read_repo_config_option(char *name, int namelen, char *value, int valuelen)
load_repo_config(void)
	return read_properties(popen("git repo-config --list", "r"),
			       "=", read_repo_config_option);
read_properties(FILE *pipe, const char *separators,
		char *value;
		size_t namelen;
		size_t valuelen;
		name = chomp_string(name);
		namelen = strcspn(name, separators);

		if (name[namelen]) {
			name[namelen] = 0;
			value = chomp_string(name + namelen + 1);
	if (load_options() == ERR)
		die("Failed to load user config.");

	/* Load the repo config file so options can be overwritten from
	if (load_repo_config() == ERR)