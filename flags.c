#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sys/xattr.h>

#include "flags.h"

ssize_t set_pax_flags(const char *flags, size_t flags_len, const char *path) {
    for (size_t i = 0; i < flags_len; i++) {
        int flag = tolower(flags[i]);
        if (!strchr(PAX_FLAGS, flag)) {
            errno = EINVAL;
            return -1;
        }
    }

    return setxattr(path, "user.pax.flags", flags, flags_len, 0);
}

ssize_t get_pax_flags(char *flags, size_t flags_len, const char *path) {
    return getxattr(path, "user.pax.flags", flags, flags_len);
}

int update_attributes(void) {
    FILE *conf = fopen("/etc/paxd.conf", "r");
    if (!conf) {
        warn("could not open /etc/paxd.conf");
        return -1;
    }

    char *line = NULL;
    size_t line_len = 0;
    int rc = 0;

    for (size_t n = 1;; n++) {
        ssize_t bytes_read = getline(&line, &line_len, conf);
        if (bytes_read == -1) {
            if (ferror(conf)) {
                warn("failed to read line from /etc/paxd.conf");
                rc = -1;
                break;
            } else {
                break;
            }
        }

        if (line[0] == '\n' || line[0] == '\0' || line[0] == '#') {
            continue; // ignore empty lines and comments
        }

        if (line[bytes_read - 1] == '\n') {
            line[bytes_read - 1] = '\0';
        }

        char *flags = line + strspn(line, " \t"); // skip initial whitespace

        const char *split = flags + strcspn(flags, " \t"); // find the end of the specified flags
        const char *path = split + strspn(split, " \t"); // find the start of the path

        if (*split == '\0' || *path != '/') {
            warnx("line %zd: ignoring invalid line: %s", n, flags);
            rc = -1;
            break;
        }

        size_t flags_len = (size_t)(split - flags);
        flags[flags_len] = 0;

        if (set_pax_flags(flags, flags_len, path) < 0) {
            if (errno == EINVAL) {
                warnx("line %zd: invalid pax flags: %s", n, flags);
                rc = -1;
            } else if (errno != ENOENT) {
                warn("failed to set pax flags on %s", path);
                rc = -1;
            }
        }
    }

    free(line);
    fclose(conf);
    return rc;
}
