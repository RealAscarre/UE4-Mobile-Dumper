//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_OFFSETS_H
#define PRIVATE_DUMPER_OFFSETS_H

uint32 MAX_SIZE = 100;
uint32 ClassCount = 0;
uint32 DumpLimit = 300000;

bool isLibDump = false, isFullDump = false, LibDumped = false,
        isUE423 = false, isDecrypt = false, isPointer = false, isFuncDec = false,
        isStringDump = false, StringDumped = false,
        isObjectDump = false, ObjectDump = false,
        isSDKWDump = false, SDKWDumped = false,
        isSDKDump = false, SDKDumped = false,
        isFindActors = false, FoundActors = false;

namespace Offsets {

    const char *DumperName = "Private Dumper v5.0";
    const char *ProcessName;
    const char *ModuleName;

    uintptr_t PointerSize;

    //Global
    uintptr_t GWorld;
    uintptr_t GNames;
    uintptr_t GUObjectArray;

    uintptr_t GNamesPointer;
    uintptr_t GWorldPointer;

    //FNameEntry
    uintptr_t FNameEntryToNameString;

    //FUObjectArray
    uintptr_t FUObjectArrayToTUObjectArray;
    uintptr_t FUObjectItemPadd;
    uintptr_t FUObjectItemSize;

    //TUObjectArray
    uintptr_t TUObjectArrayToNumElements;

    //UObject
    uintptr_t UObjectToInternalIndex;
    uintptr_t UObjectToClassPrivate;
    uintptr_t UObjectToFNameIndex;
    uintptr_t UObjectToOuterPrivate;

    //FNamePool - 4.23+
    uintptr_t FNameStride;
    uintptr_t GNamesToFNamePool;
    uintptr_t FNamePoolToCurrentBlock;
    uintptr_t FNamePoolToCurrentByteCursor;
    uintptr_t FNamePoolToBlocks;

    //FNameEntry - 4.23+
    uintptr_t FNameEntryHeader;
    uintptr_t FNameEntryToLenBit;
    uintptr_t FNameEntryToString;

    //FField - 4.23+
    uintptr_t FFieldToClass;
    uintptr_t FFieldToNext;
    uintptr_t FFieldToName;

    //UField
    uintptr_t UFieldToNext;

    //UStruct
    uintptr_t UStructToSuperStruct;
    uintptr_t UStructToChildren;
    uintptr_t UStructToChildProperties;//V - 4.23+

    //UFunction
    uintptr_t UFunctionToFunctionFlags;
    uintptr_t UFunctionToFunc;

    //UProperty
    uintptr_t UPropertyToElementSize;
    uintptr_t UPropertyToPropertyFlags;
    uintptr_t UPropertyToOffsetInternal;

    //UBoolProperty
    uintptr_t UBoolPropertyToFieldSize;
    uintptr_t UBoolPropertyToByteOffset;
    uintptr_t UBoolPropertyToByteMask;
    uintptr_t UBoolPropertyToFieldMask;

    //UObjectProperty
    uintptr_t UObjectPropertyToPropertyClass;

    //UClassProperty
    uintptr_t UClassPropertyToMetaClass;

    //UInterfaceProperty
    uintptr_t UInterfacePropertyToInterfaceClass;

    //UArrayProperty
    uintptr_t UArrayPropertyToInnerProperty;

    //UMapProperty
    uintptr_t UMapPropertyToKeyProp;
    uintptr_t UMapPropertyToValueProp;

    //USetProperty
    uintptr_t USetPropertyToElementProp;

    //UStructProperty
    uintptr_t UStructPropertyToStruct;

    //UWorld
    uintptr_t UWorldToPersistentLevel;

    //ULevel
    uintptr_t ULevelToAActors;
    uintptr_t ULevelToAActorsCount;

