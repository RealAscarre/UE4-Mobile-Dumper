//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_DUMPER_H
#define PRIVATE_DUMPER_DUMPER_H

#include <ElfFixer/elf.h>
#include <ElfFixer/fix.h>

#include "PropertyFlags.h"

/******************** Lib Dumping ********************/
void LibDump(string out) {
    string TempPath = out + "/LibDump.dat";
    ofstream Dump(TempPath, ofstream::out | ofstream::binary);
    size_t LibSize = (ModuleEnd - ModuleBase);//Getting LibSize

    if (Dump.is_open()) {
        if (isFullDump) {
            char *buffer = new char[1];
            while (LibSize != 0) {
                PVM_Read((void *) (ModuleBase++), buffer, 1);
                Dump.write(buffer, 1);
                --LibSize;
            }
        } else {
            auto *buffer = new uint8_t[LibSize];
            memset(buffer, '\0', LibSize);
            PVM_Read((void *) ModuleBase, buffer, LibSize);
            Dump.write((char *) buffer, LibSize);
        }
        Dump.close();
    }
    string RebuildOutput = out + "/" + Offsets::ModuleName;
    fix_so(TempPath.c_str(), RebuildOutput.c_str(), ModuleBase);//Fixing DumpedLib
    remove(TempPath.c_str());//Removing the Dumped .dat File
    exit(0);//Exit the Game
}

/******************** GNames Dumping ********************/
void DumpBlocks(ofstream &Dump, uint32 &count, uintptr_t FNamePool, uint32 blockId, uint32 blockSize) {
    uintptr_t It = Read<uintptr_t>(FNamePool + Offsets::FNamePoolToBlocks + (blockId * Offsets::PointerSize));
    uintptr_t End = It + blockSize - Offsets::FNameEntryToString;
    uint32 Block = blockId;
    uint16 Offset = 0;
    while (It < End) {
        uintptr_t FNameEntry = It;
        int16 FNameEntryHeader = Read<int16>(FNameEntry + Offsets::FNameEntryHeader);
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryToLenBit;
        if (StrLength) {
            bool wide = FNameEntryHeader & 1;

            ///Unicode Dumping Not Supported
            if (StrLength > 0) {
                //String Length Limit
                if (StrLength < 250) {
                    string str;
                    uint32 key = (Block << 16 | Offset);
                    uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryToString;

                    if (wide) {
                        str = WideStr::getString(StrPtr, StrLength);
                    } else {
                        str = ReadString2(StrPtr, StrLength);
                    }

                    if (isDecrypt){
                        //Dump << (wide ? "Wide" : "") << dec << "{" << StrLength << "} " << "[" << key << "]: " << XorCypher(str) << endl;
                        Dump << "[" << count << "]: " << XorCypher(str) << endl;
                    } else {
                        //Dump << (wide ? "Wide" : "") << dec << "{" << StrLength << "} " << "[" << key << "]: " << str << endl;
                        Dump << "[" << count << "]: " << str << endl;
                    }
                    count++;
                }
            } else {
                StrLength = -StrLength;//Negative lengths are for Unicode Characters
            }

            //Next
            Offset += StrLength / Offsets::FNameStride;
            uint16 bytes = Offsets::FNameEntryToString + StrLength * (wide ? sizeof(wchar_t) : sizeof(char));
            It += (bytes + Offsets::FNameStride - 1u) & ~(Offsets::FNameStride - 1u);
        } else {// Null-terminator entry found
            break;
        }
    }
}

