MODULE = OIS     PACKAGE = OIS::MouseState

int
MouseState::buttonDown(button)
    int button
  C_ARGS:
    (OIS::MouseButtonID)button

## These are "public attributes", not methods.
## Not sure how useful these are, but there you go...
int
MouseState::width()
  CODE:
    RETVAL = (*THIS).width;
  OUTPUT:
    RETVAL

int
MouseState::height()
  CODE:
    RETVAL = (*THIS).height;
  OUTPUT:
    RETVAL

int
MouseState::buttons()
  CODE:
    RETVAL = (*THIS).buttons;
  OUTPUT:
    RETVAL

## Dunno what "Axis" means for a mouse,
## so these are unwrapped (let me know).
## (for that matter, I dunno what width and height mean
## for a mouse, either)
## Axis X
## Axis Y
## Axis Z
