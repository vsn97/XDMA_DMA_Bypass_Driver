/******************************************************************

This code is a simple PCIe driver for the configuring XDMA IP from Xilinx. It writes to the BAR of the DMA Bypass Interface and can also be used to control the memory mapped PCIe registers if the AXI Lite Master Interface option is enabled in Vivado. The driver finds the region of interest in the XDMA and in my design the DMA bypass interface is connected to a BRAM and the contents written here is mapped into it.
References: https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch12.html
******************************************************************/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/pci.h>
#include<asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naarayanan");
MODULE_DESCRIPTION("Linux PCIe Driver for XDMA Xilinx(DMA Bypass)");

/* Xilinx Vendor ID and Device ID programmed by Vivado Design Suite
   Can be changed according to user parameters */

#define VEND_ID 0x10EE
#define DEV_ID 0x7028

// Region of interest for this application. If PCIe to AXI Lite Master Interface is enabled then this region/BAR would change to 2.

#define BAR 1

struct pci_dev *dev = NULL;
unsigned long address;
static void __iomem *p;

//User Parameters

u32 dmaAddr = 0x30000;
u32 size = 0x1000;

static int writeData(struct pci_dev *dev, unsigned long address)
{
  u32 byte;

  //Mapping the physical address to virtual addresses to perform R/W operations with size
  
  p = ioremap(address, size);
  if(p != NULL)
  {
     printk(KERN_INFO "DMA Bypass Region Address of Device %x is %lx\n", dev->vendor, address);
     
     iowrite32(dmaAddr, (u32 *)p);
     iowrite32(size, (u32 *)p + 1);
     byte = ioread32((u32 *)p);
     printk(KERN_INFO "Contents: %x\n", byte);
  }
  else
    {
      printk(KERN_ERR "Mapping failed");
    }
  return 0;
}
  
static int __init pcie_init(void)
{
  dev = pci_get_device(VEND_ID, DEV_ID, NULL);
  if(dev != NULL)
    {
      printk(KERN_INFO "Found PCI\n");
      pci_dev_put(dev);
      pci_enable_device(dev);
      address = pci_resource_start(dev, BAR);
      if(address)
	{
	  writeData(dev, address);
	}
    }
  else
    {
      printk(KERN_INFO "Not found\n");
    }
  return 0;
}

static void __exit pcie_exit(void)
{
  printk(KERN_INFO "Exiting the driver\n");
  
  //Unmap the physical address
     
  iounmap(p);
}

module_init(pcie_init);
module_exit(pcie_exit);


