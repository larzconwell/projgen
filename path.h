#ifndef PATH_H
#define PATH_H

extern bstring expand_path(bstring);
extern bstring path_join(const char *, ...);
extern int mkdirall(bstring, mode_t);

#endif
