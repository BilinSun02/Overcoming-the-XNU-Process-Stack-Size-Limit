# Overcoming-the-XNU-Process-Stack-Size-Limit

Unlike most Linux distributions, macOS and iOS have a relatively small stack available for newly created processes, respectively 64 MB and 1 MB. This size is hardcoded into the kernel and cannot be changed without a kernel recompile. However, I came up with a way to bypass this limit.
