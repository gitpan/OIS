#ifndef _PERLOIS_MOUSELISTENER_H_
#define _PERLOIS_MOUSELISTENER_H_

#include "perlOIS.h"

// this class implements OIS::MouseListener,
// so it can be passed to Mouse->setEventCallback,
// but it allows implementing the callbacks from Perl

class PerlOISMouseListener : public OIS::MouseListener
{
 public:
    PerlOISMouseListener();
    ~PerlOISMouseListener();

    // these are used in xs/Mouseboard.xs setEventCallback
    void setPerlObject(SV *pobj);

    // MouseListener interface
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

 private:
    bool callPerlCallback(const char *cbmeth, const OIS::MouseEvent &evt, int id);

    SV * mPerlObj;
};


#endif  /* define _PERLOIS_MOUSELISTENER_H_ */
