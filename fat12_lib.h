#ifndef    _FAT12_LIB_H_
#define    _FAT12_LIB_H_

#include "hal.h"

#define SIZE_OF_OEM                (8U)
#define SIZE_OF_SIG                (2U)
#define SIZE_OF_TEMP_ARR           (3U)
#define ONE_BYTE                   (8U)
#define BOOT_SETOR_BEGIN           (0U)
#define BYTE_PER_ENTRY             (32U)
#define BIT_PER_ENTRY_FAT          (12U)
#define MASK_DIRECTORY             (0x10U)
#define LONG_NAME_FILE             (0x0fU)
#define IS_RESUM                   (1U)
#define IS_STOP                    (0U)

#define SHIFT_DAY                  (0U)
#define MASK_DAY                   (0x001fU)
#define SHIFT_MONTH                (5U)
#define MASK_MONTH                 (0x000fU)
#define SHIFT_YEAR                 (9U)
#define MASK_YEAR                  (0xffff)
#define REFERENCE_YEAR             (1980U)

#define SHIFT_SECONDS                  (0U)
#define MASK_SECONDS                   (0x001fU)
#define SHIFT_MINUTES                  (5U)
#define MASK_MINUTES                   (0x003fU)
#define SHIFT_HOUR                     (11U)
#define MASK_HOUR                      (0xffff)

#define DATEPOSITION                   (14U)



typedef union{
    uint8_t dataBootSector[SIZE_OF_BOOT_SECTOR];
    struct{
        uint8_t jumpToBootstrap[3];
        uint8_t OEMName[8];
        uint8_t numOfBytePerSec[2];
        uint8_t numOfSecPerCluster[1];
        uint8_t numOfReservedSec[2];
        uint8_t numOfFATCopy[1];
        uint8_t numOfRootDirectoryEntry[2];
        uint8_t numOfSecInFilesystem[2];
        uint8_t mediaDescription[1];
        uint8_t numOfSecPerFAT[2];
        uint8_t secperTrack[2];
        uint8_t numOfHead[2];
        uint8_t numOfHiddenSec[2];
        uint8_t Bootstrap[480];
        uint8_t Signature[2];
    }feild;
}structbootSector;

typedef struct{
    uint8_t OEM[SIZE_OF_OEM];
    uint8_t Signature[SIZE_OF_SIG];
    uint16_t bytePerSec;
    uint16_t secPerClus;
    uint16_t numOfReseredSec;
    uint16_t numOfFatTableCopies;
    uint16_t numOfRootDirectoryEntries;
    uint16_t numOfSecInFile;
    uint16_t MediaDescriptor;
    uint16_t numOfSecPerFAT;
    uint16_t beginingOfFatTable;
    uint16_t beginingOfRootDirectory;
    uint16_t beginingOfDataRegion;
}bootSectorInfor;

typedef union{
    uint8_t entryPointData[32];
    struct
    {
        uint8_t nameOfFile[11];
        uint8_t attributies[1];
        uint8_t resered[1];
        uint8_t fileCreationTime[1];
        uint8_t creationTime[2];
        uint8_t creationDate[2];
        uint8_t accessDate[2];
        uint8_t highOrder2byteOfCluster[2];
        uint8_t modifedTime[2];
        uint8_t modifiedDate[2];
        uint8_t startingCluster[2];
        uint8_t fileSize[4];
    }feild;
}directoryEntrypointDisk;

typedef struct{
    uint8_t nameOfFile[11];
    uint8_t attributies;
    uint8_t resered;
    uint8_t fileCreationTime;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t accessDate;
    uint16_t highOrder2byteOfCluster;
    uint16_t modifedTime;
    uint16_t modifiedDate;
    uint16_t startingCluster;
    uint32_t fileSize;
}directoryEntrypointReal;

struct Node{
    directoryEntrypointReal data;
    struct Node* next;
};
typedef struct Node stackNode;

typedef struct{
    uint32_t cnt;
    stackNode* head;
    stackNode* tail;
}managerLinkedList;

bootSectorInfor getInformationBootSector(void);
void printString(const uint8_t* str, uint32_t length);
void strcopy(const uint8_t* str, uint8_t* storeStr,uint32_t length);
void strcopy2(const uint8_t* str, uint8_t* storeStr,uint32_t length, uint32_t position);
void createRootDirectory(directoryEntrypointReal* directoryEntry, uint32_t numOfEntryPoint, const bootSectorInfor* FatInfor);
void createFatTable(uint16_t* FatEntry, uint32_t numOfFatEntry, const bootSectorInfor* FatInfor);

void browserSubDirectoy(directoryEntrypointReal* directoryEntry,const uint16_t* fatEntry, const bootSectorInfor* bootSectorInformation);
void showContentOfFile(const directoryEntrypointReal* thisFileEntry,const bootSectorInfor* bootSectorInformation);


void pushStack(managerLinkedList* manager, directoryEntrypointReal* thisDirEntry);
void popStack(managerLinkedList* manager);

void printThePathOfCurrentFolder(const managerLinkedList* manager);

#endif    /* _FAT12_LIB_H_ */
