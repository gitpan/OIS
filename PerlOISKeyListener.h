#ifndef _PERLOIS_KEYLISTENER_H_
#define _PERLOIS_KEYLISTENER_H_

#include "perlOIS.h"

// this class implements OIS::KeyListener,
// so it can be passed to Keyboard->setEventCallback,
// but it allows implementing the callbacks from Perl

class PerlOISKeyListener : public OIS::KeyListener
{
 public:
    PerlOISKeyListener();
    ~PerlOISKeyListener();

    // these are used in xs/Keyboard.xs setEventCallback
    void setPerlObject(SV *pobj);

    // KeyListener interface
    bool keyPressed(const OIS::KeyEvent &evt);
    bool keyReleased(const OIS::KeyEvent &evt);

 private:
    bool callPerlCallback(const char *cbmeth, const OIS::KeyEvent &evt);

    SV * mPerlObj;
};


#endif  /* define _PERLOIS_KEYLISTENER_H_ */
