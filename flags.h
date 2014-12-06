#ifndef FLAGS_H
#define FLAGS_H

static const char PAX_FLAGS[] = "pemrs";
#define PAX_LEN (sizeof(PAX_FLAGS) - 1)

int update_attributes(void);
ssize_t set_pax_flags(const char *flags, size_t flags_len, const char *path);
ssize_t get_pax_flags(char *flags, size_t flags_len, const char *path);

#endif
