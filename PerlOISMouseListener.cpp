#include "PerlOISMouseListener.h"

// class implementing OIS::MouseListener interface,
// but using Perl callbacks

PerlOISMouseListener::PerlOISMouseListener() : mPerlObj((SV *) NULL)
{
}

PerlOISMouseListener::~PerlOISMouseListener()
{
    if (mPerlObj != (SV *)NULL && SvREFCNT(mPerlObj)) {
        SvREFCNT_dec(mPerlObj);
    }
}

bool PerlOISMouseListener::mouseMoved(const OIS::MouseEvent &evt)
{
    // no "int" arg for mouseMoved, so passing 0
    return callPerlCallback("mouseMoved", evt, 0);
}

bool PerlOISMouseListener::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return callPerlCallback("mousePressed", evt, id);
}

bool PerlOISMouseListener::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return callPerlCallback("mouseReleased", evt, id);
}

void PerlOISMouseListener::setPerlObject(SV *pobj)
{
    // xxx: is this right?
    if (pobj != (SV *)NULL && sv_isobject(pobj)) {
        // copy the SV *
        if (mPerlObj == (SV *)NULL) {
            // first time, create new SV *
            mPerlObj = newSVsv(pobj);
        } else {
            // just overwrite the SV *
            SvSetSV(mPerlObj, pobj);
        }
    } else {
        warn("Argument wasn't an object, so MouseListener wasn't set.\n");
    }
}

bool PerlOISMouseListener::callPerlCallback(const char *cbmeth, const OIS::MouseEvent &evt, int buttonID)
{
    bool retval = true;   // default to returning true

    if (mPerlObj != (SV *)NULL && sv_isobject(mPerlObj)) {
        // see `perldoc perlcall`
        dSP;

        int count;
        SV *mouseevt, *methret;

        TMOIS_OUT(mouseevt, &evt, MouseEvent);  // put C++ object into Perl

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
            XPUSHs(sv_2mortal(mouseevt));
            XPUSHs(sv_2mortal(newSViv(buttonID)));
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
