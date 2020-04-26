# XDMA_DMA_Bypass_Driver
Xilinx XDMA IP DMA Bypass Driver

This code is a simple PCIe driver for the configuring XDMA IP from Xilinx. It writes to the BAR of the DMA Bypass Interface and can also be used to control the memory mapped PCIe registers if the AXI Lite Master Interface option is enabled in Vivado. The driver finds the region of interest in the XDMA and in my design the DMA bypass interface is connected to a BRAM and the contents written here is mapped into it.

References: https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch12.html
