/* Compatibility shim: this project's headers/sources include <glad/glad.h>
 * (the classic glad v1 include path). We generate loader code with glad2,
 * whose public header is <glad/gl.h>, so this shim just forwards to it and
 * maps the old v1 symbol names onto the new v2 ones. */
#ifndef GLAD_GLAD_COMPAT_H
#define GLAD_GLAD_COMPAT_H

#include <glad/gl.h>

typedef GLADloadfunc GLADloadproc;

static inline int gladLoadGLLoader(GLADloadproc load)
{
    return gladLoadGL(load);
}

#endif /* GLAD_GLAD_COMPAT_H */
