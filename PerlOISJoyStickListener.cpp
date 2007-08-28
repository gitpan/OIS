#include "PerlOISJoyStickListener.h"

// class implementing OIS::JoyStickListener interface,
// but using Perl callbacks

PerlOISJoyStickListener::PerlOISJoyStickListener() : mPerlObj((SV *) NULL)
{
}

PerlOISJoyStickListener::~PerlOISJoyStickListener()
{
    if (mPerlObj != (SV *)NULL && SvREFCNT(mPerlObj)) {
        SvREFCNT_dec(mPerlObj);
    }
}

bool PerlOISJoyStickListener::buttonPressed(const OIS::JoyStickEvent &evt, int button)
{
    return callPerlCallback("buttonPressed", evt, button);
}

bool PerlOISJoyStickListener::buttonReleased(const OIS::JoyStickEvent &evt, int button)
{
    return callPerlCallback("buttonReleased", evt, button);
}

bool PerlOISJoyStickListener::axisMoved(const OIS::JoyStickEvent &evt, int axis)
{
    return callPerlCallback("axisMoved", evt, axis);
}

bool PerlOISJoyStickListener::sliderMoved(const OIS::JoyStickEvent &evt, int slider)
{
    return callPerlCallback("sliderMoved", evt, slider);
}

bool PerlOISJoyStickListener::povMoved(const OIS::JoyStickEvent &evt, int pov)
{
    return callPerlCallback("povMoved", evt, pov);
}

void PerlOISJoyStickListener::setPerlObject(SV *pobj)
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
        warn("Argument wasn't an object, so JoyStickListener wasn't set.\n");
    }
}

bool PerlOISJoyStickListener::callPerlCallback(const char *cbmeth, const OIS::JoyStickEvent &evt, int thingID)
{
    bool retval = true;   // default to returning true

    if (mPerlObj != (SV *)NULL && sv_isobject(mPerlObj)) {
        // see `perldoc perlcall`
        dSP;

        int count;
        SV *joyevt, *methret;

        TMOIS_OUT(joyevt, &evt, JoyStickEvent);  // put C++ object into Perl

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
            XPUSHs(sv_2mortal(joyevt));
            XPUSHs(sv_2mortal(newSViv(thingID)));
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
