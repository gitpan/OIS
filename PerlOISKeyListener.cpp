#include "PerlOISKeyListener.h"

// class implementing OIS::KeyListener interface,
// but using Perl callbacks

PerlOISKeyListener::PerlOISKeyListener() : mPerlObj((SV *) NULL)
{
}

PerlOISKeyListener::~PerlOISKeyListener()
{
    if (mPerlObj != (SV *)NULL && SvREFCNT(mPerlObj)) {
        SvREFCNT_dec(mPerlObj);
    }
}

bool PerlOISKeyListener::keyPressed(const OIS::KeyEvent &evt)
{
    return callPerlCallback("keyPressed", evt);
}

bool PerlOISKeyListener::keyReleased(const OIS::KeyEvent &evt)
{
    return callPerlCallback("keyReleased", evt);
}

void PerlOISKeyListener::setPerlObject(SV *pobj)
{
    // xxx: is this right?
    if (sv_isobject(pobj)) {
        // copy the SV *
        if (mPerlObj == (SV *)NULL) {
            // first time, create new SV *
            mPerlObj = newSVsv(pobj);
        } else {
            // just overwrite the SV *
            SvSetSV(mPerlObj, pobj);
        }
    } else {
        warn("Argument wasn't an object, so KeyListener wasn't set.\n");
    }
}

// xxx: I'd prefer this to be in a base class,
// but I don't think passing OIS::EventArg would work
// (can you call KeyEvent methods on EventArg objects?)
bool PerlOISKeyListener::callPerlCallback(const char *cbmeth, const OIS::KeyEvent &evt)
{
    bool retval = true;   // default to returning true

    if (mPerlObj != (SV *)NULL && sv_isobject(mPerlObj)) {
        // see `perldoc perlcall`
        dSP;

        int count;
        SV *keyevt, *methret;

        TMOIS_OUT(keyevt, &evt, KeyEvent);  // put C++ object into Perl

        ENTER;
        SAVETMPS;

        // call `can' to see if they implemented the callback
        PUSHMARK(SP);
        XPUSHs(mPerlObj);
        XPUSHs(sv_2mortal(newSVpv(cbmeth, strlen(cbmeth))));
        PUTBACK;

        count = call_method("can", G_SCALAR);
        SPAGAIN;
        if (count != 1) {
            croak("can didn't return a single value?");
        }

        methret = POPs;
        PUTBACK;

        if (SvTRUE(methret)) {
            // call the callback
            PUSHMARK(SP);
            XPUSHs(mPerlObj);
            XPUSHs(sv_2mortal(keyevt));
            PUTBACK;

            // finally
            count = call_method(cbmeth, G_SCALAR);
            SPAGAIN;
            if (count != 1) {
                croak("Callbacks must return a single (boolean) value");
            }

            methret = POPs;
            PUTBACK;

            retval = SvTRUE(methret) ? true : false;
        }

        FREETMPS;
        LEAVE;
    }

    return retval;
}
