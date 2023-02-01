//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_STRUCTURES_H
#define PRIVATE_DUMPER_STRUCTURES_H

#include "Memory.h"
#include "Offsets.h"

/******************** GNames Decryption ********************/
string XorCypher(string Input) {
    int key = Input.size();
    string Output = Input;

    for (int i = 0; i < Input.size(); i++)
        Output[i] = Input[i] ^ key;

    return Output;
}

struct WideStr {
    static int is_surrogate(UTF16 uc) {
        return (uc - 0xd800u) < 2048u;
    }

    static int is_high_surrogate(UTF16 uc) {
        return (uc & 0xfffffc00) == 0xd800;
    }

    static int is_low_surrogate(UTF16 uc) {
        return (uc & 0xfffffc00) == 0xdc00;
    }

    static wchar_t surrogate_to_utf32(UTF16 high, UTF16 low) {
        return (high << 10) + low - 0x35fdc00;
    }

    static wchar_t *w_str(uintptr_t str, size_t len) {
        wchar_t *output = new wchar_t[len + 1];

        UTF16 *source = ReadArray<UTF16>(str, len);

        for (int i = 0; i < len; i++) {
            const UTF16 uc = source[i];
            if (!is_surrogate(uc)) {
                output[i] = uc;
            } else {
                if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
                    output[i] = surrogate_to_utf32(uc, source[i]);
                else
                    output[i] = L'?';
            }
        }

        free(source);

        output[len] = L'\0';
        return output;
    }

    static string getString(uintptr_t StrPtr, int StrLength) {
        wstring str = w_str(StrPtr, StrLength);

        string result(MAX_SIZE, '\0');

        wcstombs((char *) result.data(), str.c_str(), MAX_SIZE);

        return result;
    }
};

