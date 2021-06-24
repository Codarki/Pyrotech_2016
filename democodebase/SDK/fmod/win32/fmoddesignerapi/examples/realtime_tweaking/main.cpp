/*===============================================================================================
 realtime_tweaking Example
 Copyright (c), Firelight Technologies Pty, Ltd 2004-2006.

 Demonstrates basic usage of FMOD's network data-driven event library (fmod_event_net.dll)
===============================================================================================*/
#include "../../api/inc/fmod_event.h"
#include "../../api/inc/fmod_event_net.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

const char *MEDIA_PATH = "..\\media";
const int   NUM_EVENTS = 3;


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
    if (type == FMOD::EVENT_CALLBACKTYPE_NET_MODIFIED)
    {
        unsigned int t = (unsigned int)param2;
        printf("%p %s %f (%d)\n", event, (unsigned int)param1 == FMOD::EVENTPROPERTY_VOLUME ? "volume" : FMOD::EVENTPROPERTY_PITCH ? "pitch" : "???", *((float *)&t), (unsigned int)userdata);
    }

    return FMOD_OK;
}


int main(int argc, char *argv[])
{
    FMOD_RESULT        result;
    FMOD::EventSystem *eventsystem;
    FMOD::EventGroup  *eventgroup;
    FMOD::Event       *event[NUM_EVENTS];
    int                key, i, userdata = 0;

    printf("======================================================================\n");
    printf("Realtime Tweaking. Copyright (c) Firelight Technologies 2004-2006.\n");
    printf("======================================================================\n");
    printf("This example shows how to initialize the FMOD Net Event System so that\n");
    printf("FMOD Designer can connect to your game and tweak events as they're\n");
    printf("playing.\n");
    printf("Start some events then connect to this app using the Audition menu\n");
    printf("in FMOD Designer. You can use 127.0.0.1 for the IP address if you\n");
    printf("don't want to use two machines. Load examples.fdp and change the \n");
    printf("volume of the playing events using the volume slider in the event\n");
    printf("property sheet\n");
    printf("======================================================================\n\n");

    result = FMOD::EventSystem_Create(&eventsystem);
    ERRCHECK(result);
    result = FMOD::NetEventSystem_Init(eventsystem);
    ERRCHECK(result);
    result = eventsystem->init(64, FMOD_INIT_NORMAL, 0, EVENT_INIT_NORMAL);
    ERRCHECK(result);
    result = eventsystem->setMediaPath((char *)MEDIA_PATH);
    ERRCHECK(result);
    result = eventsystem->load("examples.fev", 0, 0);
    ERRCHECK(result);
    result = eventsystem->getGroup("examples/examples/filters", EVENT_DEFAULT, &eventgroup);
    ERRCHECK(result);

    for (i=0; i < NUM_EVENTS; i++)
    {
        event[i] = 0;
    }

    printf("======================================================================\n");
    printf("Press 1 - 3  to start/stop events\n");
    printf("Press ESC    to quit\n");
    printf("======================================================================\n");

    key = 0;
    do
    {
        if (kbhit())
        {
            key = getch();

            if ((key >= '1') && (key <= '3'))
            {
                char *name;

                i = (int)(key - '1');

                if (event[i])
                {
                    result = event[i]->getInfo(0, &name, 0);
                    ERRCHECK(result);
                    result = event[i]->stop();
                    ERRCHECK(result);
                    event[i] = 0;
                    printf("Stopping '%s'\n", name);
                }
                else
                {
                    result = eventgroup->getEventByIndex(i, EVENT_DEFAULT, &event[i]);
                    if (result == FMOD_OK)
                    {
                        result = event[i]->getInfo(0, &name, 0);
                        ERRCHECK(result);
                        result = event[i]->setCallback(eventcallback, (void *)(userdata++));
                        ERRCHECK(result);
                        result = event[i]->start();
                        ERRCHECK(result);
                        printf("Starting '%s'\n", name);
                    }
                }
            }
        }

        result = eventsystem->update();
        ERRCHECK(result);
        result = FMOD::NetEventSystem_Update();
        ERRCHECK(result);

        Sleep(10);

    } while (key != 27);

    result = eventsystem->release();
    ERRCHECK(result);
    result = FMOD::NetEventSystem_Shutdown();
    ERRCHECK(result);

    return 0;
}
