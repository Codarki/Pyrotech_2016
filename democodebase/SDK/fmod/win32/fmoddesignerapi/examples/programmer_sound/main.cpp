/*===============================================================================================
 programmer_sound Example
 Copyright (c), Firelight Technologies Pty, Ltd 2006.

 Demonstrates how to use the "programmer sound" feature of the FMOD event system
===============================================================================================*/
#include "../../api/inc/fmod_event.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

const char   *MEDIA_PATH = "..\\media";
const char   *FSB_NAME   = "..\\media\\other.fsb";

FMOD::Sound  *fsb;
FMOD::System *sys;


void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d)\n", result);
        exit(-1);
    }
}


FMOD_RESULT F_CALLBACK eventcallback(FMOD::Event *event, FMOD::EVENT_CALLBACKTYPE type, void *param1, void *param2, void *userdata)
{
    FMOD_RESULT result;

    switch (type)
    {
        case FMOD::EVENT_CALLBACKTYPE_SOUNDDEF_CREATE :
        {
            char *name      = (char *)param1;               // [in]  (char *) name of sound definition
            int entryindex  = *((int *)param2);             // [in]  (int) index of sound definition entry
            FMOD::Sound **s = (FMOD::Sound **)param2;       // [out] (FMOD::Sound *) a valid lower level API FMOD Sound handle

            result = fsb->getSubSound(rand() % 4, s);
            ERRCHECK(result);
            printf("EVENT_CALLBACKTYPE_SOUNDDEF_CREATE %p\n", *s);
            break;
        }

        case FMOD::EVENT_CALLBACKTYPE_SOUNDDEF_RELEASE :
        {
            FMOD::Sound *s = (FMOD::Sound *)param2;         // [in]  (FMOD::Sound *) the FMOD sound handle that was previously created in EVENT_CALLBACKTYPE_SOUNDDEF_CREATE
            printf("EVENT_CALLBACKTYPE_SOUNDDEF_RELEASE %p\n", s);
            break;
        }
    }

    return FMOD_OK;
}


int main(int argc, char *argv[])
{
    FMOD_RESULT        result;
    FMOD::EventSystem *eventsystem;
    FMOD::EventGroup  *eventgroup;
    FMOD::Event       *event;
    int                key;

    printf("======================================================================\n");
    printf("Programmer Sound. Copyright (c) Firelight Technologies 2006.\n");
    printf("======================================================================\n");

    result = FMOD::EventSystem_Create(&eventsystem);
    ERRCHECK(result);
    result = eventsystem->init(64, FMOD_INIT_NORMAL, 0, EVENT_INIT_NORMAL);
    ERRCHECK(result);
    result = eventsystem->setMediaPath((char *)MEDIA_PATH);
    ERRCHECK(result);
    result = eventsystem->load("examples.fev", 0, 0);
    ERRCHECK(result);
    result = eventsystem->getGroup("examples/examples", EVENT_DEFAULT, &eventgroup);
    ERRCHECK(result);

    result = eventsystem->getSystemObject(&sys);
    ERRCHECK(result);
    result = sys->createStream(FSB_NAME, FMOD_2D | FMOD_SOFTWARE, 0, &fsb);
    ERRCHECK(result);

    printf("======================================================================\n");
    printf("Press SPACE  to start event\n");
    printf("Press ESC    to quit\n");
    printf("======================================================================\n");

    key = 0;
    do
    {
        if (kbhit())
        {
            key = getch();

            if (key == ' ')
            {
                result = eventgroup->getEvent("programmersound", EVENT_DEFAULT, &event);
                if (result == FMOD_OK)
                {
                    result = event->setCallback(eventcallback, 0);
                    ERRCHECK(result);
                    result = event->start();
                    ERRCHECK(result);
                }
            }
        }

        result = eventsystem->update();
        ERRCHECK(result);

        Sleep(10);

    } while (key != 27);

    result = eventsystem->unload();
    ERRCHECK(result);
    result = fsb->release();
    ERRCHECK(result);
    result = eventsystem->release();
    ERRCHECK(result);

    return 0;
}