string GetFNameFromID(uint32 index) {
    if (isUE423) {
        uint32 Block = index >> 16;
        uint16 Offset = index & 65535;

        uintptr_t FNamePool = ((ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool);

        uintptr_t NamePoolChunk = Read<uintptr_t>(FNamePool + Offsets::FNamePoolToBlocks + (Block * Offsets::PointerSize));
        uintptr_t FNameEntry = NamePoolChunk + (Offsets::FNameStride * Offset);

        int16 FNameEntryHeader = Read<int16>(FNameEntry + Offsets::FNameEntryHeader);
        uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryToString;
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryToLenBit;

        ///Unicode Dumping Not Supported Yet
        if (StrLength > 0 && StrLength < 250) {
            bool wide = FNameEntryHeader & 1;
            if (wide) {
                if (isDecrypt){
                    return XorCypher(WideStr::getString(StrPtr, StrLength));
                } else {
                    return WideStr::getString(StrPtr, StrLength);
                }
            } else {
                if (isDecrypt){
                    return XorCypher(ReadString2(StrPtr, StrLength));
                } else {
                    return ReadString2(StrPtr, StrLength);
                }
            }
        } else {
            return "None";
        }
    } else {
        static uintptr_t TNameEntryArray;

        TNameEntryArray = Read<uintptr_t>(ModuleBase + Offsets::GNames);

        if (isFuncDec){
            uintptr_t EncGNames = (ModuleBase + Offsets::GNames);
            if (EncGNames) {
                uintptr_t Blocks = Read<uintptr_t>(EncGNames + 0x8);

                uint32 Shift = Read<uint32>(EncGNames);
                uint32 Offset = (Offsets::PointerSize * 2) * (((Shift - 0x64) / 0x3) - 1);

                TNameEntryArray = Read<uintptr_t>(Blocks + Offset);
            }
        }

        if (isPointer){
            TNameEntryArray = Read<uintptr_t>(Read<uintptr_t>(ModuleBase + Offsets::GNames) + Offsets::GNamesPointer);
        }

        uintptr_t FNameEntryArr = Read<uintptr_t>(TNameEntryArray + ((index / 0x4000) * Offsets::PointerSize));
        uintptr_t FNameEntry = Read<uintptr_t>(FNameEntryArr + ((index % 0x4000) * Offsets::PointerSize));

        return ReadString(FNameEntry + Offsets::FNameEntryToNameString, MAX_SIZE);
    }
}

/******************** GUObject Reading ********************/
int32 GetObjectCount() {
        return Read<int32>((ModuleBase + Offsets::GUObjectArray) + Offsets::FUObjectArrayToTUObjectArray + Offsets::TUObjectArrayToNumElements);
}

uintptr_t GetUObjectFromID(uint32 index) {
    if (isUE423) {
        uintptr_t TUObjectArray = Read<uintptr_t>((ModuleBase + Offsets::GUObjectArray) + Offsets::FUObjectArrayToTUObjectArray);
        uintptr_t Chunk = Read<uintptr_t>(TUObjectArray + ((index / 0x10000) * Offsets::PointerSize));
        return Read<uintptr_t>(Chunk + Offsets::FUObjectItemPadd + (index * Offsets::FUObjectItemSize));
    } else {
        uintptr_t TUObjectArray = Read<uintptr_t>((ModuleBase + Offsets::GUObjectArray) + Offsets::FUObjectArrayToTUObjectArray);
        return Read<uintptr_t>(TUObjectArray + (index * Offsets::FUObjectItemSize));
    }
}

/******************** Unreal Engine GUObject Structure ********************/
struct UObject {
    static int32 getIndex(uintptr_t object) {
        return Read<int32>(object + Offsets::UObjectToInternalIndex);
    }

    static uintptr_t getClass(uintptr_t object) {//UClass*
        return Read<uintptr_t>(object + Offsets::UObjectToClassPrivate);
    }

    static uint32 getNameID(uintptr_t object) {
        return Read<uint32>(object + Offsets::UObjectToFNameIndex);
    }

    static uintptr_t getOuter(uintptr_t object) {//UObject*
        return Read<uintptr_t>(object + Offsets::UObjectToOuterPrivate);
    }

    static string getName(uintptr_t object) {
        return GetFNameFromID(getNameID(object));
    }

    static string getClassName(uintptr_t object) {
        return getName(getClass(object));
    }

    static bool isValid(uintptr_t object) {
        return (object > 0 && getNameID(object) > 0 && getClass(object) > 0);
    }
};

/******************** Unreal Engine UField Structure ********************/
struct UField {
    static uintptr_t getNext(uintptr_t field) {//UField*
        return Read<uintptr_t>(field + Offsets::UFieldToNext);
    }
};

/******************** Unreal Engine FField Structure ********************/
struct FField {
    static string getName(uintptr_t fField) {
        return GetFNameFromID(Read<uint32>(fField + Offsets::FFieldToName));
    }

    static string getClassName(uintptr_t fField) {
        return GetFNameFromID(Read<uint32>(Read<uintptr_t>(fField + Offsets::FFieldToClass)));
    }

    static uintptr_t getNext(uintptr_t fField) {//UField*
        return Read<uintptr_t>(fField + Offsets::FFieldToNext);
    }
};

/******************** Unreal Engine UStruct Structure ********************/
struct UStruct {
    static uintptr_t getSuperClass(uintptr_t structz) {//UStruct*
        return Read<uintptr_t>(structz + Offsets::UStructToSuperStruct);
    }

    static uintptr_t getChildren(uintptr_t structz) {//UField*
        return Read<uintptr_t>(structz + Offsets::UStructToChildren);
    }

    static uintptr_t getChildProperties(uintptr_t structz) {//UField*
        return Read<uintptr_t>(structz + Offsets::UStructToChildProperties);
    }

    static string getClassName(uintptr_t clazz) {
        return UObject::getName(clazz);
    }

    static string getClassPath(uintptr_t object) {
        uintptr_t clazz = UObject::getClass(object);
        string classname = UObject::getName(clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }

    static string getStructClassPath(uintptr_t clazz) {
        string classname = UObject::getName(clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }
};

/******************** Unreal Engine UFunction Structure ********************/
struct UFunction {
    static int32 getFunctionFlags(uintptr_t func) {
        return Read<int32>(func + Offsets::UFunctionToFunctionFlags);
    }

    static uintptr_t getFunc(uintptr_t func) {
        return Read<uintptr_t>(func + Offsets::UFunctionToFunc);
    }
};

/******************** Unreal Engine UProperty Structure ********************/
struct UProperty {
    static int32 getElementSize(uintptr_t prop) {
        return Read<int32>(prop + Offsets::UPropertyToElementSize);
    }

    static uint64 getPropertyFlags(uintptr_t prop) {
        return Read<uint64>(prop + Offsets::UPropertyToPropertyFlags);
    }

    static int32 getOffset(uintptr_t prop) {
        return Read<int32>(prop + Offsets::UPropertyToOffsetInternal);
    }
};

/******************** Unreal Engine UBoolProperty Structure ********************/
struct UBoolProperty {
    static uint8 getFieldSize(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyToFieldSize);
    }

    static uint8 getByteOffset(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyToByteOffset);
    }

    static uint8 getByteMask(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyToByteMask);
    }

    static uint8 getFieldMask(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyToFieldMask);
    }
};

/******************** Unreal Engine UObjectProperty Structure ********************/
struct UObjectProperty {
    static uintptr_t getPropertyClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::UObjectPropertyToPropertyClass);
    }
};

/******************** Unreal Engine UClassProperty Structure ********************/
struct UClassProperty {
    static uintptr_t getMetaClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::UClassPropertyToMetaClass);
    }
};

/******************** Unreal Engine UInterfaceProperty Structure ********************/
struct UInterfaceProperty {
    static uintptr_t getInterfaceClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::UInterfacePropertyToInterfaceClass);
    }
};

/******************** Unreal Engine UArrayProperty Structure ********************/
struct UArrayProperty {
    static uintptr_t getInner(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::UArrayPropertyToInnerProperty);
    }
};

/******************** Unreal Engine UMapProperty Structure ********************/
struct UMapProperty {
    static uintptr_t getKeyProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::UMapPropertyToKeyProp);
    }

    static uintptr_t getValueProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::UMapPropertyToValueProp);
    }
};

/******************** Unreal Engine USetProperty Structure ********************/
struct USetProperty {
    static uintptr_t getElementProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::USetPropertyToElementProp);
    }
};

/******************** Unreal Engine UStructProperty Structure ********************/
struct UStructProperty {
    static uintptr_t getStruct(uintptr_t prop) {//UStruct*
        return Read<uintptr_t>(prop + Offsets::UStructPropertyToStruct);
    }
};

#endif //PRIVATE_DUMPER_STRUCTURES_H
