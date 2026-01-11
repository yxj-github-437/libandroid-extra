#ifndef _SPAWN_H_
#error "Never include this file directly; instead, include <spawn.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int posix_spawn(pid_t* _Nullable __pid, const char* _Nonnull __path, const posix_spawn_file_actions_t _Nullable * _Nullable __actions, const posix_spawnattr_t _Nullable * _Nullable __attr, char* const _Nullable __argv[_Nullable], char* const _Nullable __env[_Nullable]);
int posix_spawnp(pid_t* _Nullable __pid, const char* _Nonnull __file, const posix_spawn_file_actions_t _Nullable * _Nullable __actions, const posix_spawnattr_t _Nullable * _Nullable __attr, char* const _Nullable __argv[_Nullable], char* const _Nullable __env[_Nullable]);

int posix_spawnattr_init(posix_spawnattr_t _Nullable * _Nonnull __attr);
int posix_spawnattr_destroy(posix_spawnattr_t _Nonnull * _Nonnull __attr);

int posix_spawnattr_setflags(posix_spawnattr_t _Nonnull * _Nonnull __attr, short __flags);
int posix_spawnattr_getflags(const posix_spawnattr_t _Nonnull * _Nonnull __attr, short* _Nonnull __flags);

int posix_spawnattr_setpgroup(posix_spawnattr_t _Nonnull * _Nonnull __attr, pid_t __pgroup);
int posix_spawnattr_getpgroup(const posix_spawnattr_t _Nonnull * _Nonnull __attr, pid_t* _Nonnull __pgroup);

int posix_spawnattr_setsigmask(posix_spawnattr_t _Nonnull * _Nonnull __attr, const sigset_t* _Nonnull __mask);
int posix_spawnattr_setsigmask64(posix_spawnattr_t _Nonnull * _Nonnull __attr, const sigset64_t* _Nonnull __mask);
int posix_spawnattr_getsigmask(const posix_spawnattr_t _Nonnull * _Nonnull __attr, sigset_t* _Nonnull __mask);
int posix_spawnattr_getsigmask64(const posix_spawnattr_t _Nonnull * _Nonnull __attr, sigset64_t* _Nonnull __mask);

int posix_spawnattr_setsigdefault(posix_spawnattr_t _Nonnull * _Nonnull __attr, const sigset_t* _Nonnull __mask);
int posix_spawnattr_setsigdefault64(posix_spawnattr_t _Nonnull * _Nonnull __attr, const sigset64_t* _Nonnull __mask);
int posix_spawnattr_getsigdefault(const posix_spawnattr_t _Nonnull * _Nonnull __attr, sigset_t* _Nonnull __mask);
int posix_spawnattr_getsigdefault64(const posix_spawnattr_t _Nonnull * _Nonnull __attr, sigset64_t* _Nonnull __mask);

int posix_spawnattr_setschedparam(posix_spawnattr_t _Nonnull * _Nonnull __attr, const struct sched_param* _Nonnull __param);
int posix_spawnattr_getschedparam(const posix_spawnattr_t _Nonnull * _Nonnull __attr, struct sched_param* _Nonnull __param);

int posix_spawnattr_setschedpolicy(posix_spawnattr_t _Nonnull * _Nonnull __attr, int __policy);
int posix_spawnattr_getschedpolicy(const posix_spawnattr_t _Nonnull * _Nonnull __attr, int* _Nonnull __policy);

int posix_spawn_file_actions_init(posix_spawn_file_actions_t _Nonnull * _Nonnull __actions);
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t _Nonnull * _Nonnull __actions);

int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t _Nonnull * _Nonnull __actions, int __fd, const char* _Nonnull __path, int __flags, mode_t __mode);
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t _Nonnull * _Nonnull __actions, int __fd);
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t _Nonnull * _Nonnull __actions, int __fd, int __new_fd);
__END_DECLS

#endif
