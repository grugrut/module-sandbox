#include <emacs-module.h>
#include <assert.h>

/* Declare mandatory GPL symbol.  */
int plugin_is_GPL_compatible;

/* New emacs lisp function. All function exposed to Emacs must have this prototype. */
static emacs_value
Fmod_test_return_t (emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
  return env->intern(env, "t");
}

static emacs_value
Fmod_test_mul_a_b (emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
  assert (nargs == 2);
  intmax_t a = env->extract_integer (env, args[0]);
  intmax_t b = env->extract_integer (env, args[1]);

  intmax_t r = a * b;
  return env->make_integer(env, r);
}

/* Provide FEATURE to Emacs.  */
static void
provide (emacs_env *env, const char *feature)
{
  /* call 'provide' with FEATURE converted to a symbol */

  emacs_value Qfeat = env->intern (env, feature);
  emacs_value Qprovide = env->intern (env, "provide");
  emacs_value args[] = { Qfeat };

  env->funcall (env, Qprovide, 1, args);
}

/* Bind NAME to FUN.  */
static void
bind_function (emacs_env *env, const char *name, emacs_value Sfun)
{
  /* Set the function cell of the symbol named NAME to SFUN using
     the 'fset' function.  */

  /* Convert the strings to symbols by interning them */
  emacs_value Qfset = env->intern (env, "fset");
  emacs_value Qsym = env->intern (env, name);

  /* Prepare the arguments array */
  emacs_value args[] = { Qsym, Sfun };

  /* Make the call (2 == nb of arguments) */
  env->funcall (env, Qfset, 2, args);
}



int
emacs_module_init (struct emacs_runtime *ert)
{
  emacs_env *env = ert->get_environment (ert);

#define DEFUN(lsym, csym, amin, amax, doc, data)                        \
  bind_function (env, lsym,                                             \
                 env->make_function(env, amin, amax, csym, doc, data))

  DEFUN ("mod-test-return-t", Fmod_test_return_t, 1, 1, NULL, NULL);
  DEFUN ("mod-test-mul-a-b", Fmod_test_mul_a_b, 2, 2, "Return A * B", NULL); 

#undef DEFUN

  provide (env, "mymod");

  /* loaded successfully */
  return 0;
}