    void NewUEVersion() {
        ModuleName = "libUE4.so";//Name may change for some games
#if defined(__LP64__)//For 64 Bit
        PointerSize = 0x8;

        //Global
        GWorld = 0x0;
        GNames = 0x0;
        GUObjectArray = 0x0;

        //Class: FNamePool - 4.23+
        FNameStride = 0x2;
        GNamesToFNamePool = 0x30;
        FNamePoolToCurrentBlock = 0x8;
        FNamePoolToCurrentByteCursor = 0xC;
        FNamePoolToBlocks = 0x10;

        //Class: FNameEntry - 4.23+
        FNameEntryHeader = 0x0;
        FNameEntryToLenBit = 0x6;
        FNameEntryToString = 0x2;

        //Class: FField - 4.23+
        FFieldToClass = 0x8;
        FFieldToNext = 0x20;
        FFieldToName = FFieldToNext + PointerSize;

        //Class: FUObjectArray
        FUObjectArrayToTUObjectArray = 0x0;
        FUObjectItemPadd = 0x0;
        FUObjectItemSize = 0x18;

        //Class: TUObjectArray
        TUObjectArrayToNumElements = 0x14;

        //Class: UObject
        UObjectToInternalIndex = 0xC;
        UObjectToClassPrivate = 0x10;
        UObjectToFNameIndex = 0x18;
        UObjectToOuterPrivate = 0x20;

        //Class: UField
        UFieldToNext = 0x28;

        //Class: UStruct
        UStructToSuperStruct = 0x40;
        UStructToChildren = UStructToSuperStruct + PointerSize;

        //Class: UStruct - 4.23+
        UStructToChildProperties = 0x50;

        //Class: UFunction
        UFunctionToFunctionFlags = 0xC0;
        UFunctionToFunc = UFunctionToFunctionFlags + UFieldToNext;

        //Class: UProperty
        UPropertyToElementSize = 0x38;
        UPropertyToPropertyFlags = 0x40;
        UPropertyToOffsetInternal = 0x4C;

        //Class: UBoolProperty
        UBoolPropertyToFieldSize = 0x78;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        //Class: UObjectProperty
        UObjectPropertyToPropertyClass = 0x78;

        //Class: UClassProperty
        UClassPropertyToMetaClass = 0x80;

        //Class: UInterfaceProperty
        UInterfacePropertyToInterfaceClass = 0x80;

        //Class: UArrayProperty
        UArrayPropertyToInnerProperty = 0x78;

        //Class: UMapProperty
        UMapPropertyToKeyProp = 0x78;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        //Class: USetProperty
        USetPropertyToElementProp = 0x78;

        //Class: UStructProperty
        UStructPropertyToStruct = 0x78;

        //Class: UWorld
        UWorldToPersistentLevel = 0x30;

        //Class: ULevel
        ULevelToAActors = 0x98;
        ULevelToAActorsCount = ULevelToAActors + PointerSize;
#else //32 Bit
        PointerSize = 0x4;

        //Global
        GWorld = 0x0;
        GNames = 0x0;
        GUObjectArray = 0x0;

        //Class: FNamePool - 4.23+
        FNameStride = 0x2;
        GNamesToFNamePool = 0x30;
        FNamePoolToCurrentBlock = 0x4;

        FNamePoolToCurrentByteCursor = 0x8;
        FNamePoolToBlocks = 0x10;//May be 0 sometimes

        //Class: FNameEntry - 4.23+
        FNameEntryHeader = 0x0;
        FNameEntryToLenBit = 0x6;
        FNameEntryToString = 0x2;

        //Class: FUObjectArray
        FUObjectArrayToTUObjectArray = 0x0;
        FUObjectItemPadd = 0x0;
        FUObjectItemSize = 0x10;

        //Class: TUObjectArray
        TUObjectArrayToNumElements = 0x10;

        //Class: UObject
        UObjectToInternalIndex = 0x8;
        UObjectToClassPrivate = UObjectToInternalIndex + PointerSize;
        UObjectToFNameIndex = UObjectToClassPrivate + PointerSize;
        UObjectToOuterPrivate = 0x18;

        //Class: UField
        UFieldToNext = 0x1C;

        //Class: UStruct
        UStructToSuperStruct = 0x28;
        UStructToChildren = UStructToSuperStruct + PointerSize;
        UStructToChildProperties = 0x30;// - 4.23+

        //Class: UFunction
        UFunctionToFunctionFlags = 0x58;
        UFunctionToFunc = 0x98;

        //Class: FField - 4.23+
        FFieldToClass = 0x4;
        FFieldToNext = 0x10;
        FFieldToName = FFieldToNext + PointerSize;

        //Class: UProperty
        UPropertyToElementSize = 0x24;
        UPropertyToPropertyFlags = 0x28;
        UPropertyToOffsetInternal = 0x34;

        //Class: UBoolProperty
        UBoolPropertyToFieldSize = 0x50;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        //Class: UObjectProperty
        UObjectPropertyToPropertyClass = 0x50;

        //Class: UClassProperty
        UClassPropertyToMetaClass = 0x54;

        //Class: UInterfaceProperty
        UInterfacePropertyToInterfaceClass = 0x54;

        //Class: UArrayProperty
        UArrayPropertyToInnerProperty = 0x50;

        //Class: UMapProperty
        UMapPropertyToKeyProp = 0x50;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        //Class: USetProperty
        USetPropertyToElementProp = 0x50;

        //Class: UStructProperty
        UStructPropertyToStruct = 0x50;

        //Class: UWorld
        UWorldToPersistentLevel = 0x20;

        //Class: ULevel
        ULevelToAActors = 0x70;
        ULevelToAActorsCount = ULevelToAActors + PointerSize;
#endif
    };

