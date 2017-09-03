The purpose of this folder is to provide a starting template for the creation of future labs using the STM32 Workbench build system.

How to use:
1. Enable the HAL modules desired by the followng process: 
	-Expand stm32lib/Drivers/STM32F7xx_HAL_Driver/Src
	-Right-click on the desired module
	-Go to "Resource Configurations" and click "Exclude from Build..."
	-Click "Deselect All" and hit OK
2. Replace hello.c/hello.h (placeholders with a working configuration) with desired code.
3. When distributing the lab, only include the following from the lab's folder:

Folders:
	.settings
	inc
	src
	startup

Files:
	.cproject
	.project
	Linkerscript.ld

Nothing else needs to be distributed.

Enjoy!
-Karl Nasrallah