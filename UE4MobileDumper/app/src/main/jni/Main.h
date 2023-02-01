//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_MAIN_H
#define PRIVATE_DUMPER_MAIN_H

#include "Canvas.h"
#include "Dumper.h"

void RenderHacks(Canvas Draw, int SWidth, int SHeight) {
    Draw.DrawText(Color(255, 40, 40), "Made By Ascarre", Vector2(SWidth / 8, SHeight / 12), 25);

    ProcessId = FindProcessId(Offsets::ProcessName);

    string OutputDirectory = "/sdcard/DumpedGames/";
    OutputDirectory += Offsets::ProcessName;

    if (isEqual(Offsets::ProcessName, "Game_Package_Name")) {
        Offsets::NewUEVersion();//For UE Version >= 4.23
        Offsets::OldUEVersion();//For UE Version < 4.23
        isUE423 = true;//if UE Version >= 4.23 else set to false
        isDecrypt = true;//Needs Xor Decryption
        isPointer = true;//For games using pointers to hide real Global Pointers (GName, GWorld)
        isFuncDec = true;//For games using a function to replicate Global Pointers (GName, GWorld)
    }

    ModuleBase = GetModuleBase(ProcessId, Offsets::ModuleName);
    ModuleEnd = GetModuleEnd(ProcessId, Offsets::ModuleName);

    /******************** Dumping Functions ********************/
    //LibDump
    if (isLibDump && !LibDumped) {
        LibDumped = true;
        LibDump(OutputDirectory);
    }

    //Strings Dump
    if (isStringDump && !StringDumped) {
        StringDumped = true;
        DumpStrings(OutputDirectory);
    }

    //Objects Dump
    if (isObjectDump && !ObjectDump) {
        ObjectDump = true;
        DumpObjects(OutputDirectory);
    }

    //SDKW Dump using GWorld
    if (isSDKWDump && !SDKWDumped) {
        SDKWDumped = true;
        DumpSDKW(OutputDirectory);
    }

    //SDK Dump using GUObject
    if (isSDKDump && !SDKDumped) {
        SDKDumped = true;
        DumpSDK(OutputDirectory);
    }

    if (isFindActors && !FoundActors) {
        FoundActors = true;
        DumpActors(OutputDirectory);
    }
}

#endif //PRIVATE_DUMPER_MAIN_H