    void OldUEVersion() {
        ModuleName = "libUE4.so";//Name may change for some games
#if defined(__LP64__)//64 Bit
        PointerSize = 0x8;

        //Global
        GWorld = 0x0;
        GNames = 0x0;
        GUObjectArray = 0x0;

        GNamesPointer = 0x0;
        GWorldPointer = 0x0;

        //FNameEntry
        FNameEntryToNameString = 0x10;//May be 0xC or 0x14 sometimes

        //Class: FUObjectArray
        FUObjectArrayToTUObjectArray = 0x0;

        //Class: TUObjectArray
        TUObjectArrayToNumElements = 0xC;
        FUObjectItemPadd = 0x0;
        FUObjectItemSize = 0x18;

        //Class: UObject
        UObjectToInternalIndex = 0xC;
        UObjectToClassPrivate = 0x10;
        UObjectToFNameIndex = 0x18;
        UObjectToOuterPrivate = 0x20;

        //Class: UField
        UFieldToNext = 0x28;

        //Class: UStruct
        UStructToSuperStruct = 0x30;
        UStructToChildren = UStructToSuperStruct + PointerSize;

        //Class: UFunction
        UFunctionToFunctionFlags = 0x88;
        UFunctionToFunc = UFunctionToFunctionFlags + UFieldToNext;

        //Class: UProperty
        UPropertyToElementSize = 0x34;
        UPropertyToPropertyFlags = 0x38;
        UPropertyToOffsetInternal = 0x44;

        //Class: UBoolProperty
        UBoolPropertyToFieldSize = 0x70;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        //Class: UObjectProperty
        UObjectPropertyToPropertyClass = 0x70;

        //Class: UClassProperty
        UClassPropertyToMetaClass = 0x78;

        //Class: UInterfaceProperty
        UInterfacePropertyToInterfaceClass = 0x78;

        //Class: UArrayProperty
        UArrayPropertyToInnerProperty = 0x70;

        //Class: UMapProperty
        UMapPropertyToKeyProp = 0x70;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        //Class: USetProperty
        USetPropertyToElementProp = 0x70;

        //Class: UStructProperty
        UStructPropertyToStruct = 0x70;

        //Class: UWorld
        UWorldToPersistentLevel = 0x30;

        //Class: ULevel
        ULevelToAActors = 0xA0;
        ULevelToAActorsCount = ULevelToAActors + PointerSize;
#else
        PointerSize = 0x4;

        //Global
        GWorld = 0x0;
        GNames = 0x0;
        GUObjectArray = 0x0;

        GNamesPointer = 0x0,
        GWorldPointer = 0x0;

        //FNameEntry
        FNameEntryToNameString = 0x8;

        //Class: FUObjectArray
        FUObjectArrayToTUObjectArray = 0x0;

        //Class: TUObjectArray
        TUObjectArrayToNumElements = 0x8;
        FUObjectItemPadd = 0x0;
        FUObjectItemSize = 0x10;

        //Class: UObject
        UObjectToInternalIndex = 0x8;
        UObjectToClassPrivate = 0xC;
        UObjectToFNameIndex = 0x10;
        UObjectToOuterPrivate = 0x18;

        //Class: UField
        UFieldToNext = 0x1C;

        //Class: UStruct
        UStructToSuperStruct = 0x20;
        UStructToChildren = UStructToSuperStruct + PointerSize;

        //Class: UFunction
        UFunctionToFunctionFlags = 0x58;
        UFunctionToFunc= UFunctionToFunctionFlags + UFieldToNext;

        //Class: UProperty
        UPropertyToElementSize = 0x24;
        UPropertyToPropertyFlags = 0x28;
        UPropertyToOffsetInternal = 0x34;

        //Class: UBoolProperty
        UBoolPropertyToFieldSize = 0x50;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        //Class: UObjectProperty
        UObjectPropertyToPropertyClass = 0x50;

        //Class: UClassProperty
        UClassPropertyToMetaClass = 0x54;

        //Class: UInterfaceProperty
        UInterfacePropertyToInterfaceClass = 0x54;

        //Class: UArrayProperty
        UArrayPropertyToInnerProperty = 0x50;

        //Class: UMapProperty
        UMapPropertyToKeyProp = 0x50;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        //Class: USetProperty
        USetPropertyToElementProp = 0x50;

        //Class: UStructProperty
        UStructPropertyToStruct = 0x50;

        //Class: UWorld
        UWorldToPersistentLevel = 0x20;

        //Class: ULevel
        ULevelToAActors = 0x70;
        ULevelToAActorsCount = ULevelToAActors + PointerSize;
#endif
    };
}

#endif //PRIVATE_DUMPER_OFFSETS_H
