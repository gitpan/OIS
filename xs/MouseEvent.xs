MODULE = OIS     PACKAGE = OIS::MouseEvent

## These are "public attributes", not methods.

MouseState *
MouseEvent::state()
  CODE:
    // xxx: I doubt this works...
    MouseState *state = new MouseState;
    *state = (*THIS).state;
    RETVAL = state;
  OUTPUT:
    RETVAL