void DumpStrings(string out) {
#if defined(__LP64__)
    ofstream Dump(out + "/NameDump - 64 Bit.txt", ofstream::out);
#else
    ofstream Dump(out + "/NameDump - 32 Bit.txt", ofstream::out);
#endif
    uint32 count = 0;
    if (Dump.is_open()) {
        if (isUE423) {
            uintptr_t FNamePool = (ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool;

            uint32 BlockSize = Offsets::FNameStride * 65536;
            uint32 CurrentBlock = Read<uint32>(FNamePool + Offsets::FNamePoolToCurrentBlock);
            uint32 CurrentByteCursor = Read<uint32>(FNamePool + Offsets::FNamePoolToCurrentByteCursor);

            //All Blocks Except Current
            for (uint32 BlockIdx = 0; BlockIdx < CurrentBlock; ++BlockIdx) {
                DumpBlocks(Dump, count, FNamePool, BlockIdx, BlockSize);
            }

            //Last Block
            DumpBlocks(Dump, count, FNamePool, CurrentBlock, CurrentByteCursor);
        } else {
            for (uint32 i = 0; i < DumpLimit; i++) {
                string s = GetFNameFromID(i);
                if (!s.empty()) {
                    Dump << "[" << i << "]: " << s << endl;
                    count++;
                }
            }
        }
        Dump.close();
    }
}

void DumpObjects(string out) {
#if defined(__LP64__)
    ofstream Dump(out + "/ObjectsDump - 64 Bit.txt", ofstream::out);
#else
    ofstream Dump(out + "/ObjectsDump - 32 Bit.txt", ofstream::out);
#endif
    uint32 Count = 0;
    if (Dump.is_open()) {
        int32 ObjectsCount = GetObjectCount();
        if (ObjectsCount < 10 || ObjectsCount > 999999) {
            ObjectsCount = DumpLimit;
        }
        for (int32 i = 0; i < ObjectsCount; i++) {
            uintptr_t GUObjectArray = GetUObjectFromID(i);
            if (UObject::isValid(GUObjectArray)) {
                Dump << setbase(16) << "[0x" << i << "]:" << endl;
                Dump << "Name: " << UObject::getName(GUObjectArray).c_str() << endl;
                Dump << "Class: " << UObject::getClassName(GUObjectArray).c_str() << endl;
                Dump << "ObjectPtr: 0x" << setbase(16) << GUObjectArray << endl;
                Dump << "ClassPtr: 0x" << setbase(16) << UObject::getClass(GUObjectArray) << endl;
                Dump << endl;
                Count++;
            }
        }
        Dump.close();
    }
}

void DumpSDK(string out) {
#if defined(__LP64__)
    ofstream Dump(out + "/SDK - 64 Bit.txt", ofstream::out);
#else
    ofstream Dump(out + "/SDK - 32 Bit.txt", ofstream::out);
#endif
    if (Dump.is_open()) {
        int32 oCount = GetObjectCount();
        if (oCount < 10 || oCount > 999999) {
            oCount = 300000;
        }
        for (int32 i = 0; i < oCount; i++) {
            uintptr_t UObject = GetUObjectFromID(i);
            if (UObject::isValid(UObject)) {
                DumpStructures(Dump, UObject::getClass(UObject));
            }
        }
        Dump.close();
    }
}

void DumpSDKW(string out) {
#if defined(__LP64__)
    ofstream Dump(out + "/SDKW - 64 Bit.txt", ofstream::out);
#else
    ofstream Dump(out + "/SDKW - 32 Bit.txt", ofstream::out);
#endif
    if (Dump.is_open()) {
        static uintptr_t GWorld;

        GWorld = Read<uintptr_t>(ModuleBase + Offsets::GWorld);

        if (isPointer || isFuncDec) {
            GWorld = Read<uintptr_t>(Read<uintptr_t>(ModuleBase + Offsets::GWorld) + Offsets::GWorldPointer);
        }

        if (UObject::isValid(GWorld)) {
            //Iterate World
            DumpStructures(Dump, UObject::getClass(GWorld));
            //Iterate Entities
            uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::UWorldToPersistentLevel);
            uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::ULevelToAActors);
            int AActorsCount = Read<int>(PersistentLevel + Offsets::ULevelToAActorsCount);

            for (int i = 0; i < AActorsCount; i++) {
                uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
                if (UObject::isValid(Base)) {
                    DumpStructures(Dump, UObject::getClass(Base));
                }
            }
        }
        Dump.close();
    }
}

void DumpActors(string out) {
#if defined(__LP64__)
    ofstream Dump(out + "/ActorsDump - 64 Bit.txt", ofstream::out);
#else
    ofstream Dump(out + "/ActorsDump - 32 Bit.txt", ofstream::out);
#endif
    uint32 count = 0;
    if (Dump.is_open()) {
        uintptr_t GWorld = Read<uintptr_t>(ModuleBase + Offsets::GWorld);
        if (UObject::isValid(GWorld)) {
            uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::UWorldToPersistentLevel);
            uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::ULevelToAActors);
            int AActorsCount = Read<int>(PersistentLevel + Offsets::ULevelToAActorsCount);

            for (int i = 0; i < AActorsCount; i++) {
                uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
                if (UObject::isValid(Base)) {
                    Dump << "[" << count << "]: Name - " << UObject::getName(Base) << ", Address - 0x" << hex << Base <<  endl;
                    count++;
                }
            }
        }
        Dump.close();
    }
}

#endif //PRIVATE_DUMPER_DUMPER_H