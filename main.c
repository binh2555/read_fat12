#include "main.h"




int main(void)
{
    uint8_t statusFlag = IS_RESUM;
	const bootSectorInfor bootSectorInformation = getInformationBootSector();
	const uint32_t numOfDirEntry = bootSectorInformation.numOfRootDirectoryEntries;
	const uint32_t NumOfFatEntry = (bootSectorInformation.numOfSecPerFAT * SIZE_PER_SECTOR * ONE_BYTE) / BIT_PER_ENTRY_FAT;
	directoryEntrypointReal rootDirEntry[numOfDirEntry];
	uint16_t FatEntry[NumOfFatEntry];
	managerLinkedList manager = {NULL, NULL, 0};
	uint8_t userSelection[MAX_USER_SELECTION];
	uint8_t subBuff[MAX_USER_SELECTION];
	int8_t findFolderStatus = 0;

	createRootDirectory(rootDirEntry, numOfDirEntry, &bootSectorInformation);
	createFatTable(FatEntry, NumOfFatEntry, &bootSectorInformation);

	browserRootDirectory(rootDirEntry, numOfDirEntry);
	//browserSubDirectoy(&rootDirEntry[10], &FatEntry, &bootSectorInformation);
	//browserSubDirectoy(&(manager.tail->data), &FatEntry, &bootSectorInformation);

    while (IS_RESUM == statusFlag)
    {
        printThePathOfCurrentFolder(&manager);

        memset(userSelection, 0, sizeof(userSelection));
        fflush(stdin);
        fgets(userSelection, sizeof(userSelection), stdin);
        if (   'c'  == userSelection[0]\
            && 'd'  == userSelection[1]\
            && ' '  == userSelection[2]\
            && '.'  != userSelection[3])
        {
            strcopy2(userSelection, subBuff, (strlenByNewline(userSelection) - 2), 3);
            if (0 == manager.cnt)
            {
                findFolderStatus = findFolderInRootDir(&rootDirEntry, numOfDirEntry, subBuff,&bootSectorInformation, &manager);
            }
            else
            {
                findFolderStatus = findFolderInSubDir(&FatEntry, &bootSectorInformation, subBuff, &manager);
            }
            if (4 != findFolderStatus)
            {
                printStringByNewline(subBuff);
                printf(" : not found");
            }
        }
        else if (   'l'  == userSelection[0]\
                 && 's'  == userSelection[1]\
                 && '\n' == userSelection[2])
        {
            if (0 == manager.cnt)
            {
                browserRootDirectory(&rootDirEntry, numOfDirEntry);
            }
            else
            {
                browserSubDirectoy(&(manager.tail->data), &FatEntry, &bootSectorInformation);
            }
        }
        else if (   'c' == userSelection[0]\
                 && 'd' == userSelection[1]\
                 && ' ' == userSelection[2]\
                 && '.' == userSelection[3]\
                 && '.' == userSelection[4]\
                 && '\n'== userSelection[5])
        {
            popStack(&manager);
        }
        else
        {
            printStringByNewline(userSelection);
            printf(" : not found!!!");
        }

    }

	return 0;
}


void getNameFolder(const uint8_t* userCommand, uint8_t* buff, uint32_t position)
{
    strcopy2(userCommand, buff, strlenByNewline(userCommand) - 2, position);
}
