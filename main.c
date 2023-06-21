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
	memset(userSelection, 0, sizeof(userSelection));

	createRootDirectory(rootDirEntry, numOfDirEntry, &bootSectorInformation);
	createFatTable(FatEntry, NumOfFatEntry, &bootSectorInformation);

	//browserRootDirectory(rootEntryPoint, numOfDirEntry);
	//browserSubDirectoy(&rootEntryPoint[10], &FatEntry, &bootSectorInformation);

    //pushStack(&manager, &rootEntryPoint[9]);
    //browserSubDirectoy(&(manager.tail->data), &FatEntry, &bootSectorInformation);

    int o = 0;
    while (IS_RESUM == statusFlag)
    {
        o++;
        printThePathOfCurrentFolder(&manager);

        fflush(stdin);
        fgets(userSelection, sizeof(userSelection), stdin);
        if (0 == manager.cnt)
        {
            browserRootDirectory(rootDirEntry, numOfDirEntry);
        }

        pushStack(&manager, &rootDirEntry[9]);
        if (o % 2 == 0)
        {
            popStack(&manager);
        }

        if (4 == manager.cnt)
        {
            browserSubDirectoy(&(manager.tail->data), &FatEntry, &bootSectorInformation);
        }

    }

	return 0;
}
